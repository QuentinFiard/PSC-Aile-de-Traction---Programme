//
//  TSSidebarView.m
//  Tahsis
//
//  Created by Matteo Bertozzi on 11/29/08.
//  Copyright 2008 Matteo Bertozzi. All rights reserved.
//

#import "Sidebar.h"
#import "SidebarNode.h"
#import "SidebarBadgeCell.h"
#import "NSIndexPathExt.h"

#import <objc/runtime.h>

#define kNodesPBoardType		@"myNodesPBoardType"

@interface Sidebar (Private)
- (void)selectParentFromSelection;
- (void)performAddChild:(SidebarNode *)node;
- (void)performAddFolder:(SidebarNode *)node;

- (SidebarNode *)nodeFromKey:(NSString *)key;
- (SidebarNode *)nodeFromKey:(NSString *)key rootNode:(SidebarNode *)root;

- (NSIndexPath *)indexPathFromKey:(NSString *)key;
- (NSIndexPath *)indexPathFromKey:(NSString *)key rootNode:(SidebarNode *)root path:(NSIndexPath *)rootPath;

- (BOOL)handleInternalDrops:(NSPasteboard *)pboard withIndexPath:(NSIndexPath *)indexPath;
@end

@implementation Sidebar

@synthesize dragNodesArray = _dragNodesArray;

- (id)init {
	if (self = [super init]) {
		_contents = [[NSMutableArray alloc] init];
	}

	return self;
}

- (void)dealloc {
	[_contents release];

	[super dealloc];
}

- (void)awakeFromNib {
	
	[self initializeDragAndDrop];
	
	// Scroll to the top in case the outline contents is very long
	[[[sidebarView enclosingScrollView] verticalScroller] setFloatValue:0.0];
	[[[sidebarView enclosingScrollView] contentView] scrollToPoint:NSMakePoint(0, 0)];
	[sidebarView setBackgroundColor:[NSColor colorWithCalibratedRed:0.72 green:0.74 blue:0.79 alpha:1.0]];
	
	// Make outline view appear with gradient selection, and behave like the Finder, iTunes, etc.
	[sidebarView setSelectionHighlightStyle:NSTableViewSelectionHighlightStyleSourceList];

	// drag and drop support
	[sidebarView registerForDraggedTypes:[NSArray arrayWithObjects:kNodesPBoardType, nil]];
	
	[self performSelectorOnMainThread:@selector(populateOutlineContents:) withObject:self waitUntilDone:YES];
}

- (void)setHidden:(BOOL)hidden {
	[sidebarView setHidden:hidden];
}

- (void)scrollToBegin {
	[sidebarView scrollRowToVisible:0];
}

- (void)clearSelection {
	NSArray *selection = [treeController selectionIndexPaths];
	[treeController removeSelectionIndexPaths:selection];
}

- (void)removeAllNodes {
	NSUInteger count = [_contents count];
	NSInteger i;
	
	for (i = count - 1; i >= 0; --i) {
		NSIndexPath *indexPath = [[NSIndexPath alloc] initWithIndex:i];
		[treeController removeObjectAtArrangedObjectIndexPath:indexPath];
		[indexPath release];
	}
}

- (void)removeNode:(NSString *)key {
	NSIndexPath *indexPath = [self indexPathFromKey:key];
	if (indexPath != nil)
		[treeController removeObjectAtArrangedObjectIndexPath:indexPath];
}

- (void)setBadge:(NSString *)key count:(NSUInteger)badgeCount {
	SidebarNode *node = [self nodeFromKey:key];
	[node setBadgeCount:badgeCount];
}

- (void)addFolder:(NSString *)key name:(NSString *)folderName {
	SidebarNode *node = [[SidebarNode alloc] initWithKey:key];
	[node setNodeTitle:folderName];

	[self performSelectorOnMainThread:@selector(performAddFolder:) withObject:node waitUntilDone:YES];
	
	[node release];
}

- (void)addFolder:(NSString *)key name:(NSString *)folderName action:(SEL)aSelector target:(id)target {
	SidebarNode *node = [[SidebarNode alloc] initWithKey:key];
	[node setAction:aSelector target:target];
	[node setNodeTitle:folderName];
	
	[self performSelectorOnMainThread:@selector(performAddFolder:) withObject:node waitUntilDone:YES];

	[node release];
}

