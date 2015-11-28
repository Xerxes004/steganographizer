#include <iostream>
#include <iomanip>
#include <vector>
#include <chrono>

#include "Steganographizer.h"

void showHelpAndExitWithCode(const int code);
void encrypt(const std::vector<std::string> &args, const int index);
void decrypt(const std::vector<std::string> &args, const int index);
void analyze(const std::vector<std::string> &args, const int index);

int main(int argc, char* argv[])
{
	if (argc < 2)
	{
		showHelpAndExitWithCode(1);
	}

	// modernize the arguments, because passing arrays is a nightmare
	std::vector<std::string> args(argv, argv + argc);

	try 
	{
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
			if (args.at(i).compare("-a") == 0)
			{
				analyze(args, i);
				break;
			} 
			if (i == args.size() - 1)
			{
				showHelpAndExitWithCode(1);
			}
		}
	}
	catch (std::runtime_error &ex)
	{
		std::cout << ex.what() << std::endl;
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
		showHelpAndExitWithCode(1);
	}

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
		showHelpAndExitWithCode(1);
	}

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

void analyze(const std::vector<std::string> &args, const int index)
{
	std::string imageName = "";

	Steganographizer steg;

	if ((index + 1) < args.size())
	{
		imageName = args.at(index + 1);

		if (imageName.substr(imageName.size() - 4, 4).compare(".bmp") != 0)
		{
			imageName += ".bmp";
		}
	}
	else
	{
		std::cout << "error: not enough arguments\n";
		showHelpAndExitWithCode(1);
	}

	steg.analyze(imageName);
}

void showHelpAndExitWithCode(const int code)
{
	std::cout << "options:\n    ";
	std::cout << "-e <original image> <modified image> [optional input file]";
	std::cout << "\n    ";
	std::cout << "-d <modified image> [optional output file]";
	std::cout << "\n    ";
	std::cout << "-h show this help\n";
	exit(code);
}