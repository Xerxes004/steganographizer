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
		payload = getFileString(ioFile);
	}

	std::cout << "encrypting...\n";

	auto start = std::chrono::system_clock::now();
	
	std::vector<char> originalBytes = readBytes(originalFile);
	std::vector<char> courierBytes  = equipWithPayload(originalBytes, payload);
	writeBytes(courierBytes, courierFile);

	auto delta = std::chrono::system_clock::now() - start;
	auto seconds = 
		std::chrono::duration_cast<std::chrono::milliseconds>(delta).count() / 1000.0;

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

	if (modifiedBytes.size() - throwOut < (payload.size() * sizeof(char)))
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
		auto filesz = input.tellg();

		bytes.resize(filesz);

		// makes a thread for each 512kb chunk of image
		const int chunksz = 524288;
		const int chunks = filesz / chunksz + ((chunksz % filesz == 0) ? 0 : 1);
		
		std::vector<std::thread> workerPool;
		workerPool.resize(chunks);

		auto processChunk = [&](int beg, int max)->void
		{
			std::ifstream chunkFile(fileName);

			if (!chunkFile.fail() && chunkFile.is_open())
			{
				while (beg < max)
				{
					chunkFile.seekg(beg);
					chunkFile.read(&bytes.at(beg++), 1);
				}
			}
		};

		int maxIndex = filesz;

		for (int i = 0; i < workerPool.size(); i++)
		{
			int beg = i * chunksz * sizeof(char);
			int max = (beg + chunksz) < maxIndex ? beg + chunksz : bytes.size() - 1;
			workerPool.at(i) = std::thread(processChunk, beg, max);
		}

		for (int i = 0; i < workerPool.size(); i++)
		{
			workerPool.at(i).join();
		}
	}
	else
	{
		throw std::runtime_error("Failed to open image");
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

	if (bitmapBytes.size() < 4)
	{
		throw std::runtime_error("Input image too small, check file.");
	}

	int dWord = bitmapBytes.at(0) << 23 | 
			    bitmapBytes.at(1) << 15 |
			    bitmapBytes.at(2) << 7  |
			    bitmapBytes.at(3);

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

	default:
		throw std::runtime_error("Non-microsoft BMP input, aborting.");
		break;
	}

	return throwOut;
}

// selects the proper enum type based on the dword of the BMP
ImgType Steganographizer::getImgType(const int dWord)
{
	auto type = ImgType::NOT_VALID;

	if (dWord & (TYPE_1_BMP_MASK << 15))
	{
		type = ImgType::MSFT_BMP_V1;
	} 
	else if (dWord & (TYPE_2_BMP_MASK << 15))
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

		if (c == '\0')
		{
			break;
		}
	}

	return payloadBytes;
}

// returns the bit at the specified position. The leftmost bit is bit 0 due to
// little-endian style
const int Steganographizer::getBit(const char &byte, int position)
{
	char mask = 1 << (7 - position);
	return (byte & mask) == 0 ? 0 : 1;
}

void Steganographizer::setBit(char &byte, int position, unsigned int value)
{
	if (value == 0)
	{
		byte &= ~(1 << (7 - position));
	}
	else
	{
		byte |= (1 << (7 - position));
	}
}

void Steganographizer::printByteAsBinary(const char &byte)
{
	for (int i = 0; i < 8; i++)
	{
		std::cout << getBit(byte, i);
	}
}

std::string Steganographizer::getFileString(const std::string &ioFile)
{
	std::ifstream inFile(ioFile);
	std::stringstream ss;

	if (inFile.is_open())
	{
		ss << inFile.rdbuf();
		inFile.close();
	}
	else
	{
		throw std::runtime_error("Error opening input file");
	}

	return ss.str();
}

void Steganographizer::decrypt(
	const std::string &modifiedImg, 
	const std::string ioFile)
{
	std::cout << "decrypting... this may take a few seconds\n";

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
			output.write(payload.c_str(), payload.length() - 1);
		}
		else
		{
			throw std::runtime_error("Output file failed to open!");
		}
	}
}





























