//
//  MoteurController.m
//  PSC
//
//  Created by Quentin Fiard on 23/01/12.
//  Copyright (c) 2012 École Polytechnique. All rights reserved.
//

#import "MoteurController.h"

#include <IOKit/IOKitLib.h>
#include <IOKit/serial/IOSerialKeys.h>
#include <IOKit/serial/ioss.h>
#include <IOKit/IOBSD.h>

#include <assert.h>

#include <SDL/SDL.h>

#include "Joystick.h"

@implementation MoteurController

@synthesize bsdPath;

static NSString* byteToHex(uint8_t var);

static kern_return_t FindSerialPort(io_iterator_t *matchingServices)
{
    kern_return_t           kernResult; 
    CFMutableDictionaryRef  classesToMatch;
	
	/*! @function IOServiceMatching
	 @abstract Create a matching dictionary that specifies an IOService class match.
	 @discussion A very common matching criteria for IOService is based on its class. IOServiceMatching will create a matching dictionary that specifies any IOService of a class, or its subclasses. The class is specified by C-string name.
	 @param name The class name, as a const C-string. Class matching is successful on IOService's of this class or any subclass.
	 @result The matching dictionary created, is returned on success, or zero on failure. The dictionary is commonly passed to IOServiceGetMatchingServices or IOServiceAddNotification which will consume a reference, otherwise it should be released with CFRelease by the caller. */
	
    // Serial devices are instances of class IOSerialBSDClient
    classesToMatch = IOServiceMatching(kIOSerialBSDServiceValue);
    if (classesToMatch == NULL)
    {
        printf("IOServiceMatching returned a NULL dictionary.\n");
    }
    else {
		/*!
		 @function CFDictionarySetValue
		 Sets the value of the key in the dictionary.
		 @param theDict The dictionary to which the value is to be set. If this
		 parameter is not a valid mutable CFDictionary, the behavior is
		 undefined. If the dictionary is a fixed-capacity dictionary and
		 it is full before this operation, and the key does not exist in
		 the dictionary, the behavior is undefined.
		 @param key The key of the value to set into the dictionary. If a key 
		 which matches this key is already present in the dictionary, only
		 the value is changed ("add if absent, replace if present"). If
		 no key matches the given key, the key-value pair is added to the
		 dictionary. If added, the key is retained by the dictionary,
		 using the retain callback provided
		 when the dictionary was created. If the key is not of the sort
		 expected by the key retain callback, the behavior is undefined.
		 @param value The value to add to or replace into the dictionary. The value
		 is retained by the dictionary using the retain callback provided
		 when the dictionary was created, and the previous value if any is
		 released. If the value is not of the sort expected by the
		 retain or release callbacks, the behavior is undefined.
		 */
        CFDictionarySetValue(classesToMatch,
                             CFSTR(kIOSerialBSDTypeKey),
                             CFSTR(kIOSerialBSDRS232Type));
        
        // Each serial device object has a property with key
        // kIOSerialBSDTypeKey and a value that is one of kIOSerialBSDAllTypes,
        // kIOSerialBSDModemType, or kIOSerialBSDRS232Type. You can experiment with the
        // matching by changing the last parameter in the above call to CFDictionarySetValue.
        
        // As shipped, this sample is only interested in serial ports,
        // so add this property to the CFDictionary we're matching on. 
        // This will find devices that advertise themselves as modems,
        // such as built-in and USB modems. However, this match won't find serial modems.
    }
    
    /*! @function IOServiceGetMatchingServices
	 @abstract Look up registered IOService objects that match a matching dictionary.
	 @discussion This is the preferred method of finding IOService objects currently registered by IOKit. IOServiceAddNotification can also supply this information and install a notification of new IOServices. The matching information used in the matching dictionary may vary depending on the class of service being looked up.
	 @param masterPort The master port obtained from IOMasterPort().
	 @param matching A CF dictionary containing matching information, of which one reference is consumed by this function. IOKitLib can contruct matching dictionaries for common criteria with helper functions such as IOServiceMatching, IOOpenFirmwarePathMatching.
	 @param existing An iterator handle is returned on success, and should be released by the caller when the iteration is finished.
	 @result A kern_return_t error code. */
	
    kernResult = IOServiceGetMatchingServices(kIOMasterPortDefault, classesToMatch, matchingServices);    
    if (KERN_SUCCESS != kernResult)
    {
        printf("IOServiceGetMatchingServices returned %d\n", kernResult);
    }
	
    return kernResult;
}

// Étant donné un itérateur sur les ports séries connectés à l'ordinateur, renvoie le
// chemin BSD (bsdPath = identifiant du port) du premier port de l'itérateur

static NSString* nextBSDPath(io_iterator_t serialPortIterator)
{
	io_object_t     serialPort;
    Boolean         portFound = false;
    
    // Iterate across all ports found. In this example, we bail after finding the first port.
    
    while ((serialPort = IOIteratorNext(serialPortIterator)) && !portFound)
    {
        CFStringRef   bsdPathAsCFString;
		
        // Get the callout device's path (/dev/cu.xxxxx). The callout device should almost always be
        // used: the dialin device (/dev/tty.xxxxx) would be used when monitoring a serial port for
        // incoming calls, e.g. a fax listener.
		
        bsdPathAsCFString = (CFStringRef)IORegistryEntryCreateCFProperty(serialPort,
																		 CFSTR(kIOCalloutDeviceKey),
																		 kCFAllocatorDefault,
																		 0);
        if (bsdPathAsCFString)
        {
            // Convert the path from a CFString to a stringstr
			
			const char* path = CFStringGetCStringPtr(bsdPathAsCFString, CFStringGetSystemEncoding());
			
			NSString* res = [NSString stringWithCString:path encoding:NSUTF8StringEncoding];
            CFRelease(bsdPathAsCFString);
            
            return res;
        }
		
        // Release the io_service_t now that we are done with it.
		
        (void) IOObjectRelease(serialPort);
    }
	
    return nil;
}

- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil
{
    self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil];
    if (self)
	{
		
    }
    
    return self;
}

-(void)awakeFromNib
{
	[ID addItemWithTitle:@"FE"];
	for(int i=0 ; i<0xFD ; i++)
	{
		[ID addItemWithTitle:byteToHex(i)];
	}
	[ID selectItemWithTitle:@"01"];
}

-(IBAction)rechercherPortsSerie:(id)sender
{
	[listePortsSerie removeAllItems];
	
	io_iterator_t portsSerie;
	
	FindSerialPort(&portsSerie);
	
	NSString* bsdPathNew;
	
	bool start = true;
	
	while ((bsdPathNew = nextBSDPath(portsSerie)))
	{
		if(start)
		{
			self.bsdPath = bsdPathNew;
			start = false;
		}
		
		[listePortsSerie addItemWithTitle:bsdPathNew];
	}
}

static std::string hexToBinary = "0123456789ABCDEF";

static uint8_t hexLetterToBinary(char l)
{
	uint8_t res = 0;
	
	while(hexToBinary[res]!=l)
	{
		res++;
	}
	
	assert(res<16);
	
	return res;
}

static std::vector<uint8_t> hexStringToBinary(NSString* s)
{
	s = [s uppercaseString];
	
	std::vector<uint8_t> res;
	
	assert(s.length%2==0); // Sinon les paramètres ne représentent pas une chaîne d'octets
	
	uint8_t tmp;
	
	for(int i=0 ; i<s.length/2 ; i++)
	{
		char l = [s characterAtIndex:2*i];
		tmp = hexLetterToBinary(l);
		tmp *= 16;
		
		l = [s characterAtIndex:2*i+1];
		tmp += hexLetterToBinary(l);
		
		res.push_back(tmp);
	}
	
	return res;
}

static NSString* byteToHex(uint8_t var)
{
	char l[3];
	
	l[0] = hexToBinary[var/16];
	l[1] = hexToBinary[var%16];
	l[2] = '\0';
	
	NSString* res = [NSString stringWithUTF8String:l];
	
	return res;
}



static NSString* binaryToHexString(std::vector<uint8_t> v)
{
	NSMutableString* res = [[NSMutableString alloc] init];
	
	for(int i=0 ; i<v.size() ; i++)
	{		
		[res appendString:byteToHex(v[i])];
	}
	
	return [res autorelease];
}

-(IBAction)envoyerPaquet:(id)sender
{
	if(!bsdPath || [bsdPath isEqualToString:@""])
	{
		NSLog(@"Port Invalide");
		return;
	}	
	
	InstructionPacket toSend;
	
	toSend.instruction = [commande selectedTag];
	if(toSend.instruction==0)
	{
		NSLog(@"Commande Invalide");
		return;
	}
	
	NSString* param = [parametres stringValue];
	
	toSend.parameters = hexStringToBinary(param);
	
	dynamixel->sendInstructionPacket(toSend);
	
	if(toSend.instruction <= 2)
	{
		StatusPacket response;
		if(dynamixel->receiveStatusPacket(response))
		{
			NSMutableString* responseString = [[NSMutableString alloc] init];
			
			[responseString appendFormat:@"Erreur:%@ Parametres:%@",byteToHex(response.error),binaryToHexString(response.parameters)];
			
			[reponse setStringValue:responseString];
		}
	}
	else
	{
		[reponse setStringValue:@""];
	}
}

-(IBAction)choisirNomPort:(id)sender
{
	if(dynamixel)
	{
		delete dynamixel;
	}
	self.bsdPath = [sender titleOfSelectedItem];
	
	dynamixel = new Dynamixel(std::string([bsdPath UTF8String]), hexStringToBinary([ID titleOfSelectedItem])[0]);
}

-(void)runSession
{
	while(joystickSessionRunning) 
	{
		SDL_Event event;
		while(SDL_PollEvent(&event))
		{
			if(event.type==SDL_JOYAXISMOTION || event.type==SDL_JOYHATMOTION || event.type==SDL_JOYBUTTONUP || event.type==SDL_JOYBALLMOTION || event.type==SDL_JOYBUTTONDOWN)
			{
				Joystick::handler()->handleJoystickEvent(event,dynamixel);
			}
		}
	}
}

-(IBAction)startJoystickSession:(id)sender
{
	joystickSessionRunning = true;
	
	Joystick::handler()->prepareJoystick();
	
	if(!bsdPath || [bsdPath isEqualToString:@""])
	{
		NSLog(@"Port Invalide");
		return;
	}
	
	[stopSession setEnabled:YES];
	[startSession setEnabled:NO];
	
	[self performSelectorOnMainThread:@selector(runSession) withObject:nil waitUntilDone:NO];
}

-(IBAction)stopJoystickSession:(id)sender
{
	joystickSessionRunning = false;
	
	delete dynamixel;
	
	[stopSession setEnabled:NO];
	[startSession setEnabled:YES];
}

@end
