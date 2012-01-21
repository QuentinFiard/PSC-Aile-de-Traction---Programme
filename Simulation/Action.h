//
//  Action.h
//  PSC
//
//  Created by Quentin Fiard on 17/12/11.
//  Copyright (c) 2011 École Polytechnique. All rights reserved.
//

#ifndef PSC_Action_h
#define PSC_Action_h

#include "Point.h"
#include "Vecteur.h"

class Action
{
protected:
	Action(Vecteur &force, Vecteur &moment, Point &ptAppli);
private:
	Vecteur force;
	Vecteur moment;
	Point ptAppli;
};

#endif
