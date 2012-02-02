//
//  NSBitmapImageRep+Extras.m
//  HereFileFile
//
//  Created by Adam Wulf on 10/28/09.
//  Copyright 2009 Jotlet, LLC. All rights reserved.
//

#import "NSBitmapImageRep+Extras.h"


@implementation NSBitmapImageRep (Extras)

-(WtymPixel) pixelAt:(CGPoint)point{
	unsigned char * data = [self bitmapData];
	//	int pl = [self numberOfPlanes];
	int bpp = [self bitsPerPixel];
	int w = [self bytesPerRow]/(bpp/8);
	//	int h = [self bytesPerPlane]/w;
	WtymPixel pixel;
	
    long ao;
	long ro,go,bo;
	int y = point.y;
	int x = point.x;
	ro = ((y*w + x)*bpp);
	go = ((y*w + x)*bpp)+bpp/4;
	bo = ((y*w + x)*bpp)+bpp/4*2;
    ao = ((y*w + x)*bpp)+bpp/4*3;
	pixel.r = *((data+ro/8));
	pixel.g = *((data+go/8));
	pixel.b = *((data+bo/8));
	pixel.a = *((data+ao/8));
	//	free(data);
    return pixel;
}


@end
