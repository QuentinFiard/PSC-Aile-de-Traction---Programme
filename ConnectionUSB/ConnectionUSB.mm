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

#define min(X,Y) ((X) < (Y) ? : (X) : (Y))
#define max(X,Y) ((X) > (Y) ? : (X) : (Y))

static ConnectionUSB* shared_ = NULL;
static ConnectionUSBStatus* status = nil;

static boost::condition_variable cond;
static boost::condition_variable connected_cond;
static boost::condition_variable init_cond;
static boost::condition_variable read_cond;
static boost::condition_variable send_cond;
static boost::mutex mut;
static boost::mutex iomutex;
static boost::mutex initmutex;

bool ConnectionUSB::isConnectedVar_ = false;

void ConnectionUSB::initializeConnection(bool* isDone)
{	
	io = new boost::asio::io_service();
	
	boost::asio::io_service::work work(*io);
	
	port = new boost::asio::serial_port(*io,bsdPath);
	
	boost::asio::serial_port_base::baud_rate baud_option(115200); 
	port->set_option(baud_option);
	
	boost::asio::serial_port_base::stop_bits stop_bit(boost::asio::serial_port_base::stop_bits::one);
	port->set_option(stop_bit);
	
	boost::asio::serial_port_base::parity parity(boost::asio::serial_port_base::parity::none);
	port->set_option(parity);
	
	boost::unique_lock<boost::mutex> lock(mut);
	
	*isDone = true;
	init_cond.notify_one();
	
	lock.unlock();
	
	io->run();
}

