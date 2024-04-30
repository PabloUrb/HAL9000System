Bowman: bowman.o tools.o mainFunctions.o socket.o thread.o
	gcc -o Bowman bowman.o tools.o mainFunctions.o socket.o thread.o -Wall -Wextra -lpthread -ggdb
bowman.o: bowman.c bowman.h ../tools.h mainFunctions.h socket.h
	gcc -c bowman.c -Wall -Wextra -lpthread -ggdb
mainFunctions.o: mainFunctions.c mainFunctions.h bowman.h ../tools.h socket.h
	gcc -c mainFunctions.c -Wall -Wextra -lpthread -ggdb
socket.o: socket.c socket.h bowman.h ../tools.h mainFunctions.h
	gcc -c socket.c -Wall -Wextra -lpthread -ggdb
thread.o: thread.c thread.h bowman.h socket.h ../tools.h mainFunctions.h
	gcc -c thread.c -Wall -Wextra -lpthread -ggdb
tools.o: ../tools.c ../tools.h bowman.h mainFunctions.h socket.h
	gcc -c ../tools.c -Wall -Wextra -lpthread -ggdb