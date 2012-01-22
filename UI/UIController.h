//
//  UIController.h
//  PSC
//
//  Created by Quentin Fiard on 22/01/12.
//  Copyright (c) 2012 École Polytechnique. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import <Ogre/OSX/OgreOSXCocoaView.h>
#include "AppDelegate.h"

@interface UIController : NSObject
{
	IBOutlet OgreView* ogreView;
	IBOutlet NSWindow* window;
	IBOutlet AppDelegate* delegate;
}

@property (retain,nonatomic) IBOutlet OgreView* ogreView;
@property (assign,nonatomic) IBOutlet NSWindow* window;

@property (retain,nonatomic) IBOutlet AppDelegate* delegate;

@end
