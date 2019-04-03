wav.so: ../package.o wav.o wav.code.o wav.func.o wav.string.o root.stv.o
	${CC} ${lflags} -lm $^ -o $@

root.stv.c: root.sv ../../tool/sv-build
	../../tool/sv-build root.sv $@

clear:
	rm -f *.o root.stv.c

