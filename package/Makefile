export	CFLAGS	= -fPIC
export	lflags	= -shared -fPIC -fvisibility=hidden

package: package.o model/hello.so
	echo >package

model/hello.so: hello/*
	cd hello && make && cd ..
	mv hello/hello.so model/

clear:
	rm -f package package.o model/*
	cd hello && make clear && cd ..
	cd wav && make clear && cd ..

