<h1>Steganographizer</h1>
<p>
Purpose: This program enables a user to hide ASCII text inside of a BMP image. 
Author : Wesley Kelly (c) 2015, Wesley Kelly and Cedarville University, all rights reserved
Created: 23 November 2015
</p>
<p>
To get and compile:

    $ git clone https://github.com/Xerxes004/steganographizer.git
    $ cd ./steganographizer/
    $ make
</p>
<p>
To run:
Note: test_input.bmp and romeo_and_juliet.txt are given as example files to play with

    $ ./steg &lt;option&gt; &lt;args&gt;

options:

Encrypt the given image with a message from the command line, or from the optional input file.
NOTE: input only works with ASCII (8 bit) text.
NOTE: encryption only works with BMP files that have 8-bit color values. Type 2 BMP's work the best.
    -e &lt;original image&gt; &lt;modified image&gt; [optional input file]

Decrypt the given image. The message is written to the console, or stored in the optional output file.
    -d &lt;modified image&gt; [optional output file]

Analyze the given bmp file
    -a &lt;image.bmp&gt;

Scrub the given image of all hidden info! This data is not recoverable unless you have a backup.
    -s &lt;image.bmp&gt;

    -h show this help
</p>
<p>
To clean the directory of object files and executable: 
    $ make clean
</p>