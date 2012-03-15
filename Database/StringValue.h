//
//  StringValue.h
//  PSC
//
//  Created by Quentin Fiard on 06/03/12.
//  Copyright (c) 2012 École Polytechnique. All rights reserved.
//

#ifndef PSC_StringValue_h
#define PSC_StringValue_h

#include "SingleValueData.h"
#include <string>

class StringValue : public SingleValueData
{
public:
	StringValue();
	StringValue(std::vector<uint8_t>& data);
	StringValue(std::string newValue);
	
	bool setValueFromString(std::string& text);
	std::string valueToString() const;
	
	void operator=(const SingleValueData& toCopy);
	void operator=(const StringValue& toCopy);
	void operator=(const DatabaseData& toCopy);
	
	std::vector<uint8_t> toBinary() const;
	std::string toCSVString() const;
	DataType type() const;
	
	std::string value() const;
	
private:
	std::string value_;
};

#endif
