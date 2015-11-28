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

	const unsigned short getImgType(const unsigned short dWord);

	const std::vector<char> extractPayload(
		const std::vector<char> &payloadBytes);

	int getBit(const char &byte, const short position);
    void setBit(char &byte, const unsigned short position, 
    	const unsigned short value);
};

#endif