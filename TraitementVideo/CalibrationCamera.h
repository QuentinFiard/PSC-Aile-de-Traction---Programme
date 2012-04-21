//
//  CalibrationCamera.h
//  PSC
//
//  Created by Quentin Fiard on 29/01/12.
//  Copyright (c) 2012 École Polytechnique. All rights reserved.
//

#ifndef PSC_CalibrationCamera_h
#define PSC_CalibrationCamera_h

#include <opencv2/core/core.hpp>

#import <Cocoa/Cocoa.h>

#include "Matrice.h"

@class WebcamCalibrationController;

struct Echiquier
{
	int nbLignes;
	int nbColonnes;
	
	double dimension; // Longueur du côté d'un des carrés en mètres
};

class CalibrationCamera
{
	
public:
	CalibrationCamera(WebcamCalibrationController* controller);
	
	// Renvoie true si un échiquier a été détecté dans l'image, et s'il a été analysé avec succès.
	bool analyserImageAvecEchiquier(cv::Mat image);
	
	cv::Mat& imageAnalyse(int indice);
	
	// Retourne le nombre d'images traitées avec succès
	int nbImagesTraitees();
	
	// Renvoie la matrice formée des paramètres intrinsèques caractérisant la caméra
	Matrice parametresIntrinseques();
	
	void setEchiquier(Echiquier echiquier);
	Echiquier& getEchiquier();
	
	void reinitialiser();

private:
	std::vector<cv::Point3f> pointsReels; //Position des intersections des carrés de l'échiquier, dans le repère de l'échiquier
	std::vector< std::vector<cv::Point2f> > pointsImage; //Position des points correspondants sur l'image
	
	Matrice* parametresIntrinseques_;
	
	std::vector<cv::Mat> imagesAnalyses;
	
	cv::Size imageSize;
	
	Echiquier echiquier;
	cv::Size tailleEchiquier;
	
	WebcamCalibrationController* uicontroller;
};

#endif
