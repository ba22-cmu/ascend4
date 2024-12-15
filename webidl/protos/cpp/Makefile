MODELS=../../../webmodels
WB=$(HOME)/emsdk/upstream/emscripten/tools/webidl_binder
EM=$(HOME)/emsdk/upstream/emscripten/emcc
LKCC= Dumper.cc  StringList.cc  asc_web_glue.cc  ls.cc
SMCC= Dumper.cc  StringList.cc  asc_small_glue.cc  ls.cc

CFLAGS=-Wall -I.
CXXFLAGS=-Wall -I. -g -O0
all: ls.o Dumper.o StringList.o
	$(CXX) $(CXXFLAGS) main.cpp Dumper.cc StringList.cc ls.cc -o d.x

clean:
	$(RM) -r *.o foo.clone *.html *.js *.data *.wasm d.x web_glue.* output.js all.idl

all.idl: *.widl
	cat *.widl > all.idl

one: all.idl
	$(EM)  -I. main.cpp Dumper.cc StringList.cc ls.cc -o fstest.html --embed-file .@/models
	: echo python3 -m http.server

lib: main.cpp web_glue.cpp $(LKCC) *.h web_glue.js 
	$(EM) $(LKCC) --post-js web_glue.js -o output.js -sMODULARIZE -sLINKABLE -s 'EXPORT_NAME="createMyModule"'

small: main.cpp small_glue.cpp $(SMCC) *.h small_glue.js 
	$(EM) $(SMCC) --post-js small_glue.js -o small_output.js -sMODULARIZE -s 'EXPORT_NAME="createMyModule"'

small_glue.cpp small_glue.js: all.idl
	$(WB) all.idl small_glue
	echo python3 -m http.server

web_glue.cpp web_glue.js: all.idl
	$(WB) all.idl web_glue
	echo python3 -m http.server

