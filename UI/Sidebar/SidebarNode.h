//
//  TSSidebarNodeBase.h
//  Tahsis
//
//  Created by Matteo Bertozzi on 11/29/08.
//  Copyright 2008 Matteo Bertozzi. All rights reserved.
//

#import <Cocoa/Cocoa.h>


@interface SidebarNode : NSObject<NSCoding, NSCopying> {
	@private
		NSMutableArray *_children;
		NSUInteger _badgeCount;
		NSString *_title;
		NSString *_key;
		NSString *_nibName;
		NSString *_ownerClass;
		NSString *_viewControllerClassName;
		NSImage *_icon;
		BOOL _isLeaf;
		SEL _action;
		id _target;
}

@property (readonly) SEL action;
@property (readonly) id target;

@property (readwrite) NSUInteger badgeCount;
@property (readwrite,retain) NSImage *nodeIcon;
@property (readwrite,copy) NSString *nodeTitle;
@property (readwrite,copy) NSString *nodeKey;
@property (readwrite,copy) NSString *nibName;
@property (readwrite,copy) NSString *ownerClass;
@property (readwrite,copy) NSString *viewControllerClassName;

- (id)initWithKey:(NSString *)key;
- (id)initLeafWithKey:(NSString *)key;

- (void)setChildren:(NSArray*)newChildren;
- (NSMutableArray*)children;

- (BOOL)isLeaf;
- (void)setLeaf:(BOOL)flag;

- (BOOL)isDraggable;

- (NSComparisonResult)compare:(SidebarNode*)aNode;

- (NSArray*)mutableKeys;

- (NSDictionary*)dictionaryRepresentation;
- (id)initWithDictionary:(NSDictionary*)dictionary;

- (id)parentFromArray:(NSArray*)array;
- (void)removeObjectFromChildren:(id)obj;
- (NSArray*)descendants;
- (NSArray*)allChildLeafs;
- (NSArray*)groupChildren;
- (BOOL)isDescendantOfOrOneOfNodes:(NSArray*)nodes;
- (BOOL)isDescendantOfNodes:(NSArray*)nodes;
- (NSIndexPath*)indexPathInArray:(NSArray*)array;

- (void)setAction:(SEL)aSelector target:(id)target;
- (BOOL)hasAction;

@end
