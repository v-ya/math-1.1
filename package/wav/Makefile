wav.so: ../package.o wav.o wav.code.o wav.func.o wav.string.o root.o
	${CC} ${lflags} -lm $^ -o $@

root.o: root.sv
root.c: root.sv ../../tool/sv-build
	../../tool/sv-build root.sv $@

clear:
	rm -f *.o root.c

