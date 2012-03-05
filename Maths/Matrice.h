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
#include "MatrixData.h"

class Matrice : public Vecteur
{
public:
	Matrice();
	Matrice(const void* binaryData, std::size_t size);
	Matrice(const std::vector<uint8_t>& binaryData);
	Matrice(std::size_t n, std::size_t m); // Matrice nulle
	Matrice(const Matrice& mat); // Copie
	
	static Matrice identite(std::size_t n);
	static Matrice diag(std::vector<Reel>& diag);
	
	Vecteur operator*(const Vecteur& vect); // Application à un vecteur
	Matrice operator*(const Matrice& vect); // Produit matriciel
	
	std::size_t nbLignes() const;
	std::size_t nbColonnes() const;
	
	Matrice& operator=(const Matrice& mat);
	
	Matrice transpose(); // Renvoie une nouvelle matrice, transposee de la matrice courante
	
	Vecteur operator[](std::size_t indice) const; // Vecteur colonne
	Vecteur at(std::size_t indice) const; // Vecteur colonne
	
	Reel& operator()(std::size_t i,std::size_t j);
	Reel& at(std::size_t i,std::size_t j);
	Reel operator()(std::size_t i,std::size_t j) const;
	Reel at(std::size_t i,std::size_t j) const;
	
	void afficher() const;
	
	Vecteur multiplierParTransposee(const Vecteur& vect);
	
#pragma mark - DatabaseData
	
	std::vector<uint8_t> toBinary() const;
	std::string toCSVString() const;
	DataType type() const;
	
	void operator=(const DatabaseData& toCopy);
	void operator=(const MatrixData& toCopy);
	
#pragma mark - MatrixData
	
	bool setValueAtIndexWithString(std::size_t i,std::size_t j,std::string& text);
	std::string valueAtIndexToString(std::size_t i,std::size_t j) const;
	
	void setNbLignes(std::size_t n);
	void setNbColonnes(std::size_t n);
	
private:
	std::size_t n_; // n nombre de lignes
	std::size_t m_; // m nombre de colonnes
	
	bool isTransposed;
};

#endif
