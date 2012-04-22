//
//  Camera.mm
//  PSC
//
//  Created by Quentin Fiard on 18/03/12.
//  Copyright (c) 2012 École Polytechnique. All rights reserved.
//

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include "Camera.h"
#include "Database.h"
#include "CameraStatus.h"
#include "CalibrationCamera.h"

#import "NSImage_OpenCV.h"

#include <boost/date_time.hpp>
#include <iostream>

#include "Recorder.h"

#include "Utilities.h"

#include <sstream>

#include <sys/time.h>

using namespace boost::posix_time;

@implementation ImageHandler

@synthesize camera,calibration;

-(BOOL)automaticallyDropsLateVideoFrames
{
	return YES;
}

-(void)startImageProcessing:(NSImage*)image
{
	camera->processImage(image);
}

- (void)outputVideoFrame:(CVImageBufferRef)videoFrame withSampleBuffer:(QTSampleBuffer *)sampleBuffer fromConnection:(QTCaptureConnection *)connection
{
	ptime start = microsec_clock::universal_time();
	
	NSCIImageRep *videoRep = [NSCIImageRep imageRepWithCIImage:[CIImage imageWithCVImageBuffer:videoFrame]];
	NSImage *image = [[NSImage alloc] initWithSize:[videoRep size]];
	[image addRepresentation:videoRep];
	
	//[NSThread detachNewThreadSelector:@selector(startImageProcessing:) toTarget:self withObject:image];
	
	//[self performSelectorOnMainThread:@selector(startImageProcessing:) withObject:image waitUntilDone:YES];
	
	camera->processImage(image);
	
	[image release];
	
	ptime end = microsec_clock::universal_time();
	
	std::cout << (end-start).ticks() <<std::endl;
}

@end

static Camera* shared_;

Camera::Camera() : Capteur(CAPTEUR_CAMERA)
{
	handler = [[ImageHandler alloc] init];
	handler.camera = this;
	
	decompressedSession = nil;
	camera = nil;
	session = nil;
	
	output = nil;
	
	calibration = NULL;
	
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

void Camera::useCurrentImageForCalibration(CalibrationCamera* calibration)
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
	if(output)
	{
		[output release];
		output = nil;
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
			
			output = [[QTCaptureMovieFileOutput alloc] init];
			
			/* Add inputs get the ball rolling etc */
			[session addInput:myInput error:nil];
			[decompressedSession addInput:myInput2 error:nil];
			
			for(QTCaptureView* view in views)
			{
				[view setCaptureSession:session];
			}
			
			[decompressedSession addOutput:handler error:nil];
			[decompressedSession addOutput:output error:nil];
			
			
			[output setCompressionOptions:[QTCompressionOptions compressionOptionsWithIdentifier:@"QTCompressionOptionsSD480SizeH264Video"] forConnection:[[output connections] objectAtIndex:0]];
			
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

void Camera::useCurrentImageForCalibration_(CalibrationCamera* newCalibration)
{
	calibration = newCalibration;
}

double Camera::value()
{
	return 0;
}

void Camera::startTracking()
{
	shared()->startTracking_();
}

void Camera::startTracking_()
{
	isTracking = true;
	
	std::stringstream path;
	
	path << pathToVideosDirectory();
	path << Recorder::record()->ID();
	path << "_original";
	path << ".mov";
	
	NSString* stringPath = [NSString stringWithUTF8String:path.str().c_str()];
	
	//NSLog(@"Writing video file %@",stringPath);
	
	NSURL* url = [NSURL fileURLWithPath:stringPath];
	
	//NSLog(@"Connection : %@",[output connections]);
	
	[output recordToOutputFileURL:url];
}

void Camera::stopTracking()
{
	shared()->stopTracking_();
}

void Camera::stopTracking_()
{
	isTracking = false;
	
	[output recordToOutputFileURL:nil];
}

void Camera::processImage(NSImage* image)
{
	if(calibration != NULL)
	{
		calibration->analyserImageAvecEchiquier([image CVMat]);
		
		calibration = NULL;
	}
	if(isTracking)
	{
		
	}
}