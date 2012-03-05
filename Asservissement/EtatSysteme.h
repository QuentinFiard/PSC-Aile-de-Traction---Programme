//
//  EtatSysteme.h
//  PSC
//
//  Created by Quentin Fiard on 05/03/12.
//  Copyright (c) 2012 École Polytechnique. All rights reserved.
//

#ifndef PSC_EtatSysteme_h
#define PSC_EtatSysteme_h

#include "Referentiel3D.h"
#include "Vecteur3D.h"
#include "EtatSystemeDerivee.h"

class EtatSysteme
{
	EtatSysteme(Point position, Vecteur3D vitesse, double psi);
	
	EtatSystemeDerivee calculDerivee(double commande);
	
	Point& position();
	Vecteur3D& vitesse();
	double psi();
	
private:
	Point position_;
	Vecteur3D vitesse_;
	double psi_;	
};

#endif
