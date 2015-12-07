/**
 * This is the main function for the Steg class implementation.
 * 
 * @author Wesley Kelly
 * @version 1.0
 *
 * File: Steg.h 
 * Created: 15 November 2015
 *
 * Copyright 2015 Cedarville University, its Computer Science faculty, and the
 * authors. All rights reserved.
 * 
 * Description: This function parses the input arguments in order to use the
 * Steg class most effectively. There is an argument value for encrypting,
 * decrypting, analyzing, and scrubbing images.
 */

#include <vector>

#include "Steg.h"

void showHelpAndExitWithCode(const int code);

void encrypt(const std::vector<std::string> &args, const int index);
void decrypt(const std::vector<std::string> &args, const int index);
void analyze(const std::vector<std::string> &args, const int index);
void scrub  (const std::vector<std::string> &args, const int index);

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
			if (args.at(i).compare("-s") == 0)
			{
				scrub(args, i);
				break;
			}
			if (args.at(i).compare("-h") == 0)
			{
				showHelpAndExitWithCode(0);
			}
			if (i == args.size() - 1)
			{
				std::cout << "Correct args not specified.\n";
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

	Steg steg;

	if ((index + 2) < args.size())
	{
		orgImg = args.at(index + 1);
		modImg = args.at(index + 2);

		if (orgImg.substr(orgImg.size() - 4, 4).compare(".bmp") != 0)
		{
			throw std::runtime_error("Argument is not a .bmp");
		}
		else if (modImg.substr(modImg.size() - 4, 4).compare(".bmp") != 0)
		{
			throw std::runtime_error("Argument is not a .bmp");
		}
	}
	else
	{
		std::cout << "error: not enough arguments" << std::endl;
		showHelpAndExitWithCode(1);
	}

	if ((index + 3) < args.size())
	{
		steg.encrypt(orgImg, modImg, args.at(index + 3));
	}
	else
	{
		steg.encrypt(orgImg, modImg);
	}
}

void decrypt(const std::vector<std::string> &args, const int index)
{
	std::string modImg = "";
	std::string ioFileName = "";

	Steg steg;

	if ((index + 1) < args.size())
	{
		modImg = args.at(index + 1);

		if (modImg.substr(modImg.size() - 4, 4).compare(".bmp") != 0)
		{
			throw std::runtime_error("Argument is not a .bmp");
		}
	}
	else
	{
		std::cout << "error: not enough arguments" << std::endl;
		showHelpAndExitWithCode(1);
	}

	if ((index + 2) < args.size())
	{
		steg.decrypt(modImg, args.at(index + 2));
	}
	else
	{
		steg.decrypt(modImg);
	}
}

void analyze(const std::vector<std::string> &args, const int index)
{
	std::string imageName = "";

	Steg steg;

	if ((index + 1) < args.size())
	{
		imageName = args.at(index + 1);

		if (imageName.substr(imageName.size() - 4, 4).compare(".bmp") != 0)
		{
			throw std::runtime_error("Argument is not a .bmp");
		}
	}
	else
	{
		std::cout << "error: not enough arguments" << std::endl;
		showHelpAndExitWithCode(1);
	}

	steg.analyze(imageName);
}

void scrub(const std::vector<std::string> &args, const int index)
{
	std::string imageName = "";

	Steg steg;

	if ((index + 1) < args.size())
	{
		imageName = args.at(index + 1);

		if (imageName.substr(imageName.size() - 4, 4).compare(".bmp") != 0)
		{
			throw std::runtime_error("Argument is not a .bmp");
		}
	}
	else
	{
		std::cout << "error: not enough arguments" << std::endl;
		showHelpAndExitWithCode(1);
	}

	steg.scrub(imageName);
}

void showHelpAndExitWithCode(const int code)
{
	std::cout << "options:" << std::endl;

	std::cout << "  Encrypt the given image with a message from the command \n"
		      << "  line, or from the optional input file.\n"
	  		  << "  -e <original image> <modified image> [optional input file]"
			  << std::endl;

	std::cout << "\n  Decrypt the given image. The message is written to the "
			  << "\n  console, or stored in the optional output file."
			  << "\n  -d <modified image> [optional output file]\n";

	std::cout << "\n  Analyze the given bmp file\n"
		      << "  -a <image.bmp>\n\n";

	std::cout << "  Scrub the given image of all hidden info! This data is\n"
	          << "  not recoverable unless you have a backup.\n"
			  << "  -s <image.bmp>\n\n";

	std::cout << "  -h show this help\n\n";

	exit(code);
}