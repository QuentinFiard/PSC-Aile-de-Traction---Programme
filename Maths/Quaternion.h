//
//  Quaternion.h
//  PSC
//
//  Created by Quentin Fiard on 24/12/11.
//  Copyright (c) 2011 École Polytechnique. All rights reserved.
//

#ifndef PSC_Quaternion_h
#define PSC_Quaternion_h

#include "Vecteur.h"

class Vecteur3D;
class Matrice;

class Quaternion : public Vecteur
{
public:
	Quaternion();
	Quaternion(const Reel q0, const Reel q1, const Reel q2, const Reel q3);
	Quaternion(std::vector<Reel> &qnew);
	Quaternion(const Vecteur &vect); //Création d'un quaternion pur représentant un vecteur de dimension 3
	Quaternion(const Quaternion &q); //Création d'une copie du quaternion
	
	Quaternion operator()(const Quaternion &q) const; // Composée de 2 rotations
	Quaternion operator*(const Quaternion &q) const; // Produit de 2 quaternions
	
	Vecteur3D rotation(const Vecteur &vecteur) const;
	Matrice rotation(const Matrice &mat) const;
	
	Quaternion conj() const; // Conjugué du quaternion, qui représente la rotation inverse si le quaternion est unitaire
	static Quaternion quaternionParAngleEtAxe(const Reel angle, const Vecteur& axe);
	
	// Formule donnant la dérivée temporelle du quaternion d'un solide en axe mobile en fonction du vecteur rotation instantané du solide
	Quaternion deriveeQuaternionEnAxesInertiels(const Vecteur &Omega);
	
/**********************************************************************************************
	Recopie des opérations vectorielles
*/
	
	//Opérations
	Quaternion operator+(const Quaternion &vecteur2) const; // Somme vectorielle
	Quaternion operator*(const Reel lambda) const; // Produit par un scalaire
	Quaternion operator-() const; // Moins unaire
	Quaternion operator=(const Quaternion &vecteur2); // Recopie du quaternion
	
	//Opérations auxiliaires
	Quaternion operator-(const Quaternion &vecteur2) const; // Différence
	Quaternion operator/(const Reel lambda) const; // Division par un scalaire
};

Quaternion operator*(const Reel lambda, const Quaternion& vecteur);

#endif
