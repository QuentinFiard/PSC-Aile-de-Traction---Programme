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
			return new Vecteur(binaryData);
			break;
			
		case TYPE_VECTEUR3D:
			return new Vecteur3D(binaryData);
			break;
			
		case TYPE_REFERENTIEL:
			return new Referentiel(binaryData);
			break;
			
		case TYPE_REFERENTIEL3D:
			return new Referentiel3D(binaryData);
			break;
			
		case TYPE_QUATERNION:
			return new Quaternion(binaryData);
			break;
			
		case TYPE_POINT:
			return new Point(binaryData);
			break;
			
		case TYPE_MATRICE:
			return new MatriceTemplate<Matrice>(binaryData);
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
			return new Vecteur();
			break;
			
		case TYPE_VECTEUR3D:
			return new Vecteur3D();
			break;
			
		case TYPE_REFERENTIEL:
			return new Referentiel();
			break;
			
		case TYPE_REFERENTIEL3D:
			return new Referentiel3D();
			break;
			
		case TYPE_QUATERNION:
			return new Quaternion();
			break;
			
		case TYPE_POINT:
			return new Point();
			break;
			
		case TYPE_MATRICE:
			return new MatriceTemplate<Matrice>();
			break;
			
		default:
			return NULL;
			break;
	}
}