include ../json/Makefile.lists
include Makefile.lists
MODELS=../../webmodels

WB=$(HOME)/emsdk/upstream/emscripten/tools/webidl_binder
EM=$(HOME)/emsdk/upstream/emscripten/emcc

#CXXSRCS,CXXHDRS defined in ../json/Makefile.lists
#CSRCS_CORE, CHDRS_ALL defined in Makefile.lists

CSRCS = $(CSRCS_CORE) $(CSRCS_PACKAGES)
CHDRS = $(CSRCS_ALL) 

lib: $(CXXSRCS) $(CXXHDRS) $(CSRCS) $(CHDRS) web_glue.js web_glue.cpp 
	$(EM) -I../.. $(CXXSRCS) $(CSRCS) --post-js web_glue.js -o output.js -sMODULARIZE -s 'EXPORT_NAME="createMyModule"'  -sENVIRONMENT=web --embed-file $(MODELS)@/models

web_glue.cpp web_glue.js: ascjson.idl
	$(WB) ascjson.idl web_glue
	: echo python3 -m http.server

clean:
	$(RM) web_glue.* a.out main.exe output.*

