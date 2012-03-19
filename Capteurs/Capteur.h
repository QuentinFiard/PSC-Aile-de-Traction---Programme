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

class Capteur
{
public:
	
	static Capteur* shared(std::string name);
	
	std::string name();
	
	virtual double value() = 0;
	
private:
	std::string name_;
};

#endif
