all: clean build

build:	
	gcc $(CFLAGS) -fPIC -c *.c
	gcc -shared *.o -o ../../../Plugins/so.CameraPlugin

clean:
	rm -f *.o ../../../Plugins/so.CameraPlugin
