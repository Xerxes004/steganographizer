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
	void encrypt(const std::string &originalImg, const std::string &courier, 
		 const std::string ioFile = "");

	void decrypt(const std::string &courierImg, const std::string ioFile = "");

	void analyze(const std::string &image);

	void scrub(const std::string &image);

private:
	const unsigned int TYPE_1_BMP = 0;
	const unsigned int TYPE_2_BMP = ('B' << 8) | 'M';

	void read(std::vector<char> &buffer, const std::string &fileName);
	void write(const std::vector<char> &bytes, const std::string &fileName);

	void equipPayload(std::vector<char> &modifiedBytes,
 	 	 const std::vector<char> &originalBytes, const std::string payload);
	void expandPayload(std::vector<char> &expansion, 
	     const std::string &payload);
	
	void extractPayload(std::string &payload, 
	 	 const std::vector<char> &modifiedBytes);

	int getBytesToThrowOut(const std::vector<char> &originalBytes);

	const unsigned int getImgType(const unsigned int word);

	/**
	 * Gets the bit at the position specified. Position 0 is the right-most bit.
	 * 
	 * @param byte the byte being analyzed
	 * @param position the position of interest, whose value will be returned
	 * @return the bit at the position specified
	 */
	inline const short getBit(const char &byte, const short position)
	{		
		char mask =  1 << position;

		return (byte & mask) == 0 ? 0 : 1;
	}

    /**
	 * Gets the bit at the position specified. Position 0 is the right-most bit.
	 * 
	 * @param byte the byte being modified
	 * @param position the position of interest, whose value will be changed to the
	 *        value specified
	 * @param value the value that will be set in the specified position
	 */
	inline void setBit(char &byte, const unsigned short position, 
	    	const unsigned short value)
	{
		if (value == 0)
		{
			byte &= ~(1 << position);
		}
		else
		{
			byte |= (1 << position);
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