//
//  Donnee.h
//  PSC
//
//  Created by Quentin Fiard on 11/02/12.
//  Copyright (c) 2012 École Polytechnique. All rights reserved.
//

#ifndef PSC_Donnee_h
#define PSC_Donnee_h

#include <iostream>
#include <assert.h>

#include <sqlite3.h>

#include <boost/date_time.hpp>
#include <boost/lexical_cast.hpp>

using namespace boost::posix_time;

class Record;
class DatabaseData;

template<typename T>
class Source;

template<typename T>
class Donnee
{
public:
	Donnee(const Source<T>* source,T value);
	Donnee(const Source<T>* source,boost::posix_time::ptime date,T value);
	Donnee(const Source<T>* source,boost::posix_time::ptime date, DatabaseData* value);
	Donnee(sqlite_int64 ID);
	
	void save();
	
	void setID(sqlite_int64 ID);
	sqlite3_int64 ID();
	bool hasID();
	void clearID();
	
	const Source<T>* source();
	const boost::posix_time::ptime date();
	
	const T& value() const;
	void setValue(T value);
	
	std::string toCSVString();
	
private:
	sqlite3_int64* ID_;
	const Source<T>* source_;
	const boost::posix_time::ptime date_;
	T value_;
};

#include "Database.h"
#include "Source.h"
#include "Record.h"

template<typename T>
Donnee<T>::Donnee(const Source<T>* source,boost::posix_time::ptime date,T value) : source_(source),date_(date),value_(value), ID_(NULL)
{
	
}

template<typename T>
Donnee<T>::Donnee(const Source<T>* source,boost::posix_time::ptime date, DatabaseData* value) : source_(source),date_(date),value_(*((T*)value)), ID_(NULL)
{
	
}

template<typename T>
Donnee<T>::Donnee(const Source<T>* source,T value) : source_(source),value_(value), ID_(NULL),date_(boost::gregorian::date())
{
	
}

template<typename T>
Donnee<T>::Donnee(sqlite_int64 ID)
{
	
}

template<typename T>
void Donnee<T>::save()
{
	Database::saveData(*this);
}

template<typename T>
void Donnee<T>::setID(sqlite_int64 ID)
{
	if(!ID_)
	{
		ID_ = new sqlite3_int64;
	}
	*ID_ = ID;
}

template<typename T>
sqlite3_int64 Donnee<T>::ID()
{
	return *ID_;
}

template<typename T>
bool Donnee<T>::hasID()
{
	return ID_ != NULL;
}

template<typename T>
void Donnee<T>::clearID()
{
	if(ID_)
	{
		delete ID_;
	}
	ID_ = NULL;
}

template<typename T>
const Source<T>* Donnee<T>::source()
{
	return source_;
}

template<typename T>
const boost::posix_time::ptime Donnee<T>::date()
{
	return date_;
}

template<typename T>
const T& Donnee<T>::value() const
{
	return value_;
}

template<typename T>
void Donnee<T>::setValue(T value)
{
	value_ = value;
}

template<typename T>
std::string Donnee<T>::toCSVString()
{
	std::string res;
	
	time_duration duration(date_ - source_->record()->privateDate());
	
	double dt = duration.ticks();
	dt /= 1000000;
	
	res = boost::lexical_cast<std::string>(dt);
	res += ";";
	
	res += value_.toCSVString();
	
	return res;
}

#endif
