tool: sv-build init-makec c-text
	echo >$@

sv-build: sv-build.o ../var.o ../string.o
	${CC} $^ -lm -ldl -o $@

init-makec: init-makec.o ../string.o
	${CC} $^ -lm -ldl -o $@

c-text: c-text.o ../string.o
	${CC} $^ -lm -ldl -o $@

clear:
	rm -f tool sv-build init-makec c-text *.o

