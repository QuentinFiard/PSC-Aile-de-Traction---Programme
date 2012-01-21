//
//  Vecteur3D.cpp
//  PSC
//
//  Created by Quentin Fiard on 20/12/11.
//  Copyright (c) 2011 École Polytechnique. All rights reserved.
//

#include "Vecteur3D.h"
#include <assert.h>

#include <math.h>

#pragma mark Constructeurs

Vecteur3D::Vecteur3D(const Vecteur& vect) : Vecteur(vect)
{
	assert(vect.dim() == 3);
}

Vecteur3D::Vecteur3D(Reel x, Reel y, Reel z, Referentiel* ref) : Vecteur(x,y,z,ref)
{
	
}

Vecteur3D::Vecteur3D(std::vector<Reel> &coord, Referentiel* ref) : Vecteur(coord,ref)
{
	
}

#pragma mark -
#pragma mark Fonctions

Reel Vecteur3D::angleNonOriente(const Vecteur3D& vect2)
{
	Reel CosAngle = (*this * vect2)/(norme() * vect2.norme());
	
	return acos(CosAngle);
}

#pragma mark -
#pragma mark Opérateurs

Vecteur3D Vecteur3D::operator^(const Vecteur3D &v) //Produit vectoriel
{
	changerDeReferentiel(v.ref());
	
	const Vecteur3D& u = *this;
	
	return Vecteur3D(u[1]*v[2]-u[2]*v[1],u[2]*v[0]-u[0]*v[2],u[0]*v[1]-u[1]*v[0],ref());
}

Vecteur3D Vecteur3D::operator^(const Vecteur3D &v) const//Produit vectoriel
{
	assert(ref() == v.ref());
	
	const Vecteur3D& u = *this;
	
	return Vecteur3D(u[1]*v[2]-u[2]*v[1],u[2]*v[0]-u[0]*v[2],u[0]*v[1]-u[1]*v[0],ref());
}