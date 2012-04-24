//
//  ConnectionUSBController.m
//  PSC
//
//  Created by Quentin Fiard on 18/03/12.
//  Copyright (c) 2012 École Polytechnique. All rights reserved.
//

#import "ConnectionUSBController.h"
#import "ConnectionUSB.h"
#include <math.h>

#import "SensorForDisplay.h"
#import "Vecteur.h"
#import "VectorTemplate.h"
#import "Database.h"
#import "Moteur.h"

static ConnectionUSBController* shared;

@interface IntegerFormater : NSNumberFormatter 
{
    NSUInteger limit;
}

-(id)initWithLimit:(NSUInteger)limit;

@end

@implementation IntegerFormater

-(id)initWithLimit:(NSUInteger)newlimit
{
	self = [super init];
	if(self)
	{
		limit = newlimit;
	}
	return self;
}

- (BOOL)isPartialStringValid:(NSString **)partialStringPtr
       proposedSelectedRange:(NSRangePointer)proposedSelRangePtr
              originalString:(NSString *)origString
       originalSelectedRange:(NSRange)origSelRange
            errorDescription:(NSString **)error
{
	NSUInteger value = [*partialStringPtr integerValue];
	if(![[[NSNumber numberWithInteger:value] stringValue] isEqualToString:*partialStringPtr])
	{
		return false;
	}
	if(value >= limit)
	{
		return false;
	}
	return true;
}

-(void)dealloc
{
	[super dealloc];
}

@end

@implementation ConnectionUSBController

+(ConnectionUSBController*)shared
{
	return shared;
}

- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil
{
    self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil];
    if (self) {
        shared = self;
		
		currentMinDuration = -1;
		currentNeutralDuration = -1;
		currentMaxDuration = -1;
		
		speedCoeffs.gainProportionnel = -1;
		speedCoeffs.gainDifferentiel = -1;
		speedCoeffs.gainIntegral = -1;
		
		positionCoeffs.gainProportionnel = -1;
		positionCoeffs.gainDifferentiel = -1;
		positionCoeffs.gainIntegral = -1;
		
		currentPIDPeriod = -1;
		
		formatter = [[NSNumberFormatter alloc] init];
		
		[formatter setNumberStyle:NSNumberFormatterDecimalStyle];
		[formatter setRoundingMode:NSNumberFormatterRoundHalfUp];
		[formatter setDecimalSeparator:@"."];
		[formatter setMaximumFractionDigits:5];
		
		sensorArray = [[NSMutableArray alloc] initWithCapacity:6];
		
		for(Sensor i=0 ; i<6 ; i++)
		{
			SensorForDisplay* sensor = [[SensorForDisplay alloc] initWithSensor:i];
			[self insertObject:sensor inSensorArrayAtIndex:i];
			[sensor release];
		}
    }
    
    return self;
}

-(void)awakeFromNib
{	
	if(ConnectionUSB::isConnected())
	{
		[self setConnected:YES];
	}
	else
	{
		[self setConnected:NO];
	}
}

-(IBAction)retryConnection:(id)sender
{
	ConnectionUSB::connect();
	
	[self setConnected:ConnectionUSB::isConnected()];
}

-(void)setConnected:(BOOL)isConnected
{
	if(isConnected)
	{		
		if(![connectedView isDescendantOf:[self view]])
		{
			[[self view] addSubview:connectedView];
			[connectedView setFrame:self.view.bounds];
		}
		if([unconnectedView isDescendantOf:self.view])
		{
			[unconnectedView removeFromSuperview];
		}
		
		currentMaxDuration = ConnectionUSB::readMotorSignalMaxDuration();
		currentNeutralDuration = ConnectionUSB::readMotorSignalNeutralDuration();
		currentMinDuration = ConnectionUSB::readMotorSignalMinDuration();
		
		if(Database::hasConfigurationField("SpeedPID"))
		{
			VectorTemplate<Vecteur>* coeffsPIDFromDatabase = static_cast< VectorTemplate<Vecteur>* >(Database::getConfigurationField("SpeedPID"));
			
			Vecteur coeffs = coeffsPIDFromDatabase->vector();
			
			speedCoeffs.gainProportionnel = coeffs[0];
			speedCoeffs.gainDifferentiel = coeffs[1];
			speedCoeffs.gainIntegral = coeffs[2];
			
			delete coeffsPIDFromDatabase;
		}
		
		if(Database::hasConfigurationField("PositionPID"))
		{
			VectorTemplate<Vecteur>* coeffsPIDFromDatabase = static_cast< VectorTemplate<Vecteur>* >(Database::getConfigurationField("PositionPID"));
			
			Vecteur coeffs = coeffsPIDFromDatabase->vector();
			
			positionCoeffs.gainProportionnel = coeffs[0];
			positionCoeffs.gainDifferentiel = coeffs[1];
			positionCoeffs.gainIntegral = coeffs[2];
			
			delete coeffsPIDFromDatabase;
		}
		
		if(Database::hasConfigurationField("TypeAsservissement"))
		{
			choixAsservissement.type = Database::getConfigurationFieldValue<int>("TypeAsservissement");
		}
		else
		{
			NumericValue<int> toSave(ASSERVISSEMENT_SANS);
			Database::setConfigurationField("TypeAsservissement",&toSave);
			choixAsservissement.type = ASSERVISSEMENT_SANS;
		}
		
		[self cancelSpeedPIDModif:self];
		[self cancelPositionPIDModif:self];
		[self cancelMaxSignalDurationModif:self];
		[self cancelNeutralSignalDurationModif:self];
		[self cancelMinSignalDurationModif:self];
		[self cancelChoixAsservissementModif:self];
	}
	else
	{
		if(![unconnectedView isDescendantOf:self.view])
		{
			[self.view addSubview:unconnectedView];
			[unconnectedView setFrame:self.view.bounds];
		}
		if([connectedView isDescendantOf:self.view])
		{
			[connectedView removeFromSuperview];
		}
	}
}

