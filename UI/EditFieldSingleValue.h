//
//  EditFieldSingleValue.h
//  PSC
//
//  Created by Quentin Fiard on 14/02/12.
//  Copyright (c) 2012 École Polytechnique. All rights reserved.
//

#import <Cocoa/Cocoa.h>

#import "SingleValueData.h"

@class DatabaseController;

@interface EditFieldSingleValue : NSObject
{
	IBOutlet NSWindow* window;
	
	IBOutlet DatabaseController* mainController;
	
	IBOutlet NSTextField* oldValue;
	IBOutlet NSTextField* newValue;
	
	SingleValueData* data;
	NSString* field;
	
	BOOL isNewField;
}

@property (nonatomic) BOOL isNewField;
@property (retain,nonatomic) NSWindow* window;
@property (retain,nonatomic) NSString* field;

-(void)setData:(SingleValueData*)data field:(NSString*)field;
-(void)showWindow;

-(IBAction)confirm:(id)sender;
-(IBAction)cancel:(id)sender;

@end
