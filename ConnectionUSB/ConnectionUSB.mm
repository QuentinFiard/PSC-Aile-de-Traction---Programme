//
//  ConnectionUSB.cpp
//  PSC
//
//  Created by Quentin Fiard on 02/03/12.
//  Copyright (c) 2012 École Polytechnique. All rights reserved.
//

#include "ConnectionUSB.h"
#include <string>
#import "ConnectionUSBStatus.h"

#import "Database.h"

#import "AngleAbsolu.h"
#import "VitesseRotation.h"

#import <boost/lexical_cast.hpp>
#import <boost/date_time.hpp>

#import "Utilities.h"

#import <iostream>

static ConnectionUSB* shared_;
static ConnectionUSBStatus* status;

ConnectionUSB::ConnectionUSB(std::string bsdPath) : port(io,bsdPath)
{
	boost::asio::serial_port_base::baud_rate baud_option(115200); 
	port.set_option(baud_option);
	
	boost::asio::serial_port_base::stop_bits stop_bit(boost::asio::serial_port_base::stop_bits::one);
	port.set_option(stop_bit);
	
	boost::asio::serial_port_base::parity parity(boost::asio::serial_port_base::parity::none);
	port.set_option(parity);
}

#pragma mark - Fonctions statiques

ConnectionUSB* ConnectionUSB::shared()
{
	if(!isConnected())
	{
		std::cout << "Error : port is not connected" << std::endl;
		return NULL;
	}
	return shared_;
}

void ConnectionUSB::connect()
{
	if(shared_)
	{
		delete shared_;
		shared_ = NULL;
	}
	
	if(!Database::hasConfigurationField("CarteUSB_BSDPath"))
	{
		std::cout << "Le port de la carte USB n'est pas indiqué dans la base de données" << std::endl;
		std::cout << "Merci de l'indiquer dans le champ \"CarteUSB_BSDPath\"" << std::endl;
		
		if(status)
		{
			[status setStatus:STATUS_FAILED];
		}
	}
	else
	{
		try
		{
			shared_ = new ConnectionUSB(Database::getConfigurationFieldValue<std::string>("CarteUSB_BSDPath"));
		}
		catch(boost::exception &e)
		{
			[status setStatus:STATUS_FAILED];
			return;
		}
		
		if(shared_->isConnected_())
		{
			[status setStatus:STATUS_OK];
		}
		else
		{
			[status setStatus:STATUS_FAILED];
		}
	}
}

void ConnectionUSB::setStatus(ConnectionUSBStatus* newStatus)
{
	status = [newStatus retain];
	if(isConnected())
	{
		[status setStatus:STATUS_OK];
	}
	else
	{
		connect();
	}
}

bool ConnectionUSB::isConnected()
{
	bool res;
	if(!shared_)
	{
		res = false;
	}
	else
	{
		res = shared_->isConnected_();
	}
	if(status)
	{
		if(res)
		{
			[status setStatus:STATUS_OK];
		}
		else
		{
			[status setStatus:STATUS_FAILED];
		}
	}
	return res;
}

void ConnectionUSB::setPositionForMotor(AngleAbsolu& angle, Motor motor)
{
	CommandeAngle commande = angle.consigneMoteur();
	
	std::vector<uint8_t> packet;
	
	packet.push_back(PR_MOTOR_CONTROL);
	packet.push_back(MC_SET_POSITION);
	packet.push_back(motor);
	
	uint8_t* nbTour = (uint8_t*)&commande.turn;
	
	for(int i=0 ; i<4 ; i++)
	{
		packet.push_back(*(nbTour+i));
	}
	
	uint8_t* pos = (uint8_t*)&commande.positionInTurn;
	
	packet.push_back(*pos);
	packet.push_back(*(pos+1));
	
	ConnectionUSB::sendData(packet);
}

void ConnectionUSB::setSpeedForMotor(VitesseRotation& vitesse, Motor motor)
{
	CommandeVitesse commande = vitesse.consigneMoteur();
	
	std::vector<uint8_t> packet;
	
	packet.push_back(PR_MOTOR_CONTROL);
	packet.push_back(MC_SET_SPEED);
	packet.push_back(motor);
	
	uint8_t* signal = (uint8_t*)&commande.signal;
	
	for(int i=0 ; i<4 ; i++)
	{
		packet.push_back(*(signal+i));
	}
	
	ConnectionUSB::sendData(packet);
}

AngleAbsolu ConnectionUSB::readPositionFromSensor(Sensor sensor, const AngleAbsolu* closestFrom)
{
	std::vector<uint8_t> packet;
	
	packet.push_back(PR_READSENSOR);
	packet.push_back(sensor);
	
	ConnectionUSB::sendData(packet);
	
	std::vector<uint8_t> response(ConnectionUSB::readData(3));
	
	SensorStatus status;
	status.sensor = sensor;
	status.position = *((UINT16*)response.data());
	status.error = (SensorError)response[2];
	
	AngleAbsolu res(status,closestFrom);
	
	return res;
}

