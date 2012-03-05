//
//  RecordForDisplay.h
//  PSC
//
//  Created by Quentin Fiard on 13/02/12.
//  Copyright (c) 2012 École Polytechnique. All rights reserved.
//

#import "Record.h"

@interface RecordForDisplay : NSObject
{
	Record* record_;
}

-(id)initWithRecord:(Record*)record;

@property (readonly,nonatomic) NSString* date;
@property (assign,nonatomic) NSString* tag;
@property (readonly,nonatomic) Record* record;

@end
