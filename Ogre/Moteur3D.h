//
//  Moteur3D.h
//  PSC
//
//  Created by Quentin Fiard on 22/01/12.
//  Copyright (c) 2012 École Polytechnique. All rights reserved.
//

#ifndef PSC_Moteur3D_h
#define PSC_Moteur3D_h

#import <Cocoa/Cocoa.h>
#include <Ogre/Ogre.h>

//For Cocoa View
#import"OgreOSXCocoaView.h"

#include <Ogre/OIS/OISEvents.h>
#include <Ogre/OIS/OISInputManager.h>
#include <Ogre/OIS/OISKeyboard.h>
#include <Ogre/OIS/OISMouse.h>
#include <Ogre/OIS/OISJoyStick.h>

#include <Ogre/SdkTrays.h>

@interface Moteur3D : NSObject
{
@private
	OgreView*				ogreView;
	Ogre::Root*             root;
	Ogre::RenderWindow*		renderWindow;
	Ogre::SceneManager*		sceneManager;
	Ogre::SceneNode*		sceneNode;
	Ogre::Camera*			camera;
	Ogre::Viewport*			viewport;
	Ogre::Log*              log;
	Ogre::Timer*			timer;
	
    Ogre::FrameEvent            frameEvent;
	int                         numScreenShots;
    
	bool                        shutDownOgre;
	
	Ogre::String            resourcePath;
	
	Ogre::SceneNode*			kiteNode;
	Ogre::Entity*				kiteEntity;
	
	double pitchSpeed,yawSpeed,rollSpeed;
	
#ifdef OGRE_STATIC_LIB
    Ogre::StaticPluginLoader    m_StaticPluginLoader;
#endif
}

@property (retain,nonatomic) OgreView* ogreView;

@property (nonatomic) double pitchSpeed;
@property (nonatomic) double yawSpeed;
@property (nonatomic) double rollSpeed;

-(void)initInView:(OgreView*)ogreView;

@end

#endif
