//
//  CapteurAngulaire.h
//  PSC
//
//  Created by Quentin Fiard on 18/03/12.
//  Copyright (c) 2012 École Polytechnique. All rights reserved.
//

#ifndef PSC_CapteurAngulaire_h
#define PSC_CapteurAngulaire_h

#import <Cocoa/Cocoa.h>

#include "Capteur.h"
#include "CommunicationProtocol.h"
#import "ArrowView.h"
#include "AngleAbsolu.h"

#include <boost/thread.hpp>
#include <boost/asio.hpp>

#include "Source.h"

class CapteurAngulaire : public Capteur
{
public:
	static CapteurAngulaire* sharedWithSensorNumber(Sensor sensor);
	static CapteurAngulaire* shared(GrandeurCapteur grandeur);
	
	Sensor sensorNumber();
	
	bool checkConnection();
	
	bool update();
	
	void setArrowView(ArrowView* view);
	
	boost::posix_time::ptime* lastAcquisition();
	
	AngleAbsolu* lastValue();
	
	static void updateAll();
	
	void setGrandeurMesuree(GrandeurCapteur grandeur);
	
	void saveLastValue();
	
protected:
	
	CapteurAngulaire(GrandeurCapteur grandeur, Sensor sensor);
	
	static void prepareListeCapteurs();
	
	void startThread();
	
private:
	Sensor sensorNumber_;
	
	GrandeurCapteur grandeur;
	
	AngleAbsolu* lastValue_;
	
	boost::posix_time::ptime* lastAcquisition_;
	
	ArrowView* view;
	
	Source< NumericValue<double> >* output;
};

#endif
