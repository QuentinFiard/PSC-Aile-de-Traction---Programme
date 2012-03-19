//
//  WebcamCalibrationController.m
//  PSC
//
//  Created by Quentin Fiard on 29/01/12.
//  Copyright (c) 2012 École Polytechnique. All rights reserved.
//

#import "WebcamCalibrationController.h"
#include "Matrice.h"

#import "NSImage+CIImage.h"
#import "NSImage_OpenCV.h"

#include "Database.h"
#include "StringValue.h"

#include "Status.h"

#include "Camera.h"

@implementation WebcamCalibrationController

- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil
{
    self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil];
    if (self) {
		
    }
    
    return self;
}

-(void)loadSelectedWebcam
{
	Camera::switchToSelectedCamera();
}

-(void)updateCameraList:(id)sender
{
	[cameraChoice removeAllItems];
	
	[cameraChoice addItemWithTitle:@"Choix de la caméra"];
	
	NSArray* devices = [QTCaptureDevice inputDevicesWithMediaType:QTMediaTypeVideo];
	
	for(int i=0 ; i<devices.count ; i++)
	{
		[cameraChoice addItemWithTitle:[[devices objectAtIndex:i] localizedDisplayName]];
	}
}

- (void) awakeFromNib 
{
	[self loadSelectedWebcam];
	
	Camera::displayInView(cameraView);
	
	[self updateCameraList:self];
}

-(void)dealloc
{
	Camera::removeFromView(cameraView);
	[super dealloc];
}

-(IBAction)afficherDerniereAnalyse:(id)sender
{
	if(isDisplayingCamera)
	{
		[sender setTitle:@"Revenir à la caméra"];
		
		[next setEnabled:NO];
		
		if(calibration.nbImagesTraitees()>1)
		{
			[previous setEnabled:YES];
		}
	}
	else
	{
		[sender setTitle:@"Afficher la dernière analyse"];
		[next setEnabled:NO];
		[previous setEnabled:NO];
		
		if(calibration.nbImagesTraitees()==0)
		{
			[sender setEnabled:NO];
		}
		else
		{
			[sender setEnabled:YES];
		}
	}
	
	isDisplayingCamera = !isDisplayingCamera;
	
	if(!isDisplayingCamera)
	{
		[cameraView setHidden:YES];
		[imageAnalyse setHidden:NO];
		
		[self afficherAnalyse:calibration.nbImagesTraitees()-1];
	}
	else
	{
		[cameraView setHidden:NO];
		[imageAnalyse setHidden:YES];
	}
}

-(void)afficherAnalyse:(int)indice
{
	indiceAnalyse = indice;
	[imageAnalyse setImage:[NSImage imageWithCVMat:calibration.imageAnalyse(indice)]];
}

-(IBAction)montrerAnalysePrecedente:(id)sender
{
	indiceAnalyse--;
	[next setEnabled:YES];
	if(indiceAnalyse==0)
	{
		[previous setEnabled:NO];
	}
	[self afficherAnalyse:indiceAnalyse];
}

-(IBAction)montrerAnalyseSuivante:(id)sender
{
	indiceAnalyse++;
	[previous setEnabled:YES];
	if(indiceAnalyse==calibration.nbImagesTraitees()-1)
	{
		[next setEnabled:NO];
	}
	[self afficherAnalyse:indiceAnalyse];
}

-(IBAction)calculerCoefficientsIntrinseques:(id)sender
{
	Matrice camera = calibration.parametresIntrinseques();
	
	camera.afficher();
	
	[focaleX setDoubleValue:camera(0,0)];
	[focaleY setDoubleValue:camera(1,1)];
	
	[centreOptiqueX setDoubleValue:camera(0,2)];
	[centreOptiqueY setDoubleValue:camera(1,2)];
}

-(IBAction)reinitialiserCalibration:(id)sender
{
	calibration.reinitialiser();
	
	[nbImagesAnalysees setStringValue:@"0"];
	[previous setEnabled:NO];
	[next setEnabled:NO];
	
	if(!isDisplayingCamera)
	{
		[self afficherDerniereAnalyse:affichage];
	}
	
	[focaleX setStringValue:@"Non calculé"];
	[focaleY setStringValue:@"Non calculé"];
	[centreOptiqueX setStringValue:@"Non calculé"];
	[centreOptiqueY setStringValue:@"Non calculé"];
	
	[calcul setEnabled:NO];
}

-(IBAction)analyserImageCourante:(id)sender
{
	if([[nbLignes stringValue] isEqualToString:@""] || [[nbColonnes stringValue] isEqualToString:@""] || [[dimension stringValue] isEqualToString:@""])
	{
		NSLog(@"Précisez les dimensions de l'échiquier");
		return;
	}
	Echiquier echiquier;
	
	echiquier.nbLignes = [nbLignes intValue];
	echiquier.nbColonnes = [nbColonnes intValue];
	echiquier.dimension = [[dimension stringValue] doubleValue];
	
	Echiquier& courant = calibration.getEchiquier();
	
	if(courant.nbLignes != echiquier.nbLignes || courant.nbColonnes != echiquier.nbColonnes || courant.dimension != echiquier.dimension)
	{
		[self reinitialiserCalibration:self];
		calibration.setEchiquier(echiquier);
	}
	
	Camera::useCurrentImageForCalibration(calibration);
}

-(IBAction)chooseCamera:(id)sender
{
	NSString* selectedItem = [cameraChoice titleOfSelectedItem];
	if([selectedItem isEqualToString:@"Choix de la caméra"])
	{
		StringValue* val = new StringValue("");
		Database::setConfigurationField("CameraID", val);
		delete val;
	}
	else
	{
		NSArray* devices = [QTCaptureDevice inputDevicesWithMediaType:QTMediaTypeVideo];
		
		int i=0;
		
		while(i<devices.count && ![[[devices objectAtIndex:i] localizedDisplayName] isEqualToString:selectedItem])
		{
			i++;
		}
		if(i>=devices.count)
		{
			StringValue* val = new StringValue("");
			Database::setConfigurationField("CameraID", val);
			delete val;
		}
		else
		{
			NSString* ID = [(QTCaptureDevice*)[devices objectAtIndex:i] uniqueID];
			std::string stringID([ID UTF8String]);
			StringValue* val = new StringValue(stringID);
			Database::setConfigurationField("CameraID", val);
			delete val;
		}
	}
	
	[self loadSelectedWebcam];
}

@end
