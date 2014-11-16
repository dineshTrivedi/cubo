cubo
====

frag: Chinese code
matrix_conversion: convert plain text to binary file (and other options)
matrix_sample: Plain text sample

============
Ubuntu 14.04 x64
Updating g++ to 4.9

host$ sudo add-apt-repository ppa:ubuntu-toolchain-r/test
host$ sudo apt-get update
host$ sudo apt-get install g++-4.9

Remaking g++ symbolic link

host$ cd /usr/bin
host$ sudo rm g++
host$ sudo ln -s g++-4.9 g++