-(void)dealloc
{
	[formatter release];
	shared = nil;
}

-(IBAction)sendMinSignalDuration:(id)sender
{	
	currentMinDuration = [[formatter numberFromString:[minDuration stringValue]] doubleValue]/1000;
	
	ConnectionUSB::setMotorSignalMinDuration(currentMinDuration);
	
	currentMinDuration = ConnectionUSB::readMotorSignalMinDuration();
	
	[self cancelMinSignalDurationModif:self];
}

-(IBAction)cancelMinSignalDurationModif:(id)sender
{
	if(currentMinDuration==-1)
	{
		[minDuration setStringValue:@""];
	}
	else
	{
		[minDuration setStringValue:[formatter stringFromNumber:[NSNumber numberWithDouble:currentMinDuration*1000]]];
	}
	[minDurationCancel setHidden:YES];
	[minDurationConfirm setHidden:YES];
}

-(IBAction)sendNeutralSignalDuration:(id)sender
{	
	currentNeutralDuration = [[formatter numberFromString:[neutralDuration stringValue]] doubleValue]/1000;
	
	ConnectionUSB::setMotorSignalNeutralDuration(currentNeutralDuration);
	
	currentNeutralDuration = ConnectionUSB::readMotorSignalNeutralDuration();
	
	[self cancelNeutralSignalDurationModif:self];
}

-(IBAction)cancelNeutralSignalDurationModif:(id)sender
{
	if(currentNeutralDuration==-1)
	{
		[neutralDuration setStringValue:@""];
	}
	else
	{
		[neutralDuration setStringValue:[formatter stringFromNumber:[NSNumber numberWithDouble:currentNeutralDuration*1000]]];
	}
	[neutralDurationCancel setHidden:YES];
	[neutralDurationConfirm setHidden:YES];
}

-(IBAction)sendMaxSignalDuration:(id)sender
{	
	currentMaxDuration = [[formatter numberFromString:[maxDuration stringValue]] doubleValue]/1000;
	
	ConnectionUSB::setMotorSignalMaxDuration(currentMaxDuration);
	
	currentMaxDuration = ConnectionUSB::readMotorSignalMaxDuration();
	
	[self cancelMaxSignalDurationModif:self];
}

-(IBAction)cancelMaxSignalDurationModif:(id)sender
{
	if(currentMaxDuration==-1)
	{
		[maxDuration setStringValue:@""];
	}
	else
	{
		[maxDuration setStringValue:[formatter stringFromNumber:[NSNumber numberWithDouble:currentMaxDuration*1000]]];
	}
	
	[maxDurationCancel setHidden:YES];
	[maxDurationConfirm setHidden:YES];
}

-(IBAction)sendSpeedPIDCoeffs:(id)sender
{
	speedCoeffs.gainProportionnel = [[formatter numberFromString:[PIDSpeedProportionnel stringValue]] floatValue];
	speedCoeffs.gainDifferentiel = [[formatter numberFromString:[PIDSpeedDifferentiel stringValue]] floatValue];
	speedCoeffs.gainIntegral = [[formatter numberFromString:[PIDSpeedIntegral stringValue]] floatValue];
	
	Vecteur coeffs(3);
	
	coeffs[0] = speedCoeffs.gainProportionnel;
	coeffs[1] = speedCoeffs.gainDifferentiel;
	coeffs[2] = speedCoeffs.gainIntegral;
	
	Database::setConfigurationField("SpeedPID", &coeffs);
	
	[self cancelSpeedPIDModif:self];
}

