//
//  DatabaseController.h
//  PSC
//
//  Created by Quentin Fiard on 14/02/12.
//  Copyright (c) 2012 École Polytechnique. All rights reserved.
//

#import <Cocoa/Cocoa.h>

#import <vector>

#import "EditFieldMatrix.h"
#import "EditFieldVector.h"
#import "EditFieldSingleValue.h"
#import "CreateFieldController.h"

@class ConfigurationForDisplay;

@interface DatabaseController : NSViewController <NSTableViewDelegate,NSTableViewDataSource>
{
	IBOutlet NSTableView* configurationTable;
	
	IBOutlet NSTableView* recordsTable;
	IBOutlet NSTableView* sourcesTable;
	
	NSMutableArray* recordsArray;
	NSMutableArray* sourcesArray;
	int nbOfSelectedSources;
	
	NSMutableArray* configurationArray;
	
	IBOutlet NSTableColumn* configurationName;
	IBOutlet NSTableColumn* configurationType;
	
	IBOutlet NSTableColumn* recordsName;
	IBOutlet NSTableColumn* recordsTag;
	
	IBOutlet NSTableColumn* sourcesSelection;
	IBOutlet NSTableColumn* sourcesName;
	IBOutlet NSTableColumn* sourcesType;
	
	IBOutlet NSButton* removeConfigurationButton;
	IBOutlet NSButton* removeRecordButton;
	IBOutlet NSButton* removeSourceButton;
	IBOutlet NSButton* exportButton;
	IBOutlet NSButton* editConfigurationButton;
	
	IBOutlet NSView* sourceView;
	
	IBOutlet CreateFieldController* createField;
	IBOutlet EditFieldSingleValue* editValue;
	IBOutlet EditFieldVector* editVector;
	IBOutlet EditFieldMatrix* editMatrix;
}

-(void)prepareConfigurationArray;
-(void)prepareRecordsArray;

-(void)editConfigurationField:(NSString*)name;
-(void)editNewConfigurationField:(NSString*)name type:(DataType)type;

-(IBAction)addConfigurationField:(id)sender;
-(IBAction)removeSelectedConfigurationField:(id)sender;
-(IBAction)editSelectedField:(id)sender;

-(IBAction)removeSelectedRecord:(id)sender;

-(IBAction)removeSelectedSource:(id)sender;
-(IBAction)exportSelectedSourcesToExcel:(id)sender;

-(void)didAddField:(NSString*)field type:(DataType)type atRow:(int)row;

-(void)doubleClickOnConfig;

@end
