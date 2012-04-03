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
	CapteurAngulaire* capteur = CapteurAngulaire::shared(CAPTEUR_ROTATION_MOTEUR);
	
	capteur->update();
	
	[self performSelector:@selector(update) withObject:nil afterDelay:0.005];
}

-(void)awakeFromNib
{
	CapteurAngulaire* capteur = CapteurAngulaire::shared(CAPTEUR_ROTATION_MOTEUR);
	
	capteur->setArrowView(moteur);
	
	[self update];
}

@end
