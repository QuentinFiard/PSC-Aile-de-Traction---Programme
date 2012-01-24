//
//  Status.h
//  PSC
//
//  Created by Quentin Fiard on 23/01/12.
//  Copyright (c) 2012 École Polytechnique. All rights reserved.
//

#import <Foundation/Foundation.h>

#define STATUS_OK 0
#define STATUS_WAITING 1
#define STATUS_FAILED 2

@interface Status : NSObject
{
	NSString* titre;
	NSUInteger status;
	
	id target;
}

@property (retain,nonatomic) NSString* titre;
@property (readonly) NSImage* statusImage;
@property (retain,nonatomic) id target;
@property (readonly,nonatomic) NSNumber* isOk;

@property (nonatomic) NSUInteger status;

@end