void ConnectionUSB::checkSensorError(SensorStatus &status)
{
	if((status.error & SENSOR_ERROR_PARITY) != 0)
	{
		log("sensor",	"Capteur n°" 
					   + boost::lexical_cast<std::string>(status.sensor)
					   + " - Erreur de parité");
	}
}

void ConnectionUSB::setMotorSignalMaxDuration(double seconds)
{
	assert(seconds>0.1 && seconds<5); // Basic test to check the specified value
	
	std::vector<uint8_t> packet;
	
	packet.push_back(PR_MOTOR_CONTROL);
	packet.push_back(MC_SET_MAX_SIGNAL_DURATION);
	
	float duration = (float)seconds;
	
	uint8_t* ptr = (uint8_t*)&duration;
	
	for(int i=0 ; i<4 ; i++)
	{
		packet.push_back(*(ptr+i));
	}
	
	ConnectionUSB::sendData(packet);
}

void ConnectionUSB::setMotorSignalMinDuration(double seconds)
{
	assert(seconds>0.1 && seconds<5); // Basic test to check the specified value
	
	std::vector<uint8_t> packet;
	
	packet.push_back(PR_MOTOR_CONTROL);
	packet.push_back(MC_SET_MIN_SIGNAL_DURATION);
	
	float duration = (float)seconds;
	
	uint8_t* ptr = (uint8_t*)&duration;
	
	for(int i=0 ; i<4 ; i++)
	{
		packet.push_back(*(ptr+i));
	}
	
	ConnectionUSB::sendData(packet);
}

void ConnectionUSB::appendPIDCoeffs(std::vector<uint8_t> &packet, PID_Coeffs& coeffs)
{
	uint8_t* ptr = (uint8_t*)&(coeffs.gainProportionnel);
	
	for(int i=0 ; i<4 ; i++)
	{
		packet.push_back(*(ptr+i));
	}
	
	ptr = (uint8_t*)&(coeffs.gainDifferentiel);
	
	for(int i=0 ; i<4 ; i++)
	{
		packet.push_back(*(ptr+i));
	}
	
	ptr = (uint8_t*)&(coeffs.gainIntegral);
	
	for(int i=0 ; i<4 ; i++)
	{
		packet.push_back(*(ptr+i));
	}
}

void ConnectionUSB::setSpeedPIDCoeffs(PID_Coeffs coeffs)
{
	std::vector<uint8_t> packet;
	
	packet.push_back(PR_MOTOR_CONTROL);
	packet.push_back(MC_SET_PID_SPEED_COEFFS);
	
	appendPIDCoeffs(packet,coeffs);
	
	ConnectionUSB::sendData(packet);
}

void ConnectionUSB::setPositionPIDCoeffs(PID_Coeffs coeffs)
{
	std::vector<uint8_t> packet;
	
	packet.push_back(PR_MOTOR_CONTROL);
	packet.push_back(MC_SET_PID_POSITION_COEFFS);
	
	appendPIDCoeffs(packet,coeffs);
	
	ConnectionUSB::sendData(packet);
}

static PID_Coeffs pidCoeffsFromBinary(std::vector<uint8_t> &binary)
{
	assert(binary.size()==12);
	
	PID_Coeffs res;
	
	float* ptr = (float*)binary.data();
	
	res.gainProportionnel = *ptr;
	res.gainDifferentiel = *(ptr+1);
	res.gainIntegral = *(ptr+2);
	
	return res;
}

PID_Coeffs ConnectionUSB::readSpeedPIDCoeffs()
{
	std::vector<uint8_t> packet;
	
	packet.push_back(PR_MOTOR_CONTROL);
	packet.push_back(MC_READ_PID_SPEED_COEFFS);
	
	ConnectionUSB::sendData(packet);
	
	std::vector<uint8_t> response(ConnectionUSB::readData(12));
	
	return pidCoeffsFromBinary(response);
}

PID_Coeffs ConnectionUSB::readPositionPIDCoeffs()
{
	std::vector<uint8_t> packet;
	
	packet.push_back(PR_MOTOR_CONTROL);
	packet.push_back(MC_READ_PID_POSITION_COEFFS);
	
	ConnectionUSB::sendData(packet);
	
	std::vector<uint8_t> response(ConnectionUSB::readData(12));
	
	return pidCoeffsFromBinary(response);
}

#pragma mark - Protected Functions

void ConnectionUSB::sendData(std::vector<uint8_t>& data)
{
	ConnectionUSB* shared = ConnectionUSB::shared();
	if(shared)
	{
		shared->sendData_(data);
	}
}

std::vector<uint8_t> ConnectionUSB::readData(std::size_t length)
{
	ConnectionUSB* shared = ConnectionUSB::shared();
	if(shared)
	{
		return shared->readData_(length);
	}
	return std::vector<uint8_t>();
}

#pragma mark - Fonctions membres

bool ConnectionUSB::isConnected_() const
{
	return port.is_open();
}

void ConnectionUSB::sendData_(std::vector<uint8_t>& data)
{
	boost::asio::write(port,boost::asio::buffer(data));
}

std::vector<uint8_t> ConnectionUSB::readData_(std::size_t length)
{
	std::vector<uint8_t> res(length);
	
	boost::asio::read(port,boost::asio::buffer(res));
}