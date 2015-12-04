<h1>Steganographizer</h1>

Purpose: This program enables a user to hide ASCII text inside of a BMP image. 
Author : Wesley Kelly (c) 2015, Wesley Kelly and Cedarville University, all rights reserved
Created: 23 November 2015

To get and compile:

<code>
	$ git clone https://github.com/Xerxes004/steganographizer.git
	$ cd ./steganographizer/
	$ make
</code>

To run:
Note: test_input.bmp and romeo_and_juliet.txt are given as example files to play with

<code>$ ./steg <option> <args></code>

options:

<code>-e <original image> <modified image> [optional input file]</code>
Encrypt the given image with a message from the command line, or from the optional input file.
NOTE: input only works with ASCII (8 bit) text.
NOTE: encryption only works with BMP files that have 8-bit color values. Type 2 BMP's work the best.

<code>-d <modified image> [optional output file]</code>
Decrypt the given image. The message is written to the console, or stored in the optional output file.

<code>-a <image.bmp></code>
Analyze the given bmp file

<code>-s <image.bmp></code>
Scrub the given image of all hidden info! This data is not recoverable unless you have a backup.

<code>-h show this help</code>

To clean the directory of object files and executable: 
<code>$ make clean</code>