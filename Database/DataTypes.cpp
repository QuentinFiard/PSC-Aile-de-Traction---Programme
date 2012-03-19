//
//  DataTypes.cpp
//  PSC
//
//  Created by Quentin Fiard on 15/02/12.
//  Copyright (c) 2012 École Polytechnique. All rights reserved.
//

#include "DataTypes.h"

#include "Source.h"
#include "Donnee.h"

#include "Vecteur.h"
#include "Vecteur3D.h"
#include "Quaternion.h"
#include "Matrice.h"
#include "Referentiel.h"
#include "Referentiel3D.h"
#include "Point.h"

#include "NumericValue.h"
#include "StringValue.h"

std::string typeToString(DataType type)
{
	switch(type)
	{
		case TYPE_DOUBLE:
			return "double";
			break;
			
		case TYPE_FLOAT:
			return "float";
			break;
			
		case TYPE_INT:
			return "int";
			break;
			
		case TYPE_INT64:
			return "int64";
			break;
			
		case TYPE_MATRICE:
			return "Matrice";
			break;
			
		case TYPE_POINT:
			return "Point";
			break;
			
		case TYPE_QUATERNION:
			return "Quaternion";
			break;
			
		case TYPE_REFERENTIEL:
			return "Referentiel";
			break;
			
		case TYPE_REFERENTIEL3D:
			return "Referentiel 3D";
			break;
			
		case TYPE_VECTEUR:
			return "Vecteur";
			break;
			
		case TYPE_VECTEUR3D:
			return "Vecteur 3D";
			break;
			
		case TYPE_STRING:
			return "String";
			break;
			
		default:
			return "Type non supporté";
			break;
	}
}

#pragma mark - Type

DataType typeOfTemplate(const double value)
{
	return TYPE_DOUBLE;
}

DataType typeOfTemplate(const float value)
{
	return TYPE_FLOAT;
}

DataType typeOfTemplate(const int value)
{
	return TYPE_INT;
}

DataType typeOfTemplate(const int64_t value)
{
	return TYPE_INT64;
}

DataType typeOfTemplate(const Vecteur value)
{
	return TYPE_VECTEUR;
}

DataType typeOfTemplate(const Vecteur3D value)
{
	return TYPE_VECTEUR3D;
}

DataType typeOfTemplate(const Referentiel value)
{
	return TYPE_REFERENTIEL;
}

DataType typeOfTemplate(const Referentiel3D value)
{
	return TYPE_REFERENTIEL3D;
}

DataType typeOfTemplate(const Quaternion value)
{
	return TYPE_QUATERNION;
}

DataType typeOfTemplate(const Point value)
{
	return TYPE_POINT;
}

DataType typeOfTemplate(const Matrice value)
{
	return TYPE_MATRICE;
}

DataType typeOfTemplate(const std::string value)
{
	return TYPE_STRING;
}

DataType typeOfTemplate(const NumericValue<double> value)
{
	return TYPE_DOUBLE;
}


DataType typeOfTemplate(const NumericValue<int> value)
{
	return TYPE_INT;
}

DataType typeOfTemplate(const NumericValue<int64_t> value)
{
	return TYPE_INT64;
}

DataType typeOfTemplate(const NumericValue<float> value)
{
	return TYPE_FLOAT;
}

DataType typeOfTemplate(const StringValue value)
{
	return TYPE_STRING;
}

DataType typeOfTemplate(const double* value)
{
	return TYPE_DOUBLE;
}

DataType typeOfTemplate(const float* value)
{
	return TYPE_FLOAT;
}

DataType typeOfTemplate(const int* value)
{
	return TYPE_INT;
}

DataType typeOfTemplate(const int64_t* value)
{
	return TYPE_INT64;
}

DataType typeOfTemplate(const Vecteur* value)
{
	return TYPE_VECTEUR;
}

DataType typeOfTemplate(const Vecteur3D* value)
{
	return TYPE_VECTEUR3D;
}

DataType typeOfTemplate(const Referentiel* value)
{
	return TYPE_REFERENTIEL;
}

DataType typeOfTemplate(const Referentiel3D* value)
{
	return TYPE_REFERENTIEL3D;
}

DataType typeOfTemplate(const Quaternion* value)
{
	return TYPE_QUATERNION;
}

DataType typeOfTemplate(const Point* value)
{
	return TYPE_POINT;
}

DataType typeOfTemplate(const Matrice* value)
{
	return TYPE_MATRICE;
}

DataType typeOfTemplate(const NumericValue<double>* value)
{
	return TYPE_DOUBLE;
}

DataType typeOfTemplate(const NumericValue<int>* value)
{
	return TYPE_INT;
}

DataType typeOfTemplate(const NumericValue<int64_t>* value)
{
	return TYPE_INT64;
}

DataType typeOfTemplate(const NumericValue<float>* value)
{
	return TYPE_FLOAT;
}

DataType typeOfTemplate(const StringValue* value)
{
	return value->type();
}

DataType typeOfTemplate(const std::string* value)
{
	return TYPE_STRING;
}

DataType typeOfTemplate(const SingleValueData* value)
{
	return value->type();
}

DataType typeOfTemplate(const VectorData* value)
{
	return value->type();
}

DataType typeOfTemplate(const MatrixData* value)
{
	return value->type();
}

#pragma mark - Type of data

DataType typeOfData(DataType type)
{
	switch(type) 
	{
		case TYPE_DOUBLE:
			return TYPE_SINGLEVALUEDATA;
			break;
			
		case TYPE_FLOAT:
			return TYPE_SINGLEVALUEDATA;
			break;
			
		case TYPE_INT:
			return TYPE_SINGLEVALUEDATA;
			break;
			
		case TYPE_INT64:
			return TYPE_SINGLEVALUEDATA;
			break;
			
		case TYPE_VECTEUR:
			return TYPE_VECTORDATA;
			break;
			
		case TYPE_VECTEUR3D:
			return TYPE_VECTORDATA;
			break;
			
		case TYPE_QUATERNION:
			return TYPE_VECTORDATA;
			break;
			
		case TYPE_POINT:
			return TYPE_VECTORDATA;
			break;
			
		case TYPE_MATRICE:
			return TYPE_MATRIXDATA;
			break;
			
		case TYPE_STRING:
			return TYPE_SINGLEVALUEDATA;
			break;
			
		default:
			std::cout << "Type non supporté" << std::endl;
			assert(0);
			break;
	}
}

bool isNumeric(DataType type)
{
	switch(type) 
	{
		case TYPE_DOUBLE:
			return true;
			break;
			
		case TYPE_FLOAT:
			return true;
			break;
			
		case TYPE_INT:
			return true;
			break;
			
		case TYPE_INT64:
			return true;
			break;
			
		case TYPE_VECTEUR:
			return false;
			break;
			
		case TYPE_VECTEUR3D:
			return false;
			break;
			
		case TYPE_QUATERNION:
			return false;
			break;
			
		case TYPE_POINT:
			return false;
			break;
			
		case TYPE_MATRICE:
			return false;
			break;
			
		case TYPE_STRING:
			return false;
			break;
			
		default:
			std::cout << "Type non supporté" << std::endl;
			assert(0);
			break;
	}
}