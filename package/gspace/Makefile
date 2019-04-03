gspace.so: ../package.o gspace.o root.stv.o gspace.code.o gspace.func.o
	${CC} ${lflags} $^ -o $@ -lGL -lGLEW -lglut

root.stv.c: root.sv ../../tool/sv-build
	../../tool/sv-build root.sv $@

gspace.code.o: gspace.code.c code.*.c
gspace.func.o: gspace.func.c func.*.c

clear:
	rm -f *.o root.stv.c

