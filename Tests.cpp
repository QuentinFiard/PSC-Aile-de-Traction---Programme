//
//  Tests.cpp
//  PSC
//
//  Created by Quentin Fiard on 25/12/11.
//  Copyright (c) 2011 École Polytechnique. All rights reserved.
//

#include "Tests.h"

#include "Referentiel3D.h"
#include "Vecteur3D.h"

#include <iostream>

#include <math.h>

#include "InterfaceUSB.h"

#include "Dynamixel.h"

void testerReferentiels()
{
	InterfaceUSB test;
	
	Referentiel3D* ref = Referentiel3D::ReferentielDeReference();
	ref->afficher();
	Vecteur3D ey(0,1,0,ref);
	Referentiel3D* ref2 = ref->referentielParRotationAngleAxe(M_PI/6, ey);
	ref2->afficher();
	Vecteur3D translation(10,0,0,ref2);
	Referentiel3D* ref3 = ref2->referentielParTranslation(translation);
	ref3->afficher();
	
	Point P(0, 1, 0, ref3);
	P.afficher();
	std::cout << std::endl;
	P.changerDeReferentiel(ref);
	P.afficher();
	std::cout << std::endl;
	P.changerDeReferentiel(ref3);
	P.afficher();
	std::cout << std::endl;
}

void testerCommunicationMoteur()
{
	//Dynamixel::check();
}