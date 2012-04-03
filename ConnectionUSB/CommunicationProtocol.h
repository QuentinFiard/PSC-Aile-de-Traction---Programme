//
//  CommunicationProtocol.h
//  PSC
//
//  Created by Quentin Fiard on 02/03/12.
//  Copyright (c) 2011 École Polytechnique. All rights reserved.
//

#ifndef __CommunicationProtocol__
#define __CommunicationProtocol__

#ifdef __18CXX
#include "GenericTypeDefs.h"
#else
#include <stdint.h>
#endif

#ifndef __18CXX
#ifndef UINT16
typedef uint8_t UINT8;
#endif
#ifndef UINT16
typedef uint16_t UINT16;
#endif
#ifndef INT32
typedef int32_t UINT32;
#endif
#endif

typedef UINT8 ProcessRequest;

enum
{
    PR_NOREQUEST = 0,
    PR_ECHO,        // Debug mode, where the pic reemit everything he has received
    PR_MOTOR_CONTROL,     // Toggle motor speed or position control
    PR_READSENSOR
};

typedef UINT8 MC_ACTION;

enum
{
    MC_SET_SIGNAL,
    MC_SET_POSITION,
    MC_SET_SPEED,
    MC_SET_MIN_SIGNAL_DURATION,
    MC_SET_NEUTRAL_SIGNAL_DURATION,
    MC_SET_MAX_SIGNAL_DURATION,
    MC_READ_MIN_SIGNAL_DURATION,
    MC_READ_NEUTRAL_SIGNAL_DURATION,
    MC_READ_MAX_SIGNAL_DURATION,

    MC_SET_PID_SPEED_COEFFS,
    MC_SET_PID_POSITION_COEFFS,

    MC_READ_PID_SPEED_COEFFS,
    MC_READ_PID_POSITION_COEFFS,

    MC_SET_CHOIX_ASSERVISSEMENT,
    MC_READ_CHOIX_ASSERVISSEMENT,

    MC_READ_PID_PERIOD,
    MC_SET_PID_PERIOD
	
};

/*
 Description du protocole
 
 1. Envoi du ProcessRequest
 
 Si PR_MOTORCONTROL
 
	2.	Envoi de l'action souhaitée
		Envoi du MoteurID
 
	Si MC_SET_POSITION
 
		3. Envoi du nombre de tour
		4. Envoi de la position souhaité dans le tour
 
 */

typedef struct
{
    UINT32 turn;
    UINT16 positionInTurn;
} CommandeAngle;

typedef struct
{
    float signal;
} VitesseNonAsservie;

typedef struct
{
    float radiansParSeconde;
} VitesseAsservie;

typedef union
{
    VitesseNonAsservie nonAsservie;
    VitesseAsservie asservie;
} CommandeVitesse;

typedef UINT8 Sensor;
typedef UINT8 Motor;

typedef UINT8 SensorError;

enum
{
    SENSOR_ERROR_PARITY = 0x01,
    SENSOR_ERROR_LINEARITY = 0x02,
    SENSOR_ERROR_OCF_NOT_FINISHED = 0x04,
    SENSOR_ERROR_OVERFLOW = 0x08,
    SENSOR_ERROR_MAG_INC = 0x10,
    SENSOR_ERROR_MAG_DEC = 0x20
};

typedef struct
{
    Sensor sensor;
    SensorError error;
    UINT16 position;
}  SensorStatus;

typedef struct
{
    Sensor sensor;
    UINT8 dumb[3]; // 32 bits alignement
    float vitesseRotation;
    UINT16 position;
	UINT16 dumb2; // 32 bits alignement
} SensorDataPacket;

typedef struct
{
    float gainProportionnel;
    float gainDifferentiel;
    float gainIntegral;
} PID_Coeffs;

typedef UINT8 TypeAsservissement;

enum
{
    ASSERVISSEMENT_SANS = 0,
    ASSERVISSEMENT_POSITION = 1,
    ASSERVISSEMENT_VITESSE = 2
};

typedef struct
{
    TypeAsservissement type;
    Motor motor;
    Sensor sensor;
} ChoixAsservissement;

#define MAX_SENSOR_ANGLE 0x1000

#define SAMPLE_TIME_MICROSECONDS 400

#define SECONDS_PER_TIME_QUANTA 1.66666667e-7

#endif
