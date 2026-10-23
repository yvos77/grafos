CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -g
TARGET = main
OBJS = main.o dijkstra.o

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

main.o: main.c dijkstra.h
	$(CC) $(CFLAGS) -c main.c

dijkstra.o: dijkstra.c dijkstra.h
	$(CC) $(CFLAGS) -c dijkstra.c

run: $(TARGET)
	./$(TARGET)

png: run
	dot -Tpng arvore.dot -o arvore.png
	dot -Tpng caminho_0_3.dot -o caminho_0_3.png

clean:
	rm -f $(OBJS) $(TARGET) $(TARGET).exe *.dot *.png

.PHONY: all run png clean