//
//  Solide.h
//  PSC
//
//  Created by Quentin Fiard on 26/12/11.
//  Copyright (c) 2011 École Polytechnique. All rights reserved.
//

#ifndef PSC_Solide_h
#define PSC_Solide_h

#include "Matrice.h"
#include "Point.h"
#include "Vecteur3D.h"
#include "Referentiel.h"

#include "State.h"

#include <vector>

class Solide
{
	Solide(Reel Masse, Matrice& Inerties, Referentiel* RefBarycentrique, Point& CDG);
	Solide(Solide &ref, Vecteur& vecteurEtat);
		   
	State dynamique(const State& state);
	
	Reel Masse() const;
	const Matrice& MatriceInerties() const;
	
protected:
	
	//Solide();
	
	//Constantes
	Reel Masse_;
	Matrice MatriceInerties_;
};

#endif
