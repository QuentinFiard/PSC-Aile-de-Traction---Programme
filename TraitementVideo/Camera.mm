//
//  Camera.mm
//  PSC
//
//  Created by Quentin Fiard on 18/03/12.
//  Copyright (c) 2012 École Polytechnique. All rights reserved.
//

#include "Camera.h"
#include "Database.h"
#include "CameraStatus.h"
#include "CalibrationCamera.h"

#import "NSImage_OpenCV.h"

#include <boost/date_time.hpp>
#include <iostream>

using namespace boost::posix_time;

@implementation ImageHandler

@synthesize camera,calibration;

-(void)captureOutput:(QTCaptureOutput *)captureOutput didOutputVideoFrame:(CVImageBufferRef)videoFrame withSampleBuffer:(QTSampleBuffer *)sampleBuffer fromConnection:(QTCaptureConnection *)connection
{
	NSCIImageRep *videoRep = [NSCIImageRep imageRepWithCIImage:[CIImage imageWithCVImageBuffer:videoFrame]];
	NSImage *image = [[[NSImage alloc] initWithSize:[videoRep size]] autorelease];
	[image addRepresentation:videoRep];
	//CVBufferRelease(videoFrame);
	
	//bool test = calibration.analyserImageAvecEchiquier([[NSImage imageNamed:@"test.jpg"] CVMat]);
	bool test = calibration->analyserImageAvecEchiquier([image CVMat]);
	
	if(test)
	{
		NSLog(@"%@",@"Image analysée avec succès");
	}
}

-(BOOL)automaticallyDropsLateVideoFrames
{
	return YES;
}

- (void)outputVideoFrame:(CVImageBufferRef)videoFrame withSampleBuffer:(QTSampleBuffer *)sampleBuffer fromConnection:(QTCaptureConnection *)connection
{
	ptime start = microsec_clock::universal_time();
	
	NSCIImageRep *videoRep = [NSCIImageRep imageRepWithCIImage:[CIImage imageWithCVImageBuffer:videoFrame]];
	NSImage *image = [[[NSImage alloc] initWithSize:[videoRep size]] autorelease];
	[image addRepresentation:videoRep];
	//CVBufferRelease(videoFrame);
	
	//bool test = calibration.analyserImageAvecEchiquier([[NSImage imageNamed:@"test.jpg"] CVMat]);
	//bool test = calibration->analyserImageAvecEchiquier([image CVMat]);
	
	/*if(test)
	{
		NSLog(@"%@",@"Image analysée avec succès");
	}*/
	
	ptime end = microsec_clock::universal_time();
	
	std::cout << (end-start).ticks() <<std::endl;
}

@end

static Camera* shared_;

Camera::Camera() : Capteur(CAPTEUR_CAMERA)
{
	handler = [[ImageHandler alloc] init];
	handler.camera = this;
	
	decompressedVideo = nil;
	decompressedSession = nil;
	camera = nil;
	session = nil;
	
	views = [[NSMutableArray alloc] init];
	
	switchToSelectedCamera_();
}

Camera::~Camera()
{
	[session stopRunning];
}

Camera* Camera::shared()
{
	if(!shared_)
	{
		shared_ = new Camera();
	}
	return shared_;
}

cv::Mat Camera::currentImage()
{
	return shared()->currentImage_();
}

void Camera::checkCameraStatus()
{
	shared()->checkCameraStatus_();
}

void Camera::displayInView(QTCaptureView* view)
{
	shared()->displayInView_(view);
}

void Camera::removeFromView(QTCaptureView* view)
{
	shared()->removeFromView_(view);
}

void Camera::switchToSelectedCamera()
{
	shared()->switchToSelectedCamera_();
}

cv::Mat Camera::currentImage_()
{
	
}

void Camera::useCurrentImageForCalibration(CalibrationCamera &calibration)
{
	shared()->useCurrentImageForCalibration_(calibration);
}

void Camera::checkCameraStatus_()
{
	
}

void Camera::displayInView_(QTCaptureView* view)
{
	if(![views containsObject:view])
	{
		[views addObject:view];
		
		[view setCaptureSession:session];
	}
}

void Camera::removeFromView_(QTCaptureView* view)
{
	if([views containsObject:view])
	{
		[views removeObject:view];
		
		[view setCaptureSession:nil];
	}
}

void Camera::switchToSelectedCamera_()
{
	if(session)
	{
		[session stopRunning];
		[session release];
		session = nil;
	}
	if(decompressedSession)
	{
		[decompressedSession stopRunning];
		[decompressedSession release];
		decompressedSession = nil;
	}
	if(camera)
	{
		[camera release];
		camera = nil;
	}
	if(decompressedVideo)
	{
		[decompressedVideo release];
		decompressedVideo = nil;
	}
	
	if(Database::hasConfigurationField("CameraID"))
	{
		std::string cameraID = Database::getConfigurationFieldValue<std::string>("CameraID");
		
		NSString* ID = [[NSString alloc] initWithUTF8String:cameraID.c_str()];
		
		camera = [[QTCaptureDevice deviceWithUniqueID:ID] retain];
		
		if(camera)
		{
			//Create the QT capture session
			session = [[QTCaptureSession alloc] init];
			decompressedSession = [[QTCaptureSession alloc] init];
			
			NSError* error;
			[camera open:&error];
			
			NSLog(@"%@",error);
			
			/* Create a QTKit input for the session using the iSight Device */
			QTCaptureDeviceInput *myInput = [QTCaptureDeviceInput deviceInputWithDevice:camera];
			QTCaptureDeviceInput *myInput2 = [QTCaptureDeviceInput deviceInputWithDevice:camera];
			
			decompressedVideo = [[QTCaptureDecompressedVideoOutput alloc] init];
			
			/* Add inputs get the ball rolling etc */
			[session addInput:myInput error:nil];
			[decompressedSession addInput:myInput2 error:nil];
			[decompressedSession addOutput:decompressedVideo error:nil];
			
			for(QTCaptureView* view in views)
			{
				[view setCaptureSession:session];
			}
			
			[decompressedSession addOutput:handler error:nil];
			
			/* Let the video madness begin */
			[session startRunning];
			[decompressedSession startRunning];
		}
	}
	
	if(!camera)
	{
		[[CameraStatus shared] setStatus:STATUS_FAILED];
	}
	else
	{
		[[CameraStatus shared] setStatus:STATUS_OK];
	}
}

void Camera::useCurrentImageForCalibration_(CalibrationCamera &calibration)
{
	handler.calibration = &calibration;
	//shouldAnalyseImage = YES;
	//[decompressedSession startRunning];
	//[decompressedVideo setDelegate:self];
}

double Camera::value()
{
	return 0;
}