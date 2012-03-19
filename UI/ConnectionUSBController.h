//
//  ConnectionUSBController.h
//  PSC
//
//  Created by Quentin Fiard on 18/03/12.
//  Copyright (c) 2012 École Polytechnique. All rights reserved.
//

#import <Cocoa/Cocoa.h>

@interface ConnectionUSBController : NSViewController
{
	IBOutlet NSArray* sensorArray;
	
	IBOutlet NSView* connectedView;
	IBOutlet NSView* unconnectedView;
}

+(ConnectionUSBController*)shared;

-(void)setConnected:(BOOL)isConnected;

-(IBAction)retryConnection:(id)sender;

@end
