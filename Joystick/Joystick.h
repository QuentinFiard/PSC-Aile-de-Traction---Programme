//
//  Joystick.h
//  PSC
//
//  Created by Quentin Fiard on 10/12/11.
//  Copyright (c) 2011 École Polytechnique. All rights reserved.
//

#ifndef PSC_Joystick_h
#define PSC_Joystick_h

#include <SDL/SDL.h>

class DemoApp;

class Joystick
{
private:
	SDL_Joystick* joystick;
public:
	Joystick();
	void prepareJoystick();
	void handleJoystickEvent(SDL_Event& event,DemoApp* demo);
	
	static Joystick* handler();
};

#endif
