//
//  Status.h
//  PSC
//
//  Created by Quentin Fiard on 23/01/12.
//  Copyright (c) 2012 École Polytechnique. All rights reserved.
//

#import <Foundation/Foundation.h>

enum STATUS
{
	STATUS_OK,
	STATUS_WAITING,
	STATUS_FAILED
};

@interface Status : NSObject
{
	NSString* titre;
	NSUInteger status;
	NSImage* statusImage;
	
	id target;
}

-(IBAction)retry:(id)sender;

@property (retain,nonatomic) NSString* titre;
@property (assign,nonatomic) NSImage* statusImage;
@property (assign,nonatomic) id target;
@property (readonly,nonatomic) NSNumber* isOk;

@property (nonatomic) NSUInteger status;

@end

#import "CameraStatus.h"
