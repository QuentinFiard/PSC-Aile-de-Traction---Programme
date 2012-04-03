//
//  SensorForDisplay.m
//  PSC
//
//  Created by Quentin Fiard on 18/03/12.
//  Copyright (c) 2012 École Polytechnique. All rights reserved.
//

#import "SensorForDisplay.h"

@implementation SensorForDisplay

@dynamic sensor,grandeurCapteur;

-(id)initWithSensor:(Sensor)sensor
{
	self = [super init];
	if(self)
	{
		capteur = CapteurAngulaire::sharedWithSensorNumber(sensor);
	}
	return self;
}

-(void)setGrandeurCapteur:(NSNumber *)grandeurCapteur
{
	capteur->setGrandeurMesuree(static_cast<GrandeurCapteur>([grandeurCapteur intValue]));
}

-(NSNumber*)grandeurCapteur
{
	return [NSNumber numberWithInt:capteur->grandeurMesuree()];
}

-(NSNumber*)sensor
{
	return [NSNumber numberWithInt:capteur->sensorNumber()];
}

- (id)copyWithZone:(NSZone *)zone
{
    SensorForDisplay *res = [super copyWithZone:zone];
    if (res == nil) {
        return nil;
    }
    
    res->capteur = capteur;
    
    return res;
}

@end
