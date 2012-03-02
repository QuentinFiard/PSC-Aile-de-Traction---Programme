//
//  SingleValueData.h
//  PSC
//
//  Created by Quentin Fiard on 14/02/12.
//  Copyright (c) 2012 École Polytechnique. All rights reserved.
//

#ifndef PSC_SingleValueData_h
#define PSC_SingleValueData_h

#include "DatabaseData.h"
#include <string>

class SingleValueData : public DatabaseData
{
public:
	virtual bool setValueFromString(std::string& text) = 0;
	virtual std::string valueToString() const = 0;
	
	virtual void operator=(const SingleValueData& toCopy) = 0;
};

#endif
