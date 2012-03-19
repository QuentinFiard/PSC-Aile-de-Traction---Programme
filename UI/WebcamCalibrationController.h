//
//  WebcamCalibrationController.h
//  PSC
//
//  Created by Quentin Fiard on 29/01/12.
//  Copyright (c) 2012 École Polytechnique. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import <QTKit/QTKit.h>
#import "CalibrationCamera.h"

@class AutoUpdatePopUpButton;

@interface WebcamCalibrationController : NSViewController
{
	CalibrationCamera calibration;
	
	IBOutlet NSTextField* nbLignes;
	IBOutlet NSTextField* nbColonnes;
	IBOutlet NSTextField* dimension;
	
	IBOutlet NSTextField* nbImagesAnalysees;
	IBOutlet NSTextField* focaleX;
	IBOutlet NSTextField* focaleY;
	IBOutlet NSTextField* centreOptiqueX;
	IBOutlet NSTextField* centreOptiqueY;
	
	IBOutlet NSButton* previous;
	IBOutlet NSButton* next;
	IBOutlet NSButton* affichage;
	IBOutlet NSButton* calcul;
	
	IBOutlet QTCaptureView *cameraView;
	IBOutlet NSImageView* imageAnalyse;
	
	BOOL isDisplayingCamera;
	BOOL shouldAnalyseImage;
	int indiceAnalyse;
	
	IBOutlet AutoUpdatePopUpButton* cameraChoice;
}

-(IBAction)afficherDerniereAnalyse:(id)sender;
-(IBAction)montrerAnalysePrecedente:(id)sender;
-(IBAction)montrerAnalyseSuivante:(id)sender;
-(IBAction)calculerCoefficientsIntrinseques:(id)sender;
-(IBAction)reinitialiserCalibration:(id)sender;
-(IBAction)choisirCamera:(id)sender;
-(IBAction)analyserImageCourante:(id)sender;

-(void)afficherAnalyse:(int)indice;

-(IBAction)chooseCamera:(id)sender;
-(void)updateCameraList:(id)sender;

@end

@interface AutoUpdatePopUpButton : NSPopUpButton
{
	IBOutlet WebcamCalibrationController* controller;
}

@end

@implementation AutoUpdatePopUpButton

- (void)mouseDown:(NSEvent *)theEvent {
    [controller updateCameraList:self];
	[super mouseDown:theEvent];	
}

@end
