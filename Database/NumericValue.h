//
//  NumericValue.h
//  PSC
//
//  Created by Quentin Fiard on 12/02/12.
//  Copyright (c) 2012 École Polytechnique. All rights reserved.
//

#ifndef PSC_NumericValue_h
#define PSC_NumericValue_h

#include "SingleValueData.h"

#include <vector>

#include <boost/lexical_cast.hpp>

template<typename T>
class NumericValue : public SingleValueData
{
public:
	NumericValue();
	NumericValue(T newValue);
	NumericValue(std::vector<uint8_t>& data);
	std::vector<uint8_t> toBinary() const;
	std::string toCSVString() const;
	DataType type() const;
	
	bool setValueFromString(std::string& text);
	std::string valueToString() const;
	
	void operator=(const NumericValue<T>& toCopy);
	
	void operator=(const DatabaseData& toCopy);
	void operator=(const SingleValueData& toCopy);
	
	T value() const;

private:
	T value_;
};

template<typename T>
NumericValue<T>::NumericValue()
{
	value_ = T();
}

template<typename T>
T NumericValue<T>::value() const
{
	return value_;
}

template<typename T>
NumericValue<T>::NumericValue(T newValue)
{
	value_ = newValue;
}

template<typename T>
NumericValue<T>::NumericValue(std::vector<uint8_t>& data)
{
	value_ = *((T*)data.data());
}

template<typename T>
std::vector<uint8_t> NumericValue<T>::toBinary() const
{
	return std::vector<uint8_t>((uint8_t*)&value_,(uint8_t*)&value_ + sizeof(T));
}

template<typename T>
std::string NumericValue<T>::toCSVString() const
{
	return boost::lexical_cast<std::string>(value_)+";";
}

template<typename T>
DataType NumericValue<T>::type() const
{
	return typeOfTemplate<T>();
}

template<typename T>
void NumericValue<T>::operator=(const DatabaseData& toCopy)
{
	assert(toCopy.type() == this->type());
	
	this->operator=(reinterpret_cast<const NumericValue<T>&>(toCopy));
}

template<typename T>
void NumericValue<T>::operator=(const SingleValueData& toCopy)
{
	assert(toCopy.type() == this->type());
	
	this->operator=(reinterpret_cast<const NumericValue<T>&>(toCopy));
}

template<typename T>
void NumericValue<T>::operator=(const NumericValue<T>& toCopy)
{
	value_ = toCopy.value_;
}

template<typename T>
bool NumericValue<T>::setValueFromString(std::string& text)
{
	try{
		value_ = boost::lexical_cast<T>(text);
		return true;
	} catch (boost::exception & e) {
		return false;
	}
}

template<typename T>
std::string NumericValue<T>::valueToString() const
{
	return boost::lexical_cast<std::string>(value_);
}

#endif
