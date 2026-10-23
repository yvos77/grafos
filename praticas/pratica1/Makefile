CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -g
TARGET = main
OBJS = main.o grafo_matriz.o grafo_lista.o

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

main.o: main.c grafo_matriz.h grafo_lista.h
	$(CC) $(CFLAGS) -c main.c

grafo_matriz.o: grafo_matriz.c grafo_matriz.h
	$(CC) $(CFLAGS) -c grafo_matriz.c

grafo_lista.o: grafo_lista.c grafo_lista.h
	$(CC) $(CFLAGS) -c grafo_lista.c

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(OBJS) $(TARGET) $(TARGET).exe

.PHONY: all run clean