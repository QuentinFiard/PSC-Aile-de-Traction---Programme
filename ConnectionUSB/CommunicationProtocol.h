//
//  CommunicationProtocol.h
//  PSC
//
//  Created by Quentin Fiard on 02/03/12.
//  Copyright (c) 2011 École Polytechnique. All rights reserved.
//

#ifdef __18CXX
#include "GenericTypeDefs.h"
#endif

typedef enum
{
    PR_NOREQUEST = 0,
    PR_ECHO,        // Debug mode, where the pic reemit everything he has received
    PR_MOTOR_CONTROL,     // Toggle motor speed or position control
    PR_READSENSOR
} ProcessRequest;

typedef enum
{
    MC_SET_POSITION,
    MC_SET_SPEED,
	MC_SET_MIN_SIGNAL_DURATION,
	MC_SET_MAX_SIGNAL_DURATION
} MC_ACTION;


#ifndef __18CXX
#ifndef UINT16
typedef uint16_t UINT16;
#endif
#ifndef INT32
typedef int32_t UINT32;
#endif
#endif

struct CommandeAngle
{
	UINT32 turn;
	UINT16 positionInTurn;
};

struct CommandeVitesse
{
	float speed;
};
