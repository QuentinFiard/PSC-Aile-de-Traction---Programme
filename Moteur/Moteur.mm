//
//  Moteur.cpp
//  PSC
//
//  Created by Quentin Fiard on 23/04/12.
//  Copyright (c) 2012 École Polytechnique. All rights reserved.
//

#include "Moteur.h"
#include "Recorder.h"

#include "Vecteur.h"
#include "VectorTemplate.h"

#include "ConnectionUSB.h"

#include "Joystick.h"

#define MAX_SPEED 15 // rad/s

using namespace boost::posix_time;

static Moteur* shared_ = NULL;

Moteur* Moteur::shared()
{
	if(shared_==NULL)
	{
		if(Database::hasConfigurationField("MOTEUR") && Database::hasConfigurationField("CAPTEUR_ROTATION_MOTEUR"))
		{
			shared_ = new Moteur(Database::getConfigurationFieldValue<int>("CAPTEUR_ROTATION_MOTEUR"));
		}
	}
	return shared_;
}

Moteur::Moteur(Sensor sensor) : CapteurAngulaire(CAPTEUR_ROTATION_MOTEUR, sensor), nextSpeed(0), numSpeed(0), meanSpeed(0), goal(NULL), signal(0), integralTerm(0), lastMeanSpeed(NULL), speed_output(NULL)
{
	moteur = Database::getConfigurationFieldValue<int>("MOTEUR");
	
	if(!Database::hasConfigurationField("MOTOR_SIGNAL_OFFSET_HIGH"))
	{
		NumericValue<double> toSave(0.18);
		Database::setConfigurationField("MOTOR_SIGNAL_OFFSET_HIGH",&toSave);
	}
	
	if(!Database::hasConfigurationField("MOTOR_SIGNAL_OFFSET_LOW"))
	{
		NumericValue<double> toSave(0.05);
		Database::setConfigurationField("MOTOR_SIGNAL_OFFSET_LOW",&toSave);
	}
}

double Moteur::maxSpeed()
{
	return MAX_SPEED;
}

bool Moteur::update()
{
	if(lastAcquisition() != NULL)
	{
		ptime t0(*(lastAcquisition()));
		AngleAbsolu y0(*(lastValue()));
		
		this->CapteurAngulaire::update();
		
		ptime& t1(*(lastAcquisition()));
		AngleAbsolu& y1(*(lastValue()));
		
		double dy = y1 - y0;
		time_duration duration = t1 - t0;
		
		double dt =  static_cast<double>(duration.ticks())/1000000;
		
		if(dt!=0)
		{
			double speed = dy/dt;
			double oldSpeed = speeds[nextSpeed];
			speeds[nextSpeed] = speed;
			date[nextSpeed] = t0 + duration/2;
			
			nextSpeed++;
			nextSpeed %= NUM_SPEED;
			
			meanSpeed *= numSpeed;
			meanSpeed += speed;
			
			if(numSpeed<NUM_SPEED)
			{
				numSpeed++;
			}
			else
			{
				meanSpeed -= oldSpeed;
			}
			
			meanSpeed /= numSpeed;
			
			if(numSpeed==NUM_SPEED)
			{
				meanSpeedDate = date[nextSpeed] + (t1 - date[nextSpeed])/2;
			}
			else
			{
				meanSpeedDate = date[0] + (t1 - date[0])/2;
			}
			
			signalCorrection();
			
			lastMeanSpeed = meanSpeed;
			lastMeanSpeedDate = meanSpeedDate;
		}
	}
	else
	{
		this->CapteurAngulaire::update();
	}
	
	return true;
}

void Moteur::saveLastValue()
{
	this->CapteurAngulaire::saveLastValue();
	if(Recorder::isRecording())
	{
		if(numSpeed == NUM_SPEED)
		{
			Record* r = Recorder::record();
			
			if(r->privateDate() <= meanSpeedDate)
			{
				if(speed_output == NULL || speed_output->recordID() != r->ID())
				{
					if(speed_output != NULL)
					{
						delete speed_output;
					}
					
					speed_output = new Source< NumericValue<double> >(r,"VITESSE_MOTEUR",1);
					speed_output->save();
				}			
				NumericValue<double> data(meanSpeed);
				Donnee< NumericValue<double> > toSave(speed_output,meanSpeedDate,data);
				toSave.save();
			}
		}
	}
}

