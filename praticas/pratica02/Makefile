CC = gcc
CFLAGS = -Wall -Wextra -std=c99
OBJ = main.o grafo_lista.o busca_largura.o busca_profundidade.o

main: $(OBJ)
	$(CC) $(CFLAGS) -o main $(OBJ)

%.o: %.c
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f *.o main main.exe
