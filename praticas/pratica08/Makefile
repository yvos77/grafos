CC = gcc
CFLAGS = -Wall -Wextra -std=c99
OBJ = main.o agm.o

main: $(OBJ)
	$(CC) $(CFLAGS) -o main $(OBJ)

%.o: %.c
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f *.o main main.exe
