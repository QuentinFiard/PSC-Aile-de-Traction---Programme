//
//  Point.cpp
//  PSC
//
//  Created by Quentin Fiard on 17/12/11.
//  Copyright (c) 2011 École Polytechnique. All rights reserved.
//

#include "Point.h"
#include "Referentiel.h"

#include <assert.h>

#pragma mark Constructeurs

Point::Point() : Vecteur()
{
	
}

Point::Point(unsigned char dim, Referentiel* ref) : Vecteur(dim,ref)
{
	
}

Point::Point(std::vector<Reel> &coord, Referentiel* ref) : Vecteur(coord,ref)
{
	
}

Point::Point(Reel x, Reel y, Reel z, Referentiel* ref) : Vecteur(x,y,z,ref) // Création d'un point en dimension 3
{

}

Point::Point(Reel x, Reel y, Referentiel* ref) : Vecteur(x,y,ref) // Création d'un point en dimension 2
{
	
}

Point::Point(const Point &p) : Vecteur(p)
{
	
}

Point::Point(const Vecteur &vecteur) : Vecteur(vecteur)
{
	
}

#pragma mark -
#pragma mark Opérateurs

Point Point::operator+(Vecteur &vecteur2) const // Somme vectorielle
{
	assert(dim()==vecteur2.dim() && ( (ref() && vecteur2.ref()) || (!ref() && !vecteur2.ref()) ));
	
	if(ref())
	{
		vecteur2.changerDeReferentiel(ref());
	}
	
	Point res(*this);
	
	for(int i=0 ; i<dim() ; i++)
	{
		res[i] += vecteur2[i];
	}
	
	return res;
}

Point Point::operator+(const Vecteur &vecteur2) const // Somme vectorielle
{
	assert(dim()==vecteur2.dim() && ref() == vecteur2.ref());
	
	Point res(*this);
	
	for(int i=0 ; i<dim() ; i++)
	{
		res[i] += vecteur2[i];
	}
	
	return res;
}

Point& Point::operator=(const Point &vecteur2) // Recopie du point
{
	Vecteur::operator=(vecteur2);
	return *this;
}

Vecteur Point::operator-(const Point &vecteur2) // Différence
{
	return Vecteur::operator-(vecteur2);
}

Vecteur Point::operator-(const Point &vecteur2) const // Différence
{
	return Vecteur::operator-(vecteur2);
}

#pragma mark -
#pragma mark Changement de référentiel

void Point::changerDeReferentiel(Referentiel* newRef)
{
	if( ref_ && (ref_ != newRef) )
	{
		// Les deux origines sont exprimées dans le référentiel de référence
		Vecteur entreOrigines = ref()->origin() - newRef->origin();
		
		entreOrigines.changerDeReferentiel(newRef);
		
		if(!estNul())
		{
			Vecteur::changerDeReferentiel(newRef);
		}
		
		*this += entreOrigines;
		
		setRef(newRef);
	}
}