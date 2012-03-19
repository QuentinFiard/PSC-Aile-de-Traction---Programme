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
#import <vector>
#import "CommunicationProtocol.h"

@class ConnectionUSBStatus;

class AngleAbsolu;
class VitesseRotation;

class ConnectionUSB
{
public:
	
	static void setMotorSignalMaxDuration(double seconds);
	static void setMotorSignalMinDuration(double seconds);
	
	static void setSpeedPIDCoeffs(PID_Coeffs coeffs);
	static void setPositionPIDCoeffs(PID_Coeffs coeffs);
	
	static PID_Coeffs readSpeedPIDCoeffs();
	static PID_Coeffs readPositionPIDCoeffs();
	
	static double readMotorSignalMaxDuration();
	static double readMotorSignalMinDuration();
	
	static void setPositionForMotor(AngleAbsolu& angle, Motor motor);
	static void setSpeedForMotor(VitesseRotation& vitesse, Motor motor);
	
	static AngleAbsolu readPositionFromSensor(Sensor sensor, const AngleAbsolu* closestFrom = NULL);
	
	static void connect();
	
	static void setStatus(ConnectionUSBStatus* status);
	
	static bool isConnected();
	
protected:
	
	static void appendPIDCoeffs(std::vector<uint8_t> &packet, PID_Coeffs& coeffs);
	
	ConnectionUSB(std::string bsdPath);
	static ConnectionUSB* shared();
	
	static void checkSensorError(SensorStatus &status);
	
	static void sendData(std::vector<uint8_t>& data);
	static std::vector<uint8_t> readData(std::size_t length);
	
// Fonctions membres protected
	bool isConnected_() const;
	void sendData_(std::vector<uint8_t>& data);
	std::vector<uint8_t> readData_(std::size_t length);
	
private:
	
	boost::asio::io_service io;
	boost::asio::serial_port port;
};

#endif
