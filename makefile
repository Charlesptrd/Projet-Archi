all:
	gcc -o simulateur main.c scr/*.c -Iinclude -W &&  ./simulateur assembleur.txt

leaks:
	gcc -o simulateur main.c scr/*.c -Iinclude -W && leaks -atExit --list -- ./simulateur assembleur.txt

overflow:
	gcc -o simulateur main.c scr/*.c -Iinclude -W -fsanitize=address &&  ./simulateur assembleur.txt