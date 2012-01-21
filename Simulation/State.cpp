//
//  State.cpp
//  PSC
//
//  Created by Quentin Fiard on 26/12/11.
//  Copyright (c) 2011 École Polytechnique. All rights reserved.
//

#include "State.h"

State::State(Reel theta,Reel thetapt) : Vecteur(theta,thetapt)
{

}

Reel State::theta() const
{
	return operator[](0);
}

Reel State::thetapt() const
{
	return operator[](1);
}