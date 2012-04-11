//
//  CapteursController.m
//  PSC
//
//  Created by Quentin Fiard on 22/03/12.
//  Copyright (c) 2012 École Polytechnique. All rights reserved.
//

#import "CapteursController.h"

#import "CapteurAngulaire.h"



@implementation CapteursController

- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil
{
    self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil];
    if (self) {
        // Initialization code here.
    }
    
    return self;
}

-(void)update
{
	CapteurAngulaire* capteur = CapteurAngulaire::shared(CAPTEUR_DIRECTION_VENT);
	
	if(capteur!=NULL)
	{
		capteur->update();
	}
	
	capteur = CapteurAngulaire::shared(CAPTEUR_VITESSE_VENT);
	
	if(capteur!=NULL)
	{
		capteur->update();
	}
	
	capteur = CapteurAngulaire::shared(CAPTEUR_ROTATION_MOTEUR);
	
	if(capteur!=NULL)
	{
		capteur->update();
	}
	
	capteur = CapteurAngulaire::shared(CAPTEUR_THETA);
	
	if(capteur!=NULL)
	{
		capteur->update();
	}
	
	capteur = CapteurAngulaire::shared(CAPTEUR_PHI);
	
	if(capteur!=NULL)
	{
		capteur->update();
		std::cout << "Angle : " << capteur->lastValue()->angle() << std::endl;
	}
	
	[self performSelector:@selector(update) withObject:nil afterDelay:0.005];
}

-(void)awakeFromNib
{
	CapteurAngulaire* capteur = CapteurAngulaire::shared(CAPTEUR_DIRECTION_VENT);
	
	if(capteur!=NULL)
	{
		capteur->setArrowView(girouette);
	}
	
	capteur = CapteurAngulaire::shared(CAPTEUR_VITESSE_VENT);
	
	if(capteur!=NULL)
	{
		capteur->setArrowView(anemometre);
	}
	
	capteur = CapteurAngulaire::shared(CAPTEUR_ROTATION_MOTEUR);
	
	if(capteur!=NULL)
	{
		capteur->setArrowView(moteur);
	}
	
	capteur = CapteurAngulaire::shared(CAPTEUR_THETA);
	
	if(capteur!=NULL)
	{
		capteur->setArrowView(inclinaison);
	}
	
	capteur = CapteurAngulaire::shared(CAPTEUR_PHI);
	
	if(capteur!=NULL)
	{
		capteur->setArrowView(azimut);
	}
	
	[self update];
}

@end
