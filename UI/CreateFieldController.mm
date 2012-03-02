//
//  CreateFieldController.m
//  PSC
//
//  Created by Quentin Fiard on 14/02/12.
//  Copyright (c) 2012 École Polytechnique. All rights reserved.
//

#import "CreateFieldController.h"
#import "DatabaseController.h"
#import <string>
#import "Database.h"
#import "DataTypes.h"

static const int nbOfAllowedTypesForConfig = 9;
static const char* allowedNamesForConfigTypes[9] = {"double","float","int","int64","Vecteur","Vecteur 3D","Point","Quaternion","Matrice"};
static const DataType allowedTypesForConfigTypes[9] = {TYPE_DOUBLE,TYPE_FLOAT,TYPE_INT,TYPE_INT64,TYPE_VECTEUR,TYPE_VECTEUR3D,TYPE_POINT,TYPE_QUATERNION,TYPE_MATRICE};



static DataType stringToType(std::string s)
{
	int i = 0;
	while(i<nbOfAllowedTypesForConfig && std::string(allowedNamesForConfigTypes[i]) != s)
	{
		i++;
	}
	if(i==nbOfAllowedTypesForConfig)
	{
		return -1;
	}
	else
	{
		return allowedTypesForConfigTypes[i];
	}
}

@implementation CreateFieldController

@synthesize window,type;

-(void)awakeFromNib
{
	for(int i=0 ; i<nbOfAllowedTypesForConfig ; i++)
	{
		[typeButton addItemWithTitle:[NSString stringWithUTF8String:allowedNamesForConfigTypes[i]]];
	}
}

-(void)showWindow
{
	[window makeKeyAndOrderFront:self];
	[nameField becomeFirstResponder];
}

-(IBAction)confirm:(id)sender
{
	std::string name([[nameField stringValue] UTF8String]);
	std::string typeString([[typeButton titleOfSelectedItem] UTF8String]);
	
	[window orderOut:self];
	
	[mainController editNewConfigurationField:[nameField stringValue] type:stringToType(typeString)];
	
	[mainController prepareConfigurationArray];
}

-(IBAction)cancel:(id)sender
{
	[window orderOut:self];
}

@end
