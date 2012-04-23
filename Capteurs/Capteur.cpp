//
//  Capteur.cpp
//  PSC
//
//  Created by Quentin Fiard on 18/03/12.
//  Copyright (c) 2012 École Polytechnique. All rights reserved.
//

#include "Capteur.h"

#include <assert.h>
#include <iostream>

boost::posix_time::ptime* Capteur::lastAcquisition()
{
	return lastAcquisition_;
}

GrandeurCapteur Capteur::grandeurMesuree()
{
	return grandeur_;
}

Capteur::Capteur(GrandeurCapteur grandeur) : grandeur_(grandeur), lastAcquisition_(NULL)
{
	
}

std::string Capteur::nomGrandeurMesuree()
{
	switch(grandeur_)
	{
		case CAPTEUR_NON_CONNECTE:
			return "CAPTEUR_NON_CONNECTE";
			break;
			
		case CAPTEUR_DIRECTION_VENT:
			return "CAPTEUR_DIRECTION_VENT";
			break;
			
		case CAPTEUR_VITESSE_VENT:
			return "CAPTEUR_VITESSE_VENT";
			break;
			
		case CAPTEUR_ROTATION_MOTEUR:
			return "CAPTEUR_ROTATION_MOTEUR";
			break;
			
		case CAPTEUR_THETA:
			return "CAPTEUR_THETA";
			break;
			
		case CAPTEUR_PHI:
			return "CAPTEUR_PHI";
			break;
			
		case CAPTEUR_CAMERA:
			return "CAPTEUR_CAMERA";
			break;
			
		case CAPTEUR_JOYSTICK:
			return "CAPTEUR_JOYSTICK";
			break;
			
		default:
			std::cout << "Capteur non supporté" << std::endl;
			assert(0);
			break;
	}
}