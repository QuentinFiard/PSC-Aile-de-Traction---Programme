//
//  Matrice.cpp
//  PSC
//
//  Created by Quentin Fiard on 20/12/11.
//  Copyright (c) 2011 École Polytechnique. All rights reserved.
//

#include "Matrice.h"
#include <iostream>
#include <assert.h>
#include <math.h>

#include <boost/lexical_cast.hpp>

Matrice::Matrice() : Vecteur(),n_(0),m_(0),isTransposed(false)
{
	
}

Matrice::Matrice(std::size_t n, std::size_t m) : Vecteur(n*m),n_(n),m_(m),isTransposed(false) // Matrice nulle
{
	
}

Matrice::Matrice(const Matrice& mat) : Vecteur(mat),n_(mat.n_),m_(mat.m_),isTransposed(mat.isTransposed)  // Copie
{
	
}

Matrice& Matrice::operator=(const Matrice& mat)
{
	Vecteur::operator=(mat);
	
	n_ = mat.n_;
	m_ = mat.m_;
	
	isTransposed = mat.isTransposed;
	
	return *this;
}

Matrice Matrice::identite(std::size_t n)
{
	Matrice res(n,n);
	
	for(int i=0 ; i<n ; i++)
	{
		res(i,i) = 1;
	}
	
	return res;
}

Matrice Matrice::diag(std::vector<Reel>& diag)
{
	Matrice res(diag.size(),diag.size());
	
	for(int i=0 ; i<diag.size() ; i++)
	{
		res(i,i) = diag[i];
	}
	
	return res;
}

Vecteur Matrice::operator*(const Vecteur& vect) // Produit matriciel
{
	assert(nbColonnes() == vect.dim());
	
	std::vector<Reel> coordNew(nbLignes());
	
	for(int i=0 ; i<nbLignes() ; i++)
	{
		for(int j=0 ; j<nbColonnes() ; j++)
		{
			coordNew[i] += at(i,j)*vect[j];
		}
	}
	
	return Vecteur(coordNew);
}

Vecteur Matrice::multiplierParTransposee(const Vecteur& vect)
{
	assert(nbLignes() == vect.dim());
	
	std::vector<Reel> coordNew(nbColonnes());
	
	for(int i=0 ; i<nbColonnes() ; i++)
	{
		for(int j=0 ; j<nbLignes() ; j++)
		{
			coordNew[i] += at(j,i)*vect[j];
		}
	}
	
	return Vecteur(coordNew);
}

Matrice Matrice::operator*(const Matrice& mat) // Produit matriciel
{
	assert(nbColonnes() == mat.nbLignes());
	
	Matrice res(nbLignes(),mat.nbColonnes());
	
	for(int i=0 ; i<nbLignes() ; i++)
	{
		for(int j=0 ; j<mat.nbColonnes() ; j++)
		{
			for(int k=0 ; k<nbColonnes() ; k++)
			{
				res(i,j) += at(i,k)*mat(k,j);
			}
		}
	}
	
	return res;
	
}

Vecteur Matrice::operator[](std::size_t indice) const
{
	if(!isTransposed)
	{
		std::vector<Reel>::const_iterator debut(coord.begin() + indice*nbLignes());
		std::vector<Reel> newCoord(debut,debut+nbLignes());
		return Vecteur(newCoord);
	}
	else
	{
		std::vector<Reel> newCoord(nbLignes());
		for(int i=0 ; i<nbLignes() ; i++)
		{
			newCoord[i] = at(i,indice);
		}
		return Vecteur(newCoord);
	}
	
	
}

Vecteur Matrice::at(std::size_t indice) const
{
	return operator[](indice);
}

Reel& Matrice::operator()(std::size_t i,std::size_t j)
{
	if(!isTransposed)
	{
		return Vecteur::operator[](i*nbColonnes()+j);
	}
	else
	{
		return Vecteur::operator[](j*nbColonnes()+i);
	}
}

Reel& Matrice::at(std::size_t i,std::size_t j)
{
	return operator()(i,j);
}

Reel Matrice::operator()(std::size_t i,std::size_t j) const
{
	if(!isTransposed)
	{
		return Vecteur::operator[](i*nbColonnes()+j);
	}
	else
	{
		return Vecteur::operator[](j*nbColonnes()+i);
	}
}

