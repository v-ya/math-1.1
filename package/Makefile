export	CFLAGS	= -fPIC
export	lflags	= -shared -fPIC -fvisibility=hidden

package: package.o model/hello.so model/socket.so model/sdl.so model/gspace.so
	echo >package

model/hello.so: hello/*
	cd hello && make && cd ..
	mv hello/hello.so model/

model/socket.so: socket/*
	cd socket && make && cd ..
	mv socket/socket.so model/

model/sdl.so: sdl/*
	cd sdl && make && cd ..
	mv sdl/sdl.so model/

model/gspace.so: gspace/*
	cd gspace && make && cd ..
	mv gspace/gspace.so model/

clear:
	rm -f package package.o model/*
	cd hello && make clear && cd ..
	cd socket && make clear && cd ..
	cd sdl && make clear && cd ..
	cd gspace && make clear && cd ..

