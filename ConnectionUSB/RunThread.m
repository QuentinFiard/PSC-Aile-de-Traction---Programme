//
//  RunThread.m
//  PSC
//
//  Created by Quentin Fiard on 02/05/12.
//  Copyright (c) 2012 École Polytechnique. All rights reserved.
//

#import "RunThread.h"

@implementation RunThread

-(void)resetKeepAlive
{
	
}


-(void)initializeTimer
{
	NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
    NSRunLoop* runLoop = [NSRunLoop currentRunLoop];
    //Fire timer every second to updated countdown and date/time
    keepAlive = [[NSTimer scheduledTimerWithTimeInterval:1 target:self selector:@selector(resetKeepAlive) userInfo:nil repeats:YES] retain];
    [runLoop run];
    [pool release];
}

-(id)init
{
	self = [super initWithTarget:self selector:@selector(initializeTimer) object:nil];
	if(self)
	{
		
	}
}

@end
