//
//  StatusController.h
//  PSC
//
//  Created by Quentin Fiard on 23/01/12.
//  Copyright (c) 2012 École Polytechnique. All rights reserved.
//

#import <Cocoa/Cocoa.h>

@class Status;
@class UIController;

@interface StatusController : NSViewController
{
	IBOutlet NSMutableArray* statusArray;
	UIController* uicontroller;
	
	BOOL isOnView;
}

@property (readonly,nonatomic) IBOutlet NSMutableArray* statusArray;
@property (assign,nonatomic) UIController* uicontroller;
@property (nonatomic) BOOL isOnView;

-(IBAction)retryTest:(id)sender withTitle:(id)title;

-(void)insertObject:(Status*)status inStatusArrayAtIndex:(NSUInteger)index;
-(void)removeObjectFromStatusArrayAtIndex:(NSUInteger)index;

-(void)showStatusView;

@end
