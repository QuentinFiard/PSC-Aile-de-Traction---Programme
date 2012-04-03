//
//  Camera.h
//  PSC
//
//  Created by Quentin Fiard on 18/03/12.
//  Copyright (c) 2012 École Polytechnique. All rights reserved.
//

#ifndef PSC_Camera_h
#define PSC_Camera_h

#import <Cocoa/Cocoa.h>
#import <QTKit/QTKit.h>

#include "Capteur.h"

#include <opencv2/core/core.hpp>

class CalibrationCamera;
class Camera;

@interface ImageHandler : QTCaptureDecompressedVideoOutput
{
	CalibrationCamera* calibration;
	Camera* camera;
}

@property (assign,nonatomic) Camera* camera;
@property (assign,nonatomic) CalibrationCamera* calibration;

@end

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
	void useCurrentImageForCalibration_(CalibrationCamera &calibration);
	
	double value();
	
private:
	QTCaptureDevice *camera;
	
	QTCaptureDecompressedVideoOutput* decompressedVideo;
	QTCaptureSession *decompressedSession;
	QTCaptureSession *session;
	
	NSMutableArray* views;
	
	ImageHandler* handler;
};

#endif
