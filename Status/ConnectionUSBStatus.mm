//
//  ConnectionUSBStatus.m
//  PSC
//
//  Created by Quentin Fiard on 15/03/12.
//  Copyright (c) 2012 École Polytechnique. All rights reserved.
//

#import "ConnectionUSBStatus.h"
#import "ConnectionUSB.h"

static ConnectionUSBStatus* shared;

@implementation ConnectionUSBStatus

+(ConnectionUSBStatus*)shared
{
	if(!shared)
	{
		shared = [[ConnectionUSBStatus alloc] init];
	}
	return shared;
}

-(id)init
{
	self = [super init];
	if(self)
	{
		self.titre = @"Connection USB";
		ConnectionUSB::setStatus(self);
	}
	return self;
}

-(IBAction)retry:(id)sender
{
	ConnectionUSB::connect();
}

@end
