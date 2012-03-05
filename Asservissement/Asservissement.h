//
//  Asservissement.h
//  PSC
//
//  Created by Quentin Fiard on 05/03/12.
//  Copyright (c) 2012 École Polytechnique. All rights reserved.
//

#ifndef PSC_Asservissement_h
#define PSC_Asservissement_h

#include <vector>

typedef double Longueur;

class EtatSysteme;
class CommandeMoteur;

class Asservissement
{
	std::vector<CommandeMoteur> calculCommandeOptimale(EtatSysteme& etat);
	
private:
	
	double t_horizon;
	Longueur rayonCables; 
};

#endif
