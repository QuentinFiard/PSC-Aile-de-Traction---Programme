//
//  CellView.m
//  StrongRoom
//
//  Created by Quentin FIARD on 05/08/10.
//  Copyright 2010 Lycée Henri IV. All rights reserved.
//

#import "CellView.h"
#import "SidebarBadgeCell.h"


@implementation CellView

- (id)initWithFrame:(NSRect)frame cell:(SidebarBadgeCell*)aCell {
    self = [super initWithFrame:frame];
    if (self) {
        cell = aCell;
    }
    return self;
}

- (BOOL)isFlipped
{
	return YES;
}

- (void)drawRect:(NSRect)dirtyRect {
    [cell drawInteriorWithFrame:[self bounds] inView:self];
}

@end
