//
//  Joystick.cpp
//  PSC
//
//  Created by Quentin Fiard on 10/12/11.
//  Copyright (c) 2011 École Polytechnique. All rights reserved.
//

#include "Joystick.h"
#include <iostream>
#include "Moteur3D.h"

#include "Tests.h"

#define ROTSPEED 0.03

using namespace std;

static Joystick* shared;

Joystick* Joystick::handler()
{
	if(!shared)
	{
		shared = new Joystick();
	}
	return shared;
}

Joystick::Joystick()
{
	
}

void Joystick::prepareJoystick()
{
	testerReferentiels();
	
	cout << "************* Joysticks ***************"  << endl;
	int nbJoySticks = SDL_NumJoysticks();
	cout << "Détection des joysticks : " << nbJoySticks << " détectés" << endl;
	if(nbJoySticks>1)
	{
		cout << "Le programme ne peut pour le moment prendre en charge qu'un seul joystick\n";
	}
	if(nbJoySticks>0)
	{
		joystick = SDL_JoystickOpen(0);
	}
	else
	{
		joystick = NULL;
	}
	if(joystick)
	{
		printf("Chargement du joystick\n");
		printf("Nom: %s\n", SDL_JoystickName(0));
		printf("Nombre d'axes : %d\n", SDL_JoystickNumAxes(joystick));
		printf("Nombre de boutons : %d\n", SDL_JoystickNumButtons(joystick));
		printf("Nombre de chapeaux : %d\n", SDL_JoystickNumHats(joystick));
	}
	else
		printf("Impossible de charger le joystick\n");
}

void Joystick::handleJoystickEvent(SDL_Event& event,Moteur3D* demo)
{
	if(event.type==SDL_JOYAXISMOTION)
	{
		printf("Axis %d %d %d\n",event.jaxis.which,event.jaxis.axis,event.jaxis.value);
		
		if(event.jaxis.axis == 0)
		{
			demo.pitchSpeed = ROTSPEED * event.jaxis.value/32768;
		}
		else if(event.jaxis.axis == 1)
		{
			demo.yawSpeed = -ROTSPEED * event.jaxis.value/32768;
		}
		else if(event.jaxis.axis == 2)
		{
			demo.rollSpeed = -1.5*ROTSPEED * event.jaxis.value/32768;
		}
	}
	else if(event.type==SDL_JOYHATMOTION)
	{
		printf("Hat %d %d ",event.jhat.which,event.jhat.hat);
		Uint8 value = event.jhat.value;
		if(value == SDL_HAT_CENTERED)
		{
			cout << "center";
		}
		else if(value == SDL_HAT_UP)
		{
			cout << "up";
		}
		else if(value == SDL_HAT_RIGHTUP)
		{
			cout << "right up";
		}
		else if(value == SDL_HAT_RIGHT)
		{
			cout << "right";
		}
		else if(value == SDL_HAT_RIGHTDOWN)
		{
			cout << "right down";
		}
		else if(value == SDL_HAT_DOWN)
		{
			cout << "down";
		}
		else if(value == SDL_HAT_LEFTDOWN)
		{
			cout << "left down";
		}
		else if(value == SDL_HAT_LEFT)
		{
			cout << "left";
		}
		else if(value == SDL_HAT_LEFTUP)
		{
			cout << "left up";
		}
		cout << endl;
	}
	else if(event.type == SDL_JOYBUTTONDOWN)
	{
		printf("Button %d pressed for joystick %d\n",event.jbutton.button,event.jbutton.which);
	}
	else if(event.type == SDL_JOYBUTTONUP)
	{
		printf("Button %d released for joystick %d\n",event.jbutton.button,event.jbutton.which);
	}
}