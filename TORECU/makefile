CC=gcc
OBJ = $(shell for i in *.c ; do echo $${i%.c}.o ; done )

static: $(OBJ)

%.o: %.c
	$(CC) -c $<

.PHONY: clean

clean:
	rm -f *.o
