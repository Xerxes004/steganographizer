<h1>Steganographizer</h1>
<p>
Purpose: This program enables a user to hide ASCII text inside of a BMP image. 
Author : Wesley Kelly (c) 2015, Wesley Kelly and Cedarville University, all rights reserved
Created: 23 November 2015

To get and compile:
</p>
<code>
	<p>$ git clone https://github.com/Xerxes004/steganographizer.git</p>
	<p>$ cd ./steganographizer/</p>
	<p>$ make</p>
</code>

To run:
Note: test_input.bmp and romeo_and_juliet.txt are given as example files to play with

<code><p>$ ./steg <option> <args></p></code>

options:

<code><p>-e <original image> <modified image> [optional input file]</p></code>
Encrypt the given image with a message from the command line, or from the optional input file.
NOTE: input only works with ASCII (8 bit) text.
NOTE: encryption only works with BMP files that have 8-bit color values. Type 2 BMP's work the best.

<code><p>-d <modified image> [optional output file]</p></code>
Decrypt the given image. The message is written to the console, or stored in the optional output file.

<code><p>-a <image.bmp></p></code>
Analyze the given bmp file

<code><p>-s <image.bmp></p></code>
Scrub the given image of all hidden info! This data is not recoverable unless you have a backup.

<code><p>-h show this help</p></code>

To clean the directory of object files and executable: 
<code><p>$ make clean</p></code>