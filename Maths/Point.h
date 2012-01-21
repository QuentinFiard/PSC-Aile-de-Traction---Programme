//
//  Point.h
//  PSC
//
//  Created by Quentin Fiard on 17/12/11.
//  Copyright (c) 2011 École Polytechnique. All rights reserved.
//

#ifndef PSC_Point_h
#define PSC_Point_h

#include "Vecteur.h"

class Referentiel;

class Point : public Vecteur
{
public:
	Point();
	Point(unsigned char dim, Referentiel* ref=NULL);
	Point(std::vector<Reel> &coord, Referentiel* ref=NULL);
	Point(Reel x, Reel y, Reel z, Referentiel* ref=NULL); // Création d'un point en dimension 3
	Point(Reel x, Reel y, Referentiel* ref=NULL); // Création d'un point en dimension 2
	Point(const Point &p);
	Point(const Vecteur &vecteur);
	
	void changerDeReferentiel(Referentiel* newRef); // Formules de changement de référentiel pour un point
	
	//Opérations
	Point operator+(Vecteur &vecteur2) const; // Somme vectorielle
	Point operator+(const Vecteur &vecteur2) const; // Somme vectorielle
	Point& operator=(const Point &vecteur2); // Recopie du point
	Vecteur operator-(const Point &vecteur2); // Différence
	Vecteur operator-(const Point &vecteur2) const; // Différence
	
	// Ces opérations n'ont plus de sens, on leur fait produire une erreur
	Point operator*(const Reel lambda) const; // Produit par un scalaire
	Point operator-() const; // Moins unaire
	Point operator/(const Reel lambda) const; // Division par un scalaire
};



#endif
