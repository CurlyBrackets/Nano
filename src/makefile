FLAGS = -Wall -std=c++0x -I../CWin
OBJS = main.o Application.o Line.o

ifneq ($(shell uname), Linux)
	LIBS = -lpdcurses
	DEFS = -DWIN32
else
	LIBS = -lncurses
endif

all: $(OBJS)

%.o:%.cpp
	g++ $(DEFS) $(FLAGS) -c -o $@ $<
	
clean:
	rm -rf *.o

