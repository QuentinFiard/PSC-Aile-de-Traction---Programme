//
//  EditFieldSingleValue.m
//  PSC
//
//  Created by Quentin Fiard on 14/02/12.
//  Copyright (c) 2012 École Polytechnique. All rights reserved.
//

#import "EditFieldSingleValue.h"
#import <string>
#import "Database.h"
#import "DatabaseController.h"

@implementation EditFieldSingleValue

@synthesize window,field,isNewField;

-(void)setData:(SingleValueData*)aData field:(NSString*)aField
{
	if(data!=NULL)
	{
		delete data;
	}
	
	self.field = aField;
	
	data = aData;
	
	[oldValue setStringValue:[NSString stringWithUTF8String:data->valueToString().c_str()]];
	
	[newValue setStringValue:@""];
}

-(void)showWindow
{
	[window makeKeyAndOrderFront:self];
}

-(IBAction)confirm:(id)sender
{
	std::string newValueString([[newValue stringValue] UTF8String]);
	if(data->setValueFromString(newValueString))
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

-(void)dealloc
{
	self.window = nil;
	[super dealloc];
}

@end
