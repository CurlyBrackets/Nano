FLAGS = -Wall -std=c++0x -I../CWin -I../inc
OBJS = main.o Application.o Line.o

ifeq ($(shell uname), windows32)
	LIBS = -lpdcurses
	DEFS = -DWIN32
else
	ifneq ($(shell uname), Linux)
		FLAGS += -IC:/cygwin/usr/include/ncurses
	endif
	LIBS = -lncurses
endif

all: $(OBJS)

%.o:%.cpp
	g++ $(DEFS) $(FLAGS) -c -o $@ $<
	
clean:
	rm -rf *.o

