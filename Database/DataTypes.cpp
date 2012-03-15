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

template<>
DataType typeOfTemplate<double>()
{
	return TYPE_DOUBLE;
}

template<>
DataType typeOfTemplate<float>()
{
	return TYPE_FLOAT;
}

template<>
DataType typeOfTemplate<int>()
{
	return TYPE_INT;
}

template<>
DataType typeOfTemplate<int64_t>()
{
	return TYPE_INT64;
}

template<>
DataType typeOfTemplate<Vecteur>()
{
	return TYPE_VECTEUR;
}

template<>
DataType typeOfTemplate<Vecteur3D>()
{
	return TYPE_VECTEUR3D;
}

template<>
DataType typeOfTemplate<Referentiel>()
{
	return TYPE_REFERENTIEL;
}

template<>
DataType typeOfTemplate<Referentiel3D>()
{
	return TYPE_REFERENTIEL3D;
}

template<>
DataType typeOfTemplate<Quaternion>()
{
	return TYPE_QUATERNION;
}

template<>
DataType typeOfTemplate<Point>()
{
	return TYPE_POINT;
}

template<>
DataType typeOfTemplate<Matrice>()
{
	return TYPE_MATRICE;
}

template<>
DataType typeOfTemplate< NumericValue<double> >()
{
	return typeOfTemplate<double>();
}

template<>
DataType typeOfTemplate< NumericValue<int> >()
{
	return typeOfTemplate<int>();
}

template<>
DataType typeOfTemplate< NumericValue<int64_t> >()
{
	return typeOfTemplate<int64_t>();
}

template<>
DataType typeOfTemplate< NumericValue<float> >()
{
	return typeOfTemplate<float>();
}

template<>
DataType typeOfTemplate< StringValue >()
{
	return TYPE_STRING;
}

template<>
DataType typeOfTemplate<double*>()
{
	return TYPE_DOUBLE;
}

template<>
DataType typeOfTemplate<float*>()
{
	return TYPE_FLOAT;
}

template<>
DataType typeOfTemplate<int*>()
{
	return TYPE_INT;
}

template<>
DataType typeOfTemplate<int64_t*>()
{
	return TYPE_INT64;
}

template<>
DataType typeOfTemplate<Vecteur*>()
{
	return TYPE_VECTEUR;
}

template<>
DataType typeOfTemplate<Vecteur3D*>()
{
	return TYPE_VECTEUR3D;
}

template<>
DataType typeOfTemplate<Referentiel*>()
{
	return TYPE_REFERENTIEL;
}

template<>
DataType typeOfTemplate<Referentiel3D*>()
{
	return TYPE_REFERENTIEL3D;
}

template<>
DataType typeOfTemplate<Quaternion*>()
{
	return TYPE_QUATERNION;
}

template<>
DataType typeOfTemplate<Point*>()
{
	return TYPE_POINT;
}

template<>
DataType typeOfTemplate<Matrice*>()
{
	return TYPE_MATRICE;
}

template<>
DataType typeOfTemplate< NumericValue<double>* >()
{
	return typeOfTemplate<double>();
}

template<>
DataType typeOfTemplate< NumericValue<int>* >()
{
	return typeOfTemplate<int>();
}

template<>
DataType typeOfTemplate< NumericValue<int64_t>* >()
{
	return typeOfTemplate<int64_t>();
}

template<>
DataType typeOfTemplate< NumericValue<float>* >()
{
	return typeOfTemplate<float>();
}

template<>
DataType typeOfTemplate< StringValue* >()
{
	return TYPE_STRING;
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