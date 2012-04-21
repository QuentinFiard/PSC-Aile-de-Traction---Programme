//
//  CalibrationCamera.cpp
//  PSC
//
//  Created by Quentin Fiard on 29/01/12.
//  Copyright (c) 2012 École Polytechnique. All rights reserved.
//

#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "CalibrationCamera.h"

#include "Database.h"
#include "NumericValue.h"
#include "MatriceTemplate.h"

#import "WebcamCalibrationController.h"

CalibrationCamera::CalibrationCamera(WebcamCalibrationController* aController)
{
	parametresIntrinseques_ = NULL;
	uicontroller = aController;
}

void CalibrationCamera::setEchiquier(Echiquier echiquierNouveau)
{
	echiquier.nbLignes = echiquierNouveau.nbLignes;
	echiquier.nbColonnes = echiquierNouveau.nbColonnes;
	echiquier.dimension = echiquierNouveau.dimension;
	
	pointsReels.clear();
	pointsImage.clear();
	
	int nbLignes = echiquier.nbLignes-1;
	int nbColonnes = echiquier.nbColonnes-1;
	int nbIntersections = nbLignes*nbColonnes;
	
	for(int i=0 ; i<nbIntersections ; i++)
	{
		pointsReels.push_back(echiquier.dimension*cv::Point3d(i/nbLignes,i%nbLignes,0));
	}
	
	tailleEchiquier = cv::Size(nbLignes,nbColonnes);
}

Echiquier& CalibrationCamera::getEchiquier()
{
	return echiquier;
}

int CalibrationCamera::nbImagesTraitees()
{
	return imagesAnalyses.size();
}

bool CalibrationCamera::analyserImageAvecEchiquier(cv::Mat image)
{
	if(imageSize.width == 0 && imageSize.height == 0)
	{
		imageSize = image.size();
	}
	
	if(parametresIntrinseques_)
	{
		delete parametresIntrinseques_;
		parametresIntrinseques_ = NULL;
	}
	
	cv::Mat imageNoirEtBlanc;
	std::vector<cv::Point2f> corners;
	
	cv::cvtColor(image, imageNoirEtBlanc, CV_BGR2GRAY);
	
	bool found = cv::findChessboardCorners(image, tailleEchiquier, corners, CV_CALIB_CB_ADAPTIVE_THRESH | CV_CALIB_CB_FILTER_QUADS);
	
	if(found)
	{
		cv::cornerSubPix(imageNoirEtBlanc, corners, cv::Size(11, 11), cv::Size(-1, -1), cv::TermCriteria(CV_TERMCRIT_EPS | CV_TERMCRIT_ITER, 30, 0.1));
		cv::drawChessboardCorners(imageNoirEtBlanc, tailleEchiquier, corners, found);
		
		imagesAnalyses.push_back(imageNoirEtBlanc);
		
		pointsImage.push_back(corners);
	}
	else
	{
		imagesAnalyses.push_back(imageNoirEtBlanc);
	}
	
	[uicontroller updateCalibrationStatus];
	
	return found;
}

cv::Mat& CalibrationCamera::imageAnalyse(int indice)
{
	return imagesAnalyses[indice];
}

Matrice CalibrationCamera::parametresIntrinseques()
{
	if(!parametresIntrinseques_)
	{
		parametresIntrinseques_ = new Matrice(3, 3);
		
		cv::Mat intrinsic;
		cv::Mat distCoeffs;
		cv::vector<cv::Mat> rvecs;
		cv::vector<cv::Mat> tvecs;
		
		std::vector< std::vector<cv::Point3f> > pointsReelsRecopie;
		
		for(int i=0 ; i<pointsImage.size() ; i++)
		{
			pointsReelsRecopie.push_back(pointsReels);
		}
		
		cv::calibrateCamera(pointsReelsRecopie, pointsImage, imageSize, intrinsic, distCoeffs, rvecs, tvecs);
		
		for(int i=0 ; i<3 ; i++)
		{
			for(int j=0 ; j<3 ; j++)
			{
				parametresIntrinseques_->at(i,j) = intrinsic.at<double>(i,j);
			}
		}
		
		if(Database::hasConfigurationField("MatriceCamera"))
		{
			MatriceTemplate<Matrice>* oldMatTemplate = (MatriceTemplate<Matrice>*)Database::getConfigurationField("MatriceCamera");
			const Matrice& oldMat = oldMatTemplate->matrix();
			
			NumericValue<int>* nbImagesValue = (NumericValue<int>*)Database::getConfigurationField("NbImagesAnalysees");
			int nbImages = nbImagesValue->value();
			
			*parametresIntrinseques_ = (nbImages*oldMat + pointsImage.size()*(*parametresIntrinseques_))/(nbImages+pointsImage.size());
			
			Database::setConfigurationField("MatriceCamera", parametresIntrinseques_);
			NumericValue<int> newNbImages(nbImages+pointsImage.size());
			Database::setConfigurationField("NbImagesAnalysees", &newNbImages);
		}
		else
		{
			Database::setConfigurationField("MatriceCamera", parametresIntrinseques_);
			NumericValue<int> nbImages(pointsImage.size());
			Database::setConfigurationField("NbImagesAnalysees", &nbImages);
		}
	}
	
	return Matrice(*parametresIntrinseques_);
}

void CalibrationCamera::reinitialiser()
{
	pointsImage.clear();
	imagesAnalyses.clear();
	
	if(parametresIntrinseques_)
	{
		delete parametresIntrinseques_;
		parametresIntrinseques_ = NULL;
	}
}