//
//  NSIndexPathTSExt.m
//  Tahsis
//
//  Created by Matteo Bertozzi on 11/30/08.
//  Copyright 2008 Matteo Bertozzi. All rights reserved.
//

#import "NSIndexPathExt.h"


@implementation NSIndexPath (TSExtension)
- (BOOL)isSubPathOf:(NSIndexPath *)path {
	if ([path length] > [self length])
		return NO;
	
	NSUInteger i;
	for (i = 0; i < [path length]; ++i) {
		if ([path indexAtPosition:i] != [self indexAtPosition:i])
			return NO;
	}
	
	return YES;
}
@end
