//
//  GenericSource.h
//  PSC
//
//  Created by Quentin Fiard on 15/02/12.
//  Copyright (c) 2012 École Polytechnique. All rights reserved.
//

#ifndef PSC_GenericSource_h
#define PSC_GenericSource_h

#include <string>
#include <vector>
#include <sqlite3.h>
#include "DataTypes.h"

class Record;

class GenericSource
{
public:
	
	static GenericSource* sourceWithIDNameTypeRecordIDAndSize(sqlite_int64 ID,std::string name, DataType type, Record* record, std::size_t size);
	
	virtual std::string name() const = 0;
	virtual void setName(std::string newName) = 0;
	virtual std::string CSVString(std::size_t row, bool* isRowEmpty) const = 0;
	virtual DataType type() const = 0;
	
	virtual sqlite3_int64 ID() const = 0;
	virtual void setID(sqlite3_int64 newID) = 0;
	virtual sqlite3_int64 recordID() const = 0;
	
	virtual std::size_t size() const = 0;
	virtual void setSize(std::size_t size) = 0;
	
	virtual void save() = 0;
};

#endif
