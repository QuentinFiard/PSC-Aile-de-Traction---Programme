//
//  JoystickStatus.h
//  PSC
//
//  Created by Quentin Fiard on 11/04/12.
//  Copyright (c) 2012 École Polytechnique. All rights reserved.
//

#import "Status.h"

@interface JoystickStatus : Status

+(JoystickStatus*)shared;

-(IBAction)retry:(id)sender;

@end
