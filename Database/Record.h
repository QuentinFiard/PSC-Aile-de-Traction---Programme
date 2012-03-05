//
//  Record.h
//  PSC
//
//  Created by Quentin Fiard on 11/02/12.
//  Copyright (c) 2012 École Polytechnique. All rights reserved.
//

#ifndef PSC_Record_h
#define PSC_Record_h

#include <sqlite3.h>

#include <boost/date_time.hpp>

/* Remarque :
	Toute modification d'une instance de la classe Expérience est automatiquement propagée
	à la base de données (modification de la date de début,modif du tag
 */

class Record
{
public:
	
	Record(sqlite3_int64 ID, std::string tag, boost::posix_time::ptime date);
	
	sqlite3_int64 ID();
	void setID(sqlite3_int64 newID);
	std::string displayDate();
	int64_t date();
	void setTag(std::string newTag);
	std::string tag() const;
	
	void save();
	
private:
	sqlite3_int64 ID_;
	std::string tag_;
	boost::posix_time::ptime date_;
};

#endif
