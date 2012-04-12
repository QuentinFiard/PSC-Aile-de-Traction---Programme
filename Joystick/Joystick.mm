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
#include "ConnectionUSB.h"

#include "Database.h"

#include "Tests.h"

#include "JoystickStatus.h"

#define ROTSPEED 0.03

using namespace std;

static Joystick* shared_;

Joystick* Joystick::shared()
{
	if(!shared_)
	{
		shared_ = new Joystick();
	}
	return shared_;
}

Joystick::Joystick()
{
	status = nil;
}

void Joystick::prepareJoystick()
{
	shared()->prepareJoystick_();
}

void Joystick::prepareJoystick_()
{
	testerReferentiels();
	
	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK);
	
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
		
		if(status)
		{
			[status setStatus:STATUS_OK];
		}
	}
	else
	{
		printf("Impossible de charger le joystick\n");
		
		if(status)
		{
			[status setStatus:STATUS_WAITING];
		}
	}
}

void Joystick::handleJoystickEvent(SDL_Event& event)
{
	Moteur3D* demo = [Moteur3D moteur3D];
	
	if(event.type==SDL_JOYAXISMOTION)
	{
		printf("Axis %d %d %d\n",event.jaxis.which,event.jaxis.axis,event.jaxis.value);
		
		if(event.jaxis.axis == 0)
		{
			if(demo)
			{
				demo.pitchSpeed = ROTSPEED * event.jaxis.value/32768;
			}
			
			double position = event.jaxis.value;// + 0x8000;
			position /= 0x8000;
			
			if(!Database::hasConfigurationField("MOTOR_SIGNAL_OFFSET_HIGH"))
			{
				NumericValue<double> toSave(0.05);
				Database::setConfigurationField("MOTOR_SIGNAL_OFFSET_HIGH",&toSave);
			}
			
			if(!Database::hasConfigurationField("MOTOR_SIGNAL_OFFSET_LOW"))
			{
				NumericValue<double> toSave(0.05);
				Database::setConfigurationField("MOTOR_SIGNAL_OFFSET_LOW",&toSave);
			}
			
			//dynamixel->setPosition(position);
			if(position<Database::getConfigurationFieldValue<double>("MOTOR_SIGNAL_OFFSET_HIGH") && position > -Database::getConfigurationFieldValue<double>("MOTOR_SIGNAL_OFFSET_LOW"))
			{
				position = 0;
			}
			
			if(ConnectionUSB::isConnected())
			{
				ConnectionUSB::setSignalForMotor(position,0);
			}
		}
		else if(event.jaxis.axis == 1)
		{
			if(demo)
			{
				demo.yawSpeed = -ROTSPEED * event.jaxis.value/32768;
			}
		}
		else if(event.jaxis.axis == 2)
		{
			if(demo)
			{
				demo.rollSpeed = -1.5*ROTSPEED * event.jaxis.value/32768;
			}
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

void Joystick::setStatus(JoystickStatus* status)
{
	shared()->setStatus_(status);
}

void Joystick::setStatus_(JoystickStatus* statusNew)
{
	status = statusNew;
	
	if(joystick)
	{
		[status setStatus:STATUS_OK];
	}
	else
	{
		[status setStatus:STATUS_WAITING];
	}
}

void Joystick::update()
{
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
			shared()->handleJoystickEvent(event);
		}
	}
}

bool Joystick::isConnected()
{
	return shared()->isConnected_();
}

bool Joystick::isConnected_()
{
	return (joystick != NULL);
}
