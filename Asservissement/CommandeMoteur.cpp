//
//  CommandeMoteur.cpp
//  PSC
//
//  Created by Quentin Fiard on 12/03/12.
//  Copyright (c) 2012 École Polytechnique. All rights reserved.
//

#include "CommandeMoteur.h"

using namespace boost::posix_time;

CommandeMoteur::CommandeMoteur(ptime newDate, double newCommande) : date(newDate), commande(newCommande)
{
	
}