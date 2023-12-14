Discovery: discovery.o tools.o socket.o
	gcc -o Discovery discovery.o tools.o socket.o -Wall -Wextra -lpthread -ggdb
discovery.o: discovery.c discovery.h tools.h socket.h
	gcc -c discovery.c -Wall -Wextra -lpthread -ggdb
tools.o: tools.c tools.h discovery.h socket.h
	gcc -c tools.c -Wall -Wextra -lpthread -ggdb
socket.o: socket.c socket.h discovery.h tools.h
	gcc -c socket.c -Wall -Wextra -lpthread -ggdb