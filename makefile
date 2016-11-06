CC=g++

dgtree: main.cpp read_graphs.cpp dgtree.cpp graph.h dgtree.h query.h query.cpp
	$(CC) -g -o dgtree main.cpp dgtree.cpp read_graphs.cpp graph.h dgtree.h query.h query.cpp

