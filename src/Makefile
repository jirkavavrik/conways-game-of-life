# Project: Conway's game of life simulator by Jiří Vavřík
# Autor:   Jiří Vavřík
# Date:   4. 11. 2018

CC=g++
FLAGS=`sdl2-config --cflags --libs`
EXE=conway

main: conway.cpp
	$(CC) conway.cpp $(FLAGS) -o $(EXE)

run:
	./$(EXE)
