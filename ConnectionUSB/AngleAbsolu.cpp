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

AngleAbsolu::AngleAbsolu(const AngleAbsolu& aCopier) : angle_(aCopier.angle_)
{
	
}

AngleAbsolu::AngleAbsolu(UINT16 position, const AngleAbsolu* closestFrom)
{
	angle_ = position;
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
	
	double offset = angleModulo2Pi-refModulo2Pi;
	
	if(fabs(offset)>M_PI)
	{
		if(offset>0)
		{
			this->setAngle(ref - 2*M_PI + offset);
		}
		else
		{
			this->setAngle(ref + 2*M_PI + offset);
		}
	}
	else
	{
		this->setAngle(ref + offset);
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

AngleAbsolu& AngleAbsolu::operator=(const AngleAbsolu& angle2)
{
	angle_ = angle2.angle_;
	
	return *this;
}

double AngleAbsolu::operator-(const AngleAbsolu& angle2)
{
	return angle_-angle2.angle_;
}