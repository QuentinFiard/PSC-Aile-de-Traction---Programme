//
//  EtatSysteme.cpp
//  PSC
//
//  Created by Quentin Fiard on 05/03/12.
//  Copyright (c) 2012 École Polytechnique. All rights reserved.
//

#include "EtatSysteme.h"
#include "Capteur.h"

#include "Parametres.h"

#include "ForceAero.h"

#include "Database.h"

#include <math.h>

EtatSysteme::EtatSysteme(Point position, Vecteur3D vitesse, double psi) : Vector(7), position_(position), vitesse_(vitesse), psi_(psi)
{
	(*this)(0) = position[0];
	(*this)(1) = position[1];
	(*this)(2) = position[2];
	(*this)(3) = vitesse[0];
	(*this)(4) = vitesse[1];
	(*this)(5) = vitesse[2];
	(*this)(6) = psi;
}

EtatSysteme::EtatSysteme(uint nRows_, uint nCols_=1, String name_="") : DifferentialState(nRows_,nCols_,name_)
{
	
}

EtatSysteme::EtatSysteme(DifferentialState& vect) : DifferentialState(vect),position_(vect(0),vect(1),vect(2)), vitesse_(vect(3),vect(4),vect(5)), psi_(vect(6))
{
	
}

Vector EtatSysteme::derivee(Control commande)
{
	Capteur camera;
	Capteur vent_x;
	Capteur vent_y;
	
	Capteur theta;
	Capteur theta_pt;
	Capteur phi;
	Capteur phi_pt;
	
	double rotAutourAxes = camera.valeur();
	
	Referentiel3D* ref = Referentiel3D::ReferentielDeReference();
	
	Referentiel3D* spherique = Referentiel3D::ReferentielDeReference();
	Vecteur3D axe_y = Vecteur3D(0,1,0,ref);
	Vecteur3D axe_z = Vecteur3D(0,1,0,ref);
	spherique = spherique->referentielParRotationAngleAxe(theta.valeur(), axe_y);
	spherique = spherique->referentielParRotationAngleAxe(phi.valeur(), axe_z);
	
	Vecteur3D e_r = spherique->axes(2);
	Vecteur3D e_theta = spherique->axes(0);
	Vecteur3D e_phi = spherique->axes(1);	
	
	Referentiel3D* kite = spherique->referentielParTranslation(position());
	
	kite = kite->referentielParRotationAngleAxe(rotAutourAxes, e_r);
	
	Vecteur3D petit_axe = kite->axes(1);
	
	kite = kite->referentielParRotationAngleAxe(psi(), petit_axe);
	
	Vecteur3D grand_axe = kite->axes(0);
	
	kite = kite->referentielParRotationAngleAxe(GAMMA/DEG_RAD, grand_axe);
	
	petit_axe = kite->axes(1);
	grand_axe = kite->axes(0);
	Vecteur3D normale = kite->axes(2);
	
	Vecteur3D F_poids = (VOLUME_AILE*RHO_AIR-MASSE_AILE)*CONSTANTE_GRAVITATION*axe_z;
	
	Vecteur3D F_aero = ForceAero::force(kite, Vecteur3D(vent_x.valeur(),vent_y.valeur(),0), vitesse());
	
	double r = 20;//(Database::getConfigurationField("RayonCables");
	
	Vecteur3D F_cables = (		(F_poids + F_aero)*e_r 
							+ MASSE_AILE*(	r*pow(theta_pt.valeur(),2)
										 +	r*pow(sin(theta.valeur()),2)*pow(phi_pt.valeur(),2))
						 )*e_r;
	
	Vecteur3D acceleration = (F_aero + F_poids + F_cables)/MASSE_AILE;
	
	DifferentialStateDerivative derivee(7);
	
	derivee(0) = vitesse()[0];
	derivee(1) = vitesse()[1];
	derivee(2) = vitesse()[2];
	derivee(3) = acceleration[0];
	derivee(4) = acceleration[1];
	derivee(5) = acceleration[2];
	derivee(6) = commande;
	
	return derivee;
}