//
//  StringValue.cpp
//  PSC
//
//  Created by Quentin Fiard on 06/03/12.
//  Copyright (c) 2012 École Polytechnique. All rights reserved.
//

#include "StringValue.h"

bool StringValue::setValueFromString(std::string& text)
{
	value_ = text;
	
	return true;
}

std::string StringValue::valueToString() const
{
	return value_;
}

void StringValue::operator=(const SingleValueData& toCopy)
{
	assert(0);
}

void StringValue::operator=(const DatabaseData& toCopy)
{
	assert(0);
}

void StringValue::operator=(const StringValue& toCopy)
{
	value_ = toCopy.value_;
}

StringValue::StringValue()
{
	
}

StringValue::StringValue(std::vector<uint8_t>& data) : value_((char*)data.data())
{

}

StringValue::StringValue(std::string newValue) : value_(newValue)
{
	
}

std::vector<uint8_t> StringValue::toBinary() const
{
	return std::vector<uint8_t>(value_.c_str(),value_.c_str()+value_.length()+1);
}

std::string StringValue::toCSVString() const
{
	return value_+";";
}

DataType StringValue::type() const
{
	return TYPE_STRING;
}