all:
	gcc -o simulateur main.c scr/*.c -Iinclude -W &&  ./simulateur assembleur.tx

leaks:
	gcc -o simulateur main.c scr/*.c -Iinclude -W && leaks -atExit --list -- ./simulateur assembleur.tx

overflow:
	gcc -o simulateur main.c scr/*.c -Iinclude -W -fsanitize=address &&  ./simulateur assembleur.tx