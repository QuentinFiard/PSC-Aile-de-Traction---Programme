//
//  AutoUpdatePopUpButton.h
//  PSC
//
//  Created by Quentin Fiard on 19/04/12.
//  Copyright (c) 2012 École Polytechnique. All rights reserved.
//

#import <Cocoa/Cocoa.h>

@class WebcamCalibrationController;

@interface AutoUpdatePopUpButton : NSPopUpButton
{
	IBOutlet WebcamCalibrationController* controller;
}

@end