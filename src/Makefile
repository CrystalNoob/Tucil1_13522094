CC = g++
CFLAGS = -Wall

SRC_MAIN = main.cpp
OBJ_MAIN = $(SRC_MAIN:.cpp=.o)

.PHONY: all clean test

all: clean main run

main: $(OBJ_MAIN) $(OBJ_FOO)
	$(CC) $(CFLAGS) -o $@ $^

run:
	main

%.o: %.cpp
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f main $(OBJ_MAIN)