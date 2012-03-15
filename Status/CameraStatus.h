//
//  CameraStatus.h
//  PSC
//
//  Created by Quentin Fiard on 21/02/12.
//  Copyright (c) 2012 École Polytechnique. All rights reserved.
//

#import "Status.h"

@interface CameraStatus : Status
{
	
}

+(CameraStatus*)shared;

-(IBAction)retry:(id)sender;

@end
