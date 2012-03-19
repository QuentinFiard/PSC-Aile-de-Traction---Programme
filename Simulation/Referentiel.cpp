//
//  Referentiel.cpp
//  PSC
//
//  Created by Quentin Fiard on 17/12/11.
//  Copyright (c) 2011 École Polytechnique. All rights reserved.
//

#include "Referentiel.h"
#include "Referentiel3D.h"

#include <assert.h>

#include <map>

#include <iostream>

std::map<std::size_t, Referentiel*> references;

#pragma mark Constructeurs

Referentiel::Referentiel() : origin_(), matrice()
{
	
}

Referentiel::Referentiel(const Referentiel& ref) : origin_(ref.origin_), matrice(ref.matrice)
{
	
}

Referentiel::Referentiel(const Point& origin) : origin_(origin), matrice((std::size_t)0,0)
{
	if(!origin_.ref()) // On suppose dans ce cas que l'origine est exprimée dans le repère courant
	{
		origin_.initRef(this);
	}
	else
	{
		origin_.changerDeReferentiel(Referentiel::ReferentielDeReference(origin_.dim()));
	}
}

Referentiel::Referentiel(const Point& origin, Matrice& matNew) : origin_(origin), matrice(matNew)
{
	if(!origin_.ref()) // On suppose dans ce cas que l'origine est exprimée dans le repère courant
	{
		origin_.initRef(this);
	}
	else
	{
		origin_.changerDeReferentiel(Referentiel::ReferentielDeReference(origin_.dim()));
	}
	
	if(!matrice.ref())
	{
		matrice.initRef(this);
	}
}

const std::size_t Referentiel::dim() const
{
	return matrice.nbLignes();
}

Referentiel* Referentiel::referentielParTranslation(Vecteur &translation)
{
	translation.changerDeReferentiel(Referentiel::ReferentielDeReference(dim()));
	
	Referentiel* res = new Referentiel(origin_+translation,matrice);
	
	return res;
}

Referentiel* Referentiel::ReferentielDeReference(std::size_t dim)
{
	std::map<std::size_t, Referentiel*>::iterator it = references.find(dim);
	
	if(it == references.end())
	{
		if(dim==3)
		{
			references[dim] = Referentiel3D::ReferentielDeReference();
		}
		else
		{
			Point origin(dim);
			Matrice id(Matrice::identite(dim));
			
			Referentiel* ref = new Referentiel(origin,id);
			
			references[dim] = ref;
		}
	}
	
	return references[dim];
}

void Referentiel::afficher() const
{
	std::cout << "\nAffichage du référentiel" << std::endl;
	printf("Dimension : %ld\n",dim());
	std::cout << "Origin : ";
	origin_.afficher();
	std::cout << std::endl;
	matrice.afficher();
}

Referentiel& Referentiel::operator=(const Referentiel& ref)
{
	origin_ = ref.origin_;
	matrice = ref.matrice;
	
	return *this;
}

const Point& Referentiel::origin() const
{
	return origin_;
}

Vecteur Referentiel::axes(const int i) const
{
	return matrice[i];
}

#pragma mark -
#pragma mark Changement de référentiel

Vecteur Referentiel::vecteurDeCourantVersReference(const Vecteur &vecteur)
{
	Vecteur res(matrice*vecteur);
	
	res.setRef(ReferentielDeReference(dim()));
	
	return res;
}

Vecteur Referentiel::vecteurDeReferenceVersCourant(const Vecteur &vecteur) // La base est supposée orthonormée
{
	Vecteur res(matrice.multiplierParTransposee(vecteur));
	
	res.setRef(this);
	
	return res;
}

#pragma mark - Enregistrement

Referentiel::Referentiel(const void* binaryData, std::size_t size) : matrice(binaryData,*((uint64_t*)binaryData)), origin_((uint8_t*)binaryData+(*((uint64_t*)binaryData)),size-(*((uint64_t*)binaryData)))
{
	
}

Referentiel::Referentiel(const std::vector<uint8_t>& binaryData) : matrice(binaryData.data(),*((uint64_t*)binaryData.data())), origin_((uint8_t*)binaryData.data()+(*((uint64_t*)binaryData.data())),binaryData.size()-(*((uint64_t*)binaryData.data())))
{
	
}

std::vector<uint8_t> Referentiel::toBinary() const
{
	std::vector<uint8_t> res = matrice.toBinary();
	
	std::vector<uint8_t> binaryOrigin = origin_.toBinary();
	
	res.insert(res.end(), binaryOrigin.begin(), binaryOrigin.end());
	
	return res;
}

std::string Referentiel::toCSVString() const
{
	if(this != ReferentielDeReference(dim()))
	{
		return matrice.toCSVString() + origin_.toCSVString();
	}
	else
	{
		return "";
	}
}

DataType Referentiel::type() const
{
	return typeOfTemplate(this);
}

void Referentiel::operator=(const DatabaseData& toCopy)
{
	assert(toCopy.type() == this->type());
	
	this->operator=(reinterpret_cast<const Referentiel&>(toCopy));
}