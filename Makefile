miniaudio.o: miniaudio/miniaudio.c miniaudio/miniaudio.h
	gcc -c miniaudio/miniaudio.c  -lpthread -lm

keymap.h: generate_keymap.c
	gcc -o km_gen.out generate_keymap.c
	./km_gen.out
	rm km_gen.out

ari: miniaudio.o keymap.h main.c
	gcc -o ari miniaudio.o main.c  -lX11 -lXtst -lpthread -lm

.PHONY:ari gen_keymap
