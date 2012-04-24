//
//  MotorStatus.m
//  PSC
//
//  Created by Quentin Fiard on 23/04/12.
//  Copyright (c) 2012 École Polytechnique. All rights reserved.
//

#import "MotorStatus.h"
#import "Moteur.h"

static MotorStatus* shared;

@implementation MotorStatus

+(MotorStatus*)shared
{
	if(!shared)
	{
		shared = [[MotorStatus alloc] init];
	}
	return shared;
}

-(id)init
{
	self = [super init];
	if(self)
	{
		self.titre = @"Moteur";
		
		if(Moteur::shared() == NULL)
		{
			[self setStatus:STATUS_FAILED];
		}
		else
		{
			[self setStatus:STATUS_OK];
		}
	}
	return self;
}

-(IBAction)retry:(id)sender
{
	if(Moteur::shared() == NULL)
	{
		[self setStatus:STATUS_FAILED];
	}
	else
	{
		[self setStatus:STATUS_OK];
	}
}

@end
