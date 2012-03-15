//
//  ConnectionUSBStatus.m
//  PSC
//
//  Created by Quentin Fiard on 15/03/12.
//  Copyright (c) 2012 École Polytechnique. All rights reserved.
//

#import "ConnectionUSBStatus.h"
#import "ConnectionUSB.h"

@implementation ConnectionUSBStatus

-(id)init
{
	self = [super init];
	if(self)
	{
		ConnectionUSB::connect();
	}
	return self;
}

-(IBAction)retry:(id)sender
{
	ConnectionUSB::connect();
}

@end
