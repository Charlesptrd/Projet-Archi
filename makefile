all:
	gcc -o main.exe main.c scr/*.c -Iinclude -W &&  ./main.exe assembleur.tx

leaks:
	gcc -o main.exe main.c scr/*.c -Iinclude -W && leaks -atExit --list -- ./main.exe assembleur.tx

overflow:
	gcc -o main.exe main.c scr/*.c -Iinclude -W -fsanitize=address &&  ./main.exe assembleur.tx