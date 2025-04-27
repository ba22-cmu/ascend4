include ../json/Makefile.lists
include Makefile.lists
MODELS=../../webmodels

WB=$(HOME)/emsdk/upstream/emscripten/tools/webidl_binder
EM=$(HOME)/emsdk/upstream/emscripten/emcc

#CXXSRCS,CXXHDRS defined in ../json/Makefile.lists
#CSRCS_CORE, CHDRS_ALL defined in Makefile.lists

CSRCS = $(CSRCS_CORE) $(CSRCS_PACKAGES)
CHDRS = $(CSRCS_ALL) 

RTSRCS = ../json/rt.cc rt_glue_wrapper.cc
RTHDRS = ../json/rt.hpp

CFLAGS=-DYY_NO_UNPUT
EXP=-sEXPORT_KEEPALIVE
EXP=-sEXPORT_ALL
EXP=-sEXPORTED_FUNCTIONS=@./nm_glue.txt -sEXPORTED_FUNCTIONS=_AscDriver
EXP=-sEXPORT_KEEPALIVE
EXP=-sFORCE_FILESYSTEM -sEXPORTED_RUNTIME_METHODS=FS
EMFLAGS= -O0 -g -gsource-map -sMODULARIZE=1  -sENVIRONMENT=web -s'EXPORT_NAME="createModule"'
HTEMFLAGS= -O0 -g  -sENVIRONMENT=web #--emit-symbol-map
EMBEDS= --embed-file $(MODELS)@/models -lidbfs.js

RTWB=/usr/share/emscripten/tools/webidl_binder
RTEM=/usr/bin/emcc
RTEXP=-sEXPORTED_FUNCTIONS=@./rt_exp.txt
RTEXP=
RTEMFLAGS= -g -sMODULARIZE  -s'EXPORT_NAME="createMyModule"' 
RTPOST= --extern-post-js rt_post.js
RTPOST=

# -DMODULE_DEBUG  (for compiler file resolver)
lib: $(CXXSRCS) $(CXXHDRS) $(CSRCS) $(CHDRS) web_glue.js ascjson_glue_wrapper.cc
	$(EM) -I../.. -I. $(CFLAGS) $(EXP) $(CXXSRCS) $(CSRCS) \
		ascjson_glue_wrapper.cc \
		--post-js web_glue.js \
		-o output.js $(EMFLAGS) $(EMBEDS)

rtlib: $(RTSRCS) $(RTHDRS) rt_glue.js rt_glue_wrapper.cc rt_post.js
	$(RTEM) -I../.. -I. $(RTEXP) $(RTSRCS) \
		--post-js rt_glue.js $(RTPOST) \
		-o rt.js $(RTEMFLAGS) $(EMBEDS)

ht: $(CXXSRCS) $(CXXHDRS) $(CSRCS) $(CHDRS) web_glue.js ascjson_glue_wrapper.cc
	$(EM) -I../.. -I. $(EXP) $(CXXSRCS) $(CSRCS) --post-js web_glue.js -o outh.html $(HTEMFLAGS) $(EMBEDS)

web_glue.cpp web_glue.js: ascjson.idl
	$(WB) ascjson.idl web_glue
	grep EMSCRIPTEN_KEEPALIVE web_glue.cpp |sed -e 's%^//.*%%' -e 's/.*EMSCRIPTEN_KEEPALIVE //' -e 's/[(].*//' -e 's/.* //' |grep _ |grep -v ___ > glue_funcs.txt
	nm ../json/a.out --demangle |grep ascjson | sed -e 's/.* T //' -e 's/[(].*//' >nm_glue.txt
	: echo python3 -m http.server

rt_glue.cpp rt_glue.js: rt.idl
	$(RTWB) rt.idl rt_glue
	grep EMSCRIPTEN_KEEPALIVE rt_glue.cpp |sed -e 's%^//.*%%' -e 's/.*EMSCRIPTEN_KEEPALIVE //' -e 's/[(].*//' -e 's/.* //' |grep _ |grep -v ___ > rt_funcs.txt
	nm ../json/rtx --demangle |grep rt | sed -e 's/.* T //' -e 's/[(].*//' >nm_rt_glue.txt
	: echo python3 -m http.server

clean:
	$(RM) rt_glue.* web_glue.* a.out main.exe output.* outh.* fb_glue.* *.wasm

FBEXP=-sEXPORT_ALL
fblib: fbmain.cxx foo.cxx bar.cxx foo.h bar.h fb_glue.js fb_glue.cpp
	$(RTEM) fbmain.cxx foo.cxx bar.cxx --post-js fb_glue.js \
		$(FBEXP) \
		-o fbout.js -sMODULARIZE -s 'EXPORT_NAME="createMyModule"'
	grep EMSCRIPTEN_KEEPALIVE fb_glue.cpp |sed -e 's%^//.*%%' -e 's/.*EMSCRIPTEN_KEEPALIVE //' -e 's/[(].*//' -e 's/.* //' |grep _ |grep -v ___ > fb_funcs.txt
	nm ../json/rtx --demangle |grep rt | sed -e 's/.* T //' -e 's/[(].*//' >nm_rt_glue.txt

fb_glue.cpp fb_glue.js: fb.idl
	$(RTWB) fb.idl fb_glue
