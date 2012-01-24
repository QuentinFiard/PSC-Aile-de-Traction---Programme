//
//  NSArray+Extension.m
//  Tahsis
//
//  Created by Matteo Bertozzi on 11/30/08.
//  Copyright 2008 __MyCompanyName__. All rights reserved.
//

#import "NSArrayExt.h"


@implementation NSArray (TSExtensions)

- (BOOL)containsObjectIdenticalTo:(id)obj
{ 
	return [self indexOfObjectIdenticalTo:obj] != NSNotFound; 
}

- (BOOL)containsAnyObjectsIdenticalTo:(NSArray*)objects
{
	id obj;
	for (obj in objects)
	{
		if ([self containsObjectIdenticalTo:obj])
			return YES;
	}
	return NO;
}

- (NSIndexSet*)indexesOfObjects:(NSArray*)objects
{
	NSMutableIndexSet *indexSet = [NSMutableIndexSet indexSet];
	id obj = nil;
	NSInteger index;
	for (obj in objects)
	{
		index = [self indexOfObject:obj];
		if (index != NSNotFound)
			[indexSet addIndex:index];
	}
	return indexSet;
}

@end
