include ../webcxx/Makefile.lists
include Makefile.lists

#WB=/usr/share/emscripten/tools/webidl_binder
WB=$(HOME)/emsdk/upstream/emscripten/tools/webidl_binder
EM=$(HOME)/emsdk/upstream/emscripten/emcc

#main.exe: *.cxx *.h
#	gcc *.cxx -o main.exe

#CXXSRCS,CXXHDRS defined in ../webcxx/Makefile.lists
#CSRCS_CORE, CHDRS_ALL defined in Makefile.lists

CSRCS = $(CSRCS_CORE) 
CHDRS = $(CSRCS_ALL) 

lib: $(CXXSRCS) $(CXXHDRS) $(CSRCS) $(CHDRS) web_glue.js *.cpp
	$(EM) -I.. $(CXXSRCS) $(CSRCS) --post-js web_glue.js -o output.js -sMODULARIZE -s 'EXPORT_NAME="createMyModule"'

web_glue.cpp web_glue.js: *.widl lists.widl
	cat fwd.idl *.widl > ascend.idl
	$(WB) ascend.idl web_glue
	: $(RM) ascend.idl
	: echo python3 -m http.server

# we did this to find duplicate signatures more easily.
checksigunique: *.widl 
	for i in *.widl; do  \
		echo $$i; \
		cat fwd.idl $$i > x_$$i.idl; \
		$(WB) x_$$i.idl x_glue ; \
	done

lists.widl: lists.widl.in lists.sed.sh
	./lists.sed.sh

clean:
	$(RM) web_glue.* ascend.idl a.out main.exe output.*

