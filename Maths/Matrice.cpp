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

Matrice::Matrice(unsigned char n, unsigned char m) : Vecteur(n*m),n_(n),m_(m),isTransposed(false) // Matrice nulle
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
}

Matrice Matrice::identite(unsigned char n)
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

Vecteur Matrice::operator[](unsigned char indice) const
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

Vecteur Matrice::at(unsigned char indice) const
{
	return operator[](indice);
}

Reel& Matrice::operator()(unsigned char i,unsigned char j)
{
	if(!isTransposed)
	{
		return Vecteur::operator[](i*nbLignes()+j);
	}
	else
	{
		return Vecteur::operator[](j*nbLignes()+i);
	}
}

Reel& Matrice::at(unsigned char i,unsigned char j)
{
	return operator()(i,j);
}

Reel Matrice::operator()(unsigned char i,unsigned char j) const
{
	if(!isTransposed)
	{
		return Vecteur::operator[](i*nbLignes()+j);
	}
	else
	{
		return Vecteur::operator[](j*nbLignes()+i);
	}
}

Reel Matrice::at(unsigned char i,unsigned char j) const
{
	return operator()(i,j);
}

unsigned char Matrice::nbLignes() const
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

unsigned char Matrice::nbColonnes() const
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