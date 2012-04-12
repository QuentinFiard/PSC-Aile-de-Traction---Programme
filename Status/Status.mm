//
//  Status.m
//  PSC
//
//  Created by Quentin Fiard on 23/01/12.
//  Copyright (c) 2012 École Polytechnique. All rights reserved.
//

#import "Status.h"
#import "StatusController.h"

static NSImage* image0,*image1,*image2;

@implementation Status

@synthesize titre;
@synthesize statusImage;
@synthesize isOk;
@synthesize target;

@synthesize status;


-(id)init
{
	self = [super init];
	if(self)
	{
		self.status = STATUS_WAITING;
		self.titre = @"Non défini";
		self.target = self;
	}
	return self;
}

-(void)setStatus:(NSUInteger)aStatus
{
	status = aStatus;
	switch (status)
	{
		case 0:
			if(!image0)
			{
				image0 = [[NSImage imageNamed:@"status0.png"] retain];
			}
			self.statusImage = image0;
			break;
			
		case 1:
			if(!image1)
			{
				image1 = [[NSImage imageNamed:@"status1.png"] retain];
			}
			self.statusImage = image1;
			break;
			
		case 2:
			if(!image2)
			{
				image2 = [[NSImage imageNamed:@"status2.png"] retain];
			}
			self.statusImage = image2;
			break;
			
		default:
			break;
	}
	[self setIsOk:[NSNumber numberWithBool:(status==STATUS_OK)]];
}

-(IBAction)retry:(id)sender
{
	self.status = STATUS_OK;
}

@end
