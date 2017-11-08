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
CFLAGS = -Wall -c -g -ftrapv

#  Sources
SOURCES = graph.c traversal.c utils.c main.c

#  Objects
OBJECTS = graph.o traversal.o utils.o main.o

interRouting: $(OBJECTS)
	gcc -o $@ $(OBJECTS)

graph.o: graph.c graph.h
	$(CC) $(CFLAGS) graph.c

traversal.o: traversal.c traversal.h
	$(CC) $(CFLAGS) traversal.c

utils.o: utils.c utils.h
	$(CC) $(CFLAGS) utils.c

main.o: main.c
	$(CC) $(CFLAGS) main.c

clean::
	rm -f *.o core a.out interRouting *~

depend::
	makedepend $(SOURCES)
# DO NOT DELETE
