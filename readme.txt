README.txt : Steganographizer (C++11)

Purpose: This program enables a user to hide ASCII text inside of a BMP image.
Author : Wesley Kelly (c) 2015, Wesley Kelly and Cedarville University, all
         rights reserved
Created: 23 November 2015

To get and compile:
    [adjust compiler and flags in makefile according to your needs]
$ git clone https://github.com/Xerxes004/steganographizer.git
$ cd ./steganographizer/
$ make

To run:
$ ./steg <option> <args>

    Note: test_input.bmp and romeo_and_juliet.txt are given as example files to
          play with.

options: -e -d -a -s

    Encrypt:
    -e <original image> <modified image> [optional input file]
        Encrypt the given image with a message from the command line, or from
        the optional input file. Note that input only works with ASCII (8 bit)
        text, and with BMP files that have 8-bit color values.

    Decrypt:
    -d <modified image> [optional output file]
        Decrypt the given image. The message is written to the console, or
        stored in the optional output file.

    Analyze:
    -a <image.bmp>
        Analyze the given bmp file

    Scrub:
    -s <image.bmp>
        Scrub the given image of all hidden info! This data is not recoverable
        unless you have a backup.

    Help:
    -h
        Show this help

To clean the directory of object files and executable:
$ make clean