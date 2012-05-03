//
//  CapteursController.m
//  PSC
//
//  Created by Quentin Fiard on 22/03/12.
//  Copyright (c) 2012 École Polytechnique. All rights reserved.
//

#import "CapteursController.h"

#import "CapteurAngulaire.h"

#import <boost/date_time.hpp>

using namespace boost::posix_time;


@implementation CapteursController

- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil
{
    self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil];
    if (self) {
        // Initialization code here.
		
		updateTimer = nil;
		updateThread = nil;
    }
    
    return self;
}

ptime lastUpdate;

-(void)update
{
	ptime start(microsec_clock::local_time());
	
	std::cout << "Time since last update : " << start-lastUpdate << std::endl;
	
	lastUpdate = start;
	
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
	}
	
	ptime end(microsec_clock::local_time());
	
	std::cout << "Time required for update : " << end-start << std::endl;
	
	if(!updateTimer)
	{
		updateTimer = [[NSTimer timerWithTimeInterval:0.02 target:self selector:@selector(update) userInfo:nil repeats:YES] retain];
		
		NSRunLoop* loop = [NSRunLoop currentRunLoop];
		
		[loop addTimer:updateTimer forMode:NSDefaultRunLoopMode];
		
		[loop run];
	}
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
	
	updateThread = [[NSThread alloc] initWithTarget:self selector:@selector(update) object:nil];
	[updateThread setThreadPriority:0.9];
	[updateThread start];
	//[self update];
}

@end
