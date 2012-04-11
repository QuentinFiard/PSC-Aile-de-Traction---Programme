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

class Record
{
public:
	
	Record(boost::posix_time::ptime date, std::string tag = "");
	Record(sqlite3_int64 ID);
	Record(sqlite3_int64 ID,boost::posix_time::ptime date, std::string tag);
	
	sqlite3_int64 ID();
	void setID(sqlite3_int64 newID);
	std::string displayDate();
	int64_t date();
	void setTag(std::string newTag);
	std::string tag() const;
	
	void save();
	
	boost::posix_time::ptime privateDate();
	
private:	
	sqlite3_int64 ID_;
	std::string tag_;
	boost::posix_time::ptime date_;
};

#endif
