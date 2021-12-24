.PHONY: all clean main

LUA_PATH ?= ./lua-5.4.2
INCLUDE_PATH ?= -I$(LUA_PATH)
CFLAGS ?= -g -O2 -Wall -fPIC -shared -std=c++11

all:main

main:
	g++ $(CFLAGS) $(INCLUDE_PATH) MapUtil.cpp lMapUtil.cpp -o Astar.so

clean:
	-rm -rf *.o *.so