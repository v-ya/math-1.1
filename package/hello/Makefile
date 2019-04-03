hello.so: ../package.o hello.o root.stv.o
	${CC} ${lflags} $^ -o $@

root.stv.c: root.sv ../../tool/sv-build
	../../tool/sv-build root.sv $@

clear:
	rm -f *.o root.stv.c

