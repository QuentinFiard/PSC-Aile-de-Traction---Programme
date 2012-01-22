#ifndef __AppDelegate_H__
#define __AppDelegate_H__

#include <Ogre/OgrePlatform.h>
#include <SDL/SDL.h>
#include "OgreDemoApp.h"

#if OGRE_PLATFORM != OGRE_PLATFORM_APPLE
#error This header is for use with Mac OS X only
#endif

#ifdef __OBJC__

#import <Cocoa/Cocoa.h>

// All this does is suppress some messages in the run log.  NSApplication does not
// implement buttonPressed and apps without a NIB have no target for the action.
@implementation NSApplication (_suppressUnimplementedActionWarning)
- (void) buttonPressed:(id)sender
{
    /* Do nothing */
}
@end

#if defined(MAC_OS_X_VERSION_10_6) && MAC_OS_X_VERSION_MAX_ALLOWED >= MAC_OS_X_VERSION_10_6
@interface AppDelegate : NSObject <NSApplicationDelegate>
#else
@interface AppDelegate : NSObject
#endif
{
	IBOutlet NSWindow* window;
    NSTimer *mTimer;
    DemoApp demo;
    
    NSDate *mDate;
    double mLastFrameTime;
    double mStartTime;
}

- (void)go;
- (void)renderOneFrame:(id)sender;

@property (assign) IBOutlet NSWindow *window;

@property (retain) NSTimer *mTimer;
@property (nonatomic) double mLastFrameTime;
@property (nonatomic) double mStartTime;

@end

#endif

#endif
