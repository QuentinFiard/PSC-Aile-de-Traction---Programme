//
//  Source.h
//  PSC
//
//  Created by Quentin Fiard on 11/02/12.
//  Copyright (c) 2012 École Polytechnique. All rights reserved.
//

#ifndef PSC_Source_h
#define PSC_Source_h

#include <iostream>
#include <assert.h>
#include <string>

#include "Database.h"
#include "Record.h"

#include "Utilities.h"

#include "GenericSource.h"

#include "DataTypes.h"

template<typename T>
class Source : public GenericSource
{
public:
	Source(Record* record, std::string name);
	Source(sqlite_int64 ID, Record* record, std::string name);
	
	sqlite3_int64 ID() const;
	void setID(sqlite3_int64 newID);
	sqlite3_int64 recordID() const;
	
	std::string name() const;
	void setName(std::string newName);
	
	//std::size_t size() = 0;
	
	std::string toCSVString() const;
	DataType type() const;
	
	void save();
	
private:
	Record* record_;
	sqlite3_int64 ID_;
	std::string name_;
	std::size_t size_;
	bool loadedFromDatabase;
};

template<typename T>
Source<T>::Source(Record* record, std::string name) : record_(record), name_(name)
{
	loadedFromDatabase = false;
}

template<typename T>
Source<T>::Source(sqlite_int64 ID, Record* record, std::string name) : record_(record), name_(name), ID_(ID)
{
	loadedFromDatabase = true;
}

template<typename T>
void Source<T>::save()
{
	Database::saveSource(*this);
}

template<typename T>
DataType Source<T>::type() const
{
	return typeOfTemplate<T>();
}

template<typename T>
sqlite3_int64 Source<T>::ID() const
{
	return ID_;
}

template<typename T>
void Source<T>::setID(sqlite3_int64 newID)
{
	ID_ = newID;
}

template<typename T>
sqlite3_int64 Source<T>::recordID() const
{
	return record_->ID();
}

template<typename T>
std::string Source<T>::toCSVString() const
{
	if(!loadedFromDatabase)
	{
		assert(0);
		return "";
	}
	
	std::string res = prepareForCSV(name_);
	for(int i=0 ; i<size_ ; i++)
	{
		res += ";";
	}
	return res;
}

template<typename T>
std::string Source<T>::name() const
{
	return name_;
}

template<typename T>
void Source<T>::setName(std::string newName)
{
	name_ = newName;
}


#endif
