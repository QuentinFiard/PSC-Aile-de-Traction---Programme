//
//  ConnectionUSBController.m
//  PSC
//
//  Created by Quentin Fiard on 18/03/12.
//  Copyright (c) 2012 École Polytechnique. All rights reserved.
//

#import "ConnectionUSBController.h"
#import "ConnectionUSB.h"

#import "SensorForDisplay.h"

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
	IntegerFormater* myformatter = [[IntegerFormater alloc] initWithLimit:4];
	[MoteurAsservissement setFormatter:myformatter];
	[myformatter release];
	
	myformatter = [[IntegerFormater alloc] initWithLimit:6];
	[CapteurAsservissement setFormatter:myformatter];
	[myformatter release];
	
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
		
		speedCoeffs = ConnectionUSB::readSpeedPIDCoeffs();
		positionCoeffs = ConnectionUSB::readPositionPIDCoeffs();
		
		choixAsservissement = ConnectionUSB::readChoixAsservissement();
		
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

-(IBAction)sendPIDPeriod:(id)sender
{
	currentPIDPeriod = [PIDPeriod doubleValue]/1000;
	
	ConnectionUSB::setPIDPeriod(currentPIDPeriod);
	
	currentPIDPeriod = ConnectionUSB::readPIDPeriod();
	
	[self cancelPIDPeriodModif:self];
}

-(IBAction)cancelPIDPeriodModif:(id)sender
{
	if(currentPIDPeriod==-1)
	{
		[PIDPeriod setStringValue:@""];
	}
	else
	{
		[PIDPeriod setStringValue:[formatter stringFromNumber:[NSNumber numberWithDouble:currentPIDPeriod*1000]]];
	}
	
	[PIDPeriodCancel setHidden:YES];
	[PIDPeriodConfirm setHidden:YES];
}

-(IBAction)sendSpeedPIDCoeffs:(id)sender
{
	speedCoeffs.gainProportionnel = [[formatter numberFromString:[PIDSpeedProportionnel stringValue]] floatValue];
	speedCoeffs.gainDifferentiel = [[formatter numberFromString:[PIDSpeedDifferentiel stringValue]] floatValue];
	speedCoeffs.gainIntegral = [[formatter numberFromString:[PIDSpeedIntegral stringValue]] floatValue];
	
	ConnectionUSB::setSpeedPIDCoeffs(speedCoeffs);
	
	speedCoeffs = ConnectionUSB::readSpeedPIDCoeffs();
	
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
	
	ConnectionUSB::setPositionPIDCoeffs(positionCoeffs);
	
	positionCoeffs = ConnectionUSB::readPositionPIDCoeffs();
	
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
	
	if(choixAsservissement.type != ASSERVISSEMENT_SANS)
	{
		choixAsservissement.motor = static_cast<Motor>([MoteurAsservissement intValue]);
		choixAsservissement.sensor = static_cast<Sensor>([CapteurAsservissement intValue]);
	}
	
	ConnectionUSB::setChoixAsservissement(choixAsservissement);
	
	choixAsservissement = ConnectionUSB::readChoixAsservissement();
	
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
	if([choixAsservissementControl selectedSegment] == ASSERVISSEMENT_SANS)
	{
		[MoteurAsservissement setHidden:YES];
		[CapteurAsservissement setHidden:YES];
	}
	else
	{
		[MoteurAsservissement setHidden:NO];
		[CapteurAsservissement setHidden:NO];
	}
	
	if([choixAsservissementControl selectedSegment] == choixAsservissement.type)
	{
		[choixAsservissementConfirm setHidden:YES];
		[choixAsservissementCancel setHidden:YES];
	}
	else
	{
		if(		choixAsservissement.type == ASSERVISSEMENT_SANS
		   ||	(![[CapteurAsservissement stringValue] isEqualToString:@""] && ![[MoteurAsservissement stringValue] isEqualToString:@""]))
		{
			[choixAsservissementConfirm setHidden:NO];
		}
		[choixAsservissementCancel setHidden:NO];
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
	if([control isEqual:PIDPeriod])
	{
		if([[formatter numberFromString:[fieldEditor string]] doubleValue] == currentPIDPeriod)
		{
			[PIDPeriodConfirm setHidden:YES];
			[PIDPeriodCancel setHidden:YES];
		}
		else
		{
			[PIDPeriodConfirm setHidden:NO];
			[PIDPeriodCancel setHidden:NO];
		}
		return YES;
	}
	if([control isEqual:CapteurAsservissement] || [control isEqual:MoteurAsservissement])
	{
		NSString* capteur = [CapteurAsservissement stringValue];
		NSString* moteur = [MoteurAsservissement stringValue];
		
		if([control isEqual:CapteurAsservissement])
		{
			capteur = [fieldEditor string];
		}
		else
		{
			moteur = [fieldEditor string];
		}
		
		if([capteur isEqualToString:@""] || [moteur isEqualToString:@""])
		{
			[PIDPeriodConfirm setHidden:YES];
		}
		else
		{
			if(		[choixAsservissementControl selectedSegment] == choixAsservissement.type 
			   &&	[capteur intValue] == choixAsservissement.sensor
			   &&	[moteur intValue] == choixAsservissement.motor)
			{
				[PIDPeriodConfirm setHidden:YES];
				[PIDPeriodCancel setHidden:YES];
			}
			else
			{
				[PIDPeriodConfirm setHidden:NO];
				[PIDPeriodCancel setHidden:NO];
			}
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
