//
//  VectorData.h
//  PSC
//
//  Created by Quentin Fiard on 14/02/12.
//  Copyright (c) 2012 École Polytechnique. All rights reserved.
//

#ifndef PSC_VectorData_h
#define PSC_VectorData_h

#include "DatabaseData.h"
#include <string>

class VectorData : public DatabaseData
{
public:
	virtual bool setValueAtIndexWithString(std::size_t i,std::string& text) = 0;
	virtual std::string valueAtIndexToString(std::size_t i) const = 0;
	
	virtual std::size_t dim() const = 0;
	virtual void setDim(std::size_t n) = 0;
	virtual bool isResizable() = 0;
	
	virtual void operator=(const VectorData& toCopy) = 0;
};

#endif
