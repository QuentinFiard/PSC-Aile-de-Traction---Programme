//
//  EditFieldVector.h
//  PSC
//
//  Created by Quentin Fiard on 14/02/12.
//  Copyright (c) 2012 École Polytechnique. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import "VectorData.h"

#import <vector>
#import <string>

@class DatabaseController;

@interface EditFieldVector : NSObject <NSTableViewDelegate,NSTableViewDataSource,NSTextFieldDelegate>
{
	IBOutlet NSTableView* tableView;
	
	IBOutlet NSWindow* window;
	
	IBOutlet DatabaseController* mainController;
	
	IBOutlet NSTextField* dimension;
	
	VectorData* data;
	NSString* field;
	
	std::vector<std::string> values;
	
	std::size_t n;
	
	BOOL isNewField;
}

@property (nonatomic) BOOL isNewField;	
@property (retain,nonatomic) NSWindow* window;
@property (retain,nonatomic) NSString* field;

-(void)setData:(VectorData*)data field:(NSString*)field;
-(void)showWindow;

-(IBAction)confirm:(id)sender;
-(IBAction)cancel:(id)sender;

-(IBAction)setDimension:(id)sender;

@end
