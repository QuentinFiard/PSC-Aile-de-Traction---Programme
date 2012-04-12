//
//  JoystickStatus.m
//  PSC
//
//  Created by Quentin Fiard on 11/04/12.
//  Copyright (c) 2012 École Polytechnique. All rights reserved.
//

#import "JoystickStatus.h"
#import "Joystick.h"
#import "UIController.h"

static JoystickStatus* shared;

@implementation JoystickStatus

+(JoystickStatus*)shared
{
	if(!shared)
	{
		shared = [[JoystickStatus alloc] init];
	}
	return shared;
}

-(id)init
{
	self = [super init];
	if(self)
	{
		self.titre = @"Joystick";
		Joystick::setStatus(self);
	}
	return self;
}

-(IBAction)retry:(id)sender
{
	[[UIController controller] prepareJoystick];
}

@end