void Moteur::setSpeed(double speed)
{
	if(shared())
	{
		shared()->setSpeed_(speed);
	}
}

void Moteur::setSpeed_(double speed)
{
	assert(speed <= MAX_SPEED && speed >= -MAX_SPEED);
	
	if(goal==NULL)
	{
		goal = new double;
	}
	
	*goal = speed;
}

void Moteur::setSignal(double signal)
{
	if(shared())
	{
		shared()->setSignal_(signal);
	}
}

void Moteur::setSignal_(double signal)
{
	assert(signal <= 1 && signal >= -1);
	
	if(goal==NULL)
	{
		goal = new double;
	}
	
	*goal = signal;
}

void Moteur::setPosition(double position)
{
	
}

TypeAsservissement Moteur::typeAsservissement()
{
	if(Database::hasConfigurationField("TypeAsservissement"))
	{
		return Database::getConfigurationFieldValue<int>("TypeAsservissement");
	}
	else
	{
		NumericValue<int> toSave(ASSERVISSEMENT_SANS);
		Database::setConfigurationField("TypeAsservissement",&toSave);
		return ASSERVISSEMENT_SANS;
	}
}

void Moteur::setTypeAsservissement(TypeAsservissement type)
{
	NumericValue<int> toSave(type);
	Database::setConfigurationField("TypeAsservissement",&toSave);
	
	Moteur* moteur;
	
	if((moteur = shared()))
	{
		moteur->integralTerm = 0;
		moteur->signal = 0;
	}
}

void Moteur::signalCorrection()
{
	if(goal!=NULL && (!Joystick::isActivated() || Joystick::isControllingMotor()))
	{
		switch(typeAsservissement())
		{
			case ASSERVISSEMENT_VITESSE :
			{
				if(Database::hasConfigurationField("SpeedPID") && numSpeed == NUM_SPEED)
				{
					VectorTemplate<Vecteur>* coeffsPIDFromDatabase = static_cast< VectorTemplate<Vecteur>* >(Database::getConfigurationField("SpeedPID"));
					
					Vecteur coeffs = coeffsPIDFromDatabase->vector();
					
					double gainProportionnel = coeffs[0];
					double gainDifferentiel = coeffs[1];
					double gainIntegral = coeffs[2];
					
					time_duration duration = meanSpeedDate - lastMeanSpeedDate;
					
					double dt =  static_cast<double>(duration.ticks())/1000000;
					
					double error = meanSpeed - *goal;
					
					double differentialTerm = gainDifferentiel*(meanSpeed-lastMeanSpeed)/dt;
					
					integralTerm += gainIntegral*error*dt;
					
					if(integralTerm>1)
					{
						integralTerm = 1;
					}
					if(integralTerm<-1)
					{
						integralTerm = -1;
					}
					
					signal = signal - gainProportionnel*error - differentialTerm - integralTerm;
					
					if(signal>1)
					{
						signal=1;
					}
					if(signal<-1)
					{
						signal=-1;
					}
					
					double tmp = signal;
					
					if(signal<Database::getConfigurationFieldValue<double>("MOTOR_SIGNAL_OFFSET_HIGH") && signal > -Database::getConfigurationFieldValue<double>("MOTOR_SIGNAL_OFFSET_LOW"))
					{
						tmp = 0;
					}
					
					if(ConnectionUSB::isConnected())
					{
						ConnectionUSB::setSignalForMotor(tmp,moteur);
					}
				}
				break;
			}
				
			case ASSERVISSEMENT_SANS :
			{
				if(ConnectionUSB::isConnected())
				{
					if(*goal<Database::getConfigurationFieldValue<double>("MOTOR_SIGNAL_OFFSET_HIGH") && *goal > -Database::getConfigurationFieldValue<double>("MOTOR_SIGNAL_OFFSET_LOW"))
					{
						*goal = 0;
					}
					
					ConnectionUSB::setSignalForMotor(*goal,moteur);
				}
				break;
			}
				
			default:
				break;
		}
	}
	else
	{
		if(ConnectionUSB::isConnected())
		{
			ConnectionUSB::setSignalForMotor(0,moteur);
		}
	}
}