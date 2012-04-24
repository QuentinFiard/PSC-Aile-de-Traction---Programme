//
//  MotorStatus.h
//  PSC
//
//  Created by Quentin Fiard on 23/04/12.
//  Copyright (c) 2012 École Polytechnique. All rights reserved.
//

#import "Status.h"

@interface MotorStatus : Status

+(MotorStatus*)shared;

-(IBAction)retry:(id)sender;

@end
