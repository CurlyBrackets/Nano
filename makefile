FLAGS = -Wall -std=c++0x -I./CWin
ifeq ($(shell uname), windows32)
	LIBS = -lpdcurses
else
	ifneq ($(shell uname), Linux)
		FLAGS += -IC:/cygwin/usr/include/ncurses
	endif
	LIBS = -lncurses
endif

OBJS = CWin/CWin.o src/main.o src/Application.o src/Line.o

#link
all:
	cd CWin; make lib
	cd src; make
	g++ $(OBJS) -o Nano $(LIBS)

#clean obj
clean:
	cd CWin; make clean
	cd src; make clean
	rm -rf *.o

