sdl.so: ../package.o sdl.o root.o sdl.code.o sdl.func.o
	${CC} ${lflags} $^ -o $@ -lSDL2

root.o: root.c
root.c: root.sv ../../tool/sv-build
	../../tool/sv-build root.sv $@

clear:
	rm -f *.o root.c

