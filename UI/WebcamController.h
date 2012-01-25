//
//  WebcamController.h
//  PSC
//
//  Created by Quentin Fiard on 23/01/12.
//  Copyright (c) 2012 École Polytechnique. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import <QTKit/QTKit.h>

@interface WebcamController : NSViewController
{
	IBOutlet QTCaptureView *outputView;
	
	QTCaptureSession *session;
	QTCaptureDevice *iSight;
}

@end
