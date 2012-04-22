//
//  Camera.h
//  PSC
//
//  Created by Quentin Fiard on 18/03/12.
//  Copyright (c) 2012 École Polytechnique. All rights reserved.
//

#ifndef PSC_Camera_h
#define PSC_Camera_h

#define FRAMERATE 30

#include <opencv2/core/core.hpp>

extern "C"
{
	#include <libavcodec/avcodec.h>
	#include <libavformat/avformat.h>
	#include <libswscale/swscale.h>
}

#import <Cocoa/Cocoa.h>
#import <QTKit/QTKit.h>

#include "Capteur.h"

class CalibrationCamera;
class Camera;

namespace cv
{
	class VideoWriter;
}

@interface ImageHandler : QTCaptureDecompressedVideoOutput
{
	Camera* camera;
}

@property (assign,nonatomic) Camera* camera;
@property (assign,nonatomic) CalibrationCamera* calibration;

@end

class Camera : public Capteur
{
public:
	static Camera* shared();
	
	static void checkCameraStatus();
	
	static void useCurrentImageForCalibration(CalibrationCamera* calibration);
	
	static void displayInView(QTCaptureView* view);
	static void removeFromView(QTCaptureView* view);
	
	static void switchToSelectedCamera();
	
	static bool isConnected();
	
	static void startTracking();
	static void stopTracking();
	
	void processImage(NSImage* image);
	
protected:
	
	Camera();
	~Camera();
	
	cv::Mat currentImage_();
	
	void checkCameraStatus_();
	
	void displayInView_(QTCaptureView* view);
	void removeFromView_(QTCaptureView* view);
	void switchToSelectedCamera_();
	void useCurrentImageForCalibration_(CalibrationCamera* calibration);
	
	double value();
	
	void startTracking_();
	void stopTracking_();
	
private:
	QTCaptureDevice *camera;
	QTCaptureSession *decompressedSession;
	QTCaptureSession *session;
	
	QTCaptureMovieFileOutput* output;
	BOOL isTracking;
	
	NSMutableArray* views;
	
	ImageHandler* handler;
	
	CalibrationCamera* calibration;
};

#endif