Reel Matrice::at(std::size_t i,std::size_t j) const
{
	return operator()(i,j);
}

std::size_t Matrice::nbLignes() const
{
	if(!isTransposed)
	{
		return n_;
	}
	else
	{
		return m_;
	}
}

std::size_t Matrice::nbColonnes() const
{
	if(!isTransposed)
	{
		return m_;
	}
	else
	{
		return n_;
	}
}

void Matrice::afficher() const
{
	if(nbColonnes()==0 || nbLignes() == 0)
	{
		std::cout << "Matrice nulle" << std::endl;
	}
	else
	{
		for(int i=0 ; i<nbLignes() ; i++)
		{
			printf("%lf",at(i,0));
			for(int j=1 ; j<nbColonnes() ; j++)
			{
				printf(" %lf",at(i,j));
			}
			std::cout << std::endl;
		}
	}
}

Matrice Matrice::transpose()
{
	Matrice res(*this);
	
	res.isTransposed = !isTransposed;
	
	return res;
}

#pragma mark - Enregistrement

Matrice::Matrice(const void* binaryData, std::size_t size) : n_(*((uint64_t*)binaryData+1)),m_(*((uint64_t*)binaryData+2)),Vecteur(n_*m_),isTransposed(false)
{
	
	Reel* ptr = (Reel*)((uint64_t*)binaryData+3);
	
	coord = std::vector<Reel>(ptr,ptr+n_*m_);
}

Matrice::Matrice(const std::vector<uint8_t>& binaryData) : n_(*((uint64_t*)binaryData.data()+1)),m_(*((uint64_t*)binaryData.data()+2)),Vecteur(n_*m_),isTransposed(false)
{
	Reel* ptr = (Reel*)((uint64_t*)binaryData.data()+3);
	
	coord = std::vector<Reel>(ptr,ptr+n_*m_);
}

std::vector<uint8_t> Matrice::toBinary() const
{
	std::vector<uint8_t> res;
	
	uint64_t n = nbLignes();
	uint64_t m = nbColonnes();
	
	uint64_t size = 3*8 + sizeof(Reel)*n*m;
	
	res.reserve(size);
	
	uint8_t* ptr = (uint8_t*)&size;
	
	res.insert(res.end(), ptr, ptr+8);
	
	ptr = (uint8_t*)&n;
	
	res.insert(res.end(), ptr, ptr+8);
	
	ptr = (uint8_t*)&m;
	
	res.insert(res.end(), ptr, ptr+8);
	
	for(std::size_t i=0 ; i<nbLignes() ; i++)
	{
		for(std::size_t j=0 ; j<nbColonnes() ; j++)
		{
			Reel tmp = at(i, j);
			ptr = (uint8_t*)&tmp;
			res.insert(res.end(), ptr, ptr+sizeof(Reel));
		}
	}
	
	return res;
}

std::string Matrice::toCSVString() const
{
	std::string res;
	
	if(nbLignes()==0 || nbColonnes() ==0)
	{
		return ";";
	}
	
	for(int i=0 ; i<nbLignes() ; i++)
	{
		for(int j=0 ; j<nbColonnes() ; j++)
		{
			res += boost::lexical_cast<std::string>(at(i,j));
		}
	}
	
	return res;
}

DataType Matrice::type() const
{
	return typeOfTemplate(this);
}

void Matrice::operator=(const DatabaseData& toCopy)
{
	assert(toCopy.type() == this->type());
	
	this->operator=(reinterpret_cast<const Matrice&>(toCopy));
}

void Matrice::operator=(const MatrixData& toCopy)
{
	assert(toCopy.type() == this->type());
	
	this->operator=(reinterpret_cast<const Matrice&>(toCopy));
}

#pragma mark - MatrixData

bool Matrice::setValueAtIndexWithString(std::size_t i,std::size_t j,std::string& text)
{
	try
	{
		at(i,j) = boost::lexical_cast<Reel>(text);
		return true;
	}
	catch(boost::exception &e)
	{
		return false;
	}
}

std::string Matrice::valueAtIndexToString(std::size_t i,std::size_t j) const
{
	return boost::lexical_cast<std::string>(at(i,j));
}

