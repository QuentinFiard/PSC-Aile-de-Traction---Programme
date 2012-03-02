//
//  RecordForDisplay.m
//  PSC
//
//  Created by Quentin Fiard on 13/02/12.
//  Copyright (c) 2012 École Polytechnique. All rights reserved.
//

#import "RecordForDisplay.h"

#include <boost/date_time.hpp>

using namespace boost::posix_time;

@implementation RecordForDisplay

@dynamic date,tag,record;

-(id)initWithRecord:(Record*)record
{
	record_ = record;
	
	return self;
}

-(NSString*)date
{
	return [NSString stringWithUTF8String:record_->displayDate().c_str()];
}

-(Record*)record
{
	return record_;
}

-(void)dealloc
{
	delete record_;
}

-(NSString*)tag
{
	return [NSString stringWithUTF8String:record_->tag().c_str()];
}

-(void)setTag:(NSString*)newTag
{
	record_->setTag(std::string([newTag UTF8String]));
	record_->save();
}

@end
