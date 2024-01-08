Bowman: bowman.o tools.o mainFunctions.o socket.o
	gcc -o Bowman bowman.o tools.o mainFunctions.o socket.o -Wall -Wextra -ggdb
bowman.o: bowman.c bowman.h ../tools.h mainFunctions.h socket.h
	gcc -c bowman.c -Wall -Wextra -ggdb
mainFunctions.o: mainFunctions.c mainFunctions.h bowman.h ../tools.h socket.h
	gcc -c mainFunctions.c -Wall -Wextra -ggdb
socket.o: socket.c socket.h bowman.h ../tools.h mainFunctions.h
	gcc -c socket.c -Wall -Wextra -lpthread -ggdb
tools.o: ../tools.c ../tools.h bowman.h mainFunctions.h socket.h
	gcc -c ../tools.c -Wall -Wextra -lpthread -ggdb