all: clean build

build:	
	gcc $(CFLAGS) -fPIC -Wall -c -O2 *.c
	gcc -shared *.o -o ../../../Plugins/so.WeDoPlugin

clean:
	rm -f *.o ../../../Plugins/so.WeDoPlugin
