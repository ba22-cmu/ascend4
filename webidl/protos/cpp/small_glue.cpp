
#include <emscripten.h>
#include <stdlib.h>

EM_JS_DEPS(webidl_binder, "$intArrayFromString,$UTF8ToString,$alignMemory");

extern "C" {

// Define custom allocator functions that we can force export using
// EMSCRIPTEN_KEEPALIVE.  This avoids all webidl users having to add
// malloc/free to -sEXPORTED_FUNCTIONS.
EMSCRIPTEN_KEEPALIVE void webidl_free(void* p) { free(p); }
EMSCRIPTEN_KEEPALIVE void* webidl_malloc(size_t len) { return malloc(len); }


// Interface: VoidPtr


void EMSCRIPTEN_KEEPALIVE emscripten_bind_VoidPtr___destroy___0(void** self) {
  delete self;
}

// Interface: Dumper


char* EMSCRIPTEN_KEEPALIVE emscripten_bind_Dumper_getFile_1(Dumper* self, char* fname) {
  return self->getFile(fname);
}

void EMSCRIPTEN_KEEPALIVE emscripten_bind_Dumper_printFile_1(Dumper* self, char* fname) {
  self->printFile(fname);
}

int EMSCRIPTEN_KEEPALIVE emscripten_bind_Dumper_makeFile_2(Dumper* self, char* path, char* content) {
  return self->makeFile(path, content);
}

void EMSCRIPTEN_KEEPALIVE emscripten_bind_Dumper_penv_0(Dumper* self) {
  self->penv();
}

int EMSCRIPTEN_KEEPALIVE emscripten_bind_Dumper_addNames_1(Dumper* self, char** n) {
  return self->addNames(n);
}

StringList* EMSCRIPTEN_KEEPALIVE emscripten_bind_Dumper_getNames_0(Dumper* self) {
  return self->getNames();
}

StringList* EMSCRIPTEN_KEEPALIVE emscripten_bind_Dumper_getLies_0(Dumper* self) {
  return self->getLies();
}

void EMSCRIPTEN_KEEPALIVE emscripten_bind_Dumper___destroy___0(Dumper* self) {
  delete self;
}

// Interface: StringList


void EMSCRIPTEN_KEEPALIVE emscripten_bind_StringList_add_1(StringList* self, char** s) {
  self->add(s);
}

char* EMSCRIPTEN_KEEPALIVE emscripten_bind_StringList_get_1(StringList* self, unsigned int i) {
  return self->get(i);
}

unsigned int EMSCRIPTEN_KEEPALIVE emscripten_bind_StringList_size_0(StringList* self) {
  return self->size();
}

void EMSCRIPTEN_KEEPALIVE emscripten_bind_StringList___destroy___0(StringList* self) {
  delete self;
}

}

