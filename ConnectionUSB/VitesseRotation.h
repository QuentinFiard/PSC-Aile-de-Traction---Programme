//
//  VitesseRotation.h
//  PSC
//
//  Created by Quentin Fiard on 02/03/12.
//  Copyright (c) 2012 École Polytechnique. All rights reserved.
//

#ifndef PSC_VitesseRotation_h
#define PSC_VitesseRotation_h

#include "CommunicationProtocol.h"

class VitesseRotation
{
public:
	
	VitesseRotation(double radiansParSeconde);
	
	CommandeVitesse consigneMoteur();	// Valeur comprise entre -1 et 1 interpolée 
										// à partir d'une table de la base de données
										// Ou directement la vitesse de rotation si le moteur 
										// est asservi en vitesse
	
	double speed() const;
	
private:
	double radiansParSeconde;
};

#endif
