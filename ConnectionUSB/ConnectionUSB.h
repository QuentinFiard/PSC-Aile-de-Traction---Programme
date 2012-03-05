//
//  ConnectionUSB.h
//  PSC
//
//  Created by Quentin Fiard on 02/03/12.
//  Copyright (c) 2012 École Polytechnique. All rights reserved.
//

#ifndef PSC_ConnectionUSB_h
#define PSC_ConnectionUSB_h

#import <Foundation/Foundation.h>
#import <boost/asio.hpp>

typedef uint8_t Motor;
typedef uint8_t sensor;

@class Status;
class AngleAbsolu;
class VitesseRotation;

class ConnectionUSB
{
public:
	
	void setPositionForMotor(AngleAbsolu angle);
	void setSpeedForMotor(VitesseRotation vitesse);
	
private:
	Status* status;
	
	boost::asio::io_service io;
	boost::asio::serial_port port;
};

#endif
