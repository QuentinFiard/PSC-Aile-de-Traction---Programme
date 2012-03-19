//
//  Camera.h
//  PSC
//
//  Created by Quentin Fiard on 18/03/12.
//  Copyright (c) 2012 École Polytechnique. All rights reserved.
//

#ifndef PSC_Camera_h
#define PSC_Camera_h

#include "Capteur.h"

#include <opencv2/core/core.hpp>

#import <Cocoa/Cocoa.h>
#import <QTKit/QTKit.h>

@interface CameraDelegate : NSObject
{
	
}

@end

class CalibrationCamera;

class Camera : public Capteur
{
public:
	static Camera* shared();
	
	static cv::Mat currentImage();
	
	static void checkCameraStatus();
	
	static void useCurrentImageForCalibration(CalibrationCamera &calibration);
	
	static void displayInView(QTCaptureView* view);
	static void removeFromView(QTCaptureView* view);
	
	static void switchToSelectedCamera();
	
	static bool isConnected();
	
protected:
	
	Camera();
	~Camera();
	
	cv::Mat currentImage_();
	
	void checkCameraStatus_();
	
	void displayInView_(QTCaptureView* view);
	void removeFromView_(QTCaptureView* view);
	void switchToSelectedCamera_();
	void useCurrentImageForCalibration_();
	
	double value();
	
private:
	QTCaptureDevice *camera;
	
	QTCaptureDecompressedVideoOutput* decompressedVideo;
	QTCaptureSession *decompressedSession;
	QTCaptureSession *session;
	
	NSMutableArray* views;
};

#endif