- (void)addRootFolder:(NSString *)key name:(NSString *)folderName {
	[self selectParentFromSelection];
	[self addFolder:key name:folderName];
}

- (void)addRootFolder:(NSString *)key name:(NSString *)folderName action:(SEL)aSelector target:(id)target {
	[self selectParentFromSelection];
	[self addFolder:key name:folderName action:aSelector target:target];
}

- (void)addChild:(NSString *)key url:(NSString *)url action:(SEL)aSelector target:(id)target {
	SidebarNode *node = [[SidebarNode alloc] initLeafWithKey:key];
	[node setNodeTitle:[[[NSFileManager defaultManager] displayNameAtPath:url] retain]];
	[node setNodeIcon:[[NSWorkspace sharedWorkspace] iconForFile:url]];
	[node setAction:aSelector target:target];
	
	[self performSelectorOnMainThread:@selector(performAddChild:) withObject:node waitUntilDone:YES];
	[node release];
}

- (void)addChild:(NSString *)key name:(NSString *)name icon:(NSImage *)icon nibName:(NSString*)nibName ownerClass:(NSString*)aClass action:(SEL)aSelector target:(id)target {
	SidebarNode *node = [[SidebarNode alloc] initLeafWithKey:key];
	[node setAction:aSelector target:target];
	[node setNodeTitle:name];
	[node setNodeIcon:icon];
	[node setNibName:nibName];
	[node setOwnerClass:aClass];
	
	[self performSelectorOnMainThread:@selector(performAddChild:) withObject:node waitUntilDone:YES];
	[node release];
}

- (NSArray *)contents {
	return _contents;
}

- (void)selectParentFromSelection {
	NSArray *selectedNodes = [treeController selectedNodes];
	if ([selectedNodes count] > 0) {
		NSTreeNode *firstNode = [selectedNodes objectAtIndex:0];
		NSTreeNode *parentNode = [firstNode parentNode];

		if (parentNode != nil) {
			NSIndexPath *parentIndex = [parentNode indexPath];
			[treeController setSelectionIndexPath:parentIndex];
		} else {
			NSArray *selectionIndexPaths = [treeController selectionIndexPaths];
			[treeController removeSelectionIndexPaths:selectionIndexPaths];
		}
	}
}

- (void)performAddFolder:(SidebarNode *)node {
	NSIndexPath *indexPath = nil;

	if ([[treeController selectedObjects] count] == 0) {
		indexPath = [NSIndexPath indexPathWithIndex:[_contents count]];
	} else {
		indexPath = [treeController selectionIndexPath];
		id objZero = [[treeController selectedObjects] objectAtIndex:0];
		if ([objZero isLeaf])
			[self selectParentFromSelection];
		else
			indexPath = [indexPath indexPathByAddingIndex:[[objZero children] count]];
	}

	_isBuilding = YES;
	[treeController insertObject:node atArrangedObjectIndexPath:indexPath];
	[sidebarView scrollRowToVisible:0];
	_isBuilding = NO;
}

- (void)performAddChild:(SidebarNode *)node {
	NSArray *selectedObjects = [treeController selectedObjects];
	if ([selectedObjects count] > 0) {
		if ([[selectedObjects objectAtIndex:0] isLeaf])
			[self selectParentFromSelection];
	}

	NSIndexPath *indexPath = nil;
	if ([selectedObjects count] > 0) {
		indexPath = [treeController selectionIndexPath];
		id objZero = [selectedObjects objectAtIndex:0];
		indexPath = [indexPath indexPathByAddingIndex:[[objZero children] count]];
	} else {
		indexPath = [NSIndexPath indexPathWithIndex:[_contents count]];
	}

	_isBuilding = YES;
	[treeController insertObject:node atArrangedObjectIndexPath:indexPath];
	[self selectParentFromSelection];
	[sidebarView scrollRowToVisible:0];
	_isBuilding = NO;
}

- (SidebarNode *)nodeFromKey:(NSString *)key {
	for (SidebarNode *node in _contents) {
		if ([[node nodeKey] isEqualToString:key])
			return node;

		if (![node isLeaf]) {
			SidebarNode *keyNode = [self nodeFromKey:key rootNode:node];
			if (keyNode != nil) return(keyNode);
		}
	}
	return nil;
}

