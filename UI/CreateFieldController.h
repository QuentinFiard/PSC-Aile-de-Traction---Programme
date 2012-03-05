//
//  CreateFieldController.h
//  PSC
//
//  Created by Quentin Fiard on 14/02/12.
//  Copyright (c) 2012 École Polytechnique. All rights reserved.
//

#import <Cocoa/Cocoa.h>

@class DatabaseController;

@interface CreateFieldController : NSObject
{
	IBOutlet NSTextField* nameField;
	IBOutlet NSPopUpButton* typeButton;
	
	IBOutlet NSWindow* window;
	
	IBOutlet DatabaseController* mainController;
	
	NSString* type;
}

@property (retain,nonatomic) NSWindow* window;
@property (retain,nonatomic) NSString* type;

-(void)showWindow;

-(IBAction)confirm:(id)sender;
-(IBAction)cancel:(id)sender;

@end
