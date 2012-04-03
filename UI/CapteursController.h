//
//  CapteursController.h
//  PSC
//
//  Created by Quentin Fiard on 22/03/12.
//  Copyright (c) 2012 École Polytechnique. All rights reserved.
//

#import <Cocoa/Cocoa.h>

@class ArrowView;

@interface CapteursController : NSViewController
{
	IBOutlet ArrowView* moteur;
	
	IBOutlet ArrowView* girouette;
	IBOutlet ArrowView* anemometre;
	
	IBOutlet ArrowView* azimut;
	IBOutlet ArrowView* inclinaison;
}

-(void)update;

@end
