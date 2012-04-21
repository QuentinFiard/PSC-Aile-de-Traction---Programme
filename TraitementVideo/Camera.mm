//
//  Camera.mm
//  PSC
//
//  Created by Quentin Fiard on 18/03/12.
//  Copyright (c) 2012 École Polytechnique. All rights reserved.
//

#include <opencv2/highgui/highgui.hpp>

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

static uint64_t getNanoseconds(void)
{
    struct timeval now;
    gettimeofday(&now, NULL);
    return now.tv_sec * NSEC_PER_SEC + now.tv_usec * NSEC_PER_USEC;
}

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
	
	[self performSelectorOnMainThread:@selector(startImageProcessing:) withObject:image waitUntilDone:YES];
	
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
	
	decompressedVideo = nil;
	decompressedSession = nil;
	camera = nil;
	session = nil;
	
	
	
	//************************************
	// FFMpeg recording
	
	
	isTracking = false;
	
	codecContext = NULL;
    videoStream = NULL;
    outputFormat = NULL;
    outputFormatContext = NULL;
    videoFrame = NULL;
	
    pictureBuffer = NULL;
    outputBuffer = NULL;
	
	/* must be called before using avcodec lib */
    //avcodec_init();
	
    /* register all the codecs */
    avcodec_register_all();
    av_register_all();
	
    av_log_set_level( AV_LOG_ERROR );
	
	//*************************************
	
	
	
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
}

void Camera::stopTracking()
{
	shared()->stopTracking_();
}

void Camera::stopTracking_()
{
	isTracking = false;
	
	if(codecContext!=NULL)
	{
		stopRecordingMovie();
	}
}

void Camera::createVideoFileWithSize(int width,int height)
{
	std::stringstream ss;
	ss << pathToVideosDirectory();
	ss << Recorder::record()->ID();
	ss << "_origin.avi";
	
	const char *filename = ss.str().c_str();
	
	// Use an avi container
	outputFormat = av_guess_format("avi", NULL, NULL);
	if (!outputFormat) {
		NSLog(@"Could not set output format");
	}
	
	outputFormatContext = avformat_alloc_context();
	if (!outputFormatContext)
	{
		NSLog(@"avformat_alloc_context Error!");
	}
	
	outputFormatContext->oformat = outputFormat;
	snprintf(outputFormatContext->filename, sizeof(outputFormatContext->filename), "%s", filename);
	
	// Add a video stream
	videoStream = avformat_new_stream(outputFormatContext,0);
	if (!videoStream)
	{
		NSLog(@"av_new_stream Error!");
	}
	
	// Use the MPEG4 encoder (other DiVX-style encoders aren't compatible with this container, and x264 is GPL-licensed)
	AVCodec *codec = avcodec_find_encoder(CODEC_ID_RAWVIDEO);  
	if (!codec) {
		fprintf(stderr, "codec not found\n");
		exit(1);
	}
	
	codecContext = videoStream->codec;
	
	codecContext->codec_id = codec->id;
	codecContext->codec_type = AVMEDIA_TYPE_VIDEO;
	codecContext->bit_rate = 9600000;
	codecContext->width = width;
	codecContext->height = height;
	codecContext->pix_fmt = PIX_FMT_YUV420P;
	//      codecContext->time_base = (AVRational){1,(int)round(framesPerSecond)};
	//      videoStream->time_base = (AVRational){1,(int)round(framesPerSecond)};
	codecContext->time_base = (AVRational){1,200}; // Set it to 200 FPS so that we give a little wiggle room when recording at 50 FPS
	videoStream->time_base = (AVRational){1,200};
	//      codecContext->max_b_frames = 3;
	//      codecContext->b_frame_strategy = 1;
	codecContext->qmin = 1;
	codecContext->qmax = 10;    
	//      codecContext->mb_decision = 2; // -mbd 2
	//      codecContext->me_cmp = 2; // -cmp 2
	//      codecContext->me_sub_cmp = 2; // -subcmp 2
	codecContext->keyint_min = FRAMERATE; 
	//      codecContext->flags |= CODEC_FLAG_4MV; // 4mv
	//      codecContext->flags |= CODEC_FLAG_LOOP_FILTER;
	codecContext->i_quant_factor = 0.71;
	codecContext->qcompress = 0.6;
	//      codecContext->max_qdiff = 4;
	
	//if(CODEC_ID_MPEG4 == CODEC_ID_H264)
	//	av_opt_set(codecContext->priv_data, "preset", "slow", 0);
	
	if(outputFormat->flags & AVFMT_GLOBALHEADER)
	{
		codecContext->flags |= CODEC_FLAG_GLOBAL_HEADER;
	}
	
	// Open the codec
	if (avcodec_open2(codecContext, codec, NULL) < 0) 
	{
		NSLog(@"Couldn't initialize the codec");
		return;
	}
	
	// Open the file for recording
	if (avio_open(&outputFormatContext->pb, outputFormatContext->filename, AVIO_FLAG_WRITE) < 0) 
	{ 
		NSLog(@"Couldn't open file");
		return;
	} 
	
	// Start by writing the video header
	if (avformat_write_header(outputFormatContext, NULL) < 0) 
	{ 
		NSLog(@"Couldn't write video header");
		return;
	} 
	
	// Set up the video frame and output buffers
	outputBufferSize = 2000000;
	outputBuffer = (uint8_t *)malloc(outputBufferSize);
	int size = codecContext->width * codecContext->height;
	
	int pictureBytes = avpicture_get_size(PIX_FMT_YUV420P, codecContext->width, codecContext->height);
	pictureBuffer = (uint8_t *)av_malloc(pictureBytes);
	
	videoFrame = avcodec_alloc_frame();
	videoFrame->data[0] = pictureBuffer;
	videoFrame->data[1] = videoFrame->data[0] + size;
	videoFrame->data[2] = videoFrame->data[1] + size / 4;
	videoFrame->linesize[0] = codecContext->width;
	videoFrame->linesize[1] = codecContext->width / 2;
	videoFrame->linesize[2] = codecContext->width / 2;
	
	avpicture_alloc(&inputRGBAFrame, PIX_FMT_BGRA, codecContext->width, codecContext->height);
	
	frameColorCounter = 0;
	
	movieStartTime = getNanoseconds();
}

