//
//  DatabaseData.h
//  PSC
//
//  Created by Quentin Fiard on 11/02/12.
//  Copyright (c) 2012 École Polytechnique. All rights reserved.
//

#ifndef PSC_DatabaseData_h
#define PSC_DatabaseData_h

#include <vector>
#include <string>
#include "DataTypes.h"

class DatabaseData
{
public:
	static DatabaseData* dataWithType(std::vector<uint8_t>& binaryData, DataType dataType);

	static DatabaseData* emptyDataWithType(DataType dataType);

	virtual std::vector<uint8_t> toBinary() const = 0;
	virtual std::string toCSVString() const = 0;
	virtual DataType type() const = 0;
	
	virtual void operator=(const DatabaseData& toCopy) = 0;
};

#endif
