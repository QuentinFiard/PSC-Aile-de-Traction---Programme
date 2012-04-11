//
//  GenericSource.cpp
//  PSC
//
//  Created by Quentin Fiard on 15/02/12.
//  Copyright (c) 2012 École Polytechnique. All rights reserved.
//

#include "GenericSource.h"
#include "Source.h"

#include "Vecteur.h"
#include "Vecteur3D.h"
#include "Quaternion.h"
#include "Matrice.h"
#include "Referentiel.h"
#include "Referentiel3D.h"
#include "Point.h"
#include "NumericValue.h"

GenericSource* GenericSource::sourceWithIDNameTypeRecordIDAndSize(sqlite_int64 ID,std::string name, DataType type, Record* record, std::size_t size)
{
	switch (type) 
	{
		case TYPE_DOUBLE:
			return new Source< NumericValue<double> >(ID,record,name,size);
			break;
			
		case TYPE_FLOAT:
			return new Source< NumericValue<float> >(ID,record,name,size);
			break;
			
		case TYPE_INT:
			return new Source< NumericValue<int> >(ID,record,name,size);
			break;
			
		case TYPE_INT64:
			return new Source< NumericValue<int64_t> >(ID,record,name,size);
			break;
			
		case TYPE_VECTEUR:
			return new Source<Vecteur>(ID,record,name,size);
			break;
			
		case TYPE_VECTEUR3D:
			return new Source<Vecteur3D>(ID,record,name,size);
			break;
			
		case TYPE_REFERENTIEL:
			return new Source<Referentiel>(ID,record,name,size);
			break;
			
		case TYPE_REFERENTIEL3D:
			return new Source<Referentiel3D>(ID,record,name,size);
			break;
			
		case TYPE_QUATERNION:
			return new Source<Quaternion>(ID,record,name,size);
			break;
			
		case TYPE_POINT:
			return new Source<Point>(ID,record,name,size);
			break;
			
		case TYPE_MATRICE:
			return new Source<Matrice>(ID,record,name,size);
			break;
			
		default:
			return NULL;
			break;
	}
}