gspace.so: ../package.o gspace.o root.o gspace.code.o gspace.func.o
	${CC} ${lflags} $^ -o $@ -lGL -lGLEW -lglut

root.o: root.c
root.c: root.sv ../../tool/sv-build
	../../tool/sv-build root.sv $@

gspace.code.o: gspace.code.c code.*.c
gspace.func.o: gspace.func.c func.*.c

clear:
	rm -f *.o root.c

