//
//  ConnectionUSBStatus.h
//  PSC
//
//  Created by Quentin Fiard on 15/03/12.
//  Copyright (c) 2012 École Polytechnique. All rights reserved.
//

#import "Status.h"

class ConnectionUSB;

@interface ConnectionUSBStatus : Status

+(CameraStatus*)shared;

-(IBAction)retry:(id)sender;

@end
