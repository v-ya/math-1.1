obj	= sv-build.o ../var.o ../string.o

all: sv-build

sv-build: ${obj}
	${CC} ${obj} -lm -ldl -o $@

clear:
	rm -f sv-build *.o

