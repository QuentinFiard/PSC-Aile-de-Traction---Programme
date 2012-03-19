//
//  VitesseRotation.cpp
//  PSC
//
//  Created by Quentin Fiard on 17/03/12.
//  Copyright (c) 2012 École Polytechnique. All rights reserved.
//

#include "VitesseRotation.h"
#include "Database.h"

VitesseRotation::VitesseRotation(double radianParSecondeNew) : radianParSeconde(radianParSecondeNew)
{
	
}

// Valeur comprise entre -1 et 1 interpolée 
// à partir d'une table de la base de données
CommandeVitesse VitesseRotation::consigneMoteur()
{
	return Database::commandForRequestedMotorSpeed(*this);
}

double VitesseRotation::speed() const
{
	return radianParSeconde;
}