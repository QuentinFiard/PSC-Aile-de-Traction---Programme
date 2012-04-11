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
	Source(Record* record, std::string name, std::size_t size);
	Source(sqlite_int64 ID, Record* record, std::string name, std::size_t size);
	
	sqlite3_int64 ID() const;
	void setID(sqlite3_int64 newID);
	sqlite3_int64 recordID() const;
	
	Record* record() const;
	
	std::string name() const;
	void setName(std::string newName);
	
	std::size_t size() const;
	void setSize(std::size_t size);
	
	std::string CSVString(std::size_t row, bool* isRowEmpty) const;
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
Source<T>::Source(Record* record, std::string name, std::size_t size) : record_(record), name_(name), ID_(-1), size_(size)
{
	loadedFromDatabase = false;
}

template<typename T>
Source<T>::Source(sqlite_int64 ID, Record* record, std::string name, std::size_t size) : record_(record), name_(name), ID_(ID), size_(size)
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
	return typeOfTemplate(T());
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
std::string Source<T>::CSVString(std::size_t row, bool* isRowEmpty) const
{
	*isRowEmpty = false;
	if(row==0)
	{
		std::string res = "t (s);"+prepareForCSV(name_);
		for(int i=0 ; i<size_ ; i++)
		{
			res += ";";
		}
		return res;
	}
	else
	{
		Donnee<T>* donnee = Database::donneeForSourceAtIndex(this,row-1);
		
		if(donnee != NULL)
		{
			std::string res = donnee->toCSVString();
			delete donnee;
			return res;
		}
		else
		{
			*isRowEmpty = true;
			std::string res = "";
			for(int i=0 ; i<size_+1 ; i++)
			{
				res += ";";
			}
			return res;
		}
	}
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

template<typename T>
std::size_t Source<T>::size() const
{
	return size_;
}

template<typename T>
void Source<T>::setSize(std::size_t size)
{
	size_ = size;
}

template<typename T>
Record* Source<T>::record() const
{
	return record_;
}


#endif
