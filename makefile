CFLAGS = -g -Wall -lglfw3 -ldl -lGL -lX11 -lpthread -lXrandr -lXi 

INCLUDE_PATHS = "include/"
LIBRARY_PATHS = "libraries/"


all: build run
build: main.cpp
	g++ main.cpp glad.c window.cpp $(CFLAGS) -I $(INCLUDE_PATHS) -L $(LIBRARY_PATHS) -o main.out 

run: main.out
	./main.out


.PHONY: all, run