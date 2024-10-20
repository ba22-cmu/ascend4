The webcxx c++ wrapping of the c library is a webidl-friendly fork of the
ascxx c++ wrapping. Major differences include:
- lists.h is used because c++ container support in webidl is rather variable by version.
- c++ exceptions are not used because they are variable by version in webidl and the ascxx 
  version of them is memory leaky.
- some headers, e.g. annotation, are split to per-class.
- no python/javascript callbacks are supported.
