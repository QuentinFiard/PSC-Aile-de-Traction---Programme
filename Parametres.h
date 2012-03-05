//
//  Parametres.h
//  PSC
//
//  Created by Quentin Fiard on 20/12/11.
//  Copyright (c) 2011 École Polytechnique. All rights reserved.
//

#ifndef PSC_Parametres_h
#define PSC_Parametres_h

#include <math.h>

// Paramètre de l'aile

#define MASSE_AILE 0.5	// En kilogrammes

#define VOLUME_AILE 0

#define RHO_AIR 1.184
#define g 9.81

#define Ixx 0
#define Iyy 0
#define Izz 0

//Test 2D, les valeurs sont en degrés
#define GAMMA 10

#define DEG_RAD (180/M_PI)

typedef double Reel; // changer pour modifier la précision des calculs

#endif
