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

EtatSystemeDerivee EtatSysteme::calculDerivee(double commande)
{
	Vecteur3D acceleration(0,0,0);
	
	Capteur camera;
	Capteur vent_x;
	Capteur vent_y;
	
	double rotAutourAxes = camera.valeur();
	
	double theta,phi;
	
	Referentiel3D* ref = Referentiel3D::ReferentielDeReference();
	
	Referentiel3D* spherique = Referentiel3D::ReferentielDeReference();
	Vecteur3D axe_y = Vecteur3D(0,1,0,ref);
	Vecteur3D axe_z = Vecteur3D(0,1,0,ref);
	spherique = spherique->referentielParRotationAngleAxe(theta, axe_y);
	spherique = spherique->referentielParRotationAngleAxe(phi, axe_z);
	
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
	
	//Vecteur3D F_cables = 
	
	
	
	return EtatSystemeDerivee(vitesse(),acceleration,commande);
}