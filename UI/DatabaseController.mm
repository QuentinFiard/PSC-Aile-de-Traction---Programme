//
//  DatabaseController.m
//  PSC
//
//  Created by Quentin Fiard on 14/02/12.
//  Copyright (c) 2012 École Polytechnique. All rights reserved.
//

#import "DatabaseController.h"

#import "Database.h"
#import "Record.h"
#import "GenericSource.h"

#import "SourceForDisplay.h"
#import "RecordForDisplay.h"
#import "ConfigurationForDisplay.h"

#import "EditFieldSingleValue.h"
#import "EditFieldVector.h"
#import "EditFieldMatrix.h"

#import "SingleValueData.h"
#import "VectorData.h"
#import "MatrixData.h"

@implementation DatabaseController

- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil
{
    self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil];
    if (self) {
        recordsArray = [[NSMutableArray alloc] init];
		sourcesArray = [[NSMutableArray alloc] init];
		configurationArray = [[NSMutableArray alloc] init];
		nbOfSelectedSources = 0;
    }
    
    return self;
}

-(void)awakeFromNib
{
	[self prepareRecordsArray];
	[self prepareConfigurationArray];
	
	[configurationTable setDoubleAction:@selector(doubleClickOnConfig)];
	[configurationTable setTarget:self];
}

-(void)editConfiguration:(ConfigurationForDisplay*)config
{
	switch(typeOfData(config.type))
	{
		case TYPE_SINGLEVALUEDATA:
			[editValue setData:reinterpret_cast< SingleValueData* >(Database::getConfigurationField(std::string([config.name UTF8String]))) field:config.name];
			editValue.isNewField= NO;
			[editValue showWindow];
			break;
			
		case TYPE_VECTORDATA:
			[editVector setData:reinterpret_cast< VectorData* >(Database::getConfigurationField(std::string([config.name UTF8String]))) field:config.name];
			editVector.isNewField= NO;
			[editVector showWindow];
			break;
			
		case TYPE_MATRIXDATA:
			[editMatrix setData:reinterpret_cast< MatrixData* >(Database::getConfigurationField(std::string([config.name UTF8String]))) field:config.name];
			editMatrix.isNewField= NO;
			[editMatrix showWindow];
			break;
		
		default:
			break;
	}
}

-(void)didAddField:(NSString*)field type:(DataType)type atRow:(int)row
{
	[configurationTable beginUpdates];
	
	ConfigurationForDisplay* config = [[ConfigurationForDisplay alloc] initWithName:field type:type];
	
	[configurationArray insertObject:config atIndex:row];
	
	[config release];
	
	[configurationTable insertRowsAtIndexes:[NSIndexSet indexSetWithIndex:row] withAnimation:NSTableViewAnimationSlideDown];
	
	[configurationTable endUpdates];
}

-(void)editNewConfigurationField:(NSString*)name type:(DataType)type
{
	std::vector<uint8_t> empty;
	DatabaseData* emptyData = DatabaseData::emptyDataWithType(type);
	
	switch(typeOfData(type))
	{
		case TYPE_SINGLEVALUEDATA:
			[editValue setData:reinterpret_cast< SingleValueData* >(emptyData) field:name];
			editValue.isNewField= YES;
			[editValue showWindow];
			break;
			
		case TYPE_VECTORDATA:
			[editVector setData:reinterpret_cast< VectorData* >(emptyData) field:name];
			editVector.isNewField= YES;
			[editVector showWindow];
			break;
			
		case TYPE_MATRIXDATA:
			[editMatrix setData:reinterpret_cast< MatrixData* >(emptyData) field:name];
			editMatrix.isNewField= YES;
			[editMatrix showWindow];
			break;
			
		default:
			break;
	}
}

-(IBAction)addConfigurationField:(id)sender
{
	[createField showWindow];
}

-(IBAction)removeSelectedConfigurationField:(id)sender
{
	NSInteger row = [configurationTable selectedRow];
	ConfigurationForDisplay* config = [configurationArray objectAtIndex:row];
	
	[configurationTable beginUpdates];
	
	Database::removeConfigurationField(std::string([config.name UTF8String]));
	
	[configurationArray removeObjectAtIndex:row];
	
	[configurationTable removeRowsAtIndexes:[NSIndexSet indexSetWithIndex:row] withAnimation:NSTableViewAnimationSlideUp];
	
	[configurationTable endUpdates];
}

-(IBAction)editSelectedField:(id)sender
{
	NSInteger row = [configurationTable selectedRow];
	[self editConfiguration:[configurationArray objectAtIndex:row]];
}

