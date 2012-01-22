//
//  Moteur3D.cpp
//  PSC
//
//  Created by Quentin Fiard on 22/01/12.
//  Copyright (c) 2012 École Polytechnique. All rights reserved.
//

#include "Moteur3D.h"

#include <Ogre/OSX/macUtils.h>

#include "Tests.h"

#include "Joystick.h"

#include <assert.h>

using namespace Ogre;

static bool ogreFrameworkPrepared = false;

static Moteur3D* moteur3D;

@implementation Moteur3D

@synthesize ogreView;
@synthesize yawSpeed,rollSpeed,pitchSpeed;

-(id)init
{
	shutDownOgre     = false;
	numScreenShots   = 0;
    
	root				= 0;
	sceneManager		= 0;
	renderWindow        = 0;
	camera				= 0;
	viewport			= 0;
	log					= 0;
	timer				= 0;
    
#if OGRE_PLATFORM == OGRE_PLATFORM_APPLE
    resourcePath = macBundlePath() + "/Contents/Resources/";
#elif defined(OGRE_IS_IOS)
    resourcePath = macBundlePath() + "/";
#else
    resourcePath = "";
#endif
	
    frameEvent			= Ogre::FrameEvent();
	
	return self;
}

+(void)prepareOgreFramework
{
	if(!ogreFrameworkPrepared)
	{
#ifdef OGRE_STATIC_LIB
    m_StaticPluginLoader.load();
#endif
	}
	ogreFrameworkPrepared = true;
}

-(void)prepareScene
{
	sceneManager = root->createSceneManager(ST_GENERIC, "SceneManager");
	sceneManager->setAmbientLight(Ogre::ColourValue(0.0f, 0.0f, 0.0f));
	
	camera = sceneManager->createCamera("Camera");
	camera->setPosition(Vector3(3,1,1));
	camera->lookAt(Vector3(0,0,0));
	camera->setNearClipDistance(0.1);
	
	camera->roll(Radian(M_PI_2));
    
	viewport = renderWindow->addViewport(camera);
	viewport->setBackgroundColour(ColourValue(0.8f, 0.7f, 0.6f, 1.0f));
    
	camera->setAspectRatio(Real(viewport->getActualWidth()) / Real(viewport->getActualHeight()));
	
	viewport->setCamera(camera);
					
	Ogre::String secName, typeName, archName;
	Ogre::ConfigFile cf;
    cf.load(resourcePath + "resources.cfg");
    
	Ogre::ConfigFile::SectionIterator seci = cf.getSectionIterator();
    while (seci.hasMoreElements())
    {
        secName = seci.peekNextKey();
		Ogre::ConfigFile::SettingsMultiMap *settings = seci.getNext();
        Ogre::ConfigFile::SettingsMultiMap::iterator i;
        for (i = settings->begin(); i != settings->end(); ++i)
        {
            typeName = i->first;
            archName = i->second;
#if OGRE_PLATFORM == OGRE_PLATFORM_APPLE || defined(OGRE_IS_IOS)
            // OS X does not set the working directory relative to the app,
            // In order to make things portable on OS X we need to provide
            // the loading with it's own bundle path location
            if (!Ogre::StringUtil::startsWith(archName, "/", false)) // only adjust relative dirs
                archName = Ogre::String(resourcePath + archName);
#endif
				Ogre::ResourceGroupManager::getSingleton().addResourceLocation(archName, typeName, secName);
				}
    }
	Ogre::TextureManager::getSingleton().setDefaultNumMipmaps(5);
	Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
    
	timer = OGRE_NEW Ogre::Timer();
	timer->reset();
    
	renderWindow->setActive(true);
}

-(void)setupDemoScene
{
	sceneManager->setSkyBox(true, "Examples/SpaceSkyBox");
    
	sceneManager->createLight("Light")->setPosition(75,75,75);
    
	kiteEntity = sceneManager->createEntity("Cube", "kite.mesh");
	kiteNode = sceneManager->getRootSceneNode()->createChildSceneNode("CubeNode");
	kiteNode->attachObject(kiteEntity);
}

-(void)startDemo
{    
	log->logMessage("Demo initialized!");
    
	[self setupDemoScene];
}

//|||||||||||||||||||||||||||||||||||||||||||||||

