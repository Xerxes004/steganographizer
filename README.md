<h1>Steganographizer</h1>
<p>
Purpose: This program enables a user to hide ASCII text inside of a BMP image. 
Author : Wesley Kelly (c) 2015, Wesley Kelly and Cedarville University, all rights reserved
Created: 23 November 2015
</p>
<p>
To get and compile:
</p>
<p>
<code>$ git clone https://github.com/Xerxes004/steganographizer.git
$ cd ./steganographizer/
$ make</code>
</p>
<p>
To run:
Note: test_input.bmp and romeo_and_juliet.txt are given as example files to play with

<p><code>$ ./steg &lt;option&gt; &lt;args&gt;</code></p>

options:

Encrypt the given image with a message from the command line, or from the optional input file.
NOTE: input only works with ASCII (8 bit) text.
NOTE: encryption only works with BMP files that have 8-bit color values. Type 2 BMP's work the best.
<p><code>-e &lt;original image&gt; &lt;modified image&gt; [optional input file]</code></p>

Decrypt the given image. The message is written to the console, or stored in the optional output file.
<p><code>-d &lt;modified image&gt; [optional output file]</code></p>

Analyze the given bmp file
<p><code>-a &lt;image.bmp&gt;</code></p>

Scrub the given image of all hidden info! This data is not recoverable unless you have a backup.
<p><code>-s &lt;image.bmp&gt;</code></p>

<p><code>-h show this help</code></p>
</p>
<p>
To clean the directory of object files and executable: 
    <p><code>$ make clean</code></p>
</p>