MODELS=../../../webmodels
WB=$(HOME)/emsdk/upstream/emscripten/tools/webidl_binder
EM=$(HOME)/emsdk/upstream/emscripten/emcc

# native build
all: mkt myls
	/bin/rm -rf a
	./mkt 
	./myls ./a//

mkt: mktree.c
	gcc -o mkt mktree.c -Wall

myls: ls.c
	gcc -o myls ls.c -Wall

clean:
	$(RM) -r mkt myls a *.html *.js *.data *.wasm

one:
	$(EM) fstest.c -o fstest.html --embed-file $(MODELS)@/models
	: echo python3 -m http.server

two:
	$(EM) fstest.c -o pfstest.html --preload-file $(MODELS)@/models
	: echo python3 -m http.server

