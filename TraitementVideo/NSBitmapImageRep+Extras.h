//
//  NSBitmapImageRep+Extras.h
//  HereFileFile
//
//  Created by Adam Wulf on 10/28/09.
//  Copyright 2009 Jotlet, LLC. All rights reserved.
//


#import <Cocoa/Cocoa.h>
#import <QuartzCore/QuartzCore.h>
#import <Quartz/Quartz.h>


typedef struct{
	int r;
	int g;
	int b;
	int a;
}WtymPixel;



@interface NSBitmapImageRep (Extras)



-(WtymPixel) pixelAt:(CGPoint)point;

@end
