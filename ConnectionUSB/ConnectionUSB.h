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
typedef uint8_t Sensor;

@class ConnectionUSBStatus;

class AngleAbsolu;
class VitesseRotation;

class ConnectionUSB
{
public:
	
	static void setPositionForMotor(AngleAbsolu angle, Motor motor);
	static void setSpeedForMotor(VitesseRotation vitesse, Motor motor);
	
	static AngleAbsolu readPositionFromSensor(Sensor sensor);
	
	static void connect();
	
	static void setStatus(ConnectionUSBStatus* status);
	
	static bool isConnected();
	
protected:
	
	ConnectionUSB(std::string bsdPath);
	static ConnectionUSB* shared();
	
// Fonctions membres protected
	void connect_();
	bool isConnected_() const;
	
private:
	
	boost::asio::io_service io;
	boost::asio::serial_port port;
};

#endif
