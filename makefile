#
#  File name: makefile
#
#  Author: 2017 Renato Henriques e Paulo Eusebio
#
#  Usage:
#     1) To generate the executable file for the program client,
#       run
#         $ make
#
#     2) To clean all object files and the executable file of
#        client, run
#         $ make clean
#

#  Compiler
CC = gcc

#  Compiler Flags
CFLAGS = -Wall -c -O3 -pg

#  Sources
SOURCES = graph.c traversal.c heap.c utils.c main.c

#  Objects
OBJECTS = graph.o traversal.o heap.o utils.o main.o

interRouting: $(OBJECTS)
	gcc -o $@ $(OBJECTS) -pg

graph.o: graph.c graph.h
	$(CC) $(CFLAGS) $(DEBUG) graph.c

traversal.o: traversal.c traversal.h 
	$(CC) $(CFLAGS) $(DEBUG) traversal.c

heap.o: heap.c heap.h
	$(CC) $(CFLAGS) $(DEBUG) heap.c

utils.o: utils.c utils.h
	$(CC) $(CFLAGS) $(DEBUG) utils.c

main.o: main.c
	$(CC) $(CFLAGS) $(DEBUG) main.c

debug: DEBUG = -DDEBUG

debug: interRouting

clean::
	rm -f *.o core a.out interRouting *~

depend::
	makedepend $(SOURCES)
# DO NOT DELETE
