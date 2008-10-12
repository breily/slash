# Brian Reily
# bjr5e
#
# Makefile for slash

CXX=gcc
FLAGS=-lreadline -Wall
OUTFILE=slash

OFILES=bg.o parser.o error.o init.o input.o options.o eval.o help.o aliases.o environ.o redir.o globbing.o builtin.o prompt.o slash.o

.SUFFIXES: .o .c

slash:	$(OFILES)
	$(CXX) $(FLAGS) $(OFILES) -o $(OUTFILE)

clean:
	rm -f *.o

bg.o: bg.c bg.h
parser.o: parser.c parser.h
error.o: error.c error.h
init.o: init.c init.h
input.o: input.c input.h
options.o: options.c options.h
eval.o: eval.c eval.h
help.o: help.c help.h
aliases.o: aliases.c aliases.h
environ.o: environ.c environ.h
redir.o: redir.c redir.h
globbing.o: globbing.c globbing.h
builtin.o: builtin.c builtin.h
prompt.o: prompt.c prompt.h

slash.o: slash.c prompt.c prompt.h builtin.c builtin.h globbing.c globbing.o redir.c redir.h environ.c environ.h aliases.c aliases.h help.c help.h eval.c eval.h options.c options.h input.c input.h init.c init.h error.c error.h parser.c parser.h bg.c bg.h config.h
