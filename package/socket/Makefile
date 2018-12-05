socket.so: ../package.o socket.o root.o socket.code.o socket.func.o
	${CC} ${lflags} $^ -o $@

root.o: root.c
root.c: root.sv ../../tool/sv-build
	../../tool/sv-build root.sv $@

clear:
	rm -f *.o root.c

