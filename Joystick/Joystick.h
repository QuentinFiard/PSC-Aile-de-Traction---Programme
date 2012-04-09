//
//  Joystick.h
//  PSC
//
//  Created by Quentin Fiard on 10/12/11.
//  Copyright (c) 2011 École Polytechnique. All rights reserved.
//

#ifndef PSC_Joystick_h
#define PSC_Joystick_h

#import <Cocoa/Cocoa.h>
#include <SDL/SDL.h>

#include "Dynamixel.h"

@class Moteur3D;

class Joystick
{
private:
	SDL_Joystick* joystick;
public:
	Joystick();
	void prepareJoystick();
	
	void handleJoystickEvent(SDL_Event& event,Moteur3D* demo = NULL);
	
	static Joystick* handler();
};

#endif
