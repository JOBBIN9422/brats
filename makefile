CC=g++
CFLAGS=-Wall -std=c++14 -g
LDFLAGS=-larmadillo -lSDL2

SOURCES=Robot.cpp main.cpp World.cpp
OBJECTS=$(SOURCES:.cpp=.o)
DEPS=Robot.h World.h
TARGET=sim

$(TARGET) : $(OBJECTS) 
	$(CC) $(LDFLAGS) -o $@ $^

$(OBJECTS): $(SOURCES) $(DEPS)
	$(CC) $(CFLAGS) -c $(SOURCES)

.PHONY: clean

clean:
	@rm *.o $(TARGET)
