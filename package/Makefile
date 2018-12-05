export	CFLAGS	= -fPIC
export	lflags	= -shared -fPIC -fvisibility=hidden

package: package.o model/hello.so model/socket.so
	echo >package

model/hello.so: hello/*
	cd hello && make && cd ..
	mv hello/hello.so model/

model/socket.so: socket/*
	cd socket && make && cd ..
	mv socket/socket.so model/

clear:
	rm -f package package.o model/*
	cd hello && make clear && cd ..
	cd socket && make clear && cd ..

