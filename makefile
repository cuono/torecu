CC=gcc

#all: vilde
#	make -C TORECU/ -f makefile

static:
	make -C TORECU/ -f makefile
	make d4

d4: main.o tokenizer.o retokenizer.o cutenizer.o miscellaneous.o token.o TORECU/*.o
	$(CC) -lm -o $@ $^ &> cc.txt
#	$(CC) -lm -o $@ $^ TORECU/*.o &> cc.txt

%.o: %.c
	$(CC) -c $<

.PHONY: clean

clean:
	rm -f *.o d4
	make -C TORECU/ clean
