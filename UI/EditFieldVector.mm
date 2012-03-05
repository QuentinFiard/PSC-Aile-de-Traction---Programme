//
//  EditFieldVector.m
//  PSC
//
//  Created by Quentin Fiard on 14/02/12.
//  Copyright (c) 2012 École Polytechnique. All rights reserved.
//

#import "EditFieldVector.h"
#import "Database.h"
#import "DatabaseController.h"

@implementation EditFieldVector

@synthesize window,field,isNewField;

-(void)awakeFromNib
{
	NSRect frame = NSMakeRect(170,472,120,22);
	dimension = [[NSTextField alloc] initWithFrame:frame];
	[[window contentView] addSubview:dimension];
	
	[dimension setTarget:self];
	[dimension setAction:@selector(setDimension:)];
	[dimension setDelegate:self];
	
	NSArray* columns = [NSArray arrayWithArray:[tableView tableColumns]];
	
	for(int i=0 ; i<columns.count ; i++)
	{
		[tableView removeTableColumn:[columns objectAtIndex:i]];
	}
	
	NSTableColumn* rowNumbers = [[NSTableColumn alloc] initWithIdentifier:@"RowNumbers"];
	[rowNumbers setEditable:NO];
	[rowNumbers.headerCell setStringValue:@""];
	[rowNumbers setWidth:35];
	[rowNumbers.dataCell setAlignment:NSCenterTextAlignment];
	[tableView addTableColumn:rowNumbers];
	[rowNumbers release];
	
	NSTableColumn* tmp = [[NSTableColumn alloc] initWithIdentifier:@"0"];
	[tmp.headerCell setStringValue:@"Coordonnées"];
	[tableView addTableColumn:tmp];
	[tmp release];
}

-(void)setData:(VectorData*)aData field:(NSString*)aField
{
	data = aData;
	
	if(data->isResizable())
	{
		[dimension setEditable:YES];
	}
	else
	{
		[dimension setEditable:NO];
	}
	
	self.field = aField;
	
	n = data->dim();
	
	[dimension setIntValue:n];
	
	values.clear();
	
	for(std::size_t i=0 ; i<n ; i++)
	{
		values.push_back(data->valueAtIndexToString(i));
	}
	
	[tableView reloadData];
}

-(void)showWindow
{
	[window makeKeyAndOrderFront:self];
}

-(IBAction)confirm:(id)sender
{
	data->setDim(n);
	
	bool conversionCheck = true;
	
	for(std::size_t i=0 ; i<n && conversionCheck ; i++)
	{
		conversionCheck = data->setValueAtIndexWithString(i, values[i]);
	}
	
	if(conversionCheck)
	{
		std::string stringField([field UTF8String]);
		Database::setConfigurationField(stringField,data);
		
		if(isNewField)
		{
			[mainController didAddField:field type:data->type() atRow:Database::indexOfField(stringField)];
		}
	
		[window orderOut:self];
	}
}

-(IBAction)cancel:(id)sender
{
	[window orderOut:self];
}

-(IBAction)setDimension:(id)sender
{
	int newN = [dimension intValue];
	
	if(n != newN)
	{
		std::vector<std::string> valuesNew;
		
		for(int i=0 ; i<fmin(n,newN) ; i++)
		{
			valuesNew.push_back(values[i]);
		}
		
		for(int i=fmin(n,newN) ; i<newN ; i++)
		{
			valuesNew.push_back("0");
		}
		
		values = valuesNew;
		
		n = newN;
		
		[tableView reloadData];
	}
}

#pragma mark - Table View DataSource

- (NSInteger)numberOfRowsInTableView:(NSTableView *)aTableView
{
	return n;
}

- (id)tableView:(NSTableView *)aTableView objectValueForTableColumn:(NSTableColumn *)aTableColumn row:(NSInteger)rowIndex
{
	if([[aTableColumn identifier] isEqualToString:@"RowNumbers"])
	{
		return [NSNumber numberWithInteger:rowIndex+1];
	}
	else
	{
		return [NSString stringWithUTF8String:values[rowIndex].c_str()];
	}
}

- (void)tableView:(NSTableView *)aTableView setObjectValue:(id)anObject forTableColumn:(NSTableColumn *)aTableColumn row:(NSInteger)rowIndex
{
	NSString* val;
	if([anObject isKindOfClass:[NSString class]])
	{
		val = anObject;
	}
	else if([anObject respondsToSelector:@selector(stringValue)])
	{
		val = [anObject stringValue];
	}
	values[rowIndex] = std::string([val UTF8String]);
}

-(void)dealloc
{
	self.window = nil;
	[super dealloc];
}

#pragma mark - TextField delegate

-(BOOL)control:(NSControl *)control textShouldEndEditing:(NSText *)fieldEditor
{
	if([control isEqual:dimension])
	{
		[self setDimension:dimension];
	}
	return true;
}

@end
