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

class Steganographizer
{
public:
	Steganographizer();
	
	const bool ensteginate(
		const std::string originalImg, 
	   	const std::string carrierImg, 
	   	const std::string ioFile = "");

	const bool decrypt(std::string modImg, std::string ioFile = "");

private:
	void getFileData(const std::string &ioFile, std::string &input);
	
	std::vector<char> getBytes(const std::string &fileName);
	void writeBytes(
		const std::vector<char> &bytes, 
		const std::string &fileName);

	void enstegrifyImage(
	 	const std::string &carrierImage, 
	 	const std::vector<char> &encodedBytes,
		const std::string payload);

	std::vector<char> expand(const std::string &payload);
};

#endif