#include "Steg.h"

//public 

/**
 * Stores a secret message in a BMP file. If an input file is not specified,
 * then a message from the command line is taken from the user.
 * 
 * @param original the original image into which a message will be stored
 * @param courier the output image constructed from the input, but equipped
 *                with a payload
 * @param input an optional input text file that contains the secret message
 */
void Steg::encrypt(const std::string &original, const std::string &courier, 
   	 const std::string input)
{
	std::string payload;

	if (input.compare("") == 0)
	{
		std::cout << "Input: ";
		std::getline(std::cin, payload);
	}
	else
	{
		std::vector<char> payloadBytes;
		read(payloadBytes, input);
		payload = std::string(payloadBytes.begin(), payloadBytes.end());
	}

	// getline does not input a null character, so we have to add one
	payload += '\0';

	std::vector<char> originalBytes;
	read(originalBytes, original);

	std::vector<char> courierBytes;
	equipPayload(courierBytes, originalBytes, payload);

	write(courierBytes, courier);
}

/**
 * Retrieves a secret message from the specified BMP. The secret message is
 * stored in an output file if specified, otherwise it is written to the
 * console.
 * 
 * @param courier a BMP image which contains a secret message
 * @param output an optional output text file where the secret message is stored
 */
void Steg::decrypt(const std::string &courier, const std::string output)
{
	std::vector<char> modifiedBytes;
	read(modifiedBytes, courier);
	
	std::string payload = "";
	if (extractPayload(payload, modifiedBytes))
	{	
		if (output == "")
		{
			std::cout << payload << std::endl;
		}
		else
		{
			std::ofstream outFile(output);
			
			if (outFile.is_open() && !outFile.fail())
			{
				outFile.write(payload.c_str(), payload.length());
			}
			else
			{
				throw std::runtime_error("Output file failed to open, aborting.");
			}
		}
	}
	else
	{
		std::string msg = "Input image is not encrypted!\n";
		msg += "A non-ASCII character was found.";

		throw std::runtime_error(msg);
	}
}

/**
 * Analyzes the given BMP file to allow the user to plan their message length
 * appropriately.
 * 
 * @param image the image to analyze
 */
void Steg::analyze(const std::string &image)
{
	std::vector<char> bytes;
	read(bytes, image);
	unsigned int dWord = (bytes.at(0) << 8) | bytes.at(1);
	
	auto imageType = getImgType(dWord);

	std::cout << "Analyzing " << image << std::endl;
	std::cout << "    Length     : " << bytes.size() << " bytes\n";
	std::cout << "    BMP type   : " << imageType << std::endl;
	std::cout << "    Max payload: " 
	          << ((bytes.size() - getBytesToThrowOut(bytes)) / 8) - 1
	          << " characters\n";

    bool encrypted = true;

    std::string payload;
	
	if (!extractPayload(payload, bytes))
	{
    	encrypted = false;
    }

    std::cout << "    This image ";
    
    if (encrypted)
    {
    	std::cout << "may be ";
    }
    else
    {
    	std::cout << "is probably not ";
    }

    std::cout << "encrypted.\n";
}

/**
 * Scrubs an image of all secret messages so you can hide your tracks!
 * 
 * @param image the image to scrub
 */
void Steg::scrub(const std::string &image)
{
	std::vector<char> bytes;
	read(bytes, image);
	auto throwOut = getBytesToThrowOut(bytes);

	for (int i = throwOut; i < bytes.size(); i++)
	{
		// set all low-order bits to 1
		setBit(bytes.at(i), 0, 1);
	}

	write(bytes, image);
}

//private

/**
 * Reads the bytes of a file as characters into a vector buffer.
 * 
 * @param buffer the buffer which will store the file information
 * @param fileName the name of the file to read
 */
void Steg::read(std::vector<char> &buffer, const std::string &fileName)
{
	std::ifstream input(fileName);

	if (input.is_open() && !input.fail())
	{
		input.seekg(0, input.end);
		auto fileSize = input.tellg();
		buffer.resize(fileSize);
		input.seekg(0);
		input.read(&buffer.front(), buffer.size());
	}
	else
	{
		throw std::runtime_error("Failed to open file " + fileName);
	}
}

/**
 * Writes a vector of characters to the specified file.
 * 
 * @param bytes the bytes to write
 * @param fileName the file to write to
 */
void Steg::write(const std::vector<char> &bytes, const std::string &fileName)
{
	std::ofstream output(fileName);

	if (output.is_open() && !output.fail())
	{
		output.write(bytes.data(), bytes.size());
	}
}

/**
 * Attaches a secret payload to the bytes of a BMP file.
 * 
 * @param modifiedBytes the bytes that have been modified to store the payload
 * @param originalBytes the bytes of the original BMP file
 * @param payload the secret message that will be stored in the modifiedBytes
 */
void Steg::equipPayload(std::vector<char> &modifiedBytes,
 	 const std::vector<char> &originalBytes, const std::string payload)
{
	modifiedBytes = originalBytes;

	std::vector<char> payloadExpand;
	expandPayload(payloadExpand, payload);

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
}

/**
 * Expands the payload message so that every 1 byte of payload information is
 * spread over 8 bytes of the image. One bit at a time is inserted into each
 * byte of the original image in the lowest-order position, effectively
 * concealing the data without changing the image noticeably.
 * 
 * @param bytes the bytes to write
 * @param fileName the file to write to
 */
void Steg::expandPayload(std::vector<char> &expansion, 
	 const std::string &payload)
{
	std::vector<char> payloadBytes(payload.begin(), payload.end());

	for (char bytes : payloadBytes)
	{
		for (int i = 0; i < 8; i++)
		{
			expansion.push_back(getBit(bytes, i));
		}
	}
}

/**
 * Analyzes the bytes of the file to see what kind of BMP it is, returns the
 * appropriate number of header bits to throw out. The only part of the image
 * that should be modified is the pixel information. Damaging the header could
 * make the file unreadable, or have other unforseen consequences.
 * 
 * @param bytes the bytes to write
 * @param fileName the file to write to
 */
int Steg::getBytesToThrowOut(const std::vector<char> &bitmapBytes)
{
	int throwOut = 0;

	if (bitmapBytes.size() < 2)
	{
		throw std::runtime_error("Input image too small, check file.");
	}

	const unsigned int dWord = bitmapBytes.at(1) << 8  | bitmapBytes.at(0);

	// these byte values were found on fileformat.info
	switch(getImgType((unsigned short)(dWord)))
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

/**
 * Selects the proper enum type based on the dword of the BMP.
 * 
 * @param dWord the first field of a BMP image, which varies from type to type
 */
const unsigned int Steg::getImgType(const unsigned int word)
{
	unsigned int type = 0;

	if (word == TYPE_1_BMP)
	{
		type = 1;
	} 
	else if (word == TYPE_2_BMP)
	{
		type = 2;
	}
	else
	{
		type = 3;
	}

	return type;
}

/**
 * Extracts a payload from a vector of bytes. The low-order bit of each byte
 * forms part of a character. One lower-order bit from 8 bytes forms a 
 * character.
 * 
 * @param payload the payload extracted from the bytes
 * @param modifiedBytes the bytes that have payload data in them
 */
bool Steg::extractPayload(std::string &payload, 
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
		if (isAscii(c))
		{
			payloadBytes.push_back(c);
		}
		else
		{
			// a non-ascii character was found, and the operation is aborted
			return false;
		}

		// stop at a null char
		if (payloadBytes.back() == '\0')
		{
			break;
		}
	}

	payload = std::string(payloadBytes.begin(), payloadBytes.end());

	return true;
}