//
//  UIController.m
//  PSC
//
//  Created by Quentin Fiard on 22/01/12.
//  Copyright (c) 2012 École Polytechnique. All rights reserved.
//

#import "UIController.h"
#import <Foundation/Foundation.h>

#import "SidebarNode.h"

#import "Moteur3D.h"

#import "StatusController.h"

#import <QTKit/QTKit.h>

@implementation UIController

@dynamic window;
@synthesize sidebar,sidebarContainer;
@synthesize delegate;
@synthesize statusController;

-(id)init
{
		
	return self;
}

-(void)testVideo
{
	NSArray* devices = [QTCaptureDevice inputDevices];
	
	for(int i=0 ; i<devices.count ; i++)
	{
		NSLog(@"%@ %@",[[devices objectAtIndex:i] modelUniqueID],[[devices objectAtIndex:i] deviceAttributes]);
	}
}

-(void)initializeStatusController
{
	if(!statusController)
	{
		statusController = [[StatusController alloc] initWithNibName:@"StatusController" bundle:nil];
	}
	
	statusController.uicontroller = self;
	
	[statusController view]; // Creating the view and placing it on screen
}

-(void)awakeFromNib
{
	[self testVideo];
	
	[self initializeStatusController];
}

-(void)setWindow:(NSWindow *)aWindow
{
	[window release];
	window = [aWindow retain];
	
	[window makeKeyAndOrderFront:self];
}

-(NSWindow*)window
{
	return window;
}

#pragma mark Sidebar view change

-(void)changeView:(id)sender
{
	NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
	[currentView removeFromSuperview];
	[mainViewController release];
	mainViewController = [[NSClassFromString([sender ownerClass]) alloc] init];
	[NSBundle loadNibNamed:[sender nibName] owner:mainViewController];
	
	currentView = [mainViewController view];
	
	[currentView setFrame:mainView.bounds];
	
	[mainView addSubview:currentView];
	
	[[[NSApplication sharedApplication] mainWindow] setDelegate:mainViewController];
	
	NSLog(@"%@",@"View changed");
	
	[pool drain];
}

-(void)windowWillClose
{
	[currentView removeFromSuperview];
	[mainViewController release];
}

@end
