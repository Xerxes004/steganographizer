#include "Steganographizer.h"

void Steganographizer::encrypt(
	const std::string &originalFile, 
   	const std::string &courierFile, 
   	const std::string ioFile)
{
	std::string payload;

	if (ioFile.compare("") == 0)
	{
		std::cout << "Input: ";
		std::getline(std::cin, payload);
		// getline does not input a null character, so we have to add one
		payload += '\0';
	}
	else
	{
		auto payloadBytes = readBytes(ioFile);
		payload = std::string(payloadBytes.begin(), payloadBytes.end());
		payload += '\0';
	}

	std::cout << "encrypting... ";

	auto start = TimeUtil::now();

	std::vector<char> originalBytes = readBytes(originalFile);
	std::vector<char> courierBytes  = equipWithPayload(originalBytes, payload);
	writeBytes(courierBytes, courierFile);

	auto seconds = TimeUtil::timeFrom(start);

	std::cout << "done in " << std::setprecision(2) << std::fixed 
	          << seconds << " seconds.\n";
}

const std::vector<char> Steganographizer::equipWithPayload(
 	const std::vector<char> &originalBytes,
	const std::string payload)
{
	std::vector<char> modifiedBytes(originalBytes);
	std::vector<char> payloadExpand = expandPayload(payload);

	int throwOut = getBytesToThrowOut(originalBytes);

	if (modifiedBytes.size() - throwOut < payloadExpand.size())
	{
		throw std::runtime_error("Image is not big enough to hold this data!");
	}
	
	int i = throwOut;
	for (char byte : payloadExpand)
	{
		setBit(modifiedBytes.at(i++), 0, int(byte));
	}
	return modifiedBytes;
}

std::vector<char> Steganographizer::expandPayload(const std::string &payload)
{
	std::vector<char> payloadBytes(payload.begin(), payload.end());
	std::vector<char> expansion;

	for (char bytes : payloadBytes)
	{
		for (int i = 0; i < 8; i++)
		{
			expansion.push_back(getBit(bytes, i));
		}
	}

	return expansion;
}

std::vector<char> Steganographizer::readBytes(const std::string &fileName)
{
	std::ifstream input(fileName);
	
	std::vector<char> bytes;

	if (input.is_open() && !input.fail())
	{
		input.seekg(0, input.end);
		auto fileSize = input.tellg();
		bytes.resize(fileSize);
		input.seekg(0);
		input.read(&bytes.front(), bytes.size());
	}
	else
	{
		throw std::runtime_error("Failed to open file " + fileName);
	}

	return bytes;
}

void Steganographizer::writeBytes(
	const std::vector<char> &bytes,
	const std::string &fileName)
{
	std::ofstream output(fileName);

	if (output.is_open() && !output.fail())
	{
		output.write(bytes.data(), bytes.size());
	}
}

// analyzes the bytes of the file to see what kind of BMP it is, then throws
// out the appropriate number of header bits
int Steganographizer::getBytesToThrowOut(const std::vector<char> &bitmapBytes)
{
	int throwOut = 0;

	if (bitmapBytes.size() < 2)
	{
		throw std::runtime_error("Input image too small, check file.");
	}

	const unsigned short dWord = bitmapBytes.at(1) << 8  | bitmapBytes.at(0);

	switch(getImgType(dWord))
	{
	case ImgType::MSFT_BMP_V1:
		throwOut = 10;
		break;

	case ImgType::MSFT_BMP_V2:
		throwOut = 14 + int(bitmapBytes.at(14));
		break;

	case ImgType::MSFT_BMP_V3_V4:
		throwOut = dWord;
		break;

	case ImgType::NOT_VALID:
	default:
		throw std::runtime_error("Non-microsoft BMP input, aborting.");
		break;
	}

	return throwOut;
}

// selects the proper enum type based on the dword of the BMP
ImgType Steganographizer::getImgType(const unsigned short dWord)
{
	auto type = ImgType::NOT_VALID;

	if (dWord == TYPE_1_BMP)
	{
		type = ImgType::MSFT_BMP_V1;
	} 
	else if (dWord == TYPE_2_BMP)
	{
		type = ImgType::MSFT_BMP_V2;
	}
	else
	{
		type = ImgType::MSFT_BMP_V3_V4;
	}

	return type;
}

const std::vector<char> Steganographizer::extractPayload(
	const std::vector<char> &modifiedBytes)
{
	auto dataStart = getBytesToThrowOut(modifiedBytes);
	std::vector<char> payloadBytes;

	for (int i = dataStart; i < modifiedBytes.size(); i += 8)
	{
		char c = char(0);

		for (int j = 0; j < 8; j++)
		{
			setBit(c, j, getBit(modifiedBytes.at(i + j), 0));
		}

		payloadBytes.push_back(c);

		if (payloadBytes.back() == '\0')
		{
			break;
		}
	}

	return payloadBytes;
}

const int Steganographizer::getBit(const char &byte, const short position)
{
	char mask = 1 << position;
	return (byte & mask) == 0 ? 0 : 1;
}

void Steganographizer::setBit(char &byte, const unsigned short position, 
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

void Steganographizer::printByteAsBinary(const char &byte)
{
	for (int i = 0; i < 8; i++)
	{
		std::cout << getBit(byte, i);
	}
}

void Steganographizer::decrypt(
	const std::string &modifiedImg, 
	const std::string ioFile)
{
	std::cout << "decrypting... ";

	std::vector<char> modifiedBytes = readBytes(modifiedImg);
	std::vector<char> payloadBytes  = extractPayload(modifiedBytes);
	std::string payload(payloadBytes.begin(), payloadBytes.end());

	if (ioFile == "")
	{
		std::cout << "Secret message:\n" << payload << std::endl;
	}
	else
	{
		std::ofstream output(ioFile);
		
		if (output.is_open() && !output.fail())
		{
			output.write(payload.c_str(), payload.length());
		}
		else
		{
			throw std::runtime_error("Output file failed to open, aborting.");
		}
	}
}





























