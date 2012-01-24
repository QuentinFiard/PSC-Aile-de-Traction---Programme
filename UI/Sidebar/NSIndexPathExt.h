//
//  NSIndexPathTSExt.h
//  Tahsis
//
//  Created by Matteo Bertozzi on 11/30/08.
//  Copyright 2008 Matteo Bertozzi. All rights reserved.
//

#import <Cocoa/Cocoa.h>


@interface NSIndexPath (TSExtension)
- (BOOL)isSubPathOf:(NSIndexPath *)path;
@end
