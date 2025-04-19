MODELS=../../../webmodels
WB=$(HOME)/emsdk/upstream/emscripten/tools/webidl_binder
EM=$(HOME)/emsdk/upstream/emscripten/emcc
LKCC= Dumper.cc  StringList.cc  asc_web_glue.cc  ls.cc
SMCC= Dumper.cc  StringList.cc  asc_small_glue.cc  ls.cc

CFLAGS=-Wall -I.
CXXFLAGS=-Wall -I. -g -O0 -std=c++17
all: ls.o Dumper.o StringList.o
	$(CXX) $(CXXFLAGS) main.cpp Dumper.cc StringList.cc ls.cc -o d.x

clean:
	$(RM) -r *.o foo.clone *.html *.js *.data *.wasm d.x web_glue.* output.js all.idl

all.idl: *.widl
	cat *.widl > all.idl

COMIDL= Dumper.widl StringList.widl
comex.idl: *.widl
	cat $(COMIDL) > comex.idl

one: all.idl
	$(EM)  -I. main.cpp Dumper.cc StringList.cc ls.cc -o fstest.html --embed-file .@/models
	: echo python3 -m http.server

#-----------
rt_glue.cpp rt_glue.js: rt.idl
	IDL_VERBOSE=1 $(WB) rt.idl rt_glue

rtlib: rtmain.cc rt_glue.cpp rt.cc argv.cc rt_glue.js rt.h
	$(EM)  rt.cc argv.cc rt_glue_wrapper.cc \
		--post-js rt_glue.js \
		-o rt_output.js \
		-sMODULARIZE -sLINKABLE -sENVIRONMENT=web -s 'EXPORT_NAME="createMyModule"'
	: echo python3 -m http.server

rtclean:
	$(RM) rt_glue.* rt_output.js rt.o

#-----------
ctlib: ctmain.cc ct.cc ct.h
	$(EM)  -lembind ct.cc \
		-o ct_output.js \
		-sMODULARIZE -sLINKABLE -sENVIRONMENT=web -s 'EXPORT_NAME="createMyModule"'

ctclean:
	$(RM) ct_output.js ct.o
#-----------

lib: main.cpp web_glue.cpp $(LKCC) *.h web_glue.js 
	$(EM) $(LKCC) --post-js web_glue.js -o output.js -sMODULARIZE -sLINKABLE -s 'EXPORT_NAME="createMyModule"' --embed-file $(MODELS)@/models

small: main.cpp small_glue.cpp $(SMCC) *.h small_glue.js 
	$(EM) $(SMCC) --post-js small_glue.js -o small_output.js -sMODULARIZE -s 'EXPORT_NAME="createMyModule"'  --embed-file $(MODELS)@/models

comex_glue.cpp comex_glue.js: comex.idl
	$(WB) comex.idl comex_glue
	echo python3 -m http.server

small_glue.cpp small_glue.js: all.idl
	$(WB) all.idl small_glue
	echo python3 -m http.server

web_glue.cpp web_glue.js: all.idl
	$(WB) all.idl web_glue
	echo python3 -m http.server

# string array example
ex example.js: example_bindings.cc
	$(EM) -lembind -o example.js example_bindings.cc

COMSRC= example_bindings.cc  Dumper.cc  StringList.cc  comex_small_glue.cc  ls.cc

comex1: comex.cpp comex_glue.cpp $(COMSRC) *.h comex_glue.js example.js 
	$(EM) $(COMSRC) --post-js comex_glue.js -o comex_output.js -sMODULARIZE -s 'EXPORT_NAME="createMyModule"'  --embed-file $(MODELS)@/models	 -lembind 

comex2: comex.cpp comex_glue.cpp $(COMSRC) *.h comex_glue.js example.js 
	$(EM) $(COMSRC) --post-js example.js --post-js comex_glue.js -o comex_output.js -sMODULARIZE -s 'EXPORT_NAME="createMyModule"'  --embed-file $(MODELS)@/models	 -lembind 

