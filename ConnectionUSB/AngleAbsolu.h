//
//  AngleAbsolu.h
//  PSC
//
//  Created by Quentin Fiard on 02/03/12.
//  Copyright (c) 2012 École Polytechnique. All rights reserved.
//

#ifndef PSC_AngleAbsolu_h
#define PSC_AngleAbsolu_h

#include "CommunicationProtocol.h"

class AngleAbsolu
{
public:
	AngleAbsolu(double radians);
	
	CommandeAngle consigneMoteur();
	
private:
	double angle;
}

#endif
