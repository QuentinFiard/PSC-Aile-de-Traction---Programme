//
//  UIController.h
//  PSC
//
//  Created by Quentin Fiard on 22/01/12.
//  Copyright (c) 2012 École Polytechnique. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#include "AppDelegate.h"

@class StatusController;

@interface UIController : NSObject
{
	IBOutlet NSWindow* window;
	IBOutlet AppDelegate* delegate;
	
	id mainViewController;
	id currentView;
	IBOutlet NSView* mainView;
	
	IBOutlet NSOutlineView* sidebar;
	
	StatusController* statusController;
}

@property (assign,nonatomic) IBOutlet NSWindow* window;
@property (retain,nonatomic) IBOutlet NSOutlineView* sidebar;
@property (retain,nonatomic) IBOutlet AppDelegate* delegate;

@property (retain,nonatomic) StatusController* statusController;

-(void)changeView:(id)sender;

@end
