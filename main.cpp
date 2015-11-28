#include <iostream>
#include <iomanip>
#include <vector>
#include <chrono>

#include "Steganographizer.h"

void showHelp();
void encrypt(const std::vector<std::string> &args, const int index);
void decrypt(const std::vector<std::string> &args, const int index);

int main(int argc, char* argv[])
{
	if (argc < 2)
	{
		showHelp();
	}

	// modernize the arguments, because passing arrays is a nightmare
	std::vector<std::string> args(argv, argv + argc);

	for (int i = 0; i < argc; i++)
	{
		args.push_back(std::string(argv[i]));
	}

	for (int i = 0; i < args.size(); i++)
	{
		if (args.at(i).compare("-e") == 0)
		{
			encrypt(args, i);
			break;
		}

		if (args.at(i).compare("-d") == 0)
		{
			decrypt(args, i);
			break;
		}

		if (args.at(i).compare("-h") == 0)
		{
			showHelp();
			break;
		}
	}

	return 0;
}

void encrypt(const std::vector<std::string> &args, const int index)
{
	std::string orgImg = "";
	std::string modImg = "";
	std::string ioFileName = "";

	Steganographizer steg;

	if ((index + 2) < args.size())
	{
		orgImg = args.at(index + 1);
		modImg = args.at(index + 2);

		if (orgImg.substr(orgImg.size() - 4, 4).compare(".bmp") != 0)
		{
			orgImg += ".bmp";
		}
		if (modImg.substr(modImg.size() - 4, 4).compare(".bmp") != 0)
		{
			modImg += ".bmp";
		}
	}
	else
	{
		std::cout << "error: not enough arguments\n";
		showHelp();
	}

	try
	{
		if ((index + 3) < args.size())
		{
			ioFileName = args.at(index + 3);
			steg.encrypt(orgImg, modImg, ioFileName);
		}
		else
		{
			steg.encrypt(orgImg, modImg);
		}
	}
	catch (std::runtime_error &err)
	{
		std::cout << err.what() << std::endl;
	}
}

void decrypt(const std::vector<std::string> &args, const int index)
{
	std::string orgImg = "";
	std::string modImg = "";
	std::string ioFileName = "";

	Steganographizer steg;

	if ((index + 1) < args.size())
	{
		modImg = args.at(index + 1);

		if (modImg.substr(modImg.size() - 4, 4).compare(".bmp") != 0)
		{
			modImg += ".bmp";
		}
	}
	else
	{
		std::cout << "error: not enough arguments\n";
		showHelp();
	}

	try
	{
		if ((index + 2) < args.size())
		{
			ioFileName = args.at(index + 2);
			steg.decrypt(modImg, ioFileName);
		}
		else
		{
			steg.decrypt(modImg);
		}
	}
	catch (std::runtime_error &err)
	{
		std::cout << err.what() << std::endl;
	}
}

void showHelp()
{
	std::cout << "options:\n    ";
	std::cout << "-e <original image> <modified image> [optional input file]";
	std::cout << "\n    ";
	std::cout << "-d <modified image> [optional output file]";
	std::cout << "\n    ";
	std::cout << "-h show this help\n";
	exit(0);
}