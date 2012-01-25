//
//  WebcamController.m
//  PSC
//
//  Created by Quentin Fiard on 23/01/12.
//  Copyright (c) 2012 École Polytechnique. All rights reserved.
//

#import "WebcamController.h"

@implementation WebcamController

- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil
{
    self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil];
    if (self) {
        // Initialization code here.
    }
    
    return self;
}

- (void) awakeFromNib 
{
	//Create the QT capture session
	session = [[QTCaptureSession alloc] init];
	/* Select the default Video input device */
	iSight = [QTCaptureDevice defaultInputDeviceWithMediaType:QTMediaTypeVideo];
	
	NSError* error;
	[iSight open:&error];
	
	NSLog(@"%@",error);
	
	/* Create a QTKit input for the session using the iSight Device */
	QTCaptureDeviceInput *myInput = [QTCaptureDeviceInput deviceInputWithDevice:iSight];
	
	/* Add inputs get the ball rolling etc */
	[session addInput:myInput error:nil];
	[outputView setCaptureSession:session];
	
	/* Let the video madness begin */
	[session startRunning]; 
}

-(void)dealloc
{
	[session stopRunning];
	[super dealloc];
}

@end
