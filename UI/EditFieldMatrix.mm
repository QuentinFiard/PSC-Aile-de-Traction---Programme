//
//  EditFieldMatrix.m
//  PSC
//
//  Created by Quentin Fiard on 14/02/12.
//  Copyright (c) 2012 École Polytechnique. All rights reserved.
//

#import "EditFieldMatrix.h"
#import "Database.h"
#import "DatabaseController.h"

@implementation EditFieldMatrix

@synthesize window,field,isNewField;

std::string toBase26Array = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

static NSString* toBase26(NSUInteger i)
{
	std::string res;
	bool first = true;
	while(first || i>0)
	{
		first = false;
		res = toBase26Array[i%26]+res;
		i = i/26;
	}
	return [NSString stringWithUTF8String:res.c_str()];
}

-(void)awakeFromNib
{
	NSRect frame = NSMakeRect(170,472,120,22);
	nbLignes = [[NSTextField alloc] initWithFrame:frame];
	[[window contentView] addSubview:nbLignes];
	
	frame = NSMakeRect(515,472,120,22);
	nbColonnes = [[NSTextField alloc] initWithFrame:frame];
	[[window contentView] addSubview:nbColonnes];
	
	[nbLignes setTarget:self];
	[nbLignes setAction:@selector(setNbLignes:)];
	[nbLignes setDelegate:self];
	[nbColonnes setTarget:self];
	[nbColonnes setAction:@selector(setNbColonnes:)];
	[nbColonnes setDelegate:self];
}

-(void)prepareColumns
{
	NSArray* columns = [NSArray arrayWithArray:[tableView tableColumns]];
	
	for(int i=0 ; i<columns.count ; i++)
	{
		[tableView removeTableColumn:[columns objectAtIndex:i]];
	}
	
	NSTableColumn* rowNumbers = [[NSTableColumn alloc] initWithIdentifier:@"RowNumbers"];
	[rowNumbers.headerCell setStringValue:@""];
	[rowNumbers setEditable:NO];
	[rowNumbers setWidth:35];
	[rowNumbers.dataCell setAlignment:NSCenterTextAlignment];
	[tableView addTableColumn:rowNumbers];
	[rowNumbers release];
	
	for(int i=0 ; i<m ; i++)
	{
		NSTableColumn* tmp = [[NSTableColumn alloc] initWithIdentifier:[[NSNumber numberWithInt:i] stringValue]];
		[tmp.headerCell setStringValue:toBase26(i)];
		[tableView addTableColumn:tmp];
		[tmp release];
	}
}

-(void)setData:(MatrixData*)aData field:(NSString*)aField
{
	data = aData;
	
	self.field = aField;
	
	n = data->nbLignes();
	m = data->nbColonnes();
	
	[nbLignes setIntValue:n];
	[nbColonnes setIntValue:m];
	
	[self prepareColumns];
	
	values.clear();
	
	for(std::size_t i=0 ; i<n ; i++)
	{
		for(std::size_t j=0 ; j<m ; j++)
		{
			values.push_back(data->valueAtIndexToString(i,j));
		}
	}
	
	[tableView reloadData];
}

-(void)showWindow
{
	[window makeKeyAndOrderFront:self];
}

-(IBAction)confirm:(id)sender
{
	data->setNbLignes(n);
	data->setNbColonnes(m);
	
	bool conversionCheck = true;
	
	for(std::size_t i=0 ; i<n && conversionCheck ; i++)
	{
		for(std::size_t j=0 ; j<m  && conversionCheck ; j++)
		{
			conversionCheck = data->setValueAtIndexWithString(i, j, values[m*i+j]);
		}
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

-(void)setNbLignes:(id)sender
{
	int newN = [nbLignes intValue];
	
	if(n != newN)
	{
		std::vector<std::string> valuesNew;
		
		for(int i=0 ; i<fmin(n,newN) ; i++)
		{
			for(int j=0 ; j<m ; j++)
			{
				valuesNew.push_back(values[m*i+j]);
			}
		}
		
		for(int i=fmin(n,newN) ; i<newN ; i++)
		{
			for(int j=0 ; j<m ; j++)
			{
				valuesNew.push_back("0");
			}
		}
		
		values = valuesNew;
		
		n = newN;
		
		[tableView reloadData];
	}
}

-(void)setNbColonnes:(id)sender
{
	int newM = [nbColonnes intValue];
	
	if(m != newM)
	{
		std::vector<std::string> valuesNew;
		
		for(int i=0 ; i<n ; i++)
		{
			for(int j=0 ; j<fmin(m,newM) ; j++)
			{
				valuesNew.push_back(values[m*i+j]);
			}
			for(int j=fmin(m,newM) ; j<newM ; j++)
			{
				valuesNew.push_back("0");
			}
		}
		
		values = valuesNew;
		
		m = newM;
		
		[self prepareColumns];
		
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
		return [NSString stringWithUTF8String:values[m*rowIndex+[[aTableColumn identifier] intValue]].c_str()];
	}
}

- (void)tableView:(NSTableView *)aTableView setObjectValue:(id)anObject forTableColumn:(NSTableColumn *)aTableColumn row:(NSInteger)rowIndex
{
	NSInteger column = [[aTableColumn identifier] integerValue];
	NSString* val;
	if([anObject isKindOfClass:[NSString class]])
	{
		val = anObject;
	}
	else if([anObject respondsToSelector:@selector(stringValue)])
	{
		val = [anObject stringValue];
	}
	values[m*rowIndex+column] = std::string([val UTF8String]);
}

-(void)dealloc
{
	self.window = nil;
	[super dealloc];
}

#pragma mark - TextField delegate

-(BOOL)control:(NSControl *)control textShouldEndEditing:(NSText *)fieldEditor
{
	if([control isEqual:nbLignes])
	{
		[self setNbLignes:nbLignes];
	}
	if([control isEqual:nbColonnes])
	{
		[self setNbColonnes:nbColonnes];
	}
	return true;
}


@end
