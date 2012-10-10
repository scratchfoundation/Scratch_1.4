all: clean build

build:	
	gcc $(CFLAGS) -fPIC -c `pkg-config --cflags pangocairo` *.c 
	gcc -shared `pkg-config --libs pangocairo` -lc *.o -o ../../../Plugins/so.UnicodePlugin

clean:
	rm -f *.o ../../../Plugins/so.UnicodePlugin
