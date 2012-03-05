//
//  ForceAero.h
//  PSC
//
//  Created by Quentin Fiard on 05/03/12.
//  Copyright (c) 2012 École Polytechnique. All rights reserved.
//

#ifndef PSC_ForceAero_h
#define PSC_ForceAero_h

#include "Vecteur3D.h"
#include "Referentiel3D.h"

class ForceAero
{
public:
	static Vecteur3D force(Referentiel3D* kite, Vecteur3D vent,Vecteur3D vitesse_kite);
};

#endif
