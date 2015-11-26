#ifndef STEG_H
#define STEG_H

#include <string>
#include <iostream>
#include <iomanip>
#include <vector>
#include <thread>
#include <fstream>
#include <sstream>

#include "TimeUtil.h"

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
	void encrypt(
		const std::string &originalFile, 
	   	const std::string &courierFile, 
	   	const std::string ioFile = "");

	void decrypt(const std::string &modifiedImg, const std::string ioFile = "");

private:
	const int TYPE_1_BMP_MASK = 0x00;
	const int TYPE_2_BMP_MASK = ('B' << 8) | 'M';

	std::string getFileString(const std::string &ioFile);
	
	std::vector<char> readBytes(const std::string &fileName);

	void writeBytes(
		 const std::vector<char> &bytes, 
		 const std::string &fileName);

	const std::vector<char> equipWithPayload(
	 	  const std::vector<char> &originalBytes,
		  const std::string payload);

	std::vector<char> expandPayload(const std::string &payload);
	
	int 	getBytesToThrowOut(const std::vector<char> &originalBytes);
	ImgType getImgType(const unsigned short dWord);

	const std::vector<char> extractPayload(const std::vector<char> &payloadBytes);

	const int  getBit(const char &byte, int position);
		  void setBit(char &byte, int position, unsigned int value);

	void printByteAsBinary(const char &byte);
};

#endif