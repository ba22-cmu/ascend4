SRC=main_json.cpp Driver.cpp ascjson.cpp argv.cc

SOLFLAGS=-DHAVE_QRSLV
CXXFLAGS= -I../.. -g -Wall -O0 -DYY_NO_UNPUT $(SOLFLAGS)
OBJS=Driver.o ascjson.o argv.o
SOLOBJ=../../solvers/static_solvers.o
BPREFIX=/home/ba22c/ascend4
SPREFIX=/home/ba22c/ascend4/solvers
SOLLIB=-L$(SPREFIX)/qrslv -Wl,-rpath,$(SPREFIX)/qrslv -lqrslv_ascend
#SOLLIB+=-L$(SPREFIX)/ipopt -Wl,-rpath,$(SPREFIX)/ipopt -lipopt_ascend

prog: Driver.o ascjson.o argv.o $(SOLOBJ)
	g++ -I../.. $(SRC)  -E > i.main_json.cpp
	g++ -I../.. -g $(SRC) $(SOLOBJ) $(SOLLIB) -L$(BPREFIX) -Wl,-rpath,$(BPREFIX) -lascend

../../solvers/static_solvers.o: ../../solvers/static_solvers.c ../../solvers/static_solvers.h makefile
	gcc $(CXXFLAGS) -c ../../solvers/static_solvers.c -o ../../solvers/static_solvers.o

rt:	rt.hpp rt.cc
	g++ ./rt.cc -I. -o rtx

all: a 
	g++ -I. ascjson.cpp -I../..  -E -o i.ascjson.cpp
	g++ -I. ascjson.cpp -I../.. -c -Wall

h:
	g++ HelpProcDS.cpp -I../.. -I. -c -Wall

a:
	g++ -I. argv.cc -I../.. -c -Wall

clean:
	$(RM) *.o a.out
