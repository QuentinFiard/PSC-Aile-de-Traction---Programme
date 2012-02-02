//
//  MoteurController.h
//  PSC
//
//  Created by Quentin Fiard on 23/01/12.
//  Copyright (c) 2012 École Polytechnique. All rights reserved.
//

#import <Cocoa/Cocoa.h>

#import "Dynamixel.h"

@interface MoteurController : NSViewController
{
	IBOutlet NSPopUpButton* listePortsSerie;
	
	IBOutlet NSPopUpButton* commande;
	IBOutlet NSTextField* parametres;
	IBOutlet NSPopUpButton* ID;
	
	IBOutlet NSTextField* reponse;
	
	IBOutlet NSButton* startSession;
	IBOutlet NSButton* stopSession;
	
	NSString* bsdPath;
	
	Dynamixel* dynamixel;
	
	bool joystickSessionRunning;
}

@property (retain,nonatomic) NSString* bsdPath;

-(IBAction)rechercherPortsSerie:(id)sender;

-(IBAction)envoyerPaquet:(id)sender;

-(IBAction)choisirNomPort:(id)sender;

-(IBAction)startJoystickSession:(id)sender;

-(IBAction)stopJoystickSession:(id)sender;

@end
