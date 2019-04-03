
args.o: args.c args.h script.o help.o
	${CC} args.c -c -o args.main.o
	${LD} -r args.main.o script.o help.o -o $@

script/c-file/%.st.c: script/%.math
	../tool/c-text $^ $@ $(patsubst script/%.math, _script_%, $<) --compress

help/c-file/%.st.c: help/%.txt
	../tool/c-text $^ $@ $(patsubst help/%.txt, _help_%, $<)

script.o: script/index.sv $(patsubst script/%.math, script/c-file/%.st.c, $(wildcard script/*.math))
	../tool/sv-build $< script/index.stv.c
	${CC} -c script/index.stv.c $(patsubst %, -include %, $(wildcard script/c-file/*.st.c)) -o $@

help.o: help/index.sv $(patsubst help/%.txt, help/c-file/%.st.c, $(wildcard help/*.txt))
	../tool/sv-build $< help/index.stv.c
	${CC} -c help/index.stv.c $(patsubst %, -include %, $(wildcard help/c-file/*.st.c)) -o $@


clear:
	rm -f *.o */*.o script/index.c help/index.c script/c-file/* help/c-file/*

