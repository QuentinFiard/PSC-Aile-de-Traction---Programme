//
//  SensorForDisplay.h
//  PSC
//
//  Created by Quentin Fiard on 18/03/12.
//  Copyright (c) 2012 École Polytechnique. All rights reserved.
//

#import <Foundation/Foundation.h>

#import "CapteurAngulaire.h"

@interface SensorForDisplay : NSObject
{
	CapteurAngulaire* capteur;	
}

-(id)initWithSensor:(Sensor)sensor;

@property (nonatomic) NSNumber* grandeurCapteur;
@property (readonly,nonatomic) NSNumber* sensor;

@end
