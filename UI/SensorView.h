//
//  SensorView.h
//  PSC
//
//  Created by Quentin Fiard on 26/03/12.
//  Copyright (c) 2012 École Polytechnique. All rights reserved.
//

#import <Cocoa/Cocoa.h>

@class AutoUpdateGrandeurPopUpButton;

@interface SensorView : NSView
{
	IBOutlet AutoUpdateGrandeurPopUpButton* choixGrandeur;
	IBOutlet NSTextField* numSensor;
}

@property (retain,nonatomic) IBOutlet AutoUpdateGrandeurPopUpButton* choixGrandeur;
@property (retain,nonatomic) IBOutlet NSTextField* numSensor;

@end


@interface AutoUpdateGrandeurPopUpButton : NSPopUpButton
{
	NSUInteger sensor;
}

@property (nonatomic) NSUInteger sensor;

@end

