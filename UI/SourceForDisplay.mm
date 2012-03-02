//
//  SourceForDisplay.m
//  PSC
//
//  Created by Quentin Fiard on 13/02/12.
//  Copyright (c) 2012 École Polytechnique. All rights reserved.
//

#import "SourceForDisplay.h"
#import "DataTypes.h"

@implementation SourceForDisplay

@synthesize source,selected;

@dynamic name,type;

-(id)initWithSource:(GenericSource*)aSource
{
	source = aSource;
	selected = [[NSNumber alloc] initWithBool:NO];
	
	return self;
}

-(NSString*)name
{
	return [NSString stringWithUTF8String:source->name().c_str()];
}

-(void)setName:(NSString *)name
{
	std::string newName([name UTF8String]);
	source->setName(newName);
	source->save();
}

-(NSString*)type
{
	return [NSString stringWithUTF8String:typeToString(source->type()).c_str()];
}

-(void)dealloc
{
	delete source;
	
	[super dealloc];
}

@end
