//
//  CameraStatus.m
//  PSC
//
//  Created by Quentin Fiard on 21/02/12.
//  Copyright (c) 2012 École Polytechnique. All rights reserved.
//

#import "Camera.h"
#import "CameraStatus.h"

static CameraStatus* shared;

@implementation CameraStatus

+(CameraStatus*)shared
{
	if(!shared)
	{
		shared = [[CameraStatus alloc] init];
	}
	return shared;
}

-(id)init
{
	self = [super init];
	if(self)
	{
		self.titre = @"Caméra";
	}
	return self;
}

-(IBAction)retry:(id)sender
{
	Camera::checkCameraStatus();
}

@end
