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
	AngleAbsolu(const AngleAbsolu& aCopier);
	
	AngleAbsolu(UINT16 position, const AngleAbsolu* closestFrom = NULL);
	
	CommandeAngle consigneMoteur() const;
	
	double angle() const;
	
	void setAngle(double radians);
	
	AngleAbsolu& operator=(const AngleAbsolu& angle2);
	double operator-(const AngleAbsolu& angle2);
	
private:
	double angle_;
	
};

typedef struct
{
	AngleAbsolu* lastAngle[3];
	double timeOffset[2];
} FilteredSensorData;

#endif
