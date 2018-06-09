# if in Android and need PIE, then you can use 'CC = gcc -pie -fPIE'
export	CC		= gcc #-pie -fPIE
export	MAKEFLAGS	= --no-print-directory
obj	= main.o string.o var.o error.o math.o sbuf.o file.o BaseFunction.o package.o func.o exfun.o
func	= func/function.o func/key.o func/con.o func/math.o func/string.o func/try.o func/debug.o

all: math package/package

math: ${obj} ${func}
	${CC} ${obj} ${func} -lm -ldl -o $@

func.c: func.sv tool/sv-build
	./tool/sv-build func.sv $@

tool/sv-build: tool/sv-build.c
	cd tool && make && cd ..

package/package: package/package.c package/package.h package/*/*.c package/*/*.h package/*/*.sv
	cd package && make && cd ..

run: all
	./math test.math
clear:
	rm -f math *.o func.c func/*.o output/*
	cd tool && make clear && cd ..
	cd package && make clear && cd ..

