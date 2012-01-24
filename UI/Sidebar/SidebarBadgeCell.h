//
//  TSBadgeCell.h
//  Tahsis
//
//  Created by Matteo Bertozzi on 11/29/08.
//  Copyright 2008 Matteo Bertozzi. All rights reserved.
//

#import <Cocoa/Cocoa.h>

@class FoldersView,CellView;

@interface SidebarBadgeCell : NSTextFieldCell {
	IBOutlet FoldersView* aFoldersView;
	CellView* essai;
	BOOL isDrawingAssociatedView;
	
	@private
		NSUInteger _badgeCount;
		NSImage *_icon;
}

@property (readwrite) NSUInteger badgeCount;
@property (readonly) NSImage *icon;

- (void)setIcon:(NSImage *)icon;

@end
