//
//  NSImage_OpenCV.h
//  PSC
//
//  Created by Quentin Fiard on 02/02/12.
//  Copyright (c) 2012 École Polytechnique. All rights reserved.
//

#import <AppKit/AppKit.h>
#import <opencv2/core/core.hpp>
#import <opencv2/core/operations.hpp>

@interface NSImage (NSImage_OpenCV)

+(NSImage*)imageWithCVMat:(const cv::Mat&)cvMat;
-(id)initWithCVMat:(const cv::Mat&)cvMat;

@property(nonatomic, readonly) cv::Mat CVMat;
@property(nonatomic, readonly) cv::Mat CVGrayscaleMat;

@end