//
//  AngleAbsolu.cpp
//  PSC
//
//  Created by Quentin Fiard on 17/03/12.
//  Copyright (c) 2012 École Polytechnique. All rights reserved.
//

#include "AngleAbsolu.h"
#include <iostream>
#include <math.h>

static AngleAbsolu* zero = NULL;

AngleAbsolu::AngleAbsolu(double radians)
{
	this->setAngle(radians);
}

AngleAbsolu::AngleAbsolu(SensorStatus status, const AngleAbsolu* closestFrom)
{
	angle_ = status.position;
	angle_ *= (2*M_PI)/MAX_SENSOR_ANGLE;
	
	double absoluteAngle = this->angle();
	
	double angleModulo2Pi = absoluteAngle - 2*M_PI*floor(absoluteAngle/(2*M_PI)); // Modulo 2π
	
	double ref;
	
	if(!closestFrom)
	{
		if(zero)
		{
			ref = zero->angle_;
		}
		else
		{
			ref = 0;
		}
	}
	else
	{
		ref = closestFrom->angle();
	}
	
	double refModulo2Pi = ref - 2*M_PI*floor(ref/(2*M_PI)); // Modulo 2π
	
	double offset = fabs(refModulo2Pi-angleModulo2Pi);
	
	if(offset>M_PI)
	{
		this->setAngle(ref + 2*M_PI - (angleModulo2Pi-refModulo2Pi));
	}
	else
	{
		this->setAngle(ref + (angleModulo2Pi-refModulo2Pi));
	}
}

double AngleAbsolu::angle() const
{
	if(zero)
	{
		return angle_ - zero->angle_;
	}
	return angle_;
}

void AngleAbsolu::setAngle(double radians)
{
	if(zero)
	{
		angle_ = radians + zero->angle_;
	}
	else
	{
		angle_ = radians;
	}
}

CommandeAngle AngleAbsolu::consigneMoteur() const
{
	CommandeAngle res;
	
	res.turn = floor(angle_/(2*M_PI));
	res.positionInTurn = angle_ - 2*M_PI*res.turn;
	
	return res;
}