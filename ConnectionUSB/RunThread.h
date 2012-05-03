//
//  RunThread.h
//  PSC
//
//  Created by Quentin Fiard on 02/05/12.
//  Copyright (c) 2012 École Polytechnique. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface RunThread : NSThread
{
	NSTimer* keepAlive;
}

@end