-(IBAction)removeSelectedRecord:(id)sender
{
	NSInteger row = [recordsTable selectedRow];
	RecordForDisplay* recordDisp = [recordsArray objectAtIndex:row];
	
	[recordsTable beginUpdates];
	
	Database::removeRecord(recordDisp.record);
	
	[recordsArray removeObjectAtIndex:row];
	
	[recordsTable removeRowsAtIndexes:[NSIndexSet indexSetWithIndex:row] withAnimation:NSTableViewAnimationSlideUp];
	
	[recordsTable endUpdates];
}

-(IBAction)removeSelectedSource:(id)sender
{
	NSInteger row = [sourcesTable selectedRow];
	SourceForDisplay* sourceDisp = [sourcesArray objectAtIndex:row];
	
	[sourcesTable beginUpdates];
	
	Database::removeSource(sourceDisp.source);
	
	[sourcesArray removeObjectAtIndex:row];
	
	[sourcesTable removeRowsAtIndexes:[NSIndexSet indexSetWithIndex:row] withAnimation:NSTableViewAnimationSlideUp];
	
	[sourcesTable endUpdates];
}

-(NSString*)CSVStringForSelectedSources
{
	NSMutableString* CSVFile = [[NSMutableString alloc] init];
	[CSVFile appendString:@"t (s);"];
	for(int i=0 ; i<sourcesArray.count ; i++)
	{
		SourceForDisplay* source = [sourcesArray objectAtIndex:i];
		
		if([source.selected boolValue])
		{
			NSString* string = [[NSString alloc] initWithUTF8String:source.source->toCSVString().c_str()];
			[CSVFile appendString:string];
			[string release];
		}
	}
	
	NSString* res = [NSString stringWithString:CSVFile];
	
	[CSVFile release];
	
	return res;
}

-(IBAction)exportSelectedSourcesToExcel:(id)sender
{
	NSWindow*       window = [[self view] window];
	
	// Set the default name for the file and show the panel.
	NSSavePanel*    panel = [NSSavePanel savePanel];
	[panel setNameFieldStringValue:@"donnees.csv"];
	[panel beginSheetModalForWindow:window completionHandler:^(NSInteger result){
        if (result == NSFileHandlingPanelOKButton)
        {
            NSURL*  theFile = [panel URL];
			NSString* extension = [theFile pathExtension];
			if(![extension isEqualToString:@"csv"])
			{
				theFile = [theFile URLByAppendingPathExtension:@"csv"];
			}
			
			[[self CSVStringForSelectedSources] writeToURL:theFile atomically:NO encoding:NSUTF8StringEncoding error:nil];
        }
    }];
}

-(void)prepareConfigurationArray
{
	[configurationArray removeAllObjects];
	
	std::vector< std::pair<std::string,DataType> > fields(Database::getAllConfigurationFields());
	
	for(int i=0 ; i<fields.size() ; i++)
	{
		std::pair<std::string,DataType> tmp(fields[i]);
		NSString* name = [NSString stringWithUTF8String:tmp.first.c_str()];
		
		ConfigurationForDisplay* config = [[ConfigurationForDisplay alloc] initWithName:name type:tmp.second];
		[configurationArray addObject:config];
		[config release];
	}
	
	[configurationTable reloadData];
}

-(void)prepareRecordsArray
{
	[recordsArray removeAllObjects];
	
	[sourceView setHidden:YES];
	
	[recordsTable deselectAll:self];
	[sourcesArray removeAllObjects];
	
	std::vector<Record*> records(Database::getAllRecords());
	
	for(int i=0 ; i<records.size() ; i++)
	{
		RecordForDisplay* record = [[RecordForDisplay alloc] initWithRecord:records[i]];
		[recordsArray addObject:record];
		[record release];
	}
	
	[recordsTable reloadData];
}



-(void)doubleClickOnConfig
{
	NSUInteger row = [configurationTable clickedRow];
	if(row != -1)
	{
		[self editConfiguration:[configurationArray objectAtIndex:row]];
	}
}

#pragma mark - Datasource

-(NSInteger)numberOfRowsInTableView:(NSTableView *)aTableView
{
	if([aTableView isEqual:sourcesTable])
	{
		return sourcesArray.count;
	}
	else if([aTableView isEqual:recordsTable])
	{
		return recordsArray.count;
	}
	else if([aTableView isEqual:configurationTable])
	{
		return configurationArray.count;
	}
	return 0;
}

