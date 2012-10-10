#!/bin/sh
rm -f *.o WeDoPlugin
gcc -fPIC -Wall -c -O2 *.c
gcc -shared *.o -o WeDoPlugin
rm -f *.o

