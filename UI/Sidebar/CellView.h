//
//  CellView.h
//  StrongRoom
//
//  Created by Quentin FIARD on 05/08/10.
//  Copyright 2010 Lycée Henri IV. All rights reserved.
//

#import <Cocoa/Cocoa.h>

@class SidebarBadgeCell;

@interface CellView : NSView {
	SidebarBadgeCell* cell;
}

- (id)initWithFrame:(NSRect)frame cell:(SidebarBadgeCell*)aCell;

@end
