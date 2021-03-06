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

#include "Capteur.h"
#include "Source.h"

@class JoystickStatus;

class Joystick : public Capteur
{
private:
	SDL_Joystick* joystick;
	JoystickStatus* status;
	
protected:
	
	Joystick();
	
	void setStatus_(JoystickStatus* status);
	void prepareJoystick_();
	
	bool isConnected_();
	
	Source< NumericValue<double> >* output;
	
	bool motorControl;
	
	bool isControllingMotor_();
	
	bool isActivated_();
	
public:
	
	static void prepareJoystick();
	
	static Joystick* shared();
	static void setStatus(JoystickStatus* status);
	
	static void update();
	
	static bool isConnected();
	
	void handleJoystickEvent(SDL_Event& event);
	
	static bool isControllingMotor();
	
	static bool isActivated();
};

#endif
