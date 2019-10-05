CPP = clang++
RM = rm -f
RMDIR = rm -rf
CPPFLAGS = -glldb -std=c++17 -Wall -Wextra -Wpedantic -Wunreachable-code
LDFLAGS = -glldb
LDLIBS =

SRCS = $(wildcard *.cc)
DEPS = $(wildcard *.h)
OBJS = $(subst .cc,.o,$(SRCS))

all: sudoku

sudoku: $(OBJS)
	$(CPP) $(LDFLAGS) -o $@ $(OBJS) $(LDLIBS) 

%.o: %.cpp $(DEPS)
	$(CPP) $(CPPFLAGS) -c -o $@ $<

clean:
	$(RM) $(OBJS) sudoku
	$(RMDIR) sudoku.dSYM
