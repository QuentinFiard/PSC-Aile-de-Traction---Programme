//
//  State.h
//  PSC
//
//  Created by Quentin Fiard on 26/12/11.
//  Copyright (c) 2011 École Polytechnique. All rights reserved.
//

#ifndef PSC_State_h
#define PSC_State_h


#include "Vecteur.h"

class State : public Vecteur
{
public:
	/*Point Position;
	Quaternion Q;
	Vecteur3D vitesse;
	Vecteur3D Omega;*/
	
	State(Reel theta,Reel thetapt);
	
	Reel theta() const;
	Reel thetapt() const;
};

#endif
