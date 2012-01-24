//
//  Status.m
//  PSC
//
//  Created by Quentin Fiard on 23/01/12.
//  Copyright (c) 2012 École Polytechnique. All rights reserved.
//

#import "Status.h"

static NSImage* image0,*image1,*image2;

@implementation Status

@synthesize titre;
@dynamic statusImage;
@dynamic isOk;
@synthesize target;

@synthesize status;


-(id)init
{
	self = [super init];
	if(self)
	{
		status = 1;
		self.titre = @"Non défini";
	}
	return self;
}

-(NSImage*)statusImage
{
	switch (status)
	{
		case 0:
			if(!image0)
			{
				image0 = [[NSImage imageNamed:@"status0.png"] retain];
			}
			return image0;
			break;
			
		case 1:
			if(!image1)
			{
				image1 = [[NSImage imageNamed:@"status1.png"] retain];
			}
			return image1;
			break;
			
		case 2:
			if(!image2)
			{
				image2 = [[NSImage imageNamed:@"status2.png"] retain];
			}
			return image2;
			break;
			
		default:
			break;
	}
}

-(NSNumber*)isOk
{
	return [NSNumber numberWithBool:status==0];
}

@end