-(id)tableView:(NSTableView *)aTableView objectValueForTableColumn:(NSTableColumn *)aTableColumn row:(NSInteger)rowIndex
{	
	if([aTableView isEqual:sourcesTable])
	{
		SourceForDisplay* source = [sourcesArray objectAtIndex:rowIndex];
		
		if([[aTableColumn identifier] isEqualToString:@"name"])
		{
			return source.name;
		}
		else if([[aTableColumn identifier] isEqualToString:@"type"])
		{
			return source.type;
		}
		else
		{
			return source.selected;
		}
	}
	else if([aTableView isEqual:recordsTable])
	{
		RecordForDisplay* record = [recordsArray objectAtIndex:rowIndex];
		
		if([[aTableColumn identifier] isEqualToString:@"date"])
		{
			return record.date;
		}
		else
		{
			return record.tag;
		}
	}
	else if([aTableView isEqual:configurationTable])
	{
		ConfigurationForDisplay* config = [configurationArray objectAtIndex:rowIndex];
		
		if([[aTableColumn identifier] isEqualToString:@"name"])
		{
			return config.name;
		}
		else
		{
			return config.stringType;
		}
	}
	
	return nil;
}

-(void)tableView:(NSTableView *)aTableView setObjectValue:(id)anObject forTableColumn:(NSTableColumn *)aTableColumn row:(NSInteger)rowIndex
{
	if([aTableView isEqual:sourcesTable])
	{
		SourceForDisplay* source = [sourcesArray objectAtIndex:rowIndex];
		
		if([[aTableColumn identifier] isEqualToString:@"selection"])
		{
			if(![source.selected boolValue] && [anObject boolValue])
			{
				nbOfSelectedSources++;
			}
			else if([source.selected boolValue] && ![anObject boolValue])
			{
				nbOfSelectedSources--;
			}
			
			source.selected = anObject;
			
			if(nbOfSelectedSources>0)
			{
				[exportButton setEnabled:YES];
			}
			else
			{
				[exportButton setEnabled:NO];
			}
			
		}
		else if([[aTableColumn identifier] isEqualToString:@"name"])
		{
			source.name = anObject;
		}
	}
	else if([aTableView isEqual:recordsTable])
	{
		if([[aTableColumn identifier] isEqualToString:@"tag"])
		{
			RecordForDisplay* record = [recordsArray objectAtIndex:rowIndex];
			
			record.tag = anObject;
		}
	}
	else if([aTableView isEqual:configurationTable])
	{
		if([[aTableColumn identifier] isEqualToString:@"name"])
		{
			ConfigurationForDisplay* config = [configurationArray objectAtIndex:rowIndex];
			
			config.name = anObject;
			
			int newRow = Database::indexOfField(std::string([config.name UTF8String]));
			
			[configurationTable beginUpdates];
			
			[config retain];
			
			[configurationArray removeObjectAtIndex:rowIndex];
			
			[configurationArray insertObject:config atIndex:newRow];
			
			[config release];
			
			[configurationTable moveRowAtIndex:rowIndex toIndex:newRow];
			
			[configurationTable endUpdates];
		}
	}
}

#pragma mark - Table View Delegate

-(void)tableViewSelectionDidChange:(NSNotification *)aNotification
{
	NSTableView* tableView = [aNotification object];
	
	if([tableView isEqual:recordsTable])
	{
		[sourcesArray removeAllObjects];
		
		NSInteger recordRow = [recordsTable selectedRow];
		
		if(recordRow==-1)
		{
			[sourceView setHidden:YES];
			[removeRecordButton setEnabled:NO];
		}
		else
		{
			[sourceView setHidden:NO];
			
			RecordForDisplay* selectedRecord = [recordsArray objectAtIndex:recordRow];
			
			std::vector<GenericSource*> sources(Database::getAllSourcesForRecord(selectedRecord.record));
			
			for(int i=0 ; i<sources.size() ; i++)
			{
				SourceForDisplay* source = [[SourceForDisplay alloc] initWithSource:sources[i]];
				[sourcesArray addObject:source];
				[source release];
			}
			
			[removeRecordButton setEnabled:YES];
		}
		
		[sourcesTable reloadData];
	}
	else if([tableView isEqual:configurationTable])
	{
		NSInteger fieldRow = [configurationTable selectedRow];
		
		if(fieldRow == -1)
		{
			[editConfigurationButton setEnabled:NO];
			[removeConfigurationButton setEnabled:NO];
		}
		else
		{
			[editConfigurationButton setEnabled:YES];
			[removeConfigurationButton setEnabled:YES];
		}
	}
	else if([tableView isEqual:sourcesTable])
	{
		NSInteger fieldRow = [sourcesTable selectedRow];
		
		if(fieldRow == -1)
		{
			[removeSourceButton setEnabled:NO];
		}
		else
		{
			[removeSourceButton setEnabled:YES];
		}
	}
}

@end
