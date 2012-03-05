//
//  ConfigurationForDisplay.h
//  PSC
//
//  Created by Quentin Fiard on 15/02/12.
//  Copyright (c) 2012 École Polytechnique. All rights reserved.
//

#import <Foundation/Foundation.h>

#import "DataTypes.h"

@interface ConfigurationForDisplay : NSObject
{
	NSString* name;
	DataType type;
}

-(id)initWithName:(NSString*)name type:(DataType)type;

@property (retain,nonatomic) NSString* name;
@property (readonly,nonatomic) DataType type;
@property (readonly,nonatomic) NSString* stringType;

@end
