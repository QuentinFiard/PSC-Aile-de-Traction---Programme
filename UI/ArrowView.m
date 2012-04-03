//
//  ArrowView.m
//  PSC
//
//  Created by Quentin Fiard on 23/03/12.
//  Copyright (c) 2012 École Polytechnique. All rights reserved.
//

#import "ArrowView.h"

#import <math.h>

@implementation ArrowView

- (id)initWithFrame:(NSRect)frame
{
    self = [super initWithFrame:frame];
    if (self) {
        [self setImage:[NSImage imageNamed:@"Fleche.png"]];
    }
    
    return self;
}

-(void)awakeFromNib
{
	
}

-(void)setAngle:(double)angle
{
	[self setBoundsRotation:angle*180/M_PI];
	
	[self setNeedsDisplay:YES];
}

- (void)drawRect:(NSRect)dirtyRect
{
    [super drawRect:dirtyRect];
}

@end
