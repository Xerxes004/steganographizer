#ifndef STEG_H
#define STEG_H

#include <string>
#include <iostream>
#include <stdio.h>
#include <iomanip>
#include <vector>
#include <fstream>

#include "TimeUtil.h"

// enum used to select between BMP types
enum class ImgType
{
	MSFT_BMP_V1, 
	MSFT_BMP_V2, 
	MSFT_BMP_V3_V4,
	NOT_VALID
};

class Steganographizer
{
public:
	void encrypt(const std::string &originalImg, const std::string &courier, 
		const std::string ioFile = "");

	void decrypt(const std::string &courierImg, const std::string ioFile = "");

	void analyze(const std::string &image);

private:
	const unsigned short TYPE_1_BMP = 0;
	const unsigned short TYPE_2_BMP = ('B' << 8) | 'M';

	std::vector<char> readBytes(const std::string &fileName);

	void writeBytes(const std::vector<char> &bytes, 
		const std::string &fileName);

	const std::vector<char> equipWithPayload(
		const std::vector<char> &originalBytes, const std::string payload);

	std::vector<char> expandPayload(const std::string &payload);
	
	int getBytesToThrowOut(const std::vector<char> &originalBytes);

	ImgType getImgType(const unsigned short dWord);

	const std::vector<char> extractPayload(
		const std::vector<char> &payloadBytes);

	int getBit(const char &byte, const short position);
    void setBit(char &byte, const unsigned short position, 
    	const unsigned short value);

    void printByteAsBinary(const char &byte);
};

#endif