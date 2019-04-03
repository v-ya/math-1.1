# if in Android and need PIE, then you can use 'CC = gcc -pie -fPIE'
export	CC		= gcc #-pie -fPIE
export	LD		= ld
export	MAKEFLAGS	= --no-print-directory
obj	= main.o string.o var.o error.o math.o sbuf.o file.o BaseFunction.o package.o func.stv.o thread.o init.stv.o exfun.o
func	= func/function.o func/key.o func/con.o func/math.o func/string.o func/transform.o func/index.o func/time.o func/file.o func/try.o func/thread.o func/init.o func/debug.o

all: math package/package

math: ${obj} ${func} args/args.o
	${CC} ${obj} ${func} args/args.o -lm -ldl -lpthread -o $@

error.o: error.c error.h error.string

args/args.o: args/*.c args/*.h args/*/*
	cd args && make && cd ..

func.stv.c: func.sv limit.h tool/tool
	./tool/sv-build $< $@

init.st.c: init.math tool/tool
	./tool/init-makec $< $@ _init_text --compress

init.stv.c: init.sv tool/tool init.st.c
	./tool/sv-build $< $@

tool/tool: tool/*.c
	cd tool && make && cd ..

package/package: package/package.c package/package.h package/*/*.c package/*/*.h package/*/*.sv
	cd package && make && cd ..

run: all
	./math test.math
clear:
	rm -f math *.o func.stv.c init.stv.c init.st.c func/*.o output/*
	rm -Rf math-root/
	cd args && make clear && cd ..
	cd tool && make clear && cd ..
	cd package && make clear && cd ..

