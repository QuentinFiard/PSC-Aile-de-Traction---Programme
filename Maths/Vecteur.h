//
//  Vecteur.h
//  PSC
//
//  Created by Quentin Fiard on 17/12/11.
//  Copyright (c) 2011 École Polytechnique. All rights reserved.
//

#ifndef PSC_Vecteur_h
#define PSC_Vecteur_h

#include <stddef.h>
#include <vector>
#include "Parametres.h"

class Referentiel;
class Point;

class Quaternion;

class Vecteur // Vecteur d'un espace vectoriel réel
{
public:
//Constructeurs
	Vecteur();
	Vecteur(unsigned char dim, Referentiel* ref=NULL); // Vecteur nul
	Vecteur(const Vecteur &vecteurACopier); //Copie d'un vecteur
	Vecteur(const std::vector<Reel> &coord, Referentiel* ref=NULL);
	Vecteur(Reel x, Reel y, Reel z, Referentiel* ref=NULL); // Création d'un point en dimension 3
	Vecteur(Reel x, Reel y, Referentiel* ref=NULL); // Création d'un point en dimension 2

//Opérations
	
	// REMARQUE IMPORTANTE : Pour toutes les opérations binaires nécessitant que les vecteurs soient écrits dans le même référentiel, on change le référentiel du premier vecteur pour celui du second.
	
	Vecteur operator+(const Vecteur &vecteur2); // Somme vectorielle
	Vecteur operator+(const Vecteur &vecteur2) const; // Somme vectorielle
	Vecteur operator*(const Reel lambda) const; // Produit par un scalaire
	Reel operator*(const Vecteur &vecteur2) const; // Produit scalaire
	Reel& operator[](const unsigned char i); // Coordonnées suivant le i-ème vecteur de la base vectorielle du référentiel
	Reel operator[](const unsigned char i) const;
	Vecteur operator-() const; // Moins unaire
	Vecteur& operator=(const Vecteur &vecteur2); // Recopie du vecteur
	
	bool operator==(const Vecteur &vecteur2); // Test d'égalité

//Opérateurs auxiliaires
	Vecteur operator-(const Vecteur &vecteur2); // Différence
	Vecteur operator-(const Vecteur &vecteur2) const; // Différence
	Vecteur operator/(const Reel lambda) const; // Division par un scalaire
	
	void operator+=(const Vecteur &vecteur2);
	void operator*=(const Reel lambda);
	void operator-=(const Vecteur &vecteur2);
	void operator/=(const Reel lambda);

//Fonctions
	void normaliser();
	Reel norme() const;
	Reel maxCoeff() const;
	unsigned char dim() const;
	
	bool estNul() const;

//Changement de référentiel
	void initRef(Referentiel* newRef);
	void setRef(Referentiel* newRef);
	
	virtual void changerDeReferentiel(Referentiel* newRef);
	
	Referentiel* ref() const;
	
	void afficher() const;

protected:
	std::vector<Reel> coord;
	Referentiel* ref_;
	
	void init(const std::vector<Reel> &coordNew, Referentiel* refNew);
};

Vecteur operator*(const Reel lambda, const Vecteur& vecteur);

#endif
