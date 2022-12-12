xmastree:
	gcc -Wall -o xmastree xmas.c -lm
all:
	xmastree
clean:
	rm xmastree
