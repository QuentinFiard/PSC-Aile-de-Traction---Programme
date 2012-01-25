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
	IBOutlet NSTextField* ID;
	
	IBOutlet NSTextField* reponse;
	
	NSString* bsdPath;
	
	Dynamixel* dynamixel;
}

@property (retain,nonatomic) NSString* bsdPath;

-(IBAction)rechercherPortsSerie:(id)sender;

-(IBAction)envoyerPaquet:(id)sender;

-(IBAction)choisirNomPort:(id)sender;

@end
