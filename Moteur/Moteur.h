//
//  Moteur.h
//  PSC
//
//  Created by Quentin Fiard on 23/04/12.
//  Copyright (c) 2012 École Polytechnique. All rights reserved.
//

#ifndef PSC_Moteur_h
#define PSC_Moteur_h

#include "CapteurAngulaire.h"
#include "Source.h"

#include "CommunicationProtocol.h"

#define NUM_SPEED 1

class Moteur : public CapteurAngulaire
{
public:
	
	static Moteur* shared();
	
	static double maxSpeed();
	
	bool update();
	
	void saveLastValue();
	
	static void setSpeed(double speed);
	static void setSignal(double signal);
	static void setPosition(double position);
	
	static TypeAsservissement typeAsservissement();
	static void setTypeAsservissement(TypeAsservissement type);
	
protected:
	
	void signalCorrection();
	void setSpeed_(double speed);
	void setSignal_(double signal);
	
	
	
	Motor moteur;
	
	double* goal;
	
	double signal;
	
	double integralTerm;
	
	Moteur(Sensor sensor);
	
	double speeds[NUM_SPEED];
	boost::posix_time::ptime date[NUM_SPEED];
	
	unsigned int nextSpeed;
	unsigned int numSpeed;
	
	double meanSpeed;
	boost::posix_time::ptime meanSpeedDate;
	double lastMeanSpeed;
	boost::posix_time::ptime lastMeanSpeedDate;
	
	Source< NumericValue<double> >* speed_output;
};

#endif
