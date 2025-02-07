all:
	gcc -o main.exe main.c scr/*.c -Iinclude -W &&  ./main.exe

leaks:
	gcc -o main.exe main.c scr/*.c -Iinclude -W && leaks -atExit --list -- ./main.exe

overflow:
	gcc -o main.exe main.c scr/*.c -Iinclude -W -fsanitize=address &&  ./main.exe