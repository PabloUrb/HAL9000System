Bowman: bowman.o tools.o mainFunctions.o
	gcc -o Bowman bowman.o tools.o mainFunctions.o -Wall -Wextra
bowman.o: bowman.c bowman.h tools.h mainFunctions.h
	gcc -c bowman.c -Wall -Wextra
tools.o: tools.c tools.h bowman.h mainFunctions.h
	gcc -c tools.c -Wall -Wextra
mainFunctions.o: mainFunctions.c mainFunctions.h bowman.h tools.h
	gcc -c mainFunctions.c -Wall -Wextra