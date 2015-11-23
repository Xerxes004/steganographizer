#ifndef STEG_H
#define STEG_H

#include <string>
#include <iostream>
#include <iomanip>
#include <vector>
#include <fstream>
#include <sstream>

class Steganographizer
{
public:
	Steganographizer();
	
	const bool ensteginate(
		const std::string orgImg, 
		const std::string modImg, 
	 	const std::string ioFile = "");

	const bool decrypt(std::string modImg, std::string ioFile = "");

private:
	void getFileData(const std::string &ioFile, std::string &input);
	
	std::vector<short> getBytes(
		const std::string &fileName);

	void enstegrifyImage(
	 	const std::string &carrierImage, 
	 	const std::vector<short> &encodedBytes,
		const std::string payload);
};

#endif