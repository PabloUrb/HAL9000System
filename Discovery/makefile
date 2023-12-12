Discovery: discovery.o tools.o
	gcc -o Discovery discovery.o tools.o -Wall -Wextra -lpthread
discovery.o: discovery.c discovery.h tools.h
	gcc -c discovery.c -Wall -Wextra -lpthread
tools.o: tools.c tools.h discovery.h
	gcc -c tools.c -Wall -Wextra -lpthread
socket.o: socket.c socket.h
	gcc -c socket.c -Wall -Wextra -lpthread