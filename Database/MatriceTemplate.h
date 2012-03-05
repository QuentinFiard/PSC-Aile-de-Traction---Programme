//
//  MatriceTemplate.h
//  PSC
//
//  Created by Quentin Fiard on 16/02/12.
//  Copyright (c) 2012 École Polytechnique. All rights reserved.
//

#ifndef PSC_MatriceTemplate_h
#define PSC_MatriceTemplate_h

#include "MatrixData.h"
#include <vector>
#include "DataTypes.h"
#include <assert.h>

template<typename T>
class MatriceTemplate : public MatrixData
{
public:
	MatriceTemplate();
	MatriceTemplate(std::vector<uint8_t>& data);
	
	bool setValueAtIndexWithString(std::size_t i,std::size_t j,std::string& text);
	std::string valueAtIndexToString(std::size_t i,std::size_t j) const;
	
	std::size_t nbLignes() const;
	std::size_t nbColonnes() const;
	void setNbLignes(std::size_t n);
	void setNbColonnes(std::size_t n);
	
	void operator=(const MatrixData& toCopy);
	void operator=(const MatriceTemplate<T>& toCopy);
	
	std::vector<uint8_t> toBinary() const;
	std::string toCSVString() const;
	DataType type() const;
	
	void operator=(const DatabaseData& toCopy);
	
private:
	T matrix;
};

template<typename T>
MatriceTemplate<T>::MatriceTemplate() : matrix()
{
	
}

template<typename T>
MatriceTemplate<T>::MatriceTemplate(std::vector<uint8_t>& data) : matrix(data)
{
	
}

template<typename T>
bool MatriceTemplate<T>::setValueAtIndexWithString(std::size_t i,std::size_t j,std::string& text)
{
	return matrix.setValueAtIndexWithString(i,j,text);
}

template<typename T>
std::string MatriceTemplate<T>::valueAtIndexToString(std::size_t i,std::size_t j) const
{
	return matrix.valueAtIndexToString(i,j);
}

template<typename T>
std::size_t MatriceTemplate<T>::nbLignes() const
{
	return matrix.nbLignes();
}

template<typename T>
std::size_t MatriceTemplate<T>::nbColonnes() const
{
	return matrix.nbColonnes();
}

template<typename T>
void MatriceTemplate<T>::setNbLignes(std::size_t n)
{
	matrix.setNbLignes(n);
}

template<typename T>
void MatriceTemplate<T>::setNbColonnes(std::size_t n)
{
	matrix.setNbColonnes(n);
}

template<typename T>
void MatriceTemplate<T>::operator=(const MatrixData& toCopy)
{
	assert(0);
}

template<typename T>
void MatriceTemplate<T>::operator=(const MatriceTemplate<T>& toCopy)
{
	matrix = toCopy.matrix;
}

template<typename T>
std::vector<uint8_t> MatriceTemplate<T>::toBinary() const
{
	return matrix.toBinary();
}

template<typename T>
std::string MatriceTemplate<T>::toCSVString() const
{
	return matrix.toCSVString();
}

template<typename T>
DataType MatriceTemplate<T>::type() const
{
	return typeOfTemplate<T>();
}

template<typename T>
void MatriceTemplate<T>::operator=(const DatabaseData& toCopy)
{
	assert(0);
}

#endif
