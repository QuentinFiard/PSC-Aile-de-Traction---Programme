//
//  AppDelegate.m
//  PSC
//
//  Created by Quentin Fiard on 22/01/12.
//  Copyright (c) 2012 École Polytechnique. All rights reserved.
//

#import "AppDelegate.h"
#import "UIController.h"

static id mAppDelegate;

@implementation AppDelegate

@synthesize window;

@synthesize mTimer;
@dynamic mLastFrameTime;
@dynamic mStartTime;

-(id)init
{
	int test = 0;
	test += 1;
	return self;
}

- (void)applicationDidFinishLaunching:(NSNotification *)aNotification
{
	int test = 0;
	test += 1;
	// Insert code here to initialize your application
}


- (double)mLastFrameTime
{
    return mLastFrameTime;
}

- (void)setLastFrameTime:(double)frameInterval
{
    // Frame interval defines how many display frames must pass between each time the
    // display link fires. The display link will only fire 30 times a second when the
    // frame internal is two on a display that refreshes 60 times a second. The default
    // frame interval setting of one will fire 60 times a second when the display refreshes
    // at 60 times a second. A frame interval setting of less than one results in undefined
    // behavior.
    if (frameInterval >= 1)
    {
        mLastFrameTime = frameInterval;
    }
}

-(void)awakeFromNib
{
	//[self go];
}

- (void)go {
    
	mLastFrameTime = 1;
    mStartTime = 0;
    mTimer = nil;
	
    NSAutoreleasePool * pool = [[NSAutoreleasePool alloc] init];
    mLastFrameTime = 1;
    mStartTime = 0;
    mTimer = nil;
    
    try {
        demo.startDemo();
        
        Ogre::Root::getSingleton().getRenderSystem()->_initRenderTargets();
        
        // Clear event times
		Ogre::Root::getSingleton().clearEventTimes();
    } catch( Ogre::Exception& e ) {
        std::cerr << "An exception has occurred: " <<
        e.getFullDescription().c_str() << std::endl;
    }
    
    mTimer = [NSTimer scheduledTimerWithTimeInterval:(NSTimeInterval)(1.0f / 60.0f) * mLastFrameTime
                                              target:self
                                            selector:@selector(renderOneFrame:)
                                            userInfo:nil
                                             repeats:YES];
    [pool release];
}

- (void)renderOneFrame:(id)sender
{
    if(!OgreFramework::getSingletonPtr()->isOgreToBeShutDown() &&
       Ogre::Root::getSingletonPtr() && Ogre::Root::getSingleton().isInitialised())
    {
		if(OgreFramework::getSingletonPtr()->m_pRenderWnd->isActive())
		{
			mStartTime = OgreFramework::getSingletonPtr()->m_pTimer->getMillisecondsCPU();
            
			OgreFramework::getSingletonPtr()->m_pKeyboard->capture();
			OgreFramework::getSingletonPtr()->m_pMouse->capture();
            
			OgreFramework::getSingletonPtr()->updateOgre(mLastFrameTime);
			OgreFramework::getSingletonPtr()->m_pRoot->renderOneFrame();
			
			SDL_GL_SwapBuffers();
            
			mLastFrameTime = OgreFramework::getSingletonPtr()->m_pTimer->getMillisecondsCPU() - mStartTime;
		}
    }
    else
    {
        [mTimer invalidate];
        mTimer = nil;
        [NSApp performSelector:@selector(terminate:) withObject:nil afterDelay:0.0];
    }
}

- (void)dealloc {
    if(mTimer)
    {
        [mTimer invalidate];
        mTimer = nil;
    }
    
    [super dealloc];
}

#pragma mark - NSWindowDelegate methods

- (void)windowWillClose:(NSNotification *)notification
{
	 [NSApp performSelector:@selector(terminate:) withObject:nil afterDelay:0.0];
}



@end