//
//  SimulationController.m
//  PSC
//
//  Created by Quentin Fiard on 23/01/12.
//  Copyright (c) 2012 École Polytechnique. All rights reserved.
//

#import "SimulationController.h"

#import "Moteur3D.h"

@implementation SimulationController

- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil
{
    self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil];
    if (self) {
        // Initialization code here.
    }
    
    return self;
}

-(void)awakeFromNib
{
	[[Moteur3D moteur3D] renderInView:self.view label:@"Simulation"];
}

-(void)dealloc
{
	[[Moteur3D moteur3D] removeFromView:@"Simulation"];
	[super dealloc];
}

@end
