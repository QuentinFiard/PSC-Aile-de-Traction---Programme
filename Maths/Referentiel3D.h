//
//  Referentiel3D.h
//  PSC
//
//  Created by Quentin Fiard on 22/12/11.
//  Copyright (c) 2011 École Polytechnique. All rights reserved.
//

#ifndef PSC_Referentiel3D_h
#define PSC_Referentiel3D_h

#include "Referentiel.h"
#include "Quaternion.h"

class Referentiel3D : public Referentiel
{
public:
	//Referentiel3D(const Referentiel& ref);
	Referentiel3D(const Referentiel3D& ref);
	Referentiel3D(const Point &origin, Quaternion &q);
	
	Referentiel3D* referentielParTranslation(Vecteur &translation);
	
	Referentiel3D* referentielParRotationAngleAxe(Reel angle, Vecteur& axe);
	
	Vecteur vecteurDeCourantVersReference(const Vecteur &vecteur);
	Vecteur vecteurDeReferenceVersCourant(const Vecteur &vecteur);
	
	static Referentiel3D* ReferentielDeReference();
	
	void calculerMatrice();
	
	void afficher();
	
	Vecteur3D axes(const int i) const;
	
	const unsigned char dim() const;
	
private:
	Quaternion q; // Repère l'orientation du repère direct du référentiel par rapport au repère de référence.
};

#endif
