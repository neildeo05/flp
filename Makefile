CFLAGS= -I include/ -Wall -Wextra
read: read_flp.c
	gcc $(CFLAGS) -o read read_flp.c

write: write_flp.c
	gcc $(CFLAGS) -o write write_flp.c

all: read write
