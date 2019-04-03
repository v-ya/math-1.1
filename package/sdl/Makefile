sdl.so: ../package.o sdl.o root.stv.o sdl.code.o sdl.func.o
	${CC} ${lflags} $^ -o $@ -lSDL2

root.stv.c: root.sv ../../tool/sv-build
	../../tool/sv-build root.sv $@

clear:
	rm -f *.o root.stv.c