-(IBAction)cancelSpeedPIDModif:(id)sender
{
	if(speedCoeffs.gainProportionnel==-1)
	{
		[PIDSpeedProportionnel setStringValue:@""];
	}
	else
	{
		[PIDSpeedProportionnel setStringValue:[formatter stringFromNumber:[NSNumber numberWithDouble:speedCoeffs.gainProportionnel]]];
	}
	
	if(speedCoeffs.gainDifferentiel==-1)
	{
		[PIDSpeedDifferentiel setStringValue:@""];
	}
	else
	{
		[PIDSpeedDifferentiel setStringValue:[formatter stringFromNumber:[NSNumber numberWithDouble:speedCoeffs.gainDifferentiel]]];
	}
	
	if(speedCoeffs.gainIntegral==-1)
	{
		[PIDSpeedIntegral setStringValue:@""];
	}
	else
	{
		[PIDSpeedIntegral setStringValue:[formatter stringFromNumber:[NSNumber numberWithDouble:speedCoeffs.gainIntegral]]];
	}
	
	[PIDSpeedCancel setHidden:YES];
	[PIDSpeedConfirm setHidden:YES];
}

-(IBAction)sendPositionPIDCoeffs:(id)sender
{
	positionCoeffs.gainProportionnel = [[formatter numberFromString:[PIDPositionProportionnel stringValue]] floatValue];
	positionCoeffs.gainDifferentiel = [[formatter numberFromString:[PIDPositionDifferentiel stringValue]] floatValue];
	positionCoeffs.gainIntegral = [[formatter numberFromString:[PIDPositionIntegral stringValue]] floatValue];
	
	Vecteur coeffs(3);
	
	coeffs[0] = positionCoeffs.gainProportionnel;
	coeffs[1] = positionCoeffs.gainDifferentiel;
	coeffs[2] = positionCoeffs.gainIntegral;
	
	Database::setConfigurationField("PositionPID", &coeffs);
	
	[self cancelPositionPIDModif:self];
}

-(IBAction)cancelPositionPIDModif:(id)sender
{
	if(positionCoeffs.gainProportionnel==-1)
	{
		[PIDPositionProportionnel setStringValue:@""];
	}
	else
	{
		[PIDPositionProportionnel setStringValue:[formatter stringFromNumber:[NSNumber numberWithDouble:positionCoeffs.gainProportionnel]]];
	}
	
	if(positionCoeffs.gainDifferentiel==-1)
	{
		[PIDPositionDifferentiel setStringValue:@""];
	}
	else
	{
		[PIDPositionDifferentiel setStringValue:[formatter stringFromNumber:[NSNumber numberWithDouble:positionCoeffs.gainDifferentiel]]];
	}
	
	if(positionCoeffs.gainIntegral==-1)
	{
		[PIDPositionIntegral setStringValue:@""];
	}
	else
	{
		[PIDPositionIntegral setStringValue:[formatter stringFromNumber:[NSNumber numberWithDouble:positionCoeffs.gainIntegral]]];
	}
	
	[PIDPositionCancel setHidden:YES];
	[PIDPositionConfirm setHidden:YES];
}

-(IBAction)sendChoixAsservissement:(id)sender
{	
	choixAsservissement.type = static_cast<TypeAsservissement>([choixAsservissementControl selectedSegment]);
	
	Moteur::setTypeAsservissement(choixAsservissement.type);
	
	[self cancelChoixAsservissementModif:self];
}

-(IBAction)cancelChoixAsservissementModif:(id)sender
{
	[choixAsservissementControl setSelectedSegment:choixAsservissement.type];
	
	[choixAsservissementConfirm setHidden:YES];
	[choixAsservissementCancel setHidden:YES];
}

-(IBAction)choixAsservissementDidChange:(id)sender
{
	if([choixAsservissementControl selectedSegment] == choixAsservissement.type)
	{
		[choixAsservissementConfirm setHidden:YES];
		[choixAsservissementCancel setHidden:YES];
	}
	else
	{
		[choixAsservissementConfirm setHidden:NO];
		[choixAsservissementCancel setHidden:NO];
	}
}

-(IBAction)setGoalValue:(id)sender
{
	double goalValue = [[formatter numberFromString:[positionGoal stringValue]] doubleValue];
	
	switch(Moteur::typeAsservissement())
	{
		case ASSERVISSEMENT_SANS :
			Moteur::setSignal(goalValue);
			break;
			
		case ASSERVISSEMENT_VITESSE :
			Moteur::setSpeed(goalValue);
			break;
			
		case ASSERVISSEMENT_POSITION :
			Moteur::setPosition(goalValue);
			break;
		
		default:
			break;
	}
}

