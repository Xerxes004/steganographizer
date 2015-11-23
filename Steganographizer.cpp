#include "Steganographizer.h"

Steganographizer::Steganographizer(){}

const bool Steganographizer::ensteginate(
	const std::string orgImgName, 
   	std::string carrierImgName, 
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

	auto originalBytes = this->getBytes(orgImgName);

	std::cout << "file " << orgImgName 
			  << " is " << originalBytes.size() << " bytes long.\n";

	//enstegrifyImage(carrierImgName, originalBytes, payload);

	return false;
}

std::vector<char> Steganographizer::getBytes(
	const std::string &originalImgName)
{
	std::ifstream originalImg(originalImgName);
	
	std::vector<char> bytes;

	std::cout << "reading image... this may take a few seconds\n";
	if (originalImg.is_open() && !originalImg.fail())
	{
		originalImg.seekg(0, originalImg.end);
		auto filesz = originalImg.tellg();

		bytes.resize(filesz);

		// makes a thread for each 512kb chunk of image
		std::vector<std::thread> workerPool;

		const int chunksz = 524288;
		int numChunks = filesz / chunksz + ((filesz % chunksz == 0) ? 0 : 1);

		workerPool.resize(numChunks);
		
		auto processChunk = [&](int beg, const int max)->void
		{
			std::ifstream chunkFile(originalImgName);

			if (!chunkFile.fail() && chunkFile.is_open())
			{
				while (++beg < max)
				{
					chunkFile.seekg(beg);
					char c = ' ';
					chunkFile.read(&c, 1);
					bytes.at(beg) = c;
				}
			}
		};

		int maxIndex = filesz;

		auto start = std::chrono::system_clock::now();

		for (int i = 0; i < workerPool.size(); i++)
		{
			int base = i * chunksz * sizeof(char);

			if ((base + chunksz) < maxIndex)
			{
				workerPool.at(i) = 
					std::thread(processChunk, base, base + chunksz - 1);
			}
			else
			{
				workerPool.at(i) =
					std::thread(processChunk, base, maxIndex);
			}
		}

		for (int i = 0; i < workerPool.size(); i++)
		{
			workerPool.at(i).join();
		}

		auto total = std::chrono::system_clock::now() - start;

		long long runtime = 
			std::chrono::duration_cast<std::chrono::milliseconds>(total).count();

		std::cout << "read time: " << runtime << "ms\n";
	}
	else
	{
		throw std::runtime_error("Failed to open image");
	}

	return bytes;
}

void Steganographizer::enstegrifyImage(
	const std::string &carrierImg,
	const std::vector<char> &originalBytes,
	const std::string payload)
{
	std::vector<char> modifiedBytes = originalBytes;
	std::vector<char> payloadMask = expand(payload);

	if (modifiedBytes.size() < originalBytes.size())
	{

	}

	for (int i = 0; i < modifiedBytes.size(); i++)
	{

	}
}

std::vector<char> Steganographizer::expand(const std::string &payload)
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
	std::cout << "Input File data: " << std::endl;
	std::cout << input << std::endl;
}

const bool Steganographizer::decrypt(
	std::string modifiedImg, 
	std::string ioFile)
{
	return false;
}




























