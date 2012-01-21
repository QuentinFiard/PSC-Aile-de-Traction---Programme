//
//  AileTraction.h
//  PSC
//
//  Created by Quentin Fiard on 26/12/11.
//  Copyright (c) 2011 École Polytechnique. All rights reserved.
//

#ifndef AileTraction_h
#define AileTraction_h

#include "Solide.h"

class AileTraction : public Solide
{
public:
	AileTraction();
	
	Referentiel* RefBarycentrique();
	
	State dynamique(const State& state);
};

#endif
