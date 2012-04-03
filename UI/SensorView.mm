//
//  SensorView.m
//  PSC
//
//  Created by Quentin Fiard on 26/03/12.
//  Copyright (c) 2012 École Polytechnique. All rights reserved.
//

#import "SensorView.h"

#import "Database.h"

@implementation SensorView

@synthesize choixGrandeur,numSensor;

- (id)initWithFrame:(NSRect)frame
{
    self = [super initWithFrame:frame];
    if (self) {
        // Initialization code here.
    }
    
    return self;
}

-(void)setChoixGrandeur:(AutoUpdateGrandeurPopUpButton *)aChoixGrandeur
{
	[choixGrandeur release];
	choixGrandeur = [aChoixGrandeur retain];
	
	if(numSensor)
	{
		choixGrandeur.sensor = [numSensor integerValue];
	}
}

-(void)setNumSensor:(NSTextField *)aNumSensor
{
	[numSensor release];
	numSensor = [aNumSensor retain];
	
	if(choixGrandeur)
	{
		choixGrandeur.sensor = [numSensor integerValue];
	}
}

- (void)drawRect:(NSRect)dirtyRect
{
    // Drawing code here.
}

-(void)dealloc
{
	[choixGrandeur release];
	[super dealloc];
}

@end

@implementation AutoUpdateGrandeurPopUpButton

@synthesize sensor;

-(void)updateListeGrandeurs
{
	[self removeAllItems];
	
	[self addItemWithTitle:@"Non connecté"];
	[[self lastItem] setTag:0];
	
	bool test = Database::hasConfigurationField("CAPTEUR_DIRECTION_VENT") &&Database::getConfigurationFieldValue<int>("CAPTEUR_DIRECTION_VENT") == sensor;
	
	if(!Database::hasConfigurationField("CAPTEUR_DIRECTION_VENT") || test)
	{
		[self addItemWithTitle:@"Direction du vent"];
		[[self lastItem] setTag:1];
		
		if(test)
		{
			[self selectItem:[self lastItem]];
		}
	}
	
	test = Database::hasConfigurationField("CAPTEUR_VITESSE_VENT") && Database::getConfigurationFieldValue<int>("CAPTEUR_VITESSE_VENT") == sensor;
	
	if(!Database::hasConfigurationField("CAPTEUR_VITESSE_VENT") || test)
	{
		[self addItemWithTitle:@"Vitesse du vent"];
		[[self lastItem] setTag:2];
		
		if(test)
		{
			[self selectItem:[self lastItem]];
		}
	}
	
	test = Database::hasConfigurationField("CAPTEUR_ROTATION_MOTEUR") && Database::getConfigurationFieldValue<int>("CAPTEUR_ROTATION_MOTEUR") == sensor;
	
	if(!Database::hasConfigurationField("CAPTEUR_ROTATION_MOTEUR") || test)
	{
		[self addItemWithTitle:@"Rotation axe moteur"];
		[[self lastItem] setTag:3];
		
		if(test)
		{
			[self selectItem:[self lastItem]];
		}
	}
	
	test = Database::hasConfigurationField("CAPTEUR_THETA") && Database::getConfigurationFieldValue<int>("CAPTEUR_THETA") == sensor;
	
	if(!Database::hasConfigurationField("CAPTEUR_THETA") || test)
	{
		[self addItemWithTitle:@"Inclinaison"];
		[[self lastItem] setTag:4];
		
		if(test)
		{
			[self selectItem:[self lastItem]];
		}
	}
	
	test = Database::hasConfigurationField("CAPTEUR_PHI") && Database::getConfigurationFieldValue<int>("CAPTEUR_PHI") == sensor;
	
	if(!Database::hasConfigurationField("CAPTEUR_PHI") || test)
	{
		[self addItemWithTitle:@"Azimut"];
		[[self lastItem] setTag:5];
		
		if(test)
		{
			[self selectItem:[self lastItem]];
		}
	}
}

-(void)setSensor:(NSUInteger)aSensor
{
	sensor = aSensor;
	[self updateListeGrandeurs];
	[self setNeedsDisplay:YES];
}

- (void)mouseDown:(NSEvent *)theEvent {
	[self updateListeGrandeurs];
	[super mouseDown:theEvent];	
}

@end
