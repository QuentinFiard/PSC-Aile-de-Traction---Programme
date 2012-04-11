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
#include <iostream>

using namespace boost::posix_time;
using namespace boost::gregorian;

Record::Record(boost::posix_time::ptime date, std::string tag) : tag_(tag), date_(date)
{
	
}

Record::Record(sqlite3_int64 ID) : ID_(ID)
{
	Record* tmp = Database::recordWithId(ID);
	tag_ = tmp->tag();
	date_ = tmp->privateDate();
	
	delete tmp;
}

Record::Record(sqlite3_int64 ID,boost::posix_time::ptime date, std::string tag) : ID_(ID), date_(date), tag_(tag)
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
	
	std::cout << "Offset : " << (date_ - ref).ticks();
	
	return (date_ - ref).ticks();
}

boost::posix_time::ptime Record::privateDate()
{
	return date_;
}

void Record::setID(sqlite3_int64 newID)
{
	ID_ = newID;
}