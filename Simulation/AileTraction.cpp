//
//  AileTraction.cpp
//  PSC
//
//  Created by Quentin Fiard on 26/12/11.
//  Copyright (c) 2011 École Polytechnique. All rights reserved.
//

#include "AileTraction.h"
#include "Parametres.h"

#include "Referentiel3D.h"

#include <math.h>

/*AileTraction::AileTraction()
{
	Masse_ = MASSEAILE;
	Reel inertiesTab[3] = {Ixx,Iyy,Izz};
	std::vector<Reel> inerties(inertiesTab,inertiesTab+3);
	MatriceInerties_ = Matrice::diag(inerties);
}*/

State AileTraction::dynamique(const State& state)
{
	Reel theta = state.theta();
	
	Referentiel3D* galileen = Referentiel3D::ReferentielDeReference();
	Vecteur3D axe(0,1,0,galileen);
	Referentiel3D* ref1 = galileen->referentielParRotationAngleAxe(theta, axe);
	
	Vecteur3D translationCables(LONGUEURCABLES,0,0,ref1);
	Referentiel3D* ErEthetaEy = ref1->referentielParTranslation(translationCables);
	
	const Point& CDG = ErEthetaEy->origin();
	
	Vecteur3D Er = ErEthetaEy->axes(0);
	
	Quaternion q = Quaternion::quaternionParAngleEtAxe(M_PI_2 + GAMMA*M_PI/180, axe);
	
	Vecteur3D petitAxeAile = q.rotation(Er);
}