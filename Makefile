#! /usr/bin/make -f
# Fichero Makefile para construir el ejecutable smallsh

CFLAGS=-g -Wall -Werror -Wno-unused
LDLIBS=-lfl # Para flex

OBJECTS=smallsh.o gettok.o procline.o cmd/directorio.o runcom.o buffer.o userfn.o shell.o cmd/bgproc.o listProcesses.o process.o listCommands.o command.o

smallsh: $(OBJECTS)

gettok.o: gettok.c

procline.o: procline.c

directorio.o: cmd/directorio.c

runcom.o: runcom.c

smallsh.o: smallsh.c

buffer.o: buffer.c

userfn.o: userfn.c

shell.c: shell.l

cmd/bgproc.o: cmd/bgproc.c

listProcesses.o: listProcesses.c

process.o: process.c

command.o: command.c

listCommands.o: listCommands.c





clean:
	rm -rf *~ $(OBJECTS) shell.c smallsh core

# $Id: Makefile 1175 2006-03-14 11:00:17Z pedroe $
