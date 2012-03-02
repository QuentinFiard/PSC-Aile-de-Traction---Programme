//
//  Utilities.h
//  PSC
//
//  Created by Quentin Fiard on 12/02/12.
//  Copyright (c) 2012 École Polytechnique. All rights reserved.
//

#ifndef PSC_Utilities_h
#define PSC_Utilities_h

#include <string>

template<typename T>
bool isNumber(T& value)
{
	return false;
}

std::string prepareForCSV(std::string s);

std::string pathToDatabase();

#endif
