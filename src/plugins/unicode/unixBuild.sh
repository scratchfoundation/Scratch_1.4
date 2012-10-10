#!/bin/sh
rm -f *.o UnicodePlugin
gcc -fPIC -Wall -c `pkg-config --cflags pangocairo` *.c
gcc -shared `pkg-config --libs pangocairo` -lc *.o -o UnicodePlugin
rm -f *.o
