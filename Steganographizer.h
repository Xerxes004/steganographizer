#ifndef STEG_H
#define STEG_H

#include <string>
#include <iostream>
#include <iomanip>
#include <vector>
#include <thread>
#include <mutex>
#include <fstream>
#include <sstream>

#include <chrono>


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
	Steganographizer();
	
	void encrypt(
		const std::string &original, 
	   	const std::string &courier, 
	   	const std::string ioFile = "");

	const bool decrypt(std::string modImg, std::string ioFile = "");

private:
	const int TYPE_1_BMP_MASK = 0x0;
	const int TYPE_2_BMP_MASK = ('M' << 8) | 'B'; // little endian

	void getFileData(std::string &input, const std::string &ioFile);
	
	std::vector<char> readBytes(const std::string &fileName);

	void writeBytes(
		const std::vector<char> &bytes, 
		const std::string &fileName);

	const std::vector<char> equipWithPayload(
	 	const std::vector<char> &originalBytes,
		const std::string payload);

	std::vector<char> expand(const std::string &payload);
	
	int getBytesToThrowOut(const std::vector<char> &originalBytes);
	ImgType getImgType(const int dWord);
};

#endif