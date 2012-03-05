//
//  MatrixData.h
//  PSC
//
//  Created by Quentin Fiard on 14/02/12.
//  Copyright (c) 2012 École Polytechnique. All rights reserved.
//

#ifndef PSC_MatrixData_h
#define PSC_MatrixData_h

#include "DatabaseData.h"
#include <string>

class MatrixData : public DatabaseData
{
public:
	virtual bool setValueAtIndexWithString(std::size_t i,std::size_t j,std::string& text) = 0;
	virtual std::string valueAtIndexToString(std::size_t i,std::size_t j) const = 0;
	
	virtual std::size_t nbLignes() const = 0;
	virtual std::size_t nbColonnes() const = 0;
	virtual void setNbLignes(std::size_t n) = 0;
	virtual void setNbColonnes(std::size_t n) = 0;
	
	virtual void operator=(const MatrixData& toCopy) = 0;
};

#endif
