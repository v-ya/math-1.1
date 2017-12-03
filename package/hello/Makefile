hello.so: ../package.o hello.o root.o
	${CC} ${lflags} $^ -o $@

root.o: root.sv
root.c: root.sv ../../tool/sv-build
	../../tool/sv-build root.sv $@

clear:
	rm -f *.o root.c

