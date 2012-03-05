//
//  Vecteur3D.h
//  PSC
//
//  Created by Quentin Fiard on 20/12/11.
//  Copyright (c) 2011 École Polytechnique. All rights reserved.
//

#ifndef PSC_Vecteur3D_h
#define PSC_Vecteur3D_h

#include "Vecteur.h"

class Quaternion;
class Referentiel3D;

class Vecteur3D : public Vecteur
{
public:
	Vecteur3D();
	Vecteur3D(Reel x, Reel y, Reel z, Referentiel* ref=NULL);
	Vecteur3D(std::vector<Reel> &coord, Referentiel* ref=NULL);
	Vecteur3D(const Vecteur& vect);
	Vecteur3D operator^(const Vecteur3D &vecteur2); //Produit vectoriel
	Vecteur3D operator^(const Vecteur3D &vecteur2) const; //Produit vectoriel
	
	Reel angleNonOriente(const Vecteur3D& vect2);
	
	Vecteur3D(const void* binaryData, std::size_t size);
	Vecteur3D(const std::vector<uint8_t>& binaryData);
	
#pragma mark - DatabaseData
	DataType type() const;
	void operator=(const DatabaseData& toCopy);
	
	bool isResizable();
	
// Conversion Vecteur3D vers Quaternion
};

#endif
