//
//  UIController.m
//  PSC
//
//  Created by Quentin Fiard on 22/01/12.
//  Copyright (c) 2012 École Polytechnique. All rights reserved.
//

#import "UIController.h"
#import <Foundation/Foundation.h>

#import "Moteur3D.h"

@implementation UIController

@dynamic window;
@synthesize ogreView;
@synthesize delegate;

-(id)init
{
	ogreView = [[OgreView alloc] init];
	
	//BOOL check = [NSBundle loadNibNamed:@"Application" owner:self];
	
	return self;
}

-(void)awakeFromNib
{
	//[delegate go];
	
	Moteur3D* moteur = [[Moteur3D alloc] init];
	
	[moteur initInView:ogreView];
}

-(void)setWindow:(NSWindow *)aWindow
{
	[window release];
	window = [aWindow retain];
	[window makeKeyAndOrderFront:self];
}

@end
