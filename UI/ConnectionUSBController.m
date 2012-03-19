//
//  ConnectionUSBController.m
//  PSC
//
//  Created by Quentin Fiard on 18/03/12.
//  Copyright (c) 2012 École Polytechnique. All rights reserved.
//

#import "ConnectionUSBController.h"

static ConnectionUSBController* shared;

@implementation ConnectionUSBController

+(ConnectionUSBController*)shared
{
	return shared;
}

- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil
{
    self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil];
    if (self) {
        shared = self;
    }
    
    return self;
}

-(void)setConnected:(BOOL)isConnected
{
	if(isConnected)
	{
		
	}
	else
	{
		
	}
}

-(void)dealloc
{
	shared = nil;
}

@end
