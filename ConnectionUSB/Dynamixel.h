//
//  Dynamixel.h
//  PSC
//
//  Created by Quentin Fiard on 15/01/12.
//  Copyright (c) 2012 École Polytechnique. All rights reserved.
//

#ifndef PSC_Dynamixel_h
#define PSC_Dynamixel_h

#include <vector>
#include <boost/asio.hpp>

struct InstructionPacket
{
	uint8_t ID;
	uint8_t instruction;
	std::vector<uint8_t> parameters;
};

struct StatusPacket
{
	uint8_t ID;
	uint8_t error;
	std::vector<uint8_t> parameters;
};

class Dynamixel
{
public:
	
	Dynamixel(std::string bsdPath);
	
	static bool isInstructionError(uint8_t error);
	static bool isOverloadError(uint8_t error);
	static bool isChecksumError(uint8_t error);
	static bool isRangeError(uint8_t error);
	static bool isOverheatingError(uint8_t error);
	static bool isAngleLimitError(uint8_t error);
	static bool isInputVoltageError(uint8_t error);
	
	//static void check();
	
	void sendInstructionPacket(const InstructionPacket& aEnvoyer);
	StatusPacket receiveStatusPacket();
	
private:
	boost::asio::io_service io;
	boost::asio::serial_port port;
};

#endif
