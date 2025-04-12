SRC=main_json.cpp Driver.cpp ascjson.cpp argv.cc

CXXFLAGS= -I../.. -g -Wall -O0
OBJS=Driver.o ascjson.o argv.o

prog: Driver.o ascjson.o argv.o
	g++ -I../.. $(SRC)  -E > i.main_json.cpp
	g++ -I../.. -g $(SRC) -L/home/ba22c/ascend4 -Wl,-rpath,/home/ba22c/ascend4 -lascend

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
