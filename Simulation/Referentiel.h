//
//  Referentiel.h
//  PSC
//
//  Created by Quentin Fiard on 17/12/11.
//  Copyright (c) 2011 École Polytechnique. All rights reserved.
//

#ifndef PSC_Referentiel_h
#define PSC_Referentiel_h

#include "Point.h"
#include "Matrice.h"

// Repère affine orthonormé

class Referentiel
{
public:
	Referentiel(const Referentiel& ref);
	Referentiel(const Point& origin, Matrice& mat);
	
	Referentiel* referentielParTranslation(Vecteur &translation);
	
	//Transformation de vecteur du repère courant vers le repère de référence et vis-versa
	virtual Vecteur vecteurDeCourantVersReference(const Vecteur &vecteur);
	virtual Vecteur vecteurDeReferenceVersCourant(const Vecteur &vecteur);

	static Referentiel* ReferentielDeReference(unsigned char dim);
	
	virtual const unsigned char dim() const;
	
	const Point& origin() const;
	
	Referentiel& operator=(const Referentiel& ref);
	
	void afficher() const;
	
	Vecteur axes(const int i) const;
	
protected:
	
	Referentiel(const Point& origin);
	
	Matrice matrice;
	Point origin_;
};

#endif
