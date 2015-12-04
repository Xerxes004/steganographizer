<h1>Steganographizer</h1>
<p><i><strong>Note: readme.txt is human-legible as a text file, whereas README.md is not!</strong></i><p>
<p>Purpose: This program enables a user to hide ASCII text inside of a BMP image.<br>
Author : Wesley Kelly (c) 2015, Wesley Kelly and Cedarville University, all rights reserved<br>
Created: 23 November 2015</p>
<h3>To get and compile:</h3>
<pre><code>$ git clone https://github.com/Xerxes004/steganographizer.git
$ cd ./steganographizer/
$ make</code></pre>
<h3>To run:</h3>
<p>Note: test_input.bmp and romeo_and_juliet.txt are given as example files to play with</p>
<pre><code>$ ./steg &lt;option&gt; &lt;args&gt;</code></pre>
<h3>Options:</h3>
<p>Encrypt the given image with a message from the command line, or from the optional input file.<br>
NOTE: input only works with ASCII (8 bit) text.<br>
NOTE: encryption only works with BMP files that have 8-bit color values. Type 2 BMP's work the best.</p>
<pre><code>-e &lt;original image&gt; &lt;modified image&gt; [optional input file]</code></pre>
<p>Decrypt the given image. The message is written to the console, or stored in the optional output file.</p>
<pre><code>-d &lt;modified image&gt; [optional output file]</code></pre>
<p>Analyze the given bmp file</p>
<pre><code>-a &lt;image.bmp&gt;</code></pre>
<p>Scrub the given image of all hidden info! This data is not recoverable unless you have a backup.</p>
<pre><code>-s &lt;image.bmp&gt;</code></pre>
<p>Show this help</p>
<pre><code>-h</code></pre>
<h3>To clean the directory of object files and executable: </h3>
<pre><code>$ make clean</code></pre>