//
//  ConfigurationForDisplay.m
//  PSC
//
//  Created by Quentin Fiard on 15/02/12.
//  Copyright (c) 2012 École Polytechnique. All rights reserved.
//

#import "ConfigurationForDisplay.h"

#import "Database.h"

@implementation ConfigurationForDisplay

@synthesize name,type;
@dynamic stringType;

-(id)initWithName:(NSString*)aName type:(DataType)aType
{
	name = [aName retain];
	type = aType;
	
	return self;
}

-(NSString*)stringType
{
	return [NSString stringWithUTF8String:typeToString(type).c_str()];
}

-(void)setName:(NSString *)newName
{	
	std::string newNameStr = Database::changeConfigurationFieldName(std::string([name UTF8String]),std::string([newName UTF8String]));
	
	[name release];
	name = [[NSString alloc] initWithUTF8String:newNameStr.c_str()];
}

@end