- (SidebarNode *)nodeFromKey:(NSString *)key rootNode:(SidebarNode *)root {
	for (SidebarNode *node in [root children]) {
		if ([[node nodeKey] isEqualToString:key])
			return node;

		if (![node isLeaf]) {
			SidebarNode *keyNode = [self nodeFromKey:key rootNode:node];
			if (keyNode != nil) return(keyNode);
		}
	}
	return nil;
}

- (NSIndexPath *)indexPathFromKey:(NSString *)key {
	NSUInteger count = [_contents count];
	NSUInteger i;
	for (i = 0; i < count; ++i) {
		NSIndexPath *indexPath = [NSIndexPath indexPathWithIndex:i];
		SidebarNode *node = [_contents objectAtIndex:i];
		NSLog(@"Node Key %@ %@", [node nodeKey], key);
		
		if ([[node nodeKey] isEqualToString:key])
			return indexPath;
		
		if (![node isLeaf]) {
			NSIndexPath *nodePath = [self indexPathFromKey:key rootNode:node path:indexPath];
			if (nodePath != nil) return nodePath;
		}
	}
	return nil;
}

- (NSIndexPath *)indexPathFromKey:(NSString *)key rootNode:(SidebarNode *)root path:(NSIndexPath *)rootPath {
	NSUInteger count = [[root children] count];
	NSUInteger i;
	for (i = 0; i < count; ++i) {
		NSIndexPath *indexPath = [rootPath indexPathByAddingIndex:i];
		
		SidebarNode *node = [[root children] objectAtIndex:i];
		NSLog(@"Node Key %@ %@", [node nodeKey], key);
		if ([[node nodeKey] isEqualToString:key])
			return indexPath;
			
		if (![node isLeaf]) {
			NSIndexPath *nodePath = [self indexPathFromKey:key rootNode:node path:indexPath];
			if (nodePath != nil) return nodePath;
		}
	}
	return nil;
}

/* OutlineView Delegates
 */

- (BOOL)outlineView:(NSOutlineView *)outlineView shouldSelectItem:(id)item {
	// Groups without icon is Folder so it cannot be selected
	return([[item representedObject] nodeIcon] != nil);
}

- (NSCell *)outlineView:(NSOutlineView *)outlineView
	dataCellForTableColumn:(NSTableColumn *)tableColumn
                      item:(id)item
{
	return [tableColumn dataCell];
}

- (BOOL)control:(NSControl *)control textShouldEndEditing:(NSText *)fieldEditor {
	return([[fieldEditor string] length] > 0);
}

- (BOOL)outlineView:(NSOutlineView *)outlineView
	shouldEditTableColumn:(NSTableColumn *)tableColumn
                     item:(id)item
{
	item = [item representedObject];
	if ([item nodeIcon] != nil)
		return NO;

	return YES;
}

- (void)outlineView:(NSOutlineView *)outlineView
    willDisplayCell:(NSCell*)cell
     forTableColumn:(NSTableColumn *)tableColumn
               item:(id)item
{
	if ([cell isKindOfClass:[SidebarBadgeCell class]]) {
		SidebarBadgeCell *badgeCell = (SidebarBadgeCell *) cell;
		SidebarNode *node = [item representedObject];
		[badgeCell setBadgeCount:[node badgeCount]];
		[badgeCell setIcon:[node nodeIcon]];
	}
}

-(BOOL)outlineView:(NSOutlineView*)outlineView isGroupItem:(id)item {
	return([[item representedObject] nodeIcon] == nil);
}

- (void)outlineViewSelectionDidChange:(NSNotification *)notification {
	if (_isBuilding) return;

	NSArray *selection = [treeController selectedObjects];
	if ([selection count] < 1) return;
	
	// Run Thread with selected Action
	SidebarNode *node = [selection objectAtIndex:0];
	if ([node hasAction])
		if(sel_isEqual([node action], @selector(changeView:)) && [[node target] isEqual:uicontroller])
		{
			[uicontroller performSelectorOnMainThread:[node action] withObject:node waitUntilDone:NO];
		}
		else
		{
			[NSThread detachNewThreadSelector:[node action] toTarget:[node target] withObject:node];
		}
}

#pragma mark - NSOutlineView drag and drop

