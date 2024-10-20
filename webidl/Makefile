WB=/usr/share/emscripten/tools/webidl_binder

#main.exe: *.cxx *.h
#	gcc *.cxx -o main.exe

CXXSRCS=../ascxx/*.cpp
CXXHDRS=../ascxx/*.h

lib: $(CXXSRCS) $(CXXHDRS) web_glue.js *.cc
	emcc $(CXXSRCS) --post-js web_glue.js -o output.js -sMODULARIZE -s 'EXPORT_NAME="createMyModule"'

web_glue.cpp web_glue.js: *.widl
	$(RM) all.idl
	cat *.widl > all.idl
	$(WB) all.idl web_glue
	: $(RM) all.idl
	echo python3 -m http.server

clean:
	$(RM) web_glue.* all.idl a.out main.exe output.*

