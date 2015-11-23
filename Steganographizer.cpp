#include "Steganographizer.h"

Steganographizer::Steganographizer(){}

const bool Steganographizer::ensteginate(
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
	auto originalBytes = this->getBytes(originalImg);

	enstegrifyImage(carrierImg, originalBytes, payload);

	return false;
}

void Steganographizer::enstegrifyImage(
	const std::string &carrierImg,
	const std::vector<short> &originalBytes,
	const std::string payload)
{
	std::vector<short> newBytes = originalBytes;

	for (int i = 0; i < newBytes.size(); i++)
	{

	}
}

std::vector<short> Steganographizer::getBytes(
	const std::string &originalImgName)
{
	std::ifstream originalFile(originalImgName, std::ios::binary);
	
	std::vector<short> bytes;

	if (originalFile.is_open() && !originalFile.fail())
	{
		originalFile.seekg(0, originalFile.end);

		auto length = originalFile.tellg();

		for (int i = 0; i < length; i++)
		{
			originalFile.seekg(i);
			char c = ' ';
			originalFile.read(&c, 1);
			// cast char to short because read function is dumb and doesn't
			// like shorts
			bytes.push_back(short(c));
		}
	}
	else
	{
		throw std::runtime_error("Failed to open image");
	}

	return bytes;
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
	std::cout << "Input File data: " << std::endl;
	std::cout << input << std::endl;
}

const bool Steganographizer::decrypt(
	std::string modifiedImg, 
	std::string ioFile)
{
	return false;
}





























