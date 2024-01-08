Poole: poole.o tools.o thread.o socket.o
	gcc -o Poole poole.o tools.o thread.o socket.o -Wall -Wextra -lpthread -ggdb
poole.o: poole.c poole.h ../tools.h thread.h socket.h
	gcc -c poole.c -Wall -Wextra -lpthread -ggdb
tools.o: ../tools.c ../tools.h poole.h socket.h thread.h
	gcc -c ../tools.c -Wall -Wextra -lpthread -ggdb
thread.o: thread.c thread.h poole.h socket.h ../tools.h
	gcc -c thread.c -Wall -Wextra -lpthread -ggdb
socket.o: socket.c socket.h poole.h ../tools.h thread.h
	gcc -c socket.c -Wall -Wextra -lpthread -ggdb