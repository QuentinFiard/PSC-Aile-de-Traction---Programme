//
//  Record.cpp
//  PSC
//
//  Created by Quentin Fiard on 11/02/12.
//  Copyright (c) 2012 École Polytechnique. All rights reserved.
//

#include "Record.h"
#include <assert.h>
#include "Database.h"

using namespace boost::posix_time;
using namespace boost::gregorian;

Record::Record(sqlite3_int64 ID, std::string tag, boost::posix_time::ptime date) : ID_(ID), tag_(tag), date_(date)
{
	
}

sqlite3_int64 Record::ID()
{
	return ID_;
}

std::string Record::displayDate()
{
	std::string res = to_simple_string(date_);
	
	std::size_t posPoint = res.find_last_of('.');
	
	return res.substr(0,posPoint);
}

void Record::setTag(std::string newTag)
{
	tag_ = newTag;
}

std::string Record::tag() const
{
	return tag_;
}

void Record::save()
{
	Database::saveRecord(this);
}

int64_t Record::date()
{
	ptime ref(boost::gregorian::date(2001,Jan,1));
	
	return (date_ - ref).total_microseconds();
}

void Record::setID(sqlite3_int64 newID)
{
	ID_ = newID;
}