Poole: poole.o tools.o socket.o
	gcc -o Poole poole.o tools.o socket.o -Wall -Wextra -lpthread
poole.o: poole.c poole.h ../tools.h
	gcc -c poole.c -Wall -Wextra -lpthread
tools.o: ../tools.c ../tools.h poole.h
	gcc -c ../tools.c -Wall -Wextra -lpthread
socket.o: socket.c socket.h poole.h ../tools.h
	gcc -c socket.c -Wall -Wextra -lpthread -ggdb