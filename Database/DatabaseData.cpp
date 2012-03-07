//
//  DatabaseData.cpp
//  PSC
//
//  Created by Quentin Fiard on 15/02/12.
//  Copyright (c) 2012 École Polytechnique. All rights reserved.
//

#include "DatabaseData.h"

#include "Vecteur.h"
#include "Vecteur3D.h"
#include "Quaternion.h"
#include "Matrice.h"
#include "Referentiel.h"
#include "Referentiel3D.h"
#include "Point.h"
#include "NumericValue.h"
#include "MatriceTemplate.h"
#include "VectorTemplate.h"
#include "StringValue.h"

DatabaseData* DatabaseData::dataWithType(std::vector<uint8_t>& binaryData, DataType dataType)
{
	switch (dataType) 
	{
		case TYPE_DOUBLE:
			return new NumericValue<double>(binaryData);
			break;
			
		case TYPE_FLOAT:
			return new NumericValue<float>(binaryData);
			break;
			
		case TYPE_INT:
			return new NumericValue<int>(binaryData);
			break;
			
		case TYPE_INT64:
			return new NumericValue<int64_t>(binaryData);
			break;
			
		case TYPE_VECTEUR:
			return new VectorTemplate<Vecteur>(binaryData);
			break;
			
		case TYPE_VECTEUR3D:
			return new VectorTemplate<Vecteur3D>(binaryData);
			break;
			
		case TYPE_REFERENTIEL:
			return new Referentiel(binaryData);
			break;
			
		case TYPE_REFERENTIEL3D:
			return new Referentiel3D(binaryData);
			break;
			
		case TYPE_QUATERNION:
			return new VectorTemplate<Quaternion>(binaryData);
			break;
			
		case TYPE_POINT:
			return new VectorTemplate<Point>(binaryData);
			break;
			
		case TYPE_MATRICE:
			return new MatriceTemplate<Matrice>(binaryData);
			break;
			
		case TYPE_STRING:
			return new StringValue(binaryData);
			break;
			
		default:
			return NULL;
			break;
	}
}

DatabaseData* DatabaseData::emptyDataWithType(DataType dataType)
{
	switch (dataType) 
	{
		case TYPE_DOUBLE:
			return new NumericValue<double>();
			break;
			
		case TYPE_FLOAT:
			return new NumericValue<float>();
			break;
			
		case TYPE_INT:
			return new NumericValue<int>();
			break;
			
		case TYPE_INT64:
			return new NumericValue<int64_t>();
			break;
			
		case TYPE_VECTEUR:
			return new VectorTemplate<Vecteur>();
			break;
			
		case TYPE_VECTEUR3D:
			return new VectorTemplate<Vecteur3D>();
			break;
			
		case TYPE_REFERENTIEL:
			return new Referentiel();
			break;
			
		case TYPE_REFERENTIEL3D:
			return new Referentiel3D();
			break;
			
		case TYPE_QUATERNION:
			return new VectorTemplate<Quaternion>();
			break;
			
		case TYPE_POINT:
			return new VectorTemplate<Point>();
			break;
			
		case TYPE_MATRICE:
			return new MatriceTemplate<Matrice>();
			break;
			
		case TYPE_STRING:
			return new StringValue();
			break;
			
		default:
			return NULL;
			break;
	}
}