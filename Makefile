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
LDFLAGS=

## Directories
DOBJ=obj
DSRC=src
DTEST=test

TARGET=n-dames

#Targets
## Public
all: tests program

tests: test_dir bf.test bf_dyn.test chessboard.test local_search.test

program: $(TARGET)

$(TARGET): $(DOBJ)/main.o $(DOBJ)/bf_dyn.o \
	   $(DOBJ)/chessboard.o $(DOBJ)/local_search.o \
	   $(DOBJ)/wikimethod.o $(DOBJ)/backtrack.o
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

clean:
	rm -f $(DOBJ)/*.o

clobber: clean
	rm -f $(TARGET)
	rm -f test/*.test
## Tests

test_dir:
	@mkdir -p test

bf.test: $(DSRC)/bf.c
	$(CC) $(CFLAGS) -DTEST $^ -o $(DTEST)/$@ $(LDFLAGS)

bf_dyn.test: $(DSRC)/bf_dyn.c
	$(CC) $(CFLAGS) -DTEST $^ -o $(DTEST)/$@ $(LDFLAGS)

chessboard.test: $(DSRC)/chessboard.c $(DOBJ)/bf_dyn.o
	$(CC) $(CFLAGS) -DTEST $^ -o $(DTEST)/$@ $(LDFLAGS)

local_search.test: $(DSRC)/local_search.c $(DOBJ)/chessboard.o \
		   $(DOBJ)/bf_dyn.o
	$(CC) $(CFLAGS) -DTEST $^ -o $(DTEST)/$@ $(LDFLAGS)

## Templates

$(DOBJ)/%.o: $(DSRC)/%.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $^ -o $@

