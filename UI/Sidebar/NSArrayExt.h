//
//  NSArray+Extension.h
//  Tahsis
//
//  Created by Matteo Bertozzi on 11/30/08.
//  Copyright 2008 __MyCompanyName__. All rights reserved.
//

#import <Cocoa/Cocoa.h>


@interface NSArray (TSExtensions)
- (BOOL)containsObjectIdenticalTo:(id)object;
- (BOOL)containsAnyObjectsIdenticalTo:(NSArray*)objects;
- (NSIndexSet*)indexesOfObjects:(NSArray*)objects;
@end
