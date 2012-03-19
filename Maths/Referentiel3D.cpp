//
//  Referentiel3D.cpp
//  PSC
//
//  Created by Quentin Fiard on 22/12/11.
//  Copyright (c) 2011 École Polytechnique. All rights reserved.
//

#include "Referentiel3D.h"
#include "Vecteur3D.h"
#include <assert.h>
#include <iostream>

static Referentiel3D* reference = NULL;

#pragma mark Constructeurs

Referentiel3D::Referentiel3D() : Referentiel(), q(1,0,0,0)
{
	
}

/*Referentiel3D::Referentiel3D(const Referentiel& ref)
{
	
}*/

Referentiel3D::Referentiel3D(const Referentiel3D& ref) : Referentiel(ref), q(ref.q)
{
	
}

Referentiel3D::Referentiel3D(const Point &origin, Quaternion &qNew) : Referentiel(origin), q(qNew)
{
	
}

#pragma mark -
#pragma mark Fonctions

Referentiel3D* Referentiel3D::referentielParTranslation(Vecteur &translation)
{
	translation.changerDeReferentiel(Referentiel3D::ReferentielDeReference());
	
	Referentiel3D* res = new Referentiel3D(origin_+translation,q);
	
	return res;
}

Referentiel3D* Referentiel3D::ReferentielDeReference()
{
	if(!reference)
	{
		Point origin(3);
		Quaternion q(1,0,0,0);
		
		reference = new Referentiel3D(origin,q);
		
		reference->matrice = Matrice::identite(3);
	}
	
	return reference;
}

void Referentiel3D::calculerMatrice()
{
	matrice = q.rotation(ReferentielDeReference()->matrice);
}

Referentiel3D* Referentiel3D::referentielParRotationAngleAxe(Reel angle, Vecteur& axe)
{
	assert(axe.dim() == 3);
	
	Quaternion qrotation = Quaternion::quaternionParAngleEtAxe(angle, axe);
	
	Quaternion qNew = qrotation(q);
	
	Referentiel3D* res = new Referentiel3D(origin(),qNew);
	
	return res;
}

void Referentiel3D::afficher()
{
	if(matrice.dim()==0)
	{
		calculerMatrice();
	}
	
	Referentiel::afficher();
	std::cout << "Quaternion : ";
	q.afficher();
	std::cout << std::endl;
}

const std::size_t Referentiel3D::dim() const
{
	return 3;
}

Vecteur3D Referentiel3D::axes(const int i) const
{
	return Vecteur3D(Referentiel::axes(i));
}

#pragma mark -
#pragma mark Changement de référentiel

Vecteur Referentiel3D::vecteurDeCourantVersReference(const Vecteur &vecteur)
{
	if(matrice.dim()==0)
	{
		calculerMatrice();
	}
	
	return Referentiel::vecteurDeCourantVersReference(vecteur);
}

Vecteur Referentiel3D::vecteurDeReferenceVersCourant(const Vecteur &vecteur)
{
	if(matrice.dim()==0)
	{
		calculerMatrice();
	}
	
	return Referentiel::vecteurDeReferenceVersCourant(vecteur);
}

#pragma mark - Enregistrement

Referentiel3D::Referentiel3D(const void* binaryData, std::size_t size) : Referentiel(Point((void*)(((uint8_t*)binaryData)+8+4*sizeof(Reel)),size - (8+4*sizeof(Reel)))),q(binaryData,8+4*sizeof(Reel))
{
	
}

Referentiel3D::Referentiel3D(const std::vector<uint8_t>& binaryData) : Referentiel(Point((void*)(((uint8_t*)binaryData.data())+8+4*sizeof(Reel)),binaryData.size() - (8+4*sizeof(Reel)))),q(binaryData.data(),8+4*sizeof(Reel))
{
	
}

std::vector<uint8_t> Referentiel3D::toBinary() const
{
	std::vector<uint8_t> res = q.toBinary();
	
	std::vector<uint8_t> binaryOrigin = origin_.toBinary();
	
	res.insert(res.end(), binaryOrigin.begin(), binaryOrigin.end());
	return res;
}

std::string Referentiel3D::toCSVString() const
{
	return origin_.toCSVString()+ q.toCSVString();
}

DataType Referentiel3D::type() const
{
	return typeOfTemplate(this);
}

void Referentiel3D::operator=(const DatabaseData& toCopy)
{
	assert(toCopy.type() == this->type());
	
	this->operator=(reinterpret_cast<const Referentiel3D&>(toCopy));
}