void Camera::saveImageInVideoFile(NSImage* image)
{	
	frameColorCounter++;
	
	if (codecContext == NULL)
	{       
		return;
	}
	
	CGImageSourceRef source;
	
	source = CGImageSourceCreateWithData((CFDataRef)[image
													 TIFFRepresentation], NULL);
	CGImageRef newCgImage = CGImageSourceCreateImageAtIndex(source, 0, NULL);
	
	CGDataProviderRef dataProvider = CGImageGetDataProvider(newCgImage);
	NSData* bitmapData = [image TIFFRepresentationUsingCompression:NSTIFFCompressionNone factor:1];//CGDataProviderCopyData(dataProvider);
	NSBitmapImageRep* imageRep = [[NSBitmapImageRep alloc] initWithData:bitmapData];
	uint8_t* buffer1 = (uint8_t *)[imageRep bitmapData];
	
	uint8_t* buffer = new uint8_t[[imageRep bytesPerPlane]];
	
	for(int i=0 ; i<[imageRep bytesPerPlane]/4 ; i++)
	{
		buffer[4*i] = buffer1[3*i+2];
		buffer[4*i+1] = buffer1[3*i+1];
		buffer[4*i+2] = buffer1[3*i+0];
		buffer[4*i+3] = 0xFF;
	}
	
	
	avpicture_fill(&inputRGBAFrame, buffer,
				   PIX_FMT_BGRA, codecContext->width, codecContext->height);
	
	// Take the input RGB8 texture data and convert it to a YUV 4:2:0 planar frame
	struct SwsContext * img_convert_ctx = sws_getContext(codecContext->width, codecContext->height, PIX_FMT_BGRA, codecContext->width, codecContext->height, PIX_FMT_YUV420P, SWS_FAST_BILINEAR, NULL, NULL, NULL); 
	sws_scale(img_convert_ctx, (const uint8_t* const *)inputRGBAFrame.data, inputRGBAFrame.linesize, 0, codecContext->height, videoFrame->data, videoFrame->linesize);
	
	// Encode the frame
	int out_size = avcodec_encode_video(codecContext, outputBuffer, outputBufferSize, videoFrame);  
	
	// Generate a packet and insert in the video stream
	if (out_size != 0) 
	{
		AVPacket videoPacket;
		av_init_packet(&videoPacket);
		
		if (codecContext->coded_frame->pts != AV_NOPTS_VALUE) 
		{
			uint64_t currentFrameTime = getNanoseconds();
			
			videoPacket.pts = av_rescale_q(((uint64_t)currentFrameTime - (uint64_t)movieStartTime) / 1000ull/*codecContext->coded_frame->pts*/, AV_TIME_BASE_Q/*codecContext->time_base*/, videoStream->time_base);
			
			//              NSLog(@"Frame time %lld, converted time: %lld", ((uint64_t)currentFrameTime - (uint64_t)movieStartTime) / 1000ull, videoPacket.pts);
		}
		
		if(codecContext->coded_frame->key_frame)
		{
			videoPacket.flags |= AV_PKT_FLAG_KEY;
		}
		videoPacket.stream_index = videoStream->index;
		videoPacket.data = outputBuffer;
		videoPacket.size = out_size;
		
		int ret = av_write_frame(outputFormatContext, &videoPacket);
		if (ret < 0) 
		{ 
			av_log(outputFormatContext, AV_LOG_ERROR, "%s","Error while writing frame.\n"); 
			av_free_packet(&videoPacket);
			return;
		} 
		
		av_free_packet(&videoPacket);
	}
}

void Camera::stopRecordingMovie()
{
	if (av_write_trailer(outputFormatContext) < 0) 
	{ 
		av_log(outputFormatContext, AV_LOG_ERROR, "%s","Error while writing trailer.\n"); 
		exit(1); 
	} 
	
	// Close out the file
	if (!(outputFormat->flags & AVFMT_NOFILE)) 
	{
		avio_close(outputFormatContext->pb);
	}
	
	// Free up all movie-related resources
	avcodec_close(codecContext);
	av_free(codecContext);
	codecContext = NULL;
	
	free(pictureBuffer);
	free(outputBuffer);
	
	av_free(videoFrame);
	av_free(outputFormatContext);
	av_free(videoStream);  
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
		cv::Mat cvImage = [image CVMat];
		
		if(codecContext==NULL)
		{
			createVideoFileWithSize(image.size.width,image.size.height);
		}
		
		saveImageInVideoFile(image);
	}
}