ConnectionUSB::ConnectionUSB(std::string aBsdPath) : bsdPath(aBsdPath)
{
	bool isDone = false;
	boost::unique_lock<boost::mutex> lock(mut);
	thread = new boost::thread(boost::bind(&ConnectionUSB::initializeConnection,this,&isDone));
	
	int retcode;
    int policy;
	
    pthread_t threadID = (pthread_t)thread->native_handle();
	
    struct sched_param param;
	
    if ((retcode = pthread_getschedparam(threadID, &policy, &param)) != 0)
    {
        errno = retcode;
        perror("pthread_getschedparam");
        exit(EXIT_FAILURE);
    }
	
	
    policy = SCHED_FIFO;
    param.sched_priority = 4;
	
    if ((retcode = pthread_setschedparam(threadID, policy, &param)) != 0)
    {
        errno = retcode;
        perror("pthread_setschedparam");
        exit(EXIT_FAILURE);
    }
	
	while(!isDone)
	{
		init_cond.wait(lock);
	}
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
			boost::unique_lock<boost::mutex> lock(initmutex);
			shared_ = new ConnectionUSB(Database::getConfigurationFieldValue<std::string>("CarteUSB_BSDPath"));
		}
		catch(boost::exception &e)
		{
			[status setStatus:STATUS_FAILED];
			isConnectedVar_ = false;
			return;
		}
		
		isConnectedVar_ = true;
		
		if(isConnected())
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
	boost::unique_lock<boost::mutex> lock(initmutex);
	bool res;
	if(!shared_)
	{
		res = false;
	}
	else
	{
		bool isDone = false;
		boost::unique_lock<boost::mutex> lock(mut);
		shared_->io->dispatch(boost::bind(&ConnectionUSB::isConnected_,shared_,&res,&isDone));
		while(!isDone)
		{
			connected_cond.wait(lock);
		}
	}
	res = isConnectedVar_;
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
	
	uint8_t* signal = (uint8_t*)&commande.nonAsservie.signal;
	
	for(int i=0 ; i<4 ; i++)
	{
		packet.push_back(*(signal+i));
	}
	
	ConnectionUSB::sendData(packet);
}
/*
static FilteredSensorData filterSensorDataPacket(SensorDataPacket &packet)
{
	AngleAbsolu **theta = new AngleAbsolu*[NUM_READ];
	
	theta[0] = new AngleAbsolu(packet.lastStatus[0]);
	
	for(int i=1 ; i<NUM_READ ; i++)
	{
		theta[i] = new AngleAbsolu(packet.lastStatus[i], theta[i-1]);
	}
	
	double y[NUM_READ],x[NUM_READ],xmean,ymean;
	
	y[0] = 0;
	x[0] = 0;
	xmean = 0;
	ymean = 0;
	
	for(int i=1 ; i<NUM_READ ; i++)
	{
		y[i] = *(theta[i])-(*(theta[0]));
		x[i] = x[i-1] + packet.timeOffset[i-1];
		
		xmean += x[i];
		ymean += y[i];
	}
	
	xmean /= NUM_READ;
	ymean /= NUM_READ;
	
	double covar,var;
	
	covar = 0;
	var = 0;
	
	for(int i=0 ; i<NUM_READ ; i++)
	{
		var += pow((x[i]-xmean),2);
		covar += (x[i]-xmean)*(y[i]-ymean);
	}
	
	double a,b;
	
	a = covar/var;
	b = ymean - a*xmean;
	
	double moduleA = fabs(a);
	
	double d[NUM_READ]; // distances au carré à la droite
	
	for(int i=0 ; i<NUM_READ ; i++)
	{
		d[i] = fabs(a*x[i]-y[i]+b)/moduleA;
	}
	
	int imin[3] = {-1,-1,-1};
	
	for(int i=0 ; i<NUM_READ ; i++)
	{
		if(imin[0]==-1)
		{
			imin[0] = i;
		}
		else if(imin[1] == -1)
		{
			if(d[imin[0]]>d[i])
			{
				imin[1] = imin[0];
				imin[0] = i;
			}
			else
			{
				imin[1] = i;
			}
		}
		else if(imin[2] == -1)
		{
			if(d[imin[0]]>d[i])
			{
				imin[2] = imin[1];
				imin[1] = imin[0];
				imin[0] = i;
			}
			else if(d[imin[1]]>d[i])
			{
				imin[2] = imin[1];
				imin[1] = i;
			}
			else
			{
				imin[2] = i;
			}
		}
		else
		{
			if(d[imin[0]]>d[i])
			{
				imin[2] = imin[1];
				imin[1] = imin[0];
				imin[0] = i;
			}
			else if(d[imin[1]]>d[i])
			{
				imin[2] = imin[1];
				imin[1] = i;
			}
			else if(d[imin[2]]>d[i])
			{
				imin[2] = i;
			}
		}
	}
	
	int indice[3];
	
	if(imin[0] < imin[1])
	{
		if(imin[0] < imin[2])
		{
			if(imin[1]<imin[2])
			{
				indice[0] = imin[0];
				indice[1] = imin[1];
				indice[2] = imin[2];
			}
			else
			{
				indice[0] = imin[0];
				indice[1] = imin[2];
				indice[2] = imin[1];
			}
		}
		else
		{
			indice[0] = imin[2];
			indice[1] = imin[0];
			indice[2] = imin[1];
		}
	}
	else
	{
		if(imin[1] < imin[2])
		{
			if(imin[0]<imin[2])
			{
				indice[0] = imin[1];
				indice[1] = imin[0];
				indice[2] = imin[2];
			}
			else
			{
				indice[0] = imin[1];
				indice[1] = imin[2];
				indice[2] = imin[0];
			}
		}
		else
		{
			indice[0] = imin[2];
			indice[1] = imin[1];
			indice[2] = imin[0];
		}
	}
	
	FilteredSensorData res;
	
	res.lastAngle[0] = theta[indice[0]];
	res.lastAngle[1] = theta[indice[1]];
	res.lastAngle[2] = theta[indice[2]];
	
	res.timeOffset[0] = SECONDS_PER_TIME_QUANTA * (x[indice[1]]-x[indice[0]]);
	res.timeOffset[1] = SECONDS_PER_TIME_QUANTA * (x[indice[2]]-x[indice[0]]);
	
	for(int i=0 ; i<NUM_READ ; i++)
	{
		if(i!=indice[0] && i!=indice[1] && i!=indice[2])
		{
			delete theta[i];
		}
	}
	
	return res;
}*/

static void afficherErreur(UINT8 e)
{
	e <<= 2;
	for(int i=0 ; i<6 ; i++)
	{
		std::cout << ((e & 0x80) >> 7);
		e <<= 1;
	}
}

static void afficherPosition(UINT16 e)
{
	e <<= 4;
	
	for(int i=0 ; i<12 ; i++)
	{
		std::cout << ((e & 0x8000) >> 15);
		e <<= 1;
	}
}

