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

static ConnectionUSB* shared_;
static ConnectionUSBStatus* status;

ConnectionUSB::ConnectionUSB(std::string bsdPath) : port(io,bsdPath)
{
	
}

#pragma mark - Fonctions statiques

void ConnectionUSB::connect()
{
	if(shared_)
	{
		delete shared_;
		shared_ = NULL;
	}
	
	
	
	shared_->connect_();
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

#pragma mark - Fonctions membres

bool ConnectionUSB::isConnected_() const
{
	return port.is_open();
}