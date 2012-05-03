//
//  CapteurAngulaire.cpp
//  PSC
//
//  Created by Quentin Fiard on 18/03/12.
//  Copyright (c) 2012 École Polytechnique. All rights reserved.
//

#include "CapteurAngulaire.h"
#include <vector>

#include "Database.h"

#include "ConnectionUSB.h"

#import "ArrowView.h"

#include <boost/bind.hpp>

#include <math.h>

#include "Recorder.h"
#include "Source.h"
#include "Donnee.h"

#include "Moteur.h"

#define TIME_OFFSET_MICROSECONDS 500 // Theorically 384, but accounting for acquisition time

#define SENSOR_UPDATE_PERIOD 1000 // Microseconds

#define MAX_SPEED (200*M_PI)

using namespace boost::posix_time;
using namespace boost::asio;

static int update_period = -1;

static std::vector<CapteurAngulaire*> capteurs;

CapteurAngulaire::CapteurAngulaire(GrandeurCapteur grandeur, Sensor sensor) : Capteur(grandeur), sensorNumber_(sensor)
{
	view = nil;
	lastValue_ = NULL;
	output = NULL;
}

void CapteurAngulaire::prepareListeCapteurs()
{	
	if(capteurs.size()==0)
	{
		for(int i=0 ; i<6 ; i++)
		{
			CapteurAngulaire* capteur;
			
			capteur = new CapteurAngulaire(CAPTEUR_NON_CONNECTE,i);
			
			capteurs.push_back(capteur);
		}
		
		if(Database::hasConfigurationField("CAPTEUR_DIRECTION_VENT"))
		{
			capteurs[Database::getConfigurationFieldValue<int>("CAPTEUR_DIRECTION_VENT")]->setGrandeurMesuree(CAPTEUR_DIRECTION_VENT);
		}
		
		if(Database::hasConfigurationField("CAPTEUR_VITESSE_VENT"))
		{
			capteurs[Database::getConfigurationFieldValue<int>("CAPTEUR_VITESSE_VENT")]->setGrandeurMesuree(CAPTEUR_VITESSE_VENT);
		}
		
		if(Database::hasConfigurationField("CAPTEUR_ROTATION_MOTEUR"))
		{
			capteurs[Database::getConfigurationFieldValue<int>("CAPTEUR_ROTATION_MOTEUR")]->setGrandeurMesuree(CAPTEUR_ROTATION_MOTEUR);
		}
		
		if(Database::hasConfigurationField("CAPTEUR_THETA"))
		{
			capteurs[Database::getConfigurationFieldValue<int>("CAPTEUR_THETA")]->setGrandeurMesuree(CAPTEUR_THETA);
		}
		
		if(Database::hasConfigurationField("CAPTEUR_PHI"))
		{
			capteurs[Database::getConfigurationFieldValue<int>("CAPTEUR_PHI")]->setGrandeurMesuree(CAPTEUR_PHI);
		}
	}
}

CapteurAngulaire* CapteurAngulaire::sharedWithSensorNumber(Sensor sensor)
{
	prepareListeCapteurs();
	
	return capteurs[sensor];
	
	return NULL;
}

CapteurAngulaire* CapteurAngulaire::shared(GrandeurCapteur grandeur)
{
	prepareListeCapteurs();
	
	if(grandeur == CAPTEUR_ROTATION_MOTEUR)
	{
		return Moteur::shared();
	}
	
	for(int i=0 ; i<capteurs.size() ; i++)
	{
		if(capteurs[i]->grandeurMesuree() == grandeur)
		{
			return capteurs[i];
		}
	}
	
	return NULL;
}

void CapteurAngulaire::updateAll()
{
	prepareListeCapteurs();
	
	for(int i=0 ; i<capteurs.size() ; i++)
	{
		capteurs[i]->update();
	}
}

Sensor CapteurAngulaire::sensorNumber()
{
	return sensorNumber_;
}

bool CapteurAngulaire::update()
{
	if(grandeur_ != CAPTEUR_NON_CONNECTE)
	{
		AngleAbsolu* newValue = ConnectionUSB::readPositionFromSensor(sensorNumber(),lastValue_);
		
		if(newValue)
		{
			ptime currentDate(microsec_clock::local_time());
			
			if(lastAcquisition_)
			{
				ptime& lastDate = *lastAcquisition_;
				double offset = abs(*lastValue_- *newValue);
				time_duration duration = currentDate - lastDate;
				double time =  static_cast<double>(duration.ticks())/1000000;
				/*if(offset/time>MAX_SPEED)
				{
					delete newValue;
					return true;
				}*/
			}
			
			if(lastValue_)
			{
				delete lastValue_;
			}
			
			lastValue_ = newValue;
			
			if(!lastAcquisition_)
			{
				lastAcquisition_ = new ptime(currentDate);
			}
			else
			{
				*lastAcquisition_ = currentDate;
			}
			
			saveLastValue();
			
			if(view)
			{
				double angle = lastValue_->angle();
				//std::cout << "Angle : " << angle << std::endl;
				[view setAngle:angle];
			}
			
			return true;
		}
	}
	
	return false;
}

bool CapteurAngulaire::checkConnection()
{
	return update();
}

boost::posix_time::ptime* CapteurAngulaire::lastAcquisition()
{
	return lastAcquisition_;
}

AngleAbsolu* CapteurAngulaire::lastValue()
{
	return lastValue_;
}

void CapteurAngulaire::setArrowView(ArrowView* newView)
{
	[view release];
	view = [newView retain];
}

void CapteurAngulaire::setGrandeurMesuree(GrandeurCapteur grandeur)
{
	if(grandeurMesuree() != grandeur)
	{
		if(grandeurMesuree() != CAPTEUR_NON_CONNECTE)
		{
			Database::removeConfigurationField(nomGrandeurMesuree());
		}
		
		grandeur_ = grandeur;
		
		NumericValue<int> value(sensorNumber());
		
		if(grandeurMesuree() != CAPTEUR_NON_CONNECTE)
		{
			Database::setConfigurationField(nomGrandeurMesuree(), &value);
		}
	}
}

void CapteurAngulaire::saveLastValue()
{
	if(Recorder::isRecording())
	{
		if(grandeurMesuree() != CAPTEUR_NON_CONNECTE)
		{
			Record* r = Recorder::record();
			
			if(output == NULL || output->recordID() != r->ID())
			{
				if(output != NULL)
				{
					delete output;
				}
				
				output = new Source< NumericValue<double> >(r,nomGrandeurMesuree(),1);
				output->save();
			}
			NumericValue<double> data(lastValue_->angle());
			Donnee< NumericValue<double> > toSave(output,*lastAcquisition(),data);
			//toSave.save();
		}
	}
}