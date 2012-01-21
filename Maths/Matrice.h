//
//  Matrice.h
//  PSC
//
//  Created by Quentin Fiard on 20/12/11.
//  Copyright (c) 2011 École Polytechnique. All rights reserved.
//

#ifndef PSC_Matrice_h
#define PSC_Matrice_h

#include "Vecteur.h"

class Matrice : public Vecteur
{
public:
	Matrice(unsigned char n, unsigned char m); // Matrice nulle
	Matrice(const Matrice& mat); // Copie
	
	static Matrice identite(unsigned char n);
	static Matrice diag(std::vector<Reel>& diag);
	
	Vecteur operator*(const Vecteur& vect); // Application à un vecteur
	Matrice operator*(const Matrice& vect); // Produit matriciel
	
	unsigned char nbLignes() const;
	unsigned char nbColonnes() const;
	
	Matrice& operator=(const Matrice& mat);
	
	Matrice transpose(); // Renvoie une nouvelle matrice, transposee de la matrice courante
	
	Vecteur operator[](unsigned char indice) const; // Vecteur colonne
	Vecteur at(unsigned char indice) const; // Vecteur colonne
	
	Reel& operator()(unsigned char i,unsigned char j);
	Reel& at(unsigned char i,unsigned char j);
	Reel operator()(unsigned char i,unsigned char j) const;
	Reel at(unsigned char i,unsigned char j) const;
	
	void afficher() const;
	
	Vecteur multiplierParTransposee(const Vecteur& vect);
	
private:
	unsigned char n_; // n nombre de lignes
	unsigned char m_; // m nombre de colonnes
	
	bool isTransposed;
};

#endif
