Discovery: discovery.o tools.o
	gcc -o Discovery discovery.o tools.o -Wall -Wextra
discovery.o: discovery.c discovery.h tools.h
	gcc -c discovery.c -Wall -Wextra
tools.o: tools.c tools.h discovery.h
	gcc -c tools.c -Wall -Wextra