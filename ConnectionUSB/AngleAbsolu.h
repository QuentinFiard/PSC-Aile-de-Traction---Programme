//
//  AngleAbsolu.h
//  PSC
//
//  Created by Quentin Fiard on 02/03/12.
//  Copyright (c) 2012 École Polytechnique. All rights reserved.
//

#ifndef PSC_AngleAbsolu_h
#define PSC_AngleAbsolu_h

#include <stdlib.h>
#include "CommunicationProtocol.h"

class AngleAbsolu
{
public:
	AngleAbsolu(double radians);
	
	AngleAbsolu(SensorStatus status, const AngleAbsolu* closestFrom = NULL);
	
	CommandeAngle consigneMoteur() const;
	
	double angle() const;
	
	void setAngle(double radians);
	
private:
	double angle_;
};

#endif
