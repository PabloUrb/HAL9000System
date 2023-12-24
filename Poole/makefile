Poole: poole.o tools.o
	gcc -o Poole poole.o tools.o -Wall -Wextra -lpthread
poole.o: poole.c poole.h ../tools.h
	gcc -c poole.c -Wall -Wextra -lpthread
tools.o: ../tools.c ../tools.h poole.h
	gcc -c ../tools.c -Wall -Wextra -lpthread