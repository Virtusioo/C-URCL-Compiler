
CXX := g++
FLAGS := -O2
OUTPUT := tests/ucc
SRC := $(wildcard src/*.cpp)

all:
	$(CXX) $(SRC) -o $(OUTPUT) $(FLAGS)