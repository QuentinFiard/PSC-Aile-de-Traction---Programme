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

GenericSource* GenericSource::sourceWithIDNameTypeAndRecordID(sqlite_int64 ID,std::string name, DataType type, Record* record)
{
	switch (type) 
	{
		case TYPE_DOUBLE:
			return new Source<double>(ID,record,name);
			break;
			
		case TYPE_FLOAT:
			return new Source<float>(ID,record,name);
			break;
			
		case TYPE_INT:
			return new Source<int>(ID,record,name);
			break;
			
		case TYPE_INT64:
			return new Source<int64_t>(ID,record,name);
			break;
			
		case TYPE_VECTEUR:
			return new Source<Vecteur>(ID,record,name);
			break;
			
		case TYPE_VECTEUR3D:
			return new Source<Vecteur3D>(ID,record,name);
			break;
			
		case TYPE_REFERENTIEL:
			return new Source<Referentiel>(ID,record,name);
			break;
			
		case TYPE_REFERENTIEL3D:
			return new Source<Referentiel3D>(ID,record,name);
			break;
			
		case TYPE_QUATERNION:
			return new Source<Quaternion>(ID,record,name);
			break;
			
		case TYPE_POINT:
			return new Source<Point>(ID,record,name);
			break;
			
		case TYPE_MATRICE:
			return new Source<Matrice>(ID,record,name);
			break;
			
		default:
			return NULL;
			break;
	}
}