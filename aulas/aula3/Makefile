CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -g
TARGET = main
OBJS = main.o grafo_lista.o busca_grafo.o

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

main.o: main.c grafo_lista.h busca_grafo.h
	$(CC) $(CFLAGS) -c main.c

grafo_lista.o: grafo_lista.c grafo_lista.h
	$(CC) $(CFLAGS) -c grafo_lista.c

busca_grafo.o: busca_grafo.c busca_grafo.h grafo_lista.h
	$(CC) $(CFLAGS) -c busca_grafo.c

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(OBJS) $(TARGET) $(TARGET).exe

.PHONY: all run clean