-(void)runDemo
{
	log->logMessage("Start main loop...");
	
	double timeSinceLastFrame = 0;
	double startTime = 0;
	
	bool m_bShutdown = false;
    
    renderWindow->resetStatistics();
    
	while(!m_bShutdown /*&& !OgreFramework::getSingletonPtr()->isOgreToBeShutDown()*/) 
	{
		if(renderWindow->isClosed())m_bShutdown = true;
        
		Ogre::WindowEventUtilities::messagePump();
		
		if(renderWindow->isActive())
		{
			startTime = timer->getMicrosecondsCPU();
            /*
			 #if !OGRE_IS_IOS
			 OgreFramework::getSingletonPtr()->m_pKeyboard->capture();
			 
			 OgreFramework::getSingletonPtr()->m_pJoyStick->capture();
			 #endif
			 OgreFramework::getSingletonPtr()->m_pMouse->capture();*/
			
			SDL_Event event;
			while(SDL_PollEvent(&event))
			{
				/*switch(event.type)
				 { 
				 case SDL_KEYDOWN:         
				 out<<"Oh! Key press\n";
				 break;
				 case SDL_MOUSEMOTION: 
				 out<<"Mouse Motion\n";
				 break;
				 case SDL_QUIT:
				 i=-1;
				 break;
				 default:
				 out<<"I don't know what this event is!\n";
				 }*/
				if(event.type==SDL_JOYAXISMOTION || event.type==SDL_JOYHATMOTION || event.type==SDL_JOYBUTTONUP || event.type==SDL_JOYBALLMOTION || event.type==SDL_JOYBUTTONDOWN)
				{
					Joystick::handler()->handleJoystickEvent(event,self);
				}
			}
			
			if(timeSinceLastFrame>0)
			{
				kiteNode->pitch(Ogre::Radian(pitchSpeed*timeSinceLastFrame));
				kiteNode->yaw(Ogre::Radian(yawSpeed*timeSinceLastFrame));
				kiteNode->roll(Ogre::Radian(rollSpeed*timeSinceLastFrame));
			}
            
			//OgreFramework::getSingletonPtr()->updateOgre(timeSinceLastFrame);
			root->renderOneFrame();
            
			timeSinceLastFrame = (timer->getMicrosecondsCPU() - startTime)/1000;
			
		}
		else
		{
			sleep(1);
		}
	}
    
#if !defined(OGRE_IS_IOS)
	log->logMessage("Main loop quit");
	log->logMessage("Shutdown OGRE...");
#endif
}


-(void)initInView:(OgreView*)anOgreView
{
	self.ogreView = anOgreView;
	
	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK);
	
	Joystick::handler()->prepareJoystick();
	
	testerCommunicationMoteur();
	
	new Ogre::LogManager();
	
	log = Ogre::LogManager::getSingleton().createLog("OgreLogfile.log", true, true, false);
	log->setDebugOutputEnabled(true);
    
    String pluginsPath;
    // only use plugins.cfg if not static
#ifndef OGRE_STATIC_LIB
    pluginsPath = resourcePath + "plugins.cfg";
#endif
	
	[Moteur3D prepareOgreFramework];
    
    root = new Ogre::Root(pluginsPath);//, Ogre::macBundlePath() + "/ogre.cfg");
	
	// set up the render system. Since this is running on Mac, our only option is OpenGL.
	root->setRenderSystem(root->getRenderSystemByName("OpenGL Rendering Subsystem"));
	
	// Initialise without an automatically created window
	root->initialise(false);
	
	// Ask for a new renderwindow passing in the ogreView in our nib file
	Ogre::NameValuePairList misc;
	// Pass the handle to the ogreView in our nib
	misc["externalWindowHandle"] = Ogre::StringConverter::toString((size_t)ogreView);
	// Tell OGRE that we're using cocoa, so it doesn't need to make a window for us
	misc["macAPI"] = "cocoa";
	misc["VSync"]="Yes";
	misc["VSync Interval"]="1";
	// Actually create the render window
	NSRect frame = [ogreView frame];
	root->createRenderWindow("ogre window", frame.size.width, frame.size.height, false, &misc);
	// And then get a pointer to it.
	renderWindow = [ogreView ogreWindow];
	
	[self prepareScene];
	
	[self setupDemoScene];
	
	[self runDemo];
}

@end