//
//  Dynamixel.cpp
//  PSC
//
//  Created by Quentin Fiard on 15/01/12.
//  Copyright (c) 2012 École Polytechnique. All rights reserved.
//

#include <CoreFoundation/CoreFoundation.h>

#include <IOKit/IOKitLib.h>
#include <IOKit/serial/IOSerialKeys.h>
#include <IOKit/serial/ioss.h>
#include <IOKit/IOBSD.h>

#include <string>

#include <iostream>

#include <boost/array.hpp>
#include "Dynamixel.h"
//#include <boost/asio.hpp>

/* FindSerialPort
 Source : Apple Reference Library
 Fonction : Renvoie dans matchingServices un itérateur sur l'ensemble des ports séries connectés à l'ordinateur
*/

// Returns an iterator across all known serial ports. Caller is responsible for
// releasing the iterator when iteration is complete.
kern_return_t FindSerialPort(io_iterator_t *matchingServices)
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

std::string getBSDPathOfFirstSerialPort(io_iterator_t serialPortIterator)
{
	io_object_t     serialPort;
    Boolean         portFound = false;
	
	std::string res;
    
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
            Boolean result;
            
            // Convert the path from a CFString to a stringstr
			
			const char* path = CFStringGetCStringPtr(bsdPathAsCFString, CFStringGetSystemEncoding());
			
			res = std::string(path);
            CFRelease(bsdPathAsCFString);
            
            if (!res.empty())
            {
				std::cout << "Port found with BSD path: " << res << std::endl;
                //portFound = true;
            }
        }
		
        // Release the io_service_t now that we are done with it.
		
        (void) IOObjectRelease(serialPort);
    }
	
    return res;
}

Dynamixel::Dynamixel()
{
	
}

#pragma mark - Vérifications des erreurs

bool Dynamixel::isInstructionError(uint8_t error)
{
	return (error & 0x40) != 0;
}

bool Dynamixel::isOverloadError(uint8_t error)
{
	return (error & 0x20) != 0;
}

bool Dynamixel::isChecksumError(uint8_t error)
{
	return (error & 0x10) != 0;
}

bool Dynamixel::isRangeError(uint8_t error)
{
	return (error & 0x08) != 0;
}

bool Dynamixel::isOverheatingError(uint8_t error)
{
	return (error & 0x04) != 0;
}

bool Dynamixel::isAngleLimitError(uint8_t error)
{
	return (error & 0x02) != 0;
}

bool Dynamixel::isInputVoltageError(uint8_t error)
{
	return (error & 0x01) != 0;
}

#pragma mark -
#pragma mark Privé : Opérations d'envois et de réceptions

void Dynamixel::sendInstructionPacket(const InstructionPacket& aEnvoyer)
{
	std::vector<uint8_t> buffer(aEnvoyer.parameters.size()+6);
	
	buffer[0] = 0xFF;
	buffer[1] = 0xFF;
	
	buffer[2] = aEnvoyer.ID;
	buffer[3] = aEnvoyer.parameters.size()+2;
	
	buffer[4] = aEnvoyer.instruction;
	
	uint8_t checksum = aEnvoyer.ID;
	checksum += aEnvoyer.parameters.size()+2;
	checksum += aEnvoyer.instruction;
	
	for(int i=0 ; i<aEnvoyer.parameters.size() ; i++)
	{
		buffer[5+i] = aEnvoyer.parameters[i];
		checksum += aEnvoyer.parameters[i];
	}
	
	checksum = !checksum;
	
	buffer[5+aEnvoyer.parameters.size()] = checksum;
	
	//boost::asio::write(*port, buffer);
}

StatusPacket Dynamixel::receiveStatusPacket()
{
	// On reçoit 4 octets, qui doivent être 0xFF 0xFF (début de trame)
	// suivis de l'ID du moteur et de la longueur du message
	
	boost::array<uint8_t, 4> bufferStart;
	
	//boost::asio::read(*port, bufferStart);
	
	assert(bufferStart[0]==0xFF);
	assert(bufferStart[1]==0xFF);
	
	uint8_t id = bufferStart[2];
	uint8_t length = bufferStart[3];
	
	std::vector<uint8_t> bufferEnd(length);
	
	//boost::asio::read(*port, bufferEnd);
	
	uint8_t error = bufferEnd[0];
	
	// On calcule la somme de contrôle pour vérifier que l'on peut se fier
	// aux informations reçues
	
	uint8_t checksum = id;
	checksum += length;
	
	for(int i=0 ; i<length-2 ; i++)
	{
		checksum += bufferEnd[1+i];
	}
	
	checksum = !checksum;
	
	assert(checksum == bufferEnd[length-1]);
	
	StatusPacket res;
	res.ID = id;
	res.error = error;
	res.parameters.resize(length-2);
	
	for(int i=0 ; i<length-2 ; i++)
	{
		res.parameters[i] = bufferEnd[1+i];
	}
	
	return res;
}

#pragma mark - Debug

void Dynamixel::check()
{
	io_iterator_t serialPort;
	
	FindSerialPort(&serialPort);
	
	getBSDPathOfFirstSerialPort(serialPort);
}

