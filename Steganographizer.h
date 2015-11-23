#ifndef STEG_H
#define STEG_H

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

class Steganographizer
{
public:
	Steganographizer();
	
	const bool encrypt(
		const std::string orgImg, 
		const std::string modImg, 
	 	const std::string ioFile = "");

	const bool decrypt(std::string modImg, std::string ioFile = "");

private:
	void getFileData(const std::string &ioFile, std::string &input);
	std::string enstegenate(
		const std::string &originalImg,
		const std::string &input);

};

#endif