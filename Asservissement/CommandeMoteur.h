//
//  CommandeMoteur.h
//  PSC
//
//  Created by Quentin Fiard on 05/03/12.
//  Copyright (c) 2012 École Polytechnique. All rights reserved.
//

#ifndef PSC_CommandeMoteur_h
#define PSC_CommandeMoteur_h

#include <boost/date_time/posix_time/posix_time.hpp>

class CommandeMoteur
{
public:
	CommandeMoteur(boost::posix_time::ptime date, double commande);
	
private:
	boost::posix_time::ptime date;
	double commande;
};

#endif
