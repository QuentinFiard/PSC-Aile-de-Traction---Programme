//
//  CapteurAngulaire.h
//  PSC
//
//  Created by Quentin Fiard on 18/03/12.
//  Copyright (c) 2012 École Polytechnique. All rights reserved.
//

#ifndef PSC_CapteurAngulaire_h
#define PSC_CapteurAngulaire_h

#include "Capteur.h"
#include "CommunicationProtocol.h"
#include <string>

class CapteurAngulaire : public Capteur
{
public:
	static CapteurAngulaire* shared(Sensor sensor);
	static CapteurAngulaire* shared(std::string name);
	
	Sensor sensor();
	
private:
	Sensor sensor_;

};

#endif
