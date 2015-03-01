# KnapSack Makefile
#
# Usage :
#  make		 : Build the program and tests
#  make tests    : Only build the tests
#  make knapsack : Only build the program
#  make clean	 : Remove all object files
#  make clobber	 : Remove all object files and programs (tests and program)
#  make install  : Install knapsack
#

# Configuration
## Compiler
CC=clang
ifdef DEBUG
CFLAGS=-g -DDEBUG -Wall -std=gnu99
else
CFLAGS=-O3 -msse2 -msse3 -Wall -std=gnu99
endif
LDFLAGS=-lpthread

## Directories
DOBJ=obj
DSRC=src
DTEST=test
DTPL=tpl

TARGET=n-dames

## Template maker
TPL=./ctpl.pl

#Targets
## Public
all: tests program

tests: bf.test bf_dyn.test

program: $(TARGET)

$(TARGET): $(DOBJ)/main.o $(DOBJ)/bf_dyn.o $(DOBJ)/chessboard.o
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

clean:
	rm -f $(DOBJ)/*.o

clobber: clean
	rm -f $(TARGET)
	rm -f test/*.test
## Tests

bf.test: $(DSRC)/bf.c
	$(CC) $(CFLAGS) -DTEST $^ -o $(DTEST)/$@ $(LDFLAGS)

bf_dyn.test: $(DSRC)/bf_dyn.c
	$(CC) $(CFLAGS) -DTEST $^ -o $(DTEST)/$@ $(LDFLAGS)

## Templates

$(DOBJ)/%.o: $(DSRC)/%.c
	$(CC) $(CFLAGS) -c $^ -o $@

$(DSRC)/%.h: $(DTPL)/%.ctpl
	$(TPL) $^ > $@

