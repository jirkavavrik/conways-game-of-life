# Project: Conway's game of life simulator by Jiří Vavřík
# Autor:   Jiří Vavřík
# Datum:   31. 1. 2016

CC=g++
FLAGS=`sdl2-config --cflags --libs`
EXE=conway

main: conway.cpp
	$(CC) conway.cpp $(FLAGS) -o $(EXE)
	./$(EXE)

