//
//  FoldersView.h
//  StrongRoom
//
//  Created by Quentin FIARD on 25/07/10.
//  Copyright 2010 Lycée Henri IV. All rights reserved.
//

#import <Cocoa/Cocoa.h>

@class CellView,SidebarBadgeCell;

@interface FoldersView : NSOutlineView {
	SidebarBadgeCell* currentCellunderMouse;
	NSInteger rowNumber;
	NSInteger draggedCellRowNumber;
	
	CellView* essai;
}

@property (assign,readwrite) SidebarBadgeCell* currentCellunderMouse;

@end
