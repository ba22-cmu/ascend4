# This is a work in progress. Don't try this at home.
## Dev Notes:
- as of 12/12/24 emscripten sdk, web idl bindings with enscripten generate char** for DOMString[]
  in a signature, but do nothing to actually map a javascript array of strings 
  into a char**. The workaround for now is to define StringList in c++ and an helper
  function in javascript to loop over an input array calling stringlist::add().
- The safe-compiled tcl library (but not tk) makes the ascend4/interface code useful
  at the javascript level if it is wrapped in something to StringList-ify all the
  argv signatures.
