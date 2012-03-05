//
//  EtatSystemeDerivee.h
//  PSC
//
//  Created by Quentin Fiard on 05/03/12.
//  Copyright (c) 2012 École Polytechnique. All rights reserved.
//

#ifndef PSC_EtatSystemeDerivee_h
#define PSC_EtatSystemeDerivee_h

#include "Vecteur3D.h"

class EtatSystemeDerivee
{
public:
	EtatSystemeDerivee(Vecteur3D vitesse, Vecteur3D acceleration, double psi_pt);
	
private:
	Vecteur3D vitesse;
	Vecteur3D acceleration;
	double psi_pt;
};

#endif
