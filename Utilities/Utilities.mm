//
//  Utilities.cpp
//  PSC
//
//  Created by Quentin Fiard on 12/02/12.
//  Copyright (c) 2012 École Polytechnique. All rights reserved.
//

#include <iostream>
#include "Utilities.h"

#import <Cocoa/Cocoa.h>

#import <boost/lexical_cast.hpp>
#import <boost/date_time.hpp>
#import <fstream>

using namespace boost::posix_time;

template<>
bool isNumber(bool& value)
{
	return true;
}

template<>
bool isNumber(int8_t& value)
{
	return true;
}

template<>
bool isNumber(int16_t& value)
{
	return true;
}

template<>
bool isNumber(int32_t& value)
{
	return true;
}

template<>
bool isNumber(int64_t& value)
{
	return true;
}

template<>
bool isNumber(double& value)
{
	return true;
}

template<>
bool isNumber(float& value)
{
	return true;
}

static NSString* findOrCreateDirectory(NSSearchPathDirectory searchPathDirectory,NSSearchPathDomainMask domainMask,NSString* appendComponent, NSError** errorOut)
{
    // Search for the path
    NSArray* paths = NSSearchPathForDirectoriesInDomains(
														 searchPathDirectory,
														 domainMask,
														 YES);
    if ([paths count] == 0)
    {
        // *** creation and return of error object omitted for space
        return nil;
    }
	
    // Normally only need the first path
    NSString *resolvedPath = [paths objectAtIndex:0];
	
    if (appendComponent)
    {
        resolvedPath = [resolvedPath
						stringByAppendingPathComponent:appendComponent];
    }
	
    // Create the path if it doesn't exist
    NSError *error;
    BOOL success = [[NSFileManager defaultManager]
					createDirectoryAtPath:resolvedPath
					withIntermediateDirectories:YES
					attributes:nil
					error:&error];
    if (!success) 
    {
        if (errorOut)
        {
            *errorOut = error;
        }
        return nil;
    }
	
    // If we've made it this far, we have a success
    if (errorOut)
    {
        *errorOut = nil;
    }
    return resolvedPath;
}

static NSString* applicationSupportDirectory()
{
	NSString *executableName = [[[NSBundle mainBundle] infoDictionary] objectForKey:@"CFBundleExecutable"];
    NSError *error;
    NSString *result = findOrCreateDirectory(NSApplicationSupportDirectory,NSUserDomainMask,executableName,&error);
    if (error)
    {
        NSLog(@"Unable to find or create application support directory:\n%@", error);
    }
    return result;
}

std::string prepareForCSV(std::string s)
{
	std::size_t pos = 0;
	while(pos<s.length())
	{
		pos = s.find_first_of('"',pos);
		if(pos != std::string::npos)
		{
			s.insert(pos, 1, '"');
			pos += 2;
		}
		else
		{
			pos = s.length();
		}
	}
	
	return "\""+s+"\"";
}

std::string pathToDatabase()
{
	NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];
	
	NSString* path = [applicationSupportDirectory() stringByAppendingString:@"/psc.database.sqlite"];
	
	if(![[NSFileManager defaultManager] fileExistsAtPath:path])
	{
		NSString* modelPath = [[NSBundle mainBundle] pathForResource:@"psc.database" ofType:@"sqlite"];
		[[NSFileManager defaultManager] copyItemAtPath:modelPath toPath:path error:nil];
	}
	
	std::string res([path UTF8String]);
	
	[pool release];
	
	return res;
}

std::string pathToLogFilesDirectory()
{
	NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];
	
	NSString* path = [applicationSupportDirectory() stringByAppendingString:@"/log/"];
	
	if(![[NSFileManager defaultManager] fileExistsAtPath:path])
	{
		NSError* error;
		[[NSFileManager defaultManager] createDirectoryAtPath:path withIntermediateDirectories:YES attributes:nil error:&error];
		
		if (error)
		{
			NSLog(@"Unable to find or create log files directory:\n%@", error);
		}
	}
	
	std::string res([path UTF8String]);
	
	[pool release];
	
	return res;
}

void log(std::string logName, std::string description)
{
	std::string path = pathToLogFilesDirectory() + logName + ".log";
	
	std::ofstream logFile(path.c_str(), std::ios_base::app);
	
	boost::posix_time::ptime currentDate();
	
	logFile << currentDate;
	
	logFile << " - ";
	
	logFile << description;
	
	logFile << "\n";
	
	logFile.close();
}

boost::posix_time::time_duration microsecondsTimeDuration(sqlite3_int64 offset)
{
	long microsec = offset % 60000000;
	long min = offset /= 60000000;
	
	return minutes(min) + microseconds(microsec);
}