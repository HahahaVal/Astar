.PHONY: all clean main

LUA_PATH ?= ./lua-5.4.2
INCLUDE_PATH ?= -I$(LUA_PATH)

all:main

main:
	g++ -g Astar.cpp main.cpp -o main

clean:
	-rm -rf *.o *.so