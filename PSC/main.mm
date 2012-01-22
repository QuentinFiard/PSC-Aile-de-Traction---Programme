//|||||||||||||||||||||||||||||||||||||||||||||||

#include "UIController.h"

//|||||||||||||||||||||||||||||||||||||||||||||||
#ifdef __cplusplus
extern "C" {
#endif
int main(int argc, char **argv)
{
	NSAutoreleasePool * pool = [[NSAutoreleasePool alloc] init];
	
	int retVal = NSApplicationMain(argc, (const char **) argv);
    
	[pool release];
    
	return retVal;
}
	
#ifdef __cplusplus
}
#endif

//|||||||||||||||||||||||||||||||||||||||||||||||
