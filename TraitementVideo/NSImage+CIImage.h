//
//  NSImage+CIImage.h
//  Telekinesis
//
//  Created by alcor on 7/7/07.
//  Copyright 2007 Blacktree Inc. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import "NSBitmapImageRep+Extras.h"

@interface NSImage (CICreation)
+ (NSImage *)imageWithCIImage:(CIImage *)i fromRect:(CGRect)r;
+ (NSImage *)imageWithCIImage:(CIImage *)i;
- (NSImage *) crop:(CGRect) cropRect;
- (NSImage *) imageFromRect: (NSRect) rect; //alternative to crop
- (CIImage *)newCIImage;
-(CGImageRef)newCGImageRef;
+ (NSImage *)newImageWithCGImageRef:(CGImageRef)cgImage;
- (NSImage *)cropAlpha:(int)buffer;
@end

@interface NSBitmapImageRep (CICreation)
+ (NSBitmapImageRep *)imageRepWithCIImage:(CIImage *)i fromRect:(CGRect)r;
+ (NSBitmapImageRep *)imageRepWithCIImage:(CIImage *)i;
@end