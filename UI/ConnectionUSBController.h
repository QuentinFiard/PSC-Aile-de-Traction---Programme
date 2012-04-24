//
//  ConnectionUSBController.h
//  PSC
//
//  Created by Quentin Fiard on 18/03/12.
//  Copyright (c) 2012 École Polytechnique. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import "CommunicationProtocol.h"

@class SensorForDisplay;

@interface ConnectionUSBController : NSViewController
{
	IBOutlet NSMutableArray* sensorArray;
	
	IBOutlet NSView* connectedView;
	IBOutlet NSView* unconnectedView;
	
	IBOutlet NSTextField* minDuration;
	IBOutlet NSButton* minDurationConfirm;
	IBOutlet NSButton* minDurationCancel;
	
	IBOutlet NSTextField* neutralDuration;
	IBOutlet NSButton* neutralDurationConfirm;
	IBOutlet NSButton* neutralDurationCancel;
	
	IBOutlet NSTextField* maxDuration;
	IBOutlet NSButton* maxDurationConfirm;
	IBOutlet NSButton* maxDurationCancel;
	
	IBOutlet NSTextField* PIDPositionProportionnel;
	IBOutlet NSTextField* PIDPositionDifferentiel;
	IBOutlet NSTextField* PIDPositionIntegral;
	IBOutlet NSButton* PIDPositionConfirm;
	IBOutlet NSButton* PIDPositionCancel;
	
	IBOutlet NSTextField* PIDSpeedProportionnel;
	IBOutlet NSTextField* PIDSpeedDifferentiel;
	IBOutlet NSTextField* PIDSpeedIntegral;
	IBOutlet NSButton* PIDSpeedConfirm;
	IBOutlet NSButton* PIDSpeedCancel;
	
	IBOutlet NSSegmentedControl* choixAsservissementControl;
	IBOutlet NSButton* choixAsservissementConfirm;
	IBOutlet NSButton* choixAsservissementCancel;
	
	IBOutlet NSTextField* positionGoal;
	
	NSNumberFormatter* formatter;
	
	double currentMinDuration,currentNeutralDuration,currentMaxDuration,currentPIDPeriod;
	
	PID_Coeffs positionCoeffs,speedCoeffs;
	
	ChoixAsservissement choixAsservissement;
}

+(ConnectionUSBController*)shared;

-(void)setConnected:(BOOL)isConnected;

-(IBAction)retryConnection:(id)sender;

-(IBAction)sendMinSignalDuration:(id)sender;
-(IBAction)cancelMinSignalDurationModif:(id)sender;

-(IBAction)sendNeutralSignalDuration:(id)sender;
-(IBAction)cancelNeutralSignalDurationModif:(id)sender;

-(IBAction)sendMaxSignalDuration:(id)sender;
-(IBAction)cancelMaxSignalDurationModif:(id)sender;

-(IBAction)sendSpeedPIDCoeffs:(id)sender;
-(IBAction)cancelSpeedPIDModif:(id)sender;

-(IBAction)sendPositionPIDCoeffs:(id)sender;
-(IBAction)cancelPositionPIDModif:(id)sender;

-(IBAction)sendChoixAsservissement:(id)sender;
-(IBAction)cancelChoixAsservissementModif:(id)sender;
-(IBAction)choixAsservissementDidChange:(id)sender;

-(IBAction)setGoalValue:(id)sender;


-(void)insertObject:(SensorForDisplay*)sensorDisplay inSensorArrayAtIndex:(NSUInteger)index;
-(void)removeObjectFromStatusArrayAtIndex:(NSUInteger)index;

@end
