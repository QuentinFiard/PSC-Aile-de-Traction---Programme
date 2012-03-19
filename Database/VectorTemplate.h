//
//  VectorTemplate.h
//  PSC
//
//  Created by Quentin Fiard on 06/03/12.
//  Copyright (c) 2012 École Polytechnique. All rights reserved.
//

#ifndef PSC_VectorTemplate_h
#define PSC_VectorTemplate_h

#include "VectorData.h"
#include <vector>
#include "DataTypes.h"
#include <assert.h>

template<typename T>
class VectorTemplate : public VectorData
{
public:
	VectorTemplate();
	VectorTemplate(std::vector<uint8_t>& data);
	
	bool setValueAtIndexWithString(std::size_t i,std::string& text);
	std::string valueAtIndexToString(std::size_t i) const;
	
	std::size_t dim() const;
	void setDim(std::size_t n);
	bool isResizable();
	
	void operator=(const VectorData& toCopy);
	void operator=(const VectorTemplate<T>& toCopy);
	
	std::vector<uint8_t> toBinary() const;
	std::string toCSVString() const;
	DataType type() const;
	
	void operator=(const DatabaseData& toCopy);
	
	const T& vector() const;
	
private:
	T vector_;
};

template<typename T>
VectorTemplate<T>::VectorTemplate() : vector_()
{
	
}

template<typename T>
VectorTemplate<T>::VectorTemplate(std::vector<uint8_t>& data) : vector_(data)
{
	
}

template<typename T>
const T& VectorTemplate<T>::vector() const
{
	return vector_;
}

template<typename T>
bool VectorTemplate<T>::setValueAtIndexWithString(std::size_t i,std::string& text)
{
	return vector_.setValueAtIndexWithString(i,text);
}

template<typename T>
std::string VectorTemplate<T>::valueAtIndexToString(std::size_t i) const
{
	return vector_.valueAtIndexToString(i);
}

template<typename T>
std::size_t VectorTemplate<T>::dim() const
{
	return vector_.dim();
}

template<typename T>
void VectorTemplate<T>::setDim(std::size_t n)
{
	vector_.setDim(n);
}

template<typename T>
bool VectorTemplate<T>::isResizable()
{
	return vector_.isResizable();
}

template<typename T>
void VectorTemplate<T>::operator=(const VectorData& toCopy)
{
	assert(0);
}

template<typename T>
void VectorTemplate<T>::operator=(const VectorTemplate<T>& toCopy)
{
	vector_ = toCopy.vector_;
}

template<typename T>
std::vector<uint8_t> VectorTemplate<T>::toBinary() const
{
	return vector_.toBinary();
}

template<typename T>
std::string VectorTemplate<T>::toCSVString() const
{
	return vector_.toCSVString();
}

template<typename T>
DataType VectorTemplate<T>::type() const
{
	return typeOfTemplate(this);
}

template<typename T>
void VectorTemplate<T>::operator=(const DatabaseData& toCopy)
{
	assert(0);
}

#endif
