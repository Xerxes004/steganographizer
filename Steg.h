/**
 * This class is used to store a secret message in a BMP image using 
 * Steganography.
 * 
 * @author Wesley Kelly
 * @version 1.0
 *
 * File: Steg.h 
 * Created: 15 November 2015
 *
 * Copyright 2015 Cedarville University, its Computer Science faculty, and the
 * authors. All rights reserved.
 * 
 * Description: This class encodes the bytes of a BMP image with a secret
 * message, and allows the user to recover the message later. There are also
 * built-in methods for analyzing a BMP image, and for scrubbing the secret
 * message from an already encoded image.
 */

#ifndef STEG_H
#define STEG_H

#include <string>
#include <iostream>
#include <stdio.h>
#include <iomanip>
#include <vector>
#include <fstream>

class Steg
{
public:
	void encrypt(const std::string &original, const std::string &courier, 
   	 	 const std::string input = "");

	void decrypt(const std::string &courier, const std::string output = "");

	void analyze(const std::string &image);

	void scrub(const std::string &image);

private:
	// the appropriate byte values for type 1 and 2 BMP's
	const unsigned short TYPE_1_BMP = 0;
	const unsigned short TYPE_234_BMP = ('B' << 8) | 'M';

	void read(std::vector<char> &buffer, const std::string &fileName);
 
	void write(const std::vector<char> &bytes, const std::string &fileName);

	void equipPayload(std::vector<char> &modifiedBytes,
 	 	 const std::vector<char> &originalBytes, const std::string payload);

	void expandPayload(std::vector<unsigned short> &expansion, 
	 	 const std::string &payload);
	
	bool extractPayload(std::string &payload, 
	 	 const std::vector<char> &modifiedBytes);

	const unsigned int getHeaderSize(const std::vector<char> &bitmapBytes);

	const unsigned short getBitmapType(const unsigned short word);

	/**
	 * Gets the bit at the position specified. Position 0 is the right-most bit.
	 * 
	 * @param byte the byte being analyzed
	 * @param position the position of interest, whose value will be returned
	 * @return the bit at the position specified
	 */
	inline const unsigned short getBit(const char &byte, const short position)
	{		
		char mask =  1 << position;

		return (byte & mask) == 0 ? 0 : 1;
	}

    /**
	 * Gets the bit at the position specified. Position 0 is the right-most bit.
	 * 
	 * @param byte the byte being modified
	 * @param position the position of interest, whose value will be changed to 
	 *	      the value specified
	 * @param value the value that will be set in the specified position
	 */
	inline void setBit(char &byte, const unsigned short position, 
	    	const unsigned short value)
	{
		if (value == 0)
		{
			byte &= ~(1 << position);
		}
		else if (value == 1)
		{
			byte |= (1 << position);
		}
		else
		{
			throw std::runtime_error("setBit: Invalid value specified");
		}
	}

	/**
	 * Checks to see if a character is an ascii character or not.
	 *
	 * @param c the character to check
	 * @return whether or not the character is ascii
	 */
    inline bool isAscii(const char c)
    {
    	return (int(c) <= 127 && int(c) >= 0);
    }
};

#endif