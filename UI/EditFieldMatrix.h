//
//  EditFieldMatrix.h
//  PSC
//
//  Created by Quentin Fiard on 14/02/12.
//  Copyright (c) 2012 École Polytechnique. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import "MatrixData.h"

#import <vector>
#import <string>

@class DatabaseController;

@interface EditFieldMatrix : NSObject <NSTableViewDelegate,NSTableViewDataSource,NSTextFieldDelegate>
{
	IBOutlet NSTableView* tableView;
	
	IBOutlet NSWindow* window;
	
	IBOutlet DatabaseController* mainController;
	
	NSTextField* nbLignes;
	NSTextField* nbColonnes;
	
	MatrixData* data;
	NSString* field;
	
	std::vector<std::string> values;
	
	std::size_t n;
	std::size_t m;
	
	BOOL isNewField;
}

@property (nonatomic) BOOL isNewField;	
@property (retain,nonatomic) NSWindow* window;
@property (retain,nonatomic) NSString* field;

-(void)setData:(MatrixData*)data field:(NSString*)field;
-(void)showWindow;

-(IBAction)confirm:(id)sender;
-(IBAction)cancel:(id)sender;

-(void)setNbLignes:(id)sender;
-(void)setNbColonnes:(id)sender;

@end
