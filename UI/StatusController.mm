//
//  StatusController.m
//  PSC
//
//  Created by Quentin Fiard on 23/01/12.
//  Copyright (c) 2012 École Polytechnique. All rights reserved.
//

#import "StatusController.h"

#import "Status.h"
#import "UIController.h"
#import "CameraStatus.h"
#import "ConnectionUSBStatus.h"

@implementation StatusController

@synthesize statusArray;
@synthesize uicontroller;
@synthesize isOnView;

- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil
{
    self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil];
    if (self) {
        statusArray = [[NSMutableArray alloc] init];
		isOnView = NO;
    }
    
    return self;
}

-(void)showStatusView
{
	isOnView = YES;
	
	[uicontroller.window.contentView addSubview:self.view];
	
	NSRect frame = self.view.frame;
	NSRect sidebarFrame = [uicontroller.sidebarContainer frame];
	
	frame.origin.x = sidebarFrame.size.width;
	frame.origin.y = 0;
	frame.size.width = uicontroller.window.frame.size.width - sidebarFrame.size.width;
	
	[self.view setFrame:frame];
}

-(void)awakeFromNib
{
	[self showStatusView];
	
	[self insertObject:[CameraStatus shared] inStatusArrayAtIndex:0];
	
	[self insertObject:[ConnectionUSBStatus shared] inStatusArrayAtIndex:1];
}

-(void)dealloc
{
	[statusArray release];
	[super dealloc];
}

-(void)insertObject:(Status*)status inStatusArrayAtIndex:(NSUInteger)index {
    [statusArray insertObject:status atIndex:index];
}

-(void)removeObjectFromStatusArrayAtIndex:(NSUInteger)index {
    [statusArray removeObjectAtIndex:index];
}

@end
