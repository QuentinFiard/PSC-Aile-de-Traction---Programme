//
//  AutoUpdatePopUpButton.m
//  PSC
//
//  Created by Quentin Fiard on 19/04/12.
//  Copyright (c) 2012 École Polytechnique. All rights reserved.
//

#import "AutoUpdatePopUpButton.h"
#import "WebcamCalibrationController.h"

@implementation AutoUpdatePopUpButton

- (void)mouseDown:(NSEvent *)theEvent {
    [controller updateCameraList:self];
	[super mouseDown:theEvent];	
}

@end