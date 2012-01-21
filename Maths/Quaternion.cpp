//
//  Quaternion.cpp
//  PSC
//
//  Created by Quentin Fiard on 24/12/11.
//  Copyright (c) 2011 École Polytechnique. All rights reserved.
//

#include <stddef.h>
#include "Quaternion.h"
#include "Vecteur3D.h"
#include "Matrice.h"
#include <math.h>

#include <assert.h>

Quaternion::Quaternion() : Vecteur(4)
{
	
}

Quaternion::Quaternion(const Reel q0, const Reel q1, const Reel q2, const Reel q3) : Vecteur(4)
{
	coord[0] = q0;
	coord[1] = q1;
	coord[2] = q2;
	coord[3] = q3;
}

Quaternion::Quaternion(std::vector<Reel> &qnew) : Vecteur(qnew)
{
	
}

Quaternion::Quaternion(const Quaternion &q) : Vecteur(q)
{
	
}

Quaternion::Quaternion(const Vecteur &vect) : Vecteur(4) //Création d'un quaternion pur représentant un vecteur
{
	assert(vect.dim() == 3);
	
	coord[0] = 0;
	coord[1] = vect[0];
	coord[2] = vect[1];
	coord[3] = vect[2];
}

Quaternion Quaternion::quaternionParAngleEtAxe(const Reel angle, const Vecteur& axe)
{
	assert(axe.dim() == 3);
	
	Vecteur axeunitaire(axe);
	axeunitaire.normaliser();
	
	Quaternion res;
	res[0] = cos(angle/2);
	res[1] = sin(angle/2)*axeunitaire[0];
	res[2] = sin(angle/2)*axeunitaire[1];
	res[3] = sin(angle/2)*axeunitaire[2];
	
	return res;
}

Quaternion Quaternion::conj() const// Conjugué du quaternion, qui représente la rotation inverse si le quaternion est unitaire
{
	Quaternion q(*this);
	for(int i=1 ; i<4 ; i++)
	{
		q[i] = -q[i];
	}
	return q;
}

Quaternion Quaternion::operator()(const Quaternion &q1) const // Composée de 2 rotations (q1 suivie de q2)
{
	const Quaternion& q2 = *this;
	return q1*q2; // La formule est l'opposée de celle pour les matrices
}

Quaternion Quaternion::operator*(const Quaternion &q2) const // Produit de 2 quaternions
{
	Quaternion res;
	const Quaternion& q1 = *this;
	// On utilise la définition de la multiplication sur le corps des quaternions
	res[0] = q1[0]*q2[0] - q1[1]*q2[1] - q1[2]*q2[2] - q1[3]*q2[3];
	res[1] = q1[0]*q2[1] + q1[1]*q2[0] + q1[2]*q2[3] - q1[3]*q2[2];
	res[2] = q1[0]*q2[2] + q1[2]*q2[0] + q1[3]*q2[1] - q1[1]*q2[3];
	res[3] = q1[0]*q2[3] + q1[3]*q2[0] + q1[1]*q2[2] - q1[2]*q2[1];
	return res;
}

Vecteur3D Quaternion::rotation(const Vecteur &v) const
{
	assert(v.dim() == 3);
	
	const Quaternion& q = *this;
	// On représente le vecteur par un quaternion dont la partie réelle est nulle
	Quaternion vecteur(v);
	
	// On calcule le vecteur après rotation par conjugaison par le quaternion
	Quaternion apresRotation = q.conj()*vecteur*q;
	
	std::vector<Reel> coordNew(3); // On transforme le quaternion pur en vecteur
	coordNew[0] = apresRotation[1];
	coordNew[1] = apresRotation[2];
	coordNew[2] = apresRotation[3];
	
	return Vecteur3D(coordNew,v.ref());
}

Matrice Quaternion::rotation(const Matrice &mat) const
{
	assert(mat.nbLignes() == 3 && mat.nbColonnes() == 3);
	
	Matrice res(3,3);
	
	for(int j=0 ; j<3 ; j++)
	{
		Vecteur3D axe(mat(0,j), mat(1,j), mat(2,j));
		Vecteur3D axeApresRotation(rotation(axe));
		for(int i=0 ; i<3 ; i++)
		{
			res(i,j) = axeApresRotation[i];
		}
	}
	
	return res;
}

// Formule donnant la dérivée temporelle du quaternion d'un solide en axe mobile en fonction du vecteur rotation instantané du solide
Quaternion Quaternion::deriveeQuaternionEnAxesInertiels(const Vecteur &Omega)
{
	const Quaternion& q = *this;
	return (q*Quaternion(Omega))/2;
}

#pragma mark -
#pragma mark Opérations

Quaternion Quaternion::operator+(const Quaternion &vecteur2) const // Somme vectorielle
{
	Quaternion res(*this);
	
	for(int i=0 ; i<4 ; i++)
	{
		res[i] += vecteur2[i];
	}
	
	return res;
}

Quaternion Quaternion::operator*(const Reel lambda) const // Produit par un scalaire
{
	Quaternion res(*this);
	
	for(int i=0 ; i<4 ; i++)
	{
		res[i] *= lambda;
	}
	
	return res;
}

Quaternion Quaternion::operator-() const // Moins unaire
{
	Quaternion res(*this);
	
	for(int i=0 ; i<4 ; i++)
	{
		res[i] = -res[i];
	}
	
	return res;
}

Quaternion operator*(const Reel lambda, const Quaternion& vecteur)
{
	return vecteur*lambda;
}

Quaternion Quaternion::operator=(const Quaternion &aCopier) // Recopie du quaternion
{
	coord[0] = aCopier[0];
	coord[1] = aCopier[1];
	coord[2] = aCopier[2];
	coord[3] = aCopier[3];
	return *this;
}

#pragma mark Opérations auxiliaires
Quaternion Quaternion::operator-(const Quaternion &vecteur2) const // Différence
{
	return operator+(-vecteur2);
}

Quaternion Quaternion::operator/(const Reel lambda) const // Division par un scalaire
{
	return operator*(1/lambda);
}