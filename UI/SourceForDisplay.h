//
//  SourceForDisplay.h
//  PSC
//
//  Created by Quentin Fiard on 13/02/12.
//  Copyright (c) 2012 École Polytechnique. All rights reserved.
//

#import "GenericSource.h"

@interface SourceForDisplay : NSObject
{
	GenericSource* source;
	NSNumber* selected;
}

-(id)initWithSource:(GenericSource*)source;

@property (assign,nonatomic) NSString* name;
@property (readonly,nonatomic) NSString* type;
@property (assign,nonatomic) NSNumber* selected;
@property (readonly,nonatomic) GenericSource* source;

@end