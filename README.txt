Name   : steg
Purpose: This program enables a user to hide ASCII text inside of a BMP image. 
Author : Wesley Kelly (c) 2015, Wesley Kelly and Cedarville University, all rights reserved
Created: 23 November 2015

To compile on Unix-like OS's: 'make'
- The makefile makes an executable called "steg"

To clean the directory of object files and executable: 'make clean'

Requires: Steg.h, Steg.cpp, main.cpp
- To get the source code: https://github.com/Xerxes004/steganographizer.git

NOTE: input only works with ASCII (8 bit) text.
NOTE: encryption only works with BMP files that have 8-bit color values. Type 2 BMP's work the best.

Options:

Encrypt the given image with a message from the command line, or from the optional input file.
-e <original image> <modified image> [optional input file]
	
Decrypt the given image. The message is written to the console, or stored in the optional output file.
-d <modified image> [optional output file]

Analyze the given bmp file
-a <image.bmp>

Scrub the given image of all hidden info! This data is not recoverable unless you have a backup.
-s <image.bmp>

-h show this help
