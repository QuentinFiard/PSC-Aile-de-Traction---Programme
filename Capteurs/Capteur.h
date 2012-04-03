//
//  Capteur.h
//  PSC
//
//  Created by Quentin Fiard on 18/03/12.
//  Copyright (c) 2012 École Polytechnique. All rights reserved.
//

#ifndef PSC_Capteur_h
#define PSC_Capteur_h

#include <string>

#include <boost/date_time.hpp>

typedef enum
{
	CAPTEUR_NON_CONNECTE	= 0,
	CAPTEUR_DIRECTION_VENT	= 1,
	CAPTEUR_VITESSE_VENT	= 2,
	CAPTEUR_ROTATION_MOTEUR = 3,
	CAPTEUR_THETA			= 4,
	CAPTEUR_PHI				= 5,
	CAPTEUR_CAMERA			= 6,
} GrandeurCapteur;

class Capteur
{
public:
	
	static Capteur* shared(GrandeurCapteur grandeur);
	
	boost::posix_time::ptime* lastAcquisition();
	
	GrandeurCapteur grandeurMesuree();
	
	std::string nomGrandeurMesuree();
	
protected:
	
	Capteur(GrandeurCapteur grandeur);
	
	GrandeurCapteur grandeur_;
	
	boost::posix_time::ptime* lastAcquisition_;
	
};

#endif
