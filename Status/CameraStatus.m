//
//  CameraStatus.m
//  PSC
//
//  Created by Quentin Fiard on 21/02/12.
//  Copyright (c) 2012 École Polytechnique. All rights reserved.
//

#import "CameraStatus.h"

@implementation CameraStatus

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
	
}
@end