#pragma mark - NSTextField delegate methods

- (BOOL)control:(NSControl *)control textShouldEndEditing:(NSText *)fieldEditor
{
	if([control isEqual:minDuration])
	{
		if([[formatter numberFromString:[fieldEditor string]] doubleValue] == currentMinDuration)
		{
			[minDurationConfirm setHidden:YES];
			[minDurationCancel setHidden:YES];
		}
		else
		{
			[minDurationConfirm setHidden:NO];
			[minDurationCancel setHidden:NO];
		}
		return YES;
	}
	if([control isEqual:maxDuration])
	{
		if([[formatter numberFromString:[fieldEditor string]] doubleValue] == currentMaxDuration)
		{
			[maxDurationConfirm setHidden:YES];
			[maxDurationCancel setHidden:YES];
		}
		else
		{
			[maxDurationConfirm setHidden:NO];
			[maxDurationCancel setHidden:NO];
		}
		return YES;
	}
	if(		[control isEqual:PIDSpeedProportionnel] 
	   ||	[control isEqual:PIDSpeedDifferentiel] 
	   ||	[control isEqual:PIDSpeedIntegral])
	{
		NSString* prop = [PIDSpeedProportionnel stringValue];
		NSString* diff = [PIDSpeedDifferentiel stringValue];
		NSString* inte = [PIDSpeedIntegral stringValue];
		
		if([control isEqual:PIDSpeedProportionnel])
		{
			prop = [fieldEditor string];
		}
		if([control isEqual:PIDSpeedDifferentiel])
		{
			diff = [fieldEditor string];
		}
		if([control isEqual:PIDSpeedIntegral])
		{
			inte = [fieldEditor string];
		}
		
		BOOL check = YES;
		
		check &= (		([prop isEqual:@""] && speedCoeffs.gainProportionnel==-1) 
				  ||	speedCoeffs.gainProportionnel == [[formatter numberFromString:prop] floatValue]);
		check &= (		([diff isEqual:@""] && speedCoeffs.gainDifferentiel==-1) 
				  ||	speedCoeffs.gainDifferentiel == [[formatter numberFromString:diff] floatValue]);
		check &= (		([inte isEqual:@""] && speedCoeffs.gainIntegral==-1) 
				  ||	speedCoeffs.gainIntegral == [[formatter numberFromString:inte] floatValue]);
		
		if(check)
		{
			[PIDSpeedCancel setHidden:YES];
			[PIDSpeedConfirm setHidden:YES];
		}
		else
		{
			[PIDSpeedCancel setHidden:NO];
			[PIDSpeedConfirm setHidden:NO];
		}
		return YES;
	}
	if(		[control isEqual:PIDPositionProportionnel] 
		||	[control isEqual:PIDPositionDifferentiel] 
		||	[control isEqual:PIDPositionIntegral])
	{
		NSString* prop = [PIDPositionProportionnel stringValue];
		NSString* diff = [PIDPositionDifferentiel stringValue];
		NSString* inte = [PIDPositionIntegral stringValue];
		
		if([control isEqual:PIDPositionProportionnel])
		{
			prop = [fieldEditor string];
		}
		if([control isEqual:PIDPositionDifferentiel])
		{
			diff = [fieldEditor string];
		}
		if([control isEqual:PIDPositionIntegral])
		{
			inte = [fieldEditor string];
		}
		
		BOOL check = YES;
		
		check &= (		([prop isEqual:@""] && positionCoeffs.gainProportionnel==-1) 
				  ||	positionCoeffs.gainProportionnel == [[formatter numberFromString:prop] floatValue]);
		check &= (		([diff isEqual:@""] && positionCoeffs.gainDifferentiel==-1) 
				  ||	positionCoeffs.gainDifferentiel == [[formatter numberFromString:diff] floatValue]);
		check &= (		([inte isEqual:@""] && positionCoeffs.gainIntegral==-1) 
				  ||	positionCoeffs.gainIntegral == [[formatter numberFromString:inte] floatValue]);
		
		if(check)
		{
			[PIDPositionCancel setHidden:YES];
			[PIDPositionConfirm setHidden:YES];
		}
		else
		{
			[PIDPositionCancel setHidden:NO];
			[PIDPositionConfirm setHidden:NO];
		}
		return YES;
	}
	
	return YES;
}

-(void)insertObject:(SensorForDisplay*)sensorDisplay inSensorArrayAtIndex:(NSUInteger)index {
    [sensorArray insertObject:sensorDisplay atIndex:index];
}

-(void)removeObjectFromStatusArrayAtIndex:(NSUInteger)index {
    [sensorArray removeObjectAtIndex:index];
}

@end
