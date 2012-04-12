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

@class JoystickStatus;

class Joystick
{
private:
	SDL_Joystick* joystick;
	JoystickStatus* status;
	
protected:
	
	Joystick();
	
	void setStatus_(JoystickStatus* status);
	void prepareJoystick_();
	
	void handleJoystickEvent(SDL_Event& event);
	
	void run();
	
public:
	
	static void prepareJoystick();
	
	static Joystick* shared();
	static void setStatus(JoystickStatus* status);
};

#endif
