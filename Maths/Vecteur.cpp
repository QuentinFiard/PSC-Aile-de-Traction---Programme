//
//  Vecteur.cpp
//  PSC
//
//  Created by Quentin Fiard on 17/12/11.
//  Copyright (c) 2011 École Polytechnique. All rights reserved.
//

#include "Vecteur.h"
#include <math.h>
#include "Referentiel.h"

#include <assert.h>

#include <iostream>

#pragma mark Constructeurs

void Vecteur::init(const std::vector<Reel> &coordNew, Referentiel* refNew)
{
	if(refNew)
	{
		assert(refNew->dim()==coordNew.size());
	}
	
	coord.resize(coordNew.size());
	
	for(int i=0 ; i<coord.size() ; i++)
	{
		coord[i] = coordNew[i];
	}
	
	ref_ = refNew;
}

Vecteur::Vecteur() : ref_(NULL), coord(0)
{
	
}

Vecteur::Vecteur(unsigned char dim, Referentiel* ref) : coord(dim), ref_(ref) // Vecteur nul
{
	
}

Vecteur::Vecteur(const Vecteur &vecteurACopier) //Copie d'un vecteur
{
	init(vecteurACopier.coord, vecteurACopier.ref());
}

Vecteur::Vecteur(const std::vector<Reel> &coordNew, Referentiel* refNew)
{
	init(coordNew, refNew);
}

Vecteur::Vecteur(Reel x, Reel y, Reel z, Referentiel* refNew) // Création d'un point en dimension 3
{
	if(refNew)
	{
		assert(refNew->dim()==3);
	}
	
	coord.resize(3);
	coord[0]=x;
	coord[1]=y;
	coord[2]=z;
	
	ref_ = refNew;
}

Vecteur::Vecteur(Reel x, Reel y, Referentiel* refNew) // Création d'un point en dimension 2
{
	if(refNew)
	{
		assert(refNew->dim()==2);
	}
	
	coord.resize(2);
	coord[0]=x;
	coord[1]=y;
	
	ref_ = refNew;
}

#pragma mark -
#pragma mark Opérateurs

Vecteur Vecteur::operator+(const Vecteur &vecteur2) const // Somme vectorielle
{
	assert(dim()==vecteur2.dim() && ref() == vecteur2.ref());
	
	Vecteur res(*this);
	
	for(int i=0 ; i<dim() ; i++)
	{
		res[i] += vecteur2[i];
	}
	
	return res;
}

Vecteur Vecteur::operator+(const Vecteur &vecteur2) // Somme vectorielle
{
	assert(dim()==vecteur2.dim() && ( (ref() && vecteur2.ref()) || (!ref() && !vecteur2.ref()) ));
	
	changerDeReferentiel(vecteur2.ref());
	
	Vecteur res(*this);
	
	for(int i=0 ; i<dim() ; i++)
	{
		res[i] += vecteur2[i];
	}
	
	return res;
}

Vecteur Vecteur::operator*(const Reel lambda) const // Produit par un scalaire
{
	Vecteur res(*this);
	
	for(int i=0 ; i<dim() ; i++)
	{
		res[i] *= lambda;
	}
	
	return res;
}

Reel Vecteur::operator*(const Vecteur &vecteur2) const // Produit scalaire
{
	Reel res = 0;
	for(int i=0 ; i<dim() ; i++)
	{
		res += vecteur2[i]*coord[i];
	}
	
	return res;
}

Vecteur Vecteur::operator-() const // Moins unaire
{
	Vecteur res(*this);
	
	for(int i=0 ; i<dim() ; i++)
	{
		res[i] = -res[i];
	}
	
	return res;
}

Reel& Vecteur::operator[](const unsigned char i) // Coordonnées suivant le i-ème vecteur de la base vectorielle du référentiel
{
	return coord[i];
}

Reel Vecteur::operator[](const unsigned char i) const
{
	return coord[i];
}

Vecteur& Vecteur::operator=(const Vecteur &vecteurACopier) // Recopie du vecteur
{
	init(vecteurACopier.coord, vecteurACopier.ref());
	return *this;
}

bool Vecteur::operator==(const Vecteur &vecteur2)// Test d'égalité
{
	if(this == &vecteur2)
	{
		return true;
	}
	
	if(dim() != vecteur2.dim())
	{
		return false;
	}
	
	changerDeReferentiel(vecteur2.ref());
	
	bool res = true;
	for(int i=0 ; i<dim() && res ; i++)
	{
		res &= (coord[i]==vecteur2[i]); // On pourrait peut-être introduire un seuil
	}
	return res;
}

Vecteur operator*(const Reel lambda, const Vecteur& vecteur)
{
	return vecteur*lambda;
}

#pragma mark Opérateurs auxiliaires

Vecteur Vecteur::operator-(const Vecteur &vecteur2) // Différence
{
	return operator+(-vecteur2);
}

Vecteur Vecteur::operator-(const Vecteur &vecteur2) const// Différence
{
	return operator+(-vecteur2);
}

Vecteur Vecteur::operator/(const Reel lambda) const // Division par un scalaire
{
	return operator*(1/lambda);
}

void Vecteur::operator+=(const Vecteur &vecteur2)
{
	*this = *this + vecteur2;
}

void Vecteur::operator*=(const Reel lambda)
{
	*this = *this*lambda;
}

void Vecteur::operator-=(const Vecteur &vecteur2)
{
	*this = *this - vecteur2;
}

void Vecteur::operator/=(const Reel lambda)
{
	*this = *this/lambda;
}

#pragma mark -
#pragma mark Fonctions membres

unsigned char Vecteur::dim() const
{
	return coord.size();
}

Reel Vecteur::maxCoeff() const
{
	Reel max = -1;
	for(int i=0 ; i<dim() ; i++)
	{
		if(coord[i]>max)
		{
			max = coord[i];
		}
	}
	return max;
}

Reel Vecteur::norme() const
{
	// Pour éviter les erreurs numériques liées au calcul de la racine carrée au voisinage de 0
	// On commence par chercher le plus grand coefficient
	Reel max = maxCoeff();
	
	// Puis on calcule la norme
	Reel square = 0;
	for(int i=0 ; i<dim() ; i++)
	{
		square += (coord[i]/max)*(coord[i]/max); //On évite également l'overflow lié au carré
	}
	Reel norme = max * sqrt(square);
	return norme;
}

void Vecteur::normaliser()
{
	*this /= norme();
}

bool Vecteur::estNul() const
{
	bool res = true;
	for(int i=0 ; i<dim() && res ; i++)
	{
		if(coord[i]!=0)
		{
			res = false;
		}
	}
	return res;
}

#pragma mark -
#pragma mark Changement de référentiel

void Vecteur::initRef(Referentiel* newRef)
{
	assert(!ref());
	
	ref_ = newRef;
}

void Vecteur::setRef(Referentiel* newRef)
{
	ref_ = newRef;
}

void Vecteur::changerDeReferentiel(Referentiel* newRef)
{
	if(ref_ && (ref_ != newRef))
	{
		Vecteur tmp(ref()->vecteurDeCourantVersReference(*this));
		*this = newRef->vecteurDeReferenceVersCourant(tmp);
	}
}

Referentiel* Vecteur::ref() const
{
	return ref_;
}

void Vecteur::afficher() const
{
	if(coord.size()==0)
	{
		std::cout << "Vecteur indéfini";
	}
	else
	{
		printf("%lf",coord[0]);
		for(int i=1 ; i<coord.size() ; i++)
		{
			printf(" %lf", coord[i]);
		}
	}
}