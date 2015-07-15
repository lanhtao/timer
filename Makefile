CC = gcc
XX = g++	
CFLAGS = -Wall -O –g
LIB = -lpthread

SRC = $(wildcard *.cpp)
TARGET = Timer
$(TARGET): $(SRC)
	$(XX) $^ $(LIB) -o $@	


clean:
	rm -rf *.o $(TARGET)
