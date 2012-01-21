//|||||||||||||||||||||||||||||||||||||||||||||||

#include "OgreDemoApp.h"
#include "AppDelegate.h"

//|||||||||||||||||||||||||||||||||||||||||||||||
#ifdef __cplusplus
extern "C" {
#endif
int main(int argc, char **argv)
{
	NSAutoreleasePool * pool = [[NSAutoreleasePool alloc] init];
    
    mAppDelegate = [[AppDelegate alloc] init];
    [[NSApplication sharedApplication] setDelegate:mAppDelegate];
	
	[mAppDelegate go];
	
	int retVal = NSApplicationMain(argc, (const char **) argv);
    
	[pool release];
    
	return retVal;
}
	
#ifdef __cplusplus
}
#endif

//|||||||||||||||||||||||||||||||||||||||||||||||
