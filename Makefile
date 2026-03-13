all: main.c
	gcc -o selinux-roulette main.c
clean:
	rm selinux-roulette