AngleAbsolu* ConnectionUSB::readPositionFromSensor(Sensor sensor, const AngleAbsolu* closestFrom)
{
	std::vector<uint8_t> packet;
	
	packet.push_back(PR_READSENSOR);
	packet.push_back(sensor);
	
	//std::cout << "SensorStatus : " << sizeof(SensorStatus) << std::endl;
	//std::cout << "UINT16 : " << sizeof(UINT16) << std::endl;
	//std::cout << "UINT8 : " << sizeof(UINT8) << std::endl;
	//std::cout << "SensorDataPacket : " << sizeof(SensorDataPacket) << std::endl;
	
	ConnectionUSB::sendData(packet);
	
	std::vector<uint8_t> response(ConnectionUSB::readData(sizeof(SensorDataPacket)));
	
	SensorDataPacket data = *((SensorDataPacket*)response.data());
	
	//FilteredSensorData sensorData = filterSensorDataPacket(res);
	
	//SensorStatus status = res.lastStatus[NUM_READ-1];
	
	/*if((status.error & 0x3F)!=0)
	{
		if(closestFrom)
		{
			return new AngleAbsolu(*closestFrom);
		}
		return NULL;
		std::cout << "Erreur de capteur !" << std::endl;
	}*/
	
	std::cout << data.position << std::endl;
	
	/*SensorStatus test = res.lastStatus[NUM_READ-1];
	test.position = (sensorData.lastAngle[2]->angle()*MAX_SENSOR_ANGLE)/(2*M_PI);
	test.position %= MAX_SENSOR_ANGLE;

	for(int i=0 ; i<NUM_READ ; i++)
	{
		afficherPosition(res.lastStatus[i].position);
		std::cout << " ";
		afficherErreur(res.lastStatus[i].error);
		std::cout << std::endl;
	}
	
	for(int i=0 ; i<3 ; i++)
	{
		delete sensorData.lastAngle[i];
	}*/
	
	AngleAbsolu* res = new AngleAbsolu(data.position,closestFrom);
	
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
	assert(seconds>0.0001 && seconds<0.005); // Basic test to check the specified value
	
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

void ConnectionUSB::setMotorSignalNeutralDuration(double seconds)
{
	assert(seconds>0.0001 && seconds<0.005); // Basic test to check the specified value
	
	std::vector<uint8_t> packet;
	
	packet.push_back(PR_MOTOR_CONTROL);
	packet.push_back(MC_SET_NEUTRAL_SIGNAL_DURATION);
	
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
	assert(seconds>0.0001 && seconds<0.005); // Basic test to check the specified value
	
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

void ConnectionUSB::setSignalForMotor(double signal, Motor motor)
{
	std::vector<uint8_t> packet;
	
	packet.push_back(PR_MOTOR_CONTROL);
	packet.push_back(MC_SET_SIGNAL);
	packet.push_back(motor);
	
	float toSend = (float)signal;
	
	uint8_t* ptr = (uint8_t*)&toSend;
	
	for(int i=0 ; i<4 ; i++)
	{
		packet.push_back(*(ptr+i));
	}
	
	ConnectionUSB::sendData(packet);
}

void ConnectionUSB::setPositionGoal(double positionGoal)
{
	std::vector<uint8_t> packet;
	
	packet.push_back(PR_MOTOR_CONTROL);
	packet.push_back(MC_SET_POSITION);
	
	float toSend = (float)positionGoal;
	
	uint8_t* ptr = (uint8_t*)&toSend;
	
	for(int i=0 ; i<4 ; i++)
	{
		packet.push_back(*(ptr+i));
	}
	
	ConnectionUSB::sendData(packet);
}

double ConnectionUSB::readMotorSignalMaxDuration()
{
	std::vector<uint8_t> packet;
	
	packet.push_back(PR_MOTOR_CONTROL);
	packet.push_back(MC_READ_MAX_SIGNAL_DURATION);
	
	ConnectionUSB::sendData(packet);
	
	std::vector<uint8_t> response(ConnectionUSB::readData(4));
	
	float* res = (float*)response.data();
	
	return *res;
}

double ConnectionUSB::readMotorSignalNeutralDuration()
{
	std::vector<uint8_t> packet;
	
	packet.push_back(PR_MOTOR_CONTROL);
	packet.push_back(MC_READ_NEUTRAL_SIGNAL_DURATION);
	
	ConnectionUSB::sendData(packet);
	
	std::vector<uint8_t> response(ConnectionUSB::readData(4));
	
	float* res = (float*)response.data();
	
	return *res;
}

double ConnectionUSB::readMotorSignalMinDuration()
{
	std::vector<uint8_t> packet;
	
	packet.push_back(PR_MOTOR_CONTROL);
	packet.push_back(MC_READ_MIN_SIGNAL_DURATION);
	
	ConnectionUSB::sendData(packet);
	
	std::vector<uint8_t> response(ConnectionUSB::readData(4));
	
	float* res = (float*)response.data();
	
	return *res;
}

void ConnectionUSB::setPIDPeriod(double seconds)
{
	assert(seconds>400e-6 && seconds<0.02); // Basic test to check the specified value
	
	std::vector<uint8_t> packet;
	
	packet.push_back(PR_MOTOR_CONTROL);
	packet.push_back(MC_SET_PID_PERIOD);
	
	float duration = (float)seconds;
	
	uint8_t* ptr = (uint8_t*)&duration;
	
	for(int i=0 ; i<4 ; i++)
	{
		packet.push_back(*(ptr+i));
	}
	
	ConnectionUSB::sendData(packet);
}

double ConnectionUSB::readPIDPeriod()
{
	std::vector<uint8_t> packet;
	
	packet.push_back(PR_MOTOR_CONTROL);
	packet.push_back(MC_READ_PID_PERIOD);
	
	ConnectionUSB::sendData(packet);
	
	std::vector<uint8_t> response(ConnectionUSB::readData(4));
	
	float* res = (float*)response.data();
	
	return *res;
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

void ConnectionUSB::setChoixAsservissement(ChoixAsservissement choix)
{
	std::vector<uint8_t> packet;
	
	packet.push_back(PR_MOTOR_CONTROL);
	packet.push_back(MC_SET_CHOIX_ASSERVISSEMENT);
	packet.push_back(choix.type);
	
	if(choix.type != ASSERVISSEMENT_SANS)
	{
		packet.push_back(choix.motor);
		packet.push_back(choix.sensor);
	}
	
	ConnectionUSB::sendData(packet);
}

ChoixAsservissement ConnectionUSB::readChoixAsservissement()
{
	ChoixAsservissement res;
	
	std::vector<uint8_t> packet;
	
	packet.push_back(PR_MOTOR_CONTROL);
	packet.push_back(MC_READ_CHOIX_ASSERVISSEMENT);
	
	ConnectionUSB::sendData(packet);
	
	std::vector<uint8_t> type(ConnectionUSB::readData(1));
	
	res.type = static_cast<TypeAsservissement>(type[0]);
	
	if(res.type != ASSERVISSEMENT_SANS)
	{
		std::vector<uint8_t> response(ConnectionUSB::readData(2));
		
		res.motor = static_cast<Motor>(response[0]);
		res.sensor = static_cast<Sensor>(response[1]);
	}
	
	return res;
}

#pragma mark - Protected Functions

void ConnectionUSB::sendData(std::vector<uint8_t>& data)
{
	boost::unique_lock<boost::mutex> iolock(iomutex,boost::defer_lock_t());
	iolock.lock();
	ConnectionUSB* shared = ConnectionUSB::shared();
	if(shared)
	{
		bool isDone = false;
		boost::unique_lock<boost::mutex> lock(mut);
		shared->io->post(boost::bind(&ConnectionUSB::sendData_,shared,&data,&isDone));
		while(!isDone)
		{
			send_cond.wait(lock);
		}
	}
}

std::vector<uint8_t> ConnectionUSB::readData(std::size_t length)
{
	boost::unique_lock<boost::mutex> iolock(iomutex,boost::defer_lock_t());
	iolock.lock();
	ConnectionUSB* shared = ConnectionUSB::shared();
	if(shared)
	{
		std::vector<uint8_t> res;
		bool isDone = false;
		boost::unique_lock<boost::mutex> lock(mut);
		shared->io->post(boost::bind(&ConnectionUSB::readData_,shared,length,&res,&isDone));
		while(!isDone)
		{
			read_cond.wait(lock);
		}
		return res;
	}
	return std::vector<uint8_t>();
}

#pragma mark - Fonctions membres

void ConnectionUSB::isConnected_(bool* res, bool* isDone) const
{
	*res = port->is_open();
	
	boost::unique_lock<boost::mutex> lock(mut);
	
	*isDone = true;
	connected_cond.notify_all();
}

void ConnectionUSB::sendData_(std::vector<uint8_t>* data, bool* isDone)
{
	boost::asio::async_write(*port,boost::asio::buffer(*data),boost::bind(&ConnectionUSB::sendDataCompletion, this, isDone));
}

void ConnectionUSB::sendDataCompletion(bool* isDone)
{
	boost::unique_lock<boost::mutex> lock(mut);
	
	*isDone = true;
	
	send_cond.notify_one();
}

void ConnectionUSB::readData_(std::size_t length, std::vector<uint8_t>* res, bool* isDone)
{
	res->resize(length);
	
	boost::asio::async_read(*port,boost::asio::buffer(*res),boost::bind(&ConnectionUSB::readDataCompletion, this, res, isDone));
}

void ConnectionUSB::readDataCompletion(std::vector<uint8_t>* res, bool* isDone)
{
	boost::unique_lock<boost::mutex> lock(mut);
	
	*isDone = true;
	
	read_cond.notify_one();
}