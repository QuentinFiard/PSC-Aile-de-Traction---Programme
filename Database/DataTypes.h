//
//  DataTypes.h
//  PSC
//
//  Created by Quentin Fiard on 15/02/12.
//  Copyright (c) 2012 École Polytechnique. All rights reserved.
//

#ifndef PSC_DataTypes_h
#define PSC_DataTypes_h

#include <string>
#include <assert.h>
#include <iostream>

typedef uint32_t DataType;

enum DataTypeEnum
{
	TYPE_DOUBLE,
	TYPE_FLOAT,
	TYPE_INT,
	TYPE_INT64,
	
	TYPE_VECTEUR,
	TYPE_VECTEUR3D,
	TYPE_REFERENTIEL,
	TYPE_REFERENTIEL3D,
	TYPE_QUATERNION,
	TYPE_POINT,
	TYPE_MATRICE,
	TYPE_SINGLEVALUEDATA,
	TYPE_VECTORDATA,
	TYPE_MATRIXDATA,
	
	TYPE_STRING
};

std::string typeToString(DataType type);

template<typename T>
class Source;

template<typename T>
class Donnee;

template<typename T>
DataType typeOfTemplate()
{
	std::cout << "Type non supporté";
	assert(0);
	return 0;
}

DataType typeOfData(DataType type);


#endif
