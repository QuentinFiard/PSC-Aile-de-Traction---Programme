//
//  TSSidebarView.h
//  Tahsis
//
//  Created by Matteo Bertozzi on 11/29/08.
//  Copyright 2008 Matteo Bertozzi. All rights reserved.
//

#import <Cocoa/Cocoa.h>

@class UIController;

@interface Sidebar : NSTreeController <NSOutlineViewDataSource,NSOutlineViewDelegate> {
	IBOutlet NSTreeController *treeController;
	IBOutlet NSOutlineView *sidebarView;
	IBOutlet UIController* uicontroller;
	
	@private
		NSMutableArray *_contents;
		NSArray *_dragNodesArray;
		BOOL _isBuilding;
}

@property (retain,nonatomic) NSArray *dragNodesArray;


- (NSArray *)contents;

- (void)setHidden:(BOOL)hidden;

- (void)scrollToBegin;

- (void)clearSelection;

- (void)removeAllNodes;
- (void)removeNode:(NSString *)key;

- (void)setBadge:(NSString *)key count:(NSUInteger)badgeCount;

- (void)addFolder:(NSString *)key name:(NSString *)folderName;
- (void)addFolder:(NSString *)key name:(NSString *)folderName action:(SEL)aSelector target:(id)target;

- (void)addRootFolder:(NSString *)key name:(NSString *)folderName;
- (void)addRootFolder:(NSString *)key name:(NSString *)folderName action:(SEL)aSelector target:(id)target;

- (void)addChild:(NSString *)key url:(NSString *)url action:(SEL)aSelector target:(id)target;
- (void)addChild:(NSString *)key name:(NSString *)name icon:(NSImage *)icon nibName:(NSString*)nibName action:(SEL)aSelector target:(id)target;
- (void)populateOutlineContents:(id)inObject;

-(void)initializeDragAndDrop;

@end
