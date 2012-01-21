//
//  Plan.h
//  PSC
//
//  Created by Quentin Fiard on 27/12/11.
//  Copyright (c) 2011 École Polytechnique. All rights reserved.
//

#ifndef PSC_Plan_h
#define PSC_Plan_h

#include "Vecteur3D.h"

class Plan
{
	Plan(const Vecteur3D &ex, const Vecteur3D &ey);
	
	Vecteur3D projete(const Vecteur3D& vect);
	
	Reel angleOrienteParProjection(const Vecteur3D& vect1,const Vecteur3D& vect2);
	
private:
	Vecteur3D ex;
	Vecteur3D ey;
	
	Vecteur3D normalUnitaire;
};

#endif
