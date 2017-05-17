CXX = g++
FLAGS = -W -Wall -Wextra -Wshadow -Weffc++ -pedantic -ansi -O3 -std=c++11
LIBS = -lpthread -lwebsockets

all:
	${CXX} main.cpp ${FLAGS} ${LIBS} -o main

assembly:
	@ nasm -f elf64 main.asm
	@ gcc main.o -o main