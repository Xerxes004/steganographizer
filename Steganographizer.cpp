#include "Steganographizer.h"

Steganographizer::Steganographizer(){}

void Steganographizer::encrypt(
	const std::string &original, 
   	const std::string &courier, 
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
		getFileData(payload, ioFile);
	}

	std::cout << "reading original image... this may take a few seconds\n";
	std::vector<char> originalBytes = readBytes(original);
	std::vector<char> courierBytes  = equipWithPayload(originalBytes, payload);

	writeBytes(courierBytes, courier);
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
			int max = (beg + chunksz) < maxIndex ? beg + chunksz : bytes.size();
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

const std::vector<char> Steganographizer::equipWithPayload(
 	const std::vector<char> &originalBytes,
	const std::string payload)
{
	std::vector<char> modifiedBytes = originalBytes;
	std::vector<char> payloadMask   = expand(payload);



	int throwOut = getBytesToThrowOut(originalBytes);

	

	/*for (int i = 0; i < modifiedBytes.size(); i++)
	{

	}*/

	return modifiedBytes;
}

ImgType Steganographizer::getImgType(const int dWord)
{
	auto type = ImgType::NOT_VALID;

	if (dWord & (TYPE_1_BMP_MASK << 16))
	{
		type = ImgType::MSFT_BMP_V1;
	} 
	else if (dWord & (TYPE_2_BMP_MASK << 16))
	{
		type = ImgType::MSFT_BMP_V2;
	}
	else
	{
		type = ImgType::MSFT_BMP_V3_V4;
	}

	return type;
}

int Steganographizer::getBytesToThrowOut(const std::vector<char> &originalBytes)
{
	int throwOut = 0;

	if (originalBytes.size() < 4)
	{
		throw std::runtime_error("Input image too small, check file.");
	}

	int dWord = originalBytes.at(0) << 24 
			  | originalBytes.at(1) << 16
			  | originalBytes.at(2) << 8
			  | originalBytes.at(3);

	switch(getImgType(dWord))
	{
	case ImgType::MSFT_BMP_V1:
		throwOut = 10;
		std::cout << "type 1 detected\n";
		break;

	case ImgType::MSFT_BMP_V2:
		throwOut = 14;
		std::cout << "type 2 detected\n";
		break;

	case ImgType::MSFT_BMP_V3_V4:
		std::cout << "type 3 or 4 detected\n";
		throwOut = (originalBytes.at(1) << 8) | originalBytes.at(0);
		break;

	default:
		throw std::runtime_error("Non-microsoft BMP input, aborting.");
		break;
	}

	return throwOut;
}

std::vector<char> Steganographizer::expand(const std::string &payload)
{
	std::vector<char> expansion(payload.begin(), payload.end());
	return expansion;
}

void Steganographizer::getFileData(
	std::string &input,
	const std::string &ioFile)
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





























