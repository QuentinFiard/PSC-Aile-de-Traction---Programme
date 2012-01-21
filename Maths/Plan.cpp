//
//  Plan.cpp
//  PSC
//
//  Created by Quentin Fiard on 27/12/11.
//  Copyright (c) 2011 École Polytechnique. All rights reserved.
//

#include "Plan.h"

#include <math.h>

Plan::Plan(const Vecteur3D &exNew, const Vecteur3D &eyNew) : ex(exNew), ey(eyNew), normalUnitaire(exNew)
{
	if(ey.ref() != ex.ref())
	{
		ey.changerDeReferentiel(ex.ref());
	}
	normalUnitaire = (ex^ey);
	normalUnitaire.normaliser();
}

Vecteur3D Plan::projete(const Vecteur3D& w)
{
	/*
		lambda : coefficient suivant ex du projeté
		mu : coefficient suivant ey
	 
		lambda et mu sont tels que w - proj(w) soit orthogonal à ex et ey
	 
		Ceci donne le système linéaire
			lambda ex.ex + mu ex.ey = w.ex
			lambda ex.ey + mu ey.ey = w.ex
	 
		que l'on résoud par la formule de Cardan
	 */
	
	Reel a = ex*ex;
	Reel b = ey*ey;
	Reel c = ex*ey;
	
	Reel det = a*b - c*c;
	
	Reel lambda = (w*ex * b - w*ey * c)/det;
	Reel mu = (a * w*ey - c * w*ex)/det;
	
	return lambda*ex + mu*ey;
}

Reel Plan::angleOrienteParProjection(const Vecteur3D& vect1,const Vecteur3D& vect2)
{
	Vecteur3D proj1 = projete(vect1);
	Vecteur3D proj2 = projete(vect2);
	
	Reel sinAngle = (proj1^proj2)*normalUnitaire/(proj1.norme()*proj2.norme());
	
	return asin(sinAngle);
}