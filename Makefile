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
CFLAGS=-g -msse2 -msse3 -DDEBUG -Wall -std=gnu99
else
CFLAGS=-O3 -msse2 -msse3 -msse4 -Wall -std=gnu99 -Wno-format
endif
LDFLAGS= -lm

## Directories
DOBJ=obj
DSRC=src

TARGET=n-dames

#Targets
all: program

program: $(TARGET)

$(TARGET): $(DOBJ)/main.o $(DOBJ)/bf.o \
	   $(DOBJ)/bf_dyn.o  $(DOBJ)/chessboard.o \
	   $(DOBJ)/forward_checking.o $(DOBJ)/backtrack.o  \
	   $(DOBJ)/local_search2.o $(DOBJ)/local_search3.o \
	   $(DOBJ)/wikimethod.o
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

clean:
	rm -f $(DOBJ)/*.o

clobber: clean
	rm -f $(TARGET)

## Templates

$(DOBJ)/%.o: $(DSRC)/%.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $^ -o $@
