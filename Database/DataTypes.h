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
class NumericValue;

class Vecteur;
class Vecteur3D;
class Referentiel;
class Referentiel3D;
class Quaternion;
class Point;
class Matrice;
class StringValue;
class SingleValueData;
class VectorData;
class MatrixData;

DataType typeOfTemplate(const double value);

DataType typeOfTemplate(const float value);

DataType typeOfTemplate(const int value);

DataType typeOfTemplate(const int64_t value);

DataType typeOfTemplate(const Vecteur value);

DataType typeOfTemplate(const Vecteur3D value);

DataType typeOfTemplate(const Referentiel value);

DataType typeOfTemplate(const Referentiel3D value);

DataType typeOfTemplate(const Quaternion value);

DataType typeOfTemplate(const Point value);

DataType typeOfTemplate(const Matrice value);

DataType typeOfTemplate(const std::string value);

DataType typeOfTemplate(const NumericValue<double> value);


DataType typeOfTemplate(const NumericValue<int> value);

DataType typeOfTemplate(const NumericValue<int64_t> value);

DataType typeOfTemplate(const NumericValue<float> value);

DataType typeOfTemplate(const StringValue value);

DataType typeOfTemplate(const double* value);

DataType typeOfTemplate(const float* value);

DataType typeOfTemplate(const int* value);

DataType typeOfTemplate(const int64_t* value);

DataType typeOfTemplate(const Vecteur* value);

DataType typeOfTemplate(const Vecteur3D* value);

DataType typeOfTemplate(const Referentiel* value);

DataType typeOfTemplate(const Referentiel3D* value);

DataType typeOfTemplate(const Quaternion* value);

DataType typeOfTemplate(const Point* value);

DataType typeOfTemplate(const Matrice* value);

DataType typeOfTemplate(const NumericValue<double>* value);

DataType typeOfTemplate(const NumericValue<int>* value);

DataType typeOfTemplate(const NumericValue<int64_t>* value);

DataType typeOfTemplate(const NumericValue<float>* value);

DataType typeOfTemplate(const StringValue* value);

DataType typeOfTemplate(const std::string* value);

DataType typeOfTemplate(const SingleValueData* value);

DataType typeOfTemplate(const VectorData* value);

DataType typeOfTemplate(const MatrixData* value);



DataType typeOfData(DataType type);

bool isNumeric(DataType type);


#endif
