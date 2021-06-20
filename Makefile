read: read_flp.c
	gcc -Wall -o read read_flp.c

write: write_flp.c
	gcc -Wall -o write write_flp.c

all: read write
