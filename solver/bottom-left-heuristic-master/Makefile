# Copyright (c) 2015, Monaco F. J. <moanco@icmc.usp.br>
# Modified by Matheus G. S. Beleboni

# Default target

all: main

# Objects

OBJECTS = main.o parser.o bottom-left.o globals.o heuristic-exec.o

# Binary

main : $(OBJECTS)
	g++ -Wall -Wconversion $^ -o $@

# Pattern rule for object-source dependences

%.o : %.cpp 
	g++ -Wall -Wconversion -I. -c $<

# Automatic object-header dependences

# makefiles = $(OBJECTS:%.o=%.d) 

# include $(makefiles)

%.d : %.cpp
	g++ -Wall -Wconversion -I. -MM -MT '$(<:.cpp=.o) $@' $< >$@

# Cleaning

clean:
	rm -f *.o
	rm -f *~ \#*
	rm -f *.d
	rm -f solution.*
	rm -f *.tex
	rm -f adding_*.*
	rm -f NFP_*.*
