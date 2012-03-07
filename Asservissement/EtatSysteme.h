//
//  EtatSysteme.h
//  PSC
//
//  Created by Quentin Fiard on 05/03/12.
//  Copyright (c) 2012 École Polytechnique. All rights reserved.
//

#ifndef PSC_EtatSysteme_h
#define PSC_EtatSysteme_h

#include <acado_toolkit.hpp>

#include "Referentiel3D.h"
#include "Vecteur3D.h"
#include "EtatSystemeDerivee.h"

USING_NAMESPACE_ACADO

class EtatSysteme : public DifferentialState
{
public:
	EtatSysteme(Point position, Vecteur3D vitesse, double psi);
	EtatSysteme(DifferentialState& vect);
	EtatSysteme(uint nRows_, uint nCols_=1, String name_="");
	
	DifferentialStateDerivative derivee(Control commande);
	
	Point& position();
	Vecteur3D& vitesse();
	double psi();
	
private:
	Point position_;
	Vecteur3D vitesse_;
	double psi_;	
};

#endif
