#include "Steganographizer.h"

Steganographizer::Steganographizer(){}

const bool Steganographizer::encrypt(
	const std::string originalImg, 
   	std::string carrierImg, 
   	const std::string ioFile)
{
	std::string payload = "";

	if (ioFile.compare("") == 0)
	{
		std::cout << "Input: ";
		std::cin >> payload;
	}
	else
	{
		this->getFileData(ioFile, payload);
	}

	carrierImg = this->enstegenate(originalImg, payload);

	return false;
}

std::string Steganographizer::enstegenate(
	const std::string &originalImg,
	const std::string &payload)
{

}

void Steganographizer::getFileData(
	const std::string &ioFile, 
	std::string &input)
{
	std::ifstream inFile(ioFile);

	if (inFile.is_open())
	{
		std::stringstream ss;
		ss << inFile.rdbuf();
		input = ss.str();
		inFile.close();
	}
	else
	{
		throw std::runtime_error("Error opening input file");
	}

	std::cout << input << std::endl;
}

const bool Steganographizer::decrypt(
	std::string modifiedImg, 
	std::string ioFile)
{
	return false;
}





