-(void)initializeDragAndDrop
{
	//Initialising Drag and drop
	[sidebarView registerForDraggedTypes:[NSArray arrayWithObject:kNodesPBoardType]];
	
	NSLog(@"%@",[sidebarView registeredDraggedTypes]);
	
	[sidebarView setDraggingSourceOperationMask:NSDragOperationMove forLocal:YES];
}

- (BOOL)outlineView:(NSOutlineView *)outlineView
         writeItems:(NSArray *)items
       toPasteboard:(NSPasteboard *)pboard
{
	
	NSMutableArray* indexPaths = [[NSMutableArray alloc] initWithCapacity:items.count];
	for(int i=0 ; i<items.count ; i++)
	{
		[indexPaths addObject:[[items objectAtIndex:i] indexPath]];
	}
	
	NSData* data = [NSKeyedArchiver archivedDataWithRootObject:indexPaths];
	
	[pboard declareTypes:[NSArray arrayWithObject:kNodesPBoardType] owner:self];
	
	[pboard setData:data forType:kNodesPBoardType];

	// keep track of this nodes for drag feedback in "validateDrop"
	self.dragNodesArray = items;

	return YES;
}
/*
- (NSDragOperation)outlineView:(NSOutlineView *)outlineView
                  validateDrop:(id<NSDraggingInfo>)info
                  proposedItem:(id)item
            proposedChildIndex:(NSInteger)index
{
	if (item == nil)
		return NSDragOperationGeneric;

	if (![[item representedObject] isLeaf] && index >= 0)
		return NSDragOperationMove;

	return NSDragOperationNone;
}*/

BOOL isChild(NSTreeNode* toCheck,NSTreeNode* possibleNode)
{
	for(NSTreeNode* node in [possibleNode childNodes])
	{
		if([toCheck isEqual:node])
		{
			return YES;
		}
		else if(![node isLeaf] && isChild(toCheck,node))
		{
			return YES;
		}
	}
	return NO;
}

- (NSDragOperation)outlineView:(NSOutlineView *)outlineView 
				  validateDrop:  (id <NSDraggingInfo>)info 
				  proposedItem:(id)item 
			proposedChildIndex:(NSInteger)index
{
	// Lorsque la source du drop n'est pas l'application elle-même
	// la méthode draggingSource renvoie nil. 
	// On refuse ici l'opération.
	
	//A changer pour accepter le drag and drop du finder
	if(nil==[info draggingSource]){
		return NSDragOperationNone;
	}
	
	// Refus de faire d'un noeud le fils de l'un de ses descendants
	for(NSTreeNode* draggedItem in self.dragNodesArray){
		if([item isEqual:draggedItem] || isChild(item, draggedItem) || [[item indexPath] length]>1) return NSDragOperationNone;
	}
	
	// autrement on accepte l'opération
	return NSDragOperationGeneric;
}


//TO CHANGE !!! %%
- (BOOL)outlineView:(NSOutlineView*)ov acceptDrop:(id<NSDraggingInfo>)info item:(id)targetItem childIndex:(NSInteger)index
{
	// note that "targetItem" is a NSTreeNode proxy
	BOOL result = YES;
	
	// find the index path to insert our dropped object(s)
	NSIndexPath *indexPath;
	if (targetItem)
	{
		if(index == -1)
		{
			indexPath = [[targetItem indexPath] indexPathByAddingIndex:0];
		}
		else
		{
			// drop down inside the tree node:
			// feth the index path to insert our dropped node
			indexPath = [[targetItem indexPath] indexPathByAddingIndex:index];
		}
	}
	else
	{
		// drop at the top root level
		if (index == -1)	// drop area might be ambibuous (not at a particular location)
			indexPath = [NSIndexPath indexPathWithIndex:[_contents count]];		// drop at the end of the top level
		else
			indexPath = [NSIndexPath indexPathWithIndex:index]; // drop at a particular place at the top level
	}

	NSPasteboard *pboard = [info draggingPasteboard];	// get the pasteboard
	
	// user is doing an intra-app drag within the outline view
	if ([pboard availableTypeFromArray:[NSArray arrayWithObject:kNodesPBoardType]])
		return [self handleInternalDrops:pboard withIndexPath:indexPath];
	
	return result;
}

- (BOOL)handleInternalDrops:(NSPasteboard *)pboard withIndexPath:(NSIndexPath *)indexPath {
	// user is doing an intra app drag within the outline view:
	NSArray *newNodes = self.dragNodesArray;

	// move the items to their new place (we do this backwards, otherwise they will end up in reverse order)
	NSInteger i;
	for (i = ([newNodes count] - 1); i >=0; i--) {
		NSTreeNode *node = [newNodes objectAtIndex:i];
		if (![node isLeaf] && [indexPath isSubPathOf:[node indexPath]])
			return NO;

		[treeController moveNode:node toIndexPath:indexPath];
	}
	
	// keep the moved nodes selected
	NSMutableArray* indexPathList = [NSMutableArray array];
	for (i = 0; i < [newNodes count]; i++)
		[indexPathList addObject:[[newNodes objectAtIndex:i] indexPath]];

	[treeController setSelectionIndexPaths:indexPathList];
	
	[sidebarView reloadData];
	
	return YES;
}

- (NSUInteger)hitTestForEvent:(NSEvent *)event
                      inRect:(NSRect)cellFrame
                      ofView:(NSView *)controlView
{
	if ([controlView isKindOfClass:[NSOutlineView class]]) {
		NSOutlineView *outlineView = (NSOutlineView *) controlView;
		NSInteger selectedRow = [outlineView selectedRow];
		SidebarNode *node = [[outlineView itemAtRow:selectedRow] representedObject];
		if (![node isDraggable])
			return NSCellHitTrackableArea;
	}

	return NSCellHitContentArea;
}

#pragma mark - Filling Source View

- (void)populateOutlineContents:(id)inObject {
	NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
	
	NSBundle *mainBundle = [NSBundle mainBundle];
	
	NSString *path = [mainBundle pathForResource:@"sidebarContent" ofType:@"xml"];
	
	NSData *sidebarXMLContent = [NSData dataWithContentsOfFile:path];
	
	NSError *error = nil;
	
	NSXMLDocument *document = [[NSXMLDocument alloc] initWithData:sidebarXMLContent options:0 error:&error];
	
	if(error)
	{
		NSLog(@"Error while parsing XML file for sidebar content : %@",[error localizedDescription]);
	}
	
	NSArray* rootNodes = [document nodesForXPath:@"//folder" error:&error];
	
	[document release];
	
	if(error)
	{
		NSLog(@"Error while getting folders from XML sidebar content file : %@",[error localizedDescription]);
	}
	
	for(int i=0 ; i<[rootNodes count] ; i++)
	{
		NSXMLNode *folder = [rootNodes objectAtIndex:i];
		
		NSArray *tmp = [folder nodesForXPath:@"name" error:&error];
		
		NSXMLNode *tmpNode = [tmp objectAtIndex:0];
		
		[self addRootFolder:[NSString stringWithFormat:@"%d",i] name:[[tmpNode stringValue] uppercaseString]];
		
		NSArray* nodes = [folder nodesForXPath:@"node" error:&error];
		
		if(error)
		{
			NSLog(@"Error while getting nodes from XML sidebar content file : %@",[error localizedDescription]);
		}
		
		for(int j=0 ; j<[nodes count] ; j++)
		{
			NSXMLNode* node = [nodes objectAtIndex:j];
			
			tmp = [node nodesForXPath:@"name" error:&error];
			tmpNode = [tmp objectAtIndex:0];
			NSString* name = [tmpNode stringValue];
			
			tmp = [node nodesForXPath:@"imageName" error:&error];
			tmpNode = [tmp objectAtIndex:0];
			NSImage* image = [NSImage imageNamed:[tmpNode stringValue]];
			
			tmp = [node nodesForXPath:@"nibName" error:&error];
			tmpNode = [tmp objectAtIndex:0];
			NSString* nibName = [tmpNode stringValue];
			
			tmp = [node nodesForXPath:@"ownerClass" error:&error];
			tmpNode = [tmp objectAtIndex:0];
			NSString* ownerClass = [tmpNode stringValue];
			
			[self addChild:[NSString stringWithFormat:@"%d.%d",i,j]
					  name:name 
					  icon:image 
				   nibName:nibName 
				ownerClass:ownerClass 
					action:@selector(changeView:) 
					target:uicontroller];
		}
	}
	
	[self clearSelection];
	
	[pool release];
}

@end
