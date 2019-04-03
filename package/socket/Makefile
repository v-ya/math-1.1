socket.so: ../package.o socket.o root.stv.o socket.code.o socket.func.o
	${CC} ${lflags} $^ -o $@

root.stv.c: root.sv ../../tool/sv-build
	../../tool/sv-build root.sv $@

clear:
	rm -f *.o root.stv.c

