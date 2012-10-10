#!/bin/sh
# Fedora Linux script; resulting plugin works on the XO
rm -f *.o ScratchPlugin
gcc -fPIC -Wall -c *.c
gcc -shared *.o -o ScratchPlugin
rm -f *.o