void Matrice::setNbLignes(std::size_t n)
{
	std::vector<Reel> newCoord(n*nbColonnes());
	for(int i=0 ; i<fmin(nbLignes(),n) ; i++)
	{
		for(int j=0 ; j<nbColonnes() ; j++)
		{
			newCoord[nbColonnes()*i+j] = at(i,j);
		}
	}
	
	for(int i=fmin(nbLignes(),n) ; i<n ; i++)
	{
		for(int j=0 ; j<nbColonnes() ; j++)
		{
			newCoord[nbColonnes()*i+j] = 0;
		}
	}
	
	m_ = nbColonnes();
	n_ = n;
	
	coord = newCoord;
	isTransposed = false;
	
}

void Matrice::setNbColonnes(std::size_t m)
{
	std::vector<Reel> newCoord(nbLignes()*m);
	for(int i=0 ; i<nbLignes() ; i++)
	{
		for(int j=0 ; j<fmin(nbColonnes(),m) ; j++)
		{
			newCoord[m*i+j] = at(i,j);
		}
		for(int j=fmin(nbColonnes(),m) ; j<m ; j++)
		{
			newCoord[m*i+j] = 0;
		}
	}
	
	n_ = nbLignes();
	m_ = m;
	
	coord = newCoord;
	isTransposed = false;
}

#pragma mark - Recopie des opérations vectorielles

Matrice Matrice::operator+(const Matrice &mat2) // Somme vectorielle
{
	assert(nbLignes() == mat2.nbLignes() && nbColonnes() == mat2.nbColonnes());
	
	Matrice res(nbLignes(),nbColonnes());
	
	for(int i=0 ; i<nbLignes() ; i++)
	{
		for(int j=0 ; j<nbColonnes() ; j++)
		{
			res(i,j) = at(i, j) + mat2.at(i, j);
		}
	}
	
	return res;
}

Matrice Matrice::operator+(const Matrice &mat2) const // Somme vectorielle
{
	assert(nbLignes() == mat2.nbLignes() && nbColonnes() == mat2.nbColonnes());
	
	Matrice res(nbLignes(),nbColonnes());
	
	for(int i=0 ; i<nbLignes() ; i++)
	{
		for(int j=0 ; j<nbColonnes() ; j++)
		{
			res(i,j) = at(i, j) + mat2.at(i, j);
		}
	}
	
	return res;
}

Matrice Matrice::operator*(const Reel lambda) const // Produit par un scalaire
{
	Matrice res(nbLignes(),nbColonnes());
	
	for(int i=0 ; i<nbLignes() ; i++)
	{
		for(int j=0 ; j<nbColonnes() ; j++)
		{
			res(i,j) = at(i, j)*lambda;
		}
	}
	
	return res;
}

Matrice Matrice::operator-() const // Moins unaire
{
	Matrice res(nbLignes(),nbColonnes());
	
	for(int i=0 ; i<nbLignes() ; i++)
	{
		for(int j=0 ; j<nbColonnes() ; j++)
		{
			res(i,j) = -at(i, j);
		}
	}
	
	return res;
}

//Opérateurs auxiliaires
Matrice Matrice::operator-(const Matrice &mat2) // Différence
{
	return *this + (-mat2);
}

Matrice Matrice::operator-(const Matrice &mat2) const // Différence
{
	return *this + (-mat2);
}

Matrice Matrice::operator/(const Reel lambda) const // Division par un scalaire
{
	return *this*(1/lambda);
}

void Matrice::operator+=(const Matrice &mat2)
{
	assert(nbLignes() == mat2.nbLignes() && nbColonnes() == mat2.nbColonnes());
	
	for(int i=0 ; i<nbLignes() ; i++)
	{
		for(int j=0 ; j<nbColonnes() ; j++)
		{
			at(i, j) += mat2.at(i, j);
		}
	}
}

void Matrice::operator*=(const Reel lambda)
{
	for(int i=0 ; i<nbLignes() ; i++)
	{
		for(int j=0 ; j<nbColonnes() ; j++)
		{
			at(i, j) *= lambda;
		}
	}
}

void Matrice::operator-=(const Matrice &mat2)
{
	operator+(-mat2);
}

void Matrice::operator/=(const Reel lambda)
{
	operator*=(1/lambda);
}

Matrice operator*(const Reel lambda, const Matrice& mat)
{
	return mat*lambda;
}