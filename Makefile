all: main.c
	gcc -o selinux-roulette -l selinux main.c
clean:
	rm selinux-roulette
