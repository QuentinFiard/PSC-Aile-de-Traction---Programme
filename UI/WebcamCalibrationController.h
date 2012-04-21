//
//  WebcamCalibrationController.h
//  PSC
//
//  Created by Quentin Fiard on 29/01/12.
//  Copyright (c) 2012 École Polytechnique. All rights reserved.
//

#ifndef __WebcamCalibrationController__
#define __WebcamCalibrationController__

#import <Cocoa/Cocoa.h>
#import <QTKit/QTKit.h>

class CalibrationCamera;

@class AutoUpdatePopUpButton;

@interface WebcamCalibrationController : NSViewController
{
	CalibrationCamera* calibration;
	
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

-(void)updateCalibrationStatus;

@end

#endif
