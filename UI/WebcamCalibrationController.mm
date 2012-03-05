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

@implementation WebcamCalibrationController

- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil
{
    self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil];
    if (self) {
		isDisplayingCamera = YES;
		shouldAnalyseImage = NO;
    }
    
    return self;
}

- (void) awakeFromNib 
{
	//Create the QT capture session
	session = [[QTCaptureSession alloc] init];
	decompressedSession = [[QTCaptureSession alloc] init];
	/* Select the default Video input device */
	iSight = [QTCaptureDevice defaultInputDeviceWithMediaType:QTMediaTypeVideo];
	
	NSError* error;
	[iSight open:&error];
	
	NSLog(@"%@",error);
	
	/* Create a QTKit input for the session using the iSight Device */
	QTCaptureDeviceInput *myInput = [QTCaptureDeviceInput deviceInputWithDevice:iSight];
	QTCaptureDeviceInput *myInput2 = [QTCaptureDeviceInput deviceInputWithDevice:iSight];
	
	decompressedVideo = [[QTCaptureDecompressedVideoOutput alloc] init];
	
	/* Add inputs get the ball rolling etc */
	[session addInput:myInput error:nil];
	[decompressedSession addInput:myInput2 error:nil];
	[decompressedSession addOutput:decompressedVideo error:nil];
	[cameraView setCaptureSession:session];
	//[cameraView setDelegate:self];
	
	
	/* Let the video madness begin */
	[session startRunning]; 
}

-(void)dealloc
{
	[session stopRunning];
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

-(IBAction)choisirCamera:(id)sender
{
	
}

- (CIImage *)view:(QTCaptureView *)view willDisplayImage:(CIImage *)image
{
	if(shouldAnalyseImage)
	{
		shouldAnalyseImage = NO;
		NSImage *capturedImage = [[NSImage alloc] init];
		[capturedImage addRepresentation:[NSCIImageRep
										  imageRepWithCIImage:image]];
		
		//bool test = calibration.analyserImageAvecEchiquier([[NSImage imageNamed:@"test.jpg"] CVMat]);
		bool test = calibration.analyserImageAvecEchiquier([capturedImage CVMat]);
		
		[capturedImage release];
		
		if(test)
		{
			NSLog(@"%@",@"Image analysée avec succès");
		}
		
		[nbImagesAnalysees setIntValue:calibration.nbImagesTraitees()];
		if(isDisplayingCamera && calibration.nbImagesTraitees()>0)
		{
			[affichage setEnabled:YES];
		}
	}
	return nil;
	return image;
}

-(void)captureOutput:(QTCaptureOutput *)captureOutput didOutputVideoFrame:(CVImageBufferRef)videoFrame withSampleBuffer:(QTSampleBuffer *)sampleBuffer fromConnection:(QTCaptureConnection *)connection
{
	[decompressedSession stopRunning];
	[decompressedVideo setDelegate:nil];
	
	NSCIImageRep *videoRep = [NSCIImageRep imageRepWithCIImage:[CIImage imageWithCVImageBuffer:videoFrame]];
	NSImage *image = [[[NSImage alloc] initWithSize:[videoRep size]] autorelease];
	[image addRepresentation:videoRep];
	//CVBufferRelease(videoFrame);
	
	//bool test = calibration.analyserImageAvecEchiquier([[NSImage imageNamed:@"test.jpg"] CVMat]);
	bool test = calibration.analyserImageAvecEchiquier([image CVMat]);
	
	if(test)
	{
		NSLog(@"%@",@"Image analysée avec succès");
	}
	
	[nbImagesAnalysees setIntValue:calibration.nbImagesTraitees()];
	if(isDisplayingCamera && calibration.nbImagesTraitees()>0)
	{
		[affichage setEnabled:YES];
	}
	if(calibration.nbImagesTraitees()>0)
	{
		[calcul setEnabled:YES];
	}
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
	
	//shouldAnalyseImage = YES;
	[decompressedSession startRunning];
	[decompressedVideo setDelegate:self];
}

@end