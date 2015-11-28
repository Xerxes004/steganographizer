#include "Steg.h"

void Steg::encrypt(
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

	std::vector<char> originalBytes = readBytes(originalFile);
	std::vector<char> courierBytes  = equipWithPayload(originalBytes, payload);
	writeBytes(courierBytes, courierFile);
}

const std::vector<char> Steg::equipWithPayload(
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

std::vector<char> Steg::expandPayload(const std::string &payload)
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

std::vector<char> Steg::readBytes(const std::string &fileName)
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

void Steg::writeBytes(
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
int Steg::getBytesToThrowOut(const std::vector<char> &bitmapBytes)
{
	int throwOut = 0;

	if (bitmapBytes.size() < 2)
	{
		throw std::runtime_error("Input image too small, check file.");
	}

	const unsigned short dWord = bitmapBytes.at(1) << 8  | bitmapBytes.at(0);

	switch(getImgType(dWord))
	{
	case 1:
		throwOut = 10;
		break;

	case 2:
		throwOut = 14 + int(bitmapBytes.at(14));
		break;

	case 3:
		throwOut = dWord;
		break;

	default:
		throw std::runtime_error("Non-microsoft BMP input, aborting.");
		break;
	}

	return throwOut;
}

// selects the proper enum type based on the dword of the BMP
const unsigned short Steg::getImgType(const unsigned short dWord)
{
	unsigned short type = 0;

	if (dWord == TYPE_1_BMP)
	{
		type = 1;
	} 
	else if (dWord == TYPE_2_BMP)
	{
		type = 2;
	}
	else
	{
		type = 3;
	}

	return type;
}

const std::vector<char> Steg::extractPayload(
	const std::vector<char> &modifiedBytes)
{
	auto dataStart = getBytesToThrowOut(modifiedBytes);
	std::vector<char> payloadBytes;

	for (int i = dataStart; i < modifiedBytes.size(); i += 8)
	{
		char c = char(0);

		for (int j = 0; j < 8; j++)
		{
			// set the j'th bit in c equal to the lowest-order bit at i + j of
			// the modified bytes. This effectively looks at the next 8 bytes, 
			// and reconstructs a character from their lowest-order bits.
			setBit(c, j, getBit(modifiedBytes.at(i + j), 0));
		}
		if (int(c) <= 127 && int(c) >= 0)
		{
			payloadBytes.push_back(c);
		}
		else
		{
			std::string msg = "Input image is not encrypted!\n";
			msg += "A non-alphabetical ASCII character was found.";

			throw std::runtime_error(msg.c_str());
		}

		if (payloadBytes.back() == '\0')
		{
			break;
		}
	}

	return payloadBytes;
}

int Steg::getBit(const char &byte, const short position)
{
	char mask = 1 << position;
	return (byte & mask) == 0 ? 0 : 1;
}

void Steg::setBit(char &byte, const unsigned short position, 
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

void Steg::decrypt(
	const std::string &courier, 
	const std::string ioFile)
{
	std::vector<char> modifiedBytes = readBytes(courier);
	std::vector<char> payloadBytes  = extractPayload(modifiedBytes);
	std::string payload(payloadBytes.begin(), payloadBytes.end());

	if (ioFile == "")
	{
		std::cout << "Secret message:" << std::endl << payload << std::endl;
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

void Steg::analyze(const std::string &image)
{
	auto bytes = readBytes(image);
	auto imageType = getImgType((bytes.at(0) << 8) | bytes.at(1));

	std::string tab = "    ";

	std::cout << "Analyzing " << image << std::endl;
	std::cout << tab << "Length     : " << bytes.size() << " bytes\n";
	std::cout << tab << "BMP type   : " << imageType << std::endl;
	std::cout << tab << "Max payload: " 
	          << ((bytes.size() - getBytesToThrowOut(bytes)) / 8) - 1
	          << " characters\n";

}



























