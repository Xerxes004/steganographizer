#include <iostream>
#include <vector>

#include "Steganographizer.h"

void showHelp();

int main(int argc, char* argv[])
{
	if (argc < 2)
	{
		showHelp();
	}

	// modernize the arguments because passing arrays is a bear
	std::vector<std::string> args;

	for (int i = 0; i < argc; i++)
	{
		args.push_back(std::string(argv[i]));
	}

	std::string originalImageName = "";
	std::string modifiedImageName = "";
	std::string ioFileName = "";

	Steganographizer steg;// = new Steganographizer();

	for (int i = 0; i < args.size(); i++)
	{
		if (args.at(i).compare("-e") == 0)
		{
			if ((i + 2) < args.size())
			{
				originalImageName = args.at(i + 1);
				modifiedImageName = args.at(i + 2);
			}
			else
			{
				std::cout << "error: not enough arguments\n";
				showHelp();
			}

			try
			{
				if ((i + 3) < args.size())
				{
					ioFileName = args.at(i + 3);
					steg.ensteginate(originalImageName, modifiedImageName, ioFileName);
				}
				else
				{
					steg.ensteginate(originalImageName, modifiedImageName);
				}
			}
			catch (std::runtime_error &err)
			{
				std::cout << err.what() << std::endl;
				showHelp();
			}
		}
		if (args.at(i).compare("-d") == 0)
		{
			if ((i + 1) < args.size())
			{
				modifiedImageName = args.at(i + 1);
			}
			else
			{
				std::cout << "error: not enough arguments\n";
				showHelp();
			}

			try
			{
				if ((i + 2) < args.size())
				{
					ioFileName = args.at(i + 2);
					steg.decrypt(modifiedImageName, ioFileName);
				}
				else
				{
					steg.decrypt(modifiedImageName);
				}
			}
			catch (std::runtime_error &err)
			{
				std::cout << err.what() << std::endl;
				showHelp();
			}
		}
		if (args.at(i).compare("-h") == 0)
		{
			showHelp();
		}
	}

	return 0;
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