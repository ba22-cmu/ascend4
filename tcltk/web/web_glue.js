
// Bindings utilities

/** @suppress {duplicate} (TODO: avoid emitting this multiple times, it is redundant) */
function WrapperObject() {
}
WrapperObject.prototype = Object.create(WrapperObject.prototype);
WrapperObject.prototype.constructor = WrapperObject;
WrapperObject.prototype.__class__ = WrapperObject;
WrapperObject.__cache__ = {};
Module['WrapperObject'] = WrapperObject;

/** @suppress {duplicate} (TODO: avoid emitting this multiple times, it is redundant)
    @param {*=} __class__ */
function getCache(__class__) {
  return (__class__ || WrapperObject).__cache__;
}
Module['getCache'] = getCache;

/** @suppress {duplicate} (TODO: avoid emitting this multiple times, it is redundant)
    @param {*=} __class__ */
function wrapPointer(ptr, __class__) {
  var cache = getCache(__class__);
  var ret = cache[ptr];
  if (ret) return ret;
  ret = Object.create((__class__ || WrapperObject).prototype);
  ret.ptr = ptr;
  return cache[ptr] = ret;
}
Module['wrapPointer'] = wrapPointer;

/** @suppress {duplicate} (TODO: avoid emitting this multiple times, it is redundant) */
function castObject(obj, __class__) {
  return wrapPointer(obj.ptr, __class__);
}
Module['castObject'] = castObject;

Module['NULL'] = wrapPointer(0);

/** @suppress {duplicate} (TODO: avoid emitting this multiple times, it is redundant) */
function destroy(obj) {
  if (!obj['__destroy__']) throw 'Error: Cannot destroy object. (Did you create it yourself?)';
  obj['__destroy__']();
  // Remove from cache, so the object can be GC'd and refs added onto it released
  delete getCache(obj.__class__)[obj.ptr];
}
Module['destroy'] = destroy;

/** @suppress {duplicate} (TODO: avoid emitting this multiple times, it is redundant) */
function compare(obj1, obj2) {
  return obj1.ptr === obj2.ptr;
}
Module['compare'] = compare;

/** @suppress {duplicate} (TODO: avoid emitting this multiple times, it is redundant) */
function getPointer(obj) {
  return obj.ptr;
}
Module['getPointer'] = getPointer;

/** @suppress {duplicate} (TODO: avoid emitting this multiple times, it is redundant) */
function getClass(obj) {
  return obj.__class__;
}
Module['getClass'] = getClass;

// Converts big (string or array) values into a C-style storage, in temporary space

/** @suppress {duplicate} (TODO: avoid emitting this multiple times, it is redundant) */
var ensureCache = {
  buffer: 0,  // the main buffer of temporary storage
  size: 0,   // the size of buffer
  pos: 0,    // the next free offset in buffer
  temps: [], // extra allocations
  needed: 0, // the total size we need next time

  prepare() {
    if (ensureCache.needed) {
      // clear the temps
      for (var i = 0; i < ensureCache.temps.length; i++) {
        Module['_webidl_free'](ensureCache.temps[i]);
      }
      ensureCache.temps.length = 0;
      // prepare to allocate a bigger buffer
      Module['_webidl_free'](ensureCache.buffer);
      ensureCache.buffer = 0;
      ensureCache.size += ensureCache.needed;
      // clean up
      ensureCache.needed = 0;
    }
    if (!ensureCache.buffer) { // happens first time, or when we need to grow
      ensureCache.size += 128; // heuristic, avoid many small grow events
      ensureCache.buffer = Module['_webidl_malloc'](ensureCache.size);
      assert(ensureCache.buffer);
    }
    ensureCache.pos = 0;
  },
  alloc(array, view) {
    assert(ensureCache.buffer);
    var bytes = view.BYTES_PER_ELEMENT;
    var len = array.length * bytes;
    len = alignMemory(len, 8); // keep things aligned to 8 byte boundaries
    var ret;
    if (ensureCache.pos + len >= ensureCache.size) {
      // we failed to allocate in the buffer, ensureCache time around :(
      assert(len > 0); // null terminator, at least
      ensureCache.needed += len;
      ret = Module['_webidl_malloc'](len);
      ensureCache.temps.push(ret);
    } else {
      // we can allocate in the buffer
      ret = ensureCache.buffer + ensureCache.pos;
      ensureCache.pos += len;
    }
    return ret;
  },
  copy(array, view, offset) {
    offset /= view.BYTES_PER_ELEMENT;
    for (var i = 0; i < array.length; i++) {
      view[offset + i] = array[i];
    }
  },
};

/** @suppress {duplicate} (TODO: avoid emitting this multiple times, it is redundant) */
function ensureString(value) {
  if (typeof value === 'string') {
    var intArray = intArrayFromString(value);
    var offset = ensureCache.alloc(intArray, HEAP8);
    ensureCache.copy(intArray, HEAP8, offset);
    return offset;
  }
  return value;
}

/** @suppress {duplicate} (TODO: avoid emitting this multiple times, it is redundant) */
function ensureInt8(value) {
  if (typeof value === 'object') {
    var offset = ensureCache.alloc(value, HEAP8);
    ensureCache.copy(value, HEAP8, offset);
    return offset;
  }
  return value;
}

/** @suppress {duplicate} (TODO: avoid emitting this multiple times, it is redundant) */
function ensureInt16(value) {
  if (typeof value === 'object') {
    var offset = ensureCache.alloc(value, HEAP16);
    ensureCache.copy(value, HEAP16, offset);
    return offset;
  }
  return value;
}

/** @suppress {duplicate} (TODO: avoid emitting this multiple times, it is redundant) */
function ensureInt32(value) {
  if (typeof value === 'object') {
    var offset = ensureCache.alloc(value, HEAP32);
    ensureCache.copy(value, HEAP32, offset);
    return offset;
  }
  return value;
}

/** @suppress {duplicate} (TODO: avoid emitting this multiple times, it is redundant) */
function ensureFloat32(value) {
  if (typeof value === 'object') {
    var offset = ensureCache.alloc(value, HEAPF32);
    ensureCache.copy(value, HEAPF32, offset);
    return offset;
  }
  return value;
}

/** @suppress {duplicate} (TODO: avoid emitting this multiple times, it is redundant) */
function ensureFloat64(value) {
  if (typeof value === 'object') {
    var offset = ensureCache.alloc(value, HEAPF64);
    ensureCache.copy(value, HEAPF64, offset);
    return offset;
  }
  return value;
}

// Interface: VoidPtr

/** @suppress {undefinedVars, duplicate} @this{Object} */
function VoidPtr() { throw "cannot construct a VoidPtr, no constructor in IDL" }
VoidPtr.prototype = Object.create(WrapperObject.prototype);
VoidPtr.prototype.constructor = VoidPtr;
VoidPtr.prototype.__class__ = VoidPtr;
VoidPtr.__cache__ = {};
Module['VoidPtr'] = VoidPtr;

/** @suppress {undefinedVars, duplicate} @this{Object} */
VoidPtr.prototype['__destroy__'] = VoidPtr.prototype.__destroy__ = function() {
  var self = this.ptr;
  _emscripten_bind_VoidPtr___destroy___0(self);
};

// Interface: rt

/** @suppress {undefinedVars, duplicate} @this{Object} */
function rt() { throw "cannot construct a rt, no constructor in IDL" }
rt.prototype = Object.create(WrapperObject.prototype);
rt.prototype.constructor = rt;
rt.prototype.__class__ = rt;
rt.__cache__ = {};
Module['rt'] = rt;
/** @suppress {undefinedVars, duplicate} @this{Object} */
rt.prototype['get_rc'] = rt.prototype.get_rc = function() {
  var self = this.ptr;
  return _emscripten_bind_rt_get_rc_0(self);
};

/** @suppress {checkTypes} */
Object.defineProperty(rt.prototype, 'rc', { get: rt.prototype.get_rc });
/** @suppress {undefinedVars, duplicate} @this{Object} */
rt.prototype['get_v'] = rt.prototype.get_v = function() {
  var self = this.ptr;
  return UTF8ToString(_emscripten_bind_rt_get_v_0(self));
};

/** @suppress {checkTypes} */
Object.defineProperty(rt.prototype, 'v', { get: rt.prototype.get_v });
/** @suppress {undefinedVars, duplicate} @this{Object} */
rt.prototype['get_t'] = rt.prototype.get_t = function() {
  var self = this.ptr;
  return _emscripten_bind_rt_get_t_0(self);
};

/** @suppress {checkTypes} */
Object.defineProperty(rt.prototype, 't', { get: rt.prototype.get_t });

/** @suppress {undefinedVars, duplicate} @this{Object} */
rt.prototype['__destroy__'] = rt.prototype.__destroy__ = function() {
  var self = this.ptr;
  _emscripten_bind_rt___destroy___0(self);
};

// Interface: ascjson

/** @suppress {undefinedVars, duplicate} @this{Object} */
function ascjson(vargv) {
  ensureCache.prepare();
  if (vargv && typeof vargv === 'object') vargv = vargv.ptr;
  else vargv = ensureString(vargv);
  this.ptr = _emscripten_bind_ascjson_ascjson_1(vargv);
  getCache(ascjson)[this.ptr] = this;
};

ascjson.prototype = Object.create(WrapperObject.prototype);
ascjson.prototype.constructor = ascjson;
ascjson.prototype.__class__ = ascjson;
ascjson.__cache__ = {};
Module['ascjson'] = ascjson;
/** @suppress {undefinedVars, duplicate} @this{Object} */
ascjson.prototype['libr_query'] = ascjson.prototype.libr_query = function(vargv) {
  var self = this.ptr;
  ensureCache.prepare();
  if (vargv && typeof vargv === 'object') vargv = vargv.ptr;
  else vargv = ensureString(vargv);
  return wrapPointer(_emscripten_bind_ascjson_libr_query_1(self, vargv), rt);
};

/** @suppress {undefinedVars, duplicate} @this{Object} */
ascjson.prototype['asc_compiler_option'] = ascjson.prototype.asc_compiler_option = function(vargv) {
  var self = this.ptr;
  ensureCache.prepare();
  if (vargv && typeof vargv === 'object') vargv = vargv.ptr;
  else vargv = ensureString(vargv);
  return wrapPointer(_emscripten_bind_ascjson_asc_compiler_option_1(self, vargv), rt);
};

/** @suppress {undefinedVars, duplicate} @this{Object} */
ascjson.prototype['libr_parsestring'] = ascjson.prototype.libr_parsestring = function(vargv) {
  var self = this.ptr;
  ensureCache.prepare();
  if (vargv && typeof vargv === 'object') vargv = vargv.ptr;
  else vargv = ensureString(vargv);
  return wrapPointer(_emscripten_bind_ascjson_libr_parsestring_1(self, vargv), rt);
};

/** @suppress {undefinedVars, duplicate} @this{Object} */
ascjson.prototype['librread'] = ascjson.prototype.librread = function(vargv) {
  var self = this.ptr;
  ensureCache.prepare();
  if (vargv && typeof vargv === 'object') vargv = vargv.ptr;
  else vargv = ensureString(vargv);
  return wrapPointer(_emscripten_bind_ascjson_librread_1(self, vargv), rt);
};

/** @suppress {undefinedVars, duplicate} @this{Object} */
ascjson.prototype['libr_moduleinfo'] = ascjson.prototype.libr_moduleinfo = function(vargv) {
  var self = this.ptr;
  ensureCache.prepare();
  if (vargv && typeof vargv === 'object') vargv = vargv.ptr;
  else vargv = ensureString(vargv);
  return wrapPointer(_emscripten_bind_ascjson_libr_moduleinfo_1(self, vargv), rt);
};

/** @suppress {undefinedVars, duplicate} @this{Object} */
ascjson.prototype['libr_destroy_types'] = ascjson.prototype.libr_destroy_types = function(vargv) {
  var self = this.ptr;
  ensureCache.prepare();
  if (vargv && typeof vargv === 'object') vargv = vargv.ptr;
  else vargv = ensureString(vargv);
  return wrapPointer(_emscripten_bind_ascjson_libr_destroy_types_1(self, vargv), rt);
};

/** @suppress {undefinedVars, duplicate} @this{Object} */
ascjson.prototype['libr_hide_type'] = ascjson.prototype.libr_hide_type = function(vargv) {
  var self = this.ptr;
  ensureCache.prepare();
  if (vargv && typeof vargv === 'object') vargv = vargv.ptr;
  else vargv = ensureString(vargv);
  return wrapPointer(_emscripten_bind_ascjson_libr_hide_type_1(self, vargv), rt);
};

/** @suppress {undefinedVars, duplicate} @this{Object} */
ascjson.prototype['libr_unhide_type'] = ascjson.prototype.libr_unhide_type = function(vargv) {
  var self = this.ptr;
  ensureCache.prepare();
  if (vargv && typeof vargv === 'object') vargv = vargv.ptr;
  else vargv = ensureString(vargv);
  return wrapPointer(_emscripten_bind_ascjson_libr_unhide_type_1(self, vargv), rt);
};

/** @suppress {undefinedVars, duplicate} @this{Object} */
ascjson.prototype['libr_type_is_shown'] = ascjson.prototype.libr_type_is_shown = function(vargv) {
  var self = this.ptr;
  ensureCache.prepare();
  if (vargv && typeof vargv === 'object') vargv = vargv.ptr;
  else vargv = ensureString(vargv);
  return wrapPointer(_emscripten_bind_ascjson_libr_type_is_shown_1(self, vargv), rt);
};

/** @suppress {undefinedVars, duplicate} @this{Object} */
ascjson.prototype['libr_types_in_module'] = ascjson.prototype.libr_types_in_module = function(vargv) {
  var self = this.ptr;
  ensureCache.prepare();
  if (vargv && typeof vargv === 'object') vargv = vargv.ptr;
  else vargv = ensureString(vargv);
  return wrapPointer(_emscripten_bind_ascjson_libr_types_in_module_1(self, vargv), rt);
};

/** @suppress {undefinedVars, duplicate} @this{Object} */
ascjson.prototype['Asc_ExtractTypeHC'] = ascjson.prototype.Asc_ExtractTypeHC = function(vargv) {
  var self = this.ptr;
  ensureCache.prepare();
  if (vargv && typeof vargv === 'object') vargv = vargv.ptr;
  else vargv = ensureString(vargv);
  return wrapPointer(_emscripten_bind_ascjson_Asc_ExtractTypeHC_1(self, vargv), rt);
};

/** @suppress {undefinedVars, duplicate} @this{Object} */
ascjson.prototype['qlfdid'] = ascjson.prototype.qlfdid = function(vargv) {
  var self = this.ptr;
  ensureCache.prepare();
  if (vargv && typeof vargv === 'object') vargv = vargv.ptr;
  else vargv = ensureString(vargv);
  return wrapPointer(_emscripten_bind_ascjson_qlfdid_1(self, vargv), rt);
};

/** @suppress {undefinedVars, duplicate} @this{Object} */
ascjson.prototype['bgetproc'] = ascjson.prototype.bgetproc = function(vargv) {
  var self = this.ptr;
  ensureCache.prepare();
  if (vargv && typeof vargv === 'object') vargv = vargv.ptr;
  else vargv = ensureString(vargv);
  return wrapPointer(_emscripten_bind_ascjson_bgetproc_1(self, vargv), rt);
};

/** @suppress {undefinedVars, duplicate} @this{Object} */
ascjson.prototype['brow_runmethod'] = ascjson.prototype.brow_runmethod = function(vargv) {
  var self = this.ptr;
  ensureCache.prepare();
  if (vargv && typeof vargv === 'object') vargv = vargv.ptr;
  else vargv = ensureString(vargv);
  return wrapPointer(_emscripten_bind_ascjson_brow_runmethod_1(self, vargv), rt);
};

/** @suppress {undefinedVars, duplicate} @this{Object} */
ascjson.prototype['brow_assign'] = ascjson.prototype.brow_assign = function(vargv) {
  var self = this.ptr;
  ensureCache.prepare();
  if (vargv && typeof vargv === 'object') vargv = vargv.ptr;
  else vargv = ensureString(vargv);
  return wrapPointer(_emscripten_bind_ascjson_brow_assign_1(self, vargv), rt);
};

/** @suppress {undefinedVars, duplicate} @this{Object} */
ascjson.prototype['qassgn3'] = ascjson.prototype.qassgn3 = function(vargv) {
  var self = this.ptr;
  ensureCache.prepare();
  if (vargv && typeof vargv === 'object') vargv = vargv.ptr;
  else vargv = ensureString(vargv);
  return wrapPointer(_emscripten_bind_ascjson_qassgn3_1(self, vargv), rt);
};

/** @suppress {undefinedVars, duplicate} @this{Object} */
ascjson.prototype['x__brow_iname'] = ascjson.prototype.x__brow_iname = function(vargv) {
  var self = this.ptr;
  ensureCache.prepare();
  if (vargv && typeof vargv === 'object') vargv = vargv.ptr;
  else vargv = ensureString(vargv);
  return wrapPointer(_emscripten_bind_ascjson_x__brow_iname_1(self, vargv), rt);
};

/** @suppress {undefinedVars, duplicate} @this{Object} */
ascjson.prototype['x__brow_isrelation'] = ascjson.prototype.x__brow_isrelation = function(vargv) {
  var self = this.ptr;
  ensureCache.prepare();
  if (vargv && typeof vargv === 'object') vargv = vargv.ptr;
  else vargv = ensureString(vargv);
  return wrapPointer(_emscripten_bind_ascjson_x__brow_isrelation_1(self, vargv), rt);
};

/** @suppress {undefinedVars, duplicate} @this{Object} */
ascjson.prototype['x__brow_ismodel'] = ascjson.prototype.x__brow_ismodel = function(vargv) {
  var self = this.ptr;
  ensureCache.prepare();
  if (vargv && typeof vargv === 'object') vargv = vargv.ptr;
  else vargv = ensureString(vargv);
  return wrapPointer(_emscripten_bind_ascjson_x__brow_ismodel_1(self, vargv), rt);
};

/** @suppress {undefinedVars, duplicate} @this{Object} */
ascjson.prototype['bgetrels'] = ascjson.prototype.bgetrels = function(vargv) {
  var self = this.ptr;
  ensureCache.prepare();
  if (vargv && typeof vargv === 'object') vargv = vargv.ptr;
  else vargv = ensureString(vargv);
  return wrapPointer(_emscripten_bind_ascjson_bgetrels_1(self, vargv), rt);
};

/** @suppress {undefinedVars, duplicate} @this{Object} */
ascjson.prototype['bgetrelspf'] = ascjson.prototype.bgetrelspf = function(vargv) {
  var self = this.ptr;
  ensureCache.prepare();
  if (vargv && typeof vargv === 'object') vargv = vargv.ptr;
  else vargv = ensureString(vargv);
  return wrapPointer(_emscripten_bind_ascjson_bgetrelspf_1(self, vargv), rt);
};

/** @suppress {undefinedVars, duplicate} @this{Object} */
ascjson.prototype['x__brow_relsforatom'] = ascjson.prototype.x__brow_relsforatom = function(vargv) {
  var self = this.ptr;
  ensureCache.prepare();
  if (vargv && typeof vargv === 'object') vargv = vargv.ptr;
  else vargv = ensureString(vargv);
  return wrapPointer(_emscripten_bind_ascjson_x__brow_relsforatom_1(self, vargv), rt);
};

/** @suppress {undefinedVars, duplicate} @this{Object} */
ascjson.prototype['x__brow_islogrel'] = ascjson.prototype.x__brow_islogrel = function(vargv) {
  var self = this.ptr;
  ensureCache.prepare();
  if (vargv && typeof vargv === 'object') vargv = vargv.ptr;
  else vargv = ensureString(vargv);
  return wrapPointer(_emscripten_bind_ascjson_x__brow_islogrel_1(self, vargv), rt);
};

/** @suppress {undefinedVars, duplicate} @this{Object} */
ascjson.prototype['bgetlogrels'] = ascjson.prototype.bgetlogrels = function(vargv) {
  var self = this.ptr;
  ensureCache.prepare();
  if (vargv && typeof vargv === 'object') vargv = vargv.ptr;
  else vargv = ensureString(vargv);
  return wrapPointer(_emscripten_bind_ascjson_bgetlogrels_1(self, vargv), rt);
};

/** @suppress {undefinedVars, duplicate} @this{Object} */
ascjson.prototype['bgetlogrelspf'] = ascjson.prototype.bgetlogrelspf = function(vargv) {
  var self = this.ptr;
  ensureCache.prepare();
  if (vargv && typeof vargv === 'object') vargv = vargv.ptr;
  else vargv = ensureString(vargv);
  return wrapPointer(_emscripten_bind_ascjson_bgetlogrelspf_1(self, vargv), rt);
};

/** @suppress {undefinedVars, duplicate} @this{Object} */
ascjson.prototype['x__brow_logrelsforatom'] = ascjson.prototype.x__brow_logrelsforatom = function(vargv) {
  var self = this.ptr;
  ensureCache.prepare();
  if (vargv && typeof vargv === 'object') vargv = vargv.ptr;
  else vargv = ensureString(vargv);
  return wrapPointer(_emscripten_bind_ascjson_x__brow_logrelsforatom_1(self, vargv), rt);
};

/** @suppress {undefinedVars, duplicate} @this{Object} */
ascjson.prototype['bgetcondrels'] = ascjson.prototype.bgetcondrels = function(vargv) {
  var self = this.ptr;
  ensureCache.prepare();
  if (vargv && typeof vargv === 'object') vargv = vargv.ptr;
  else vargv = ensureString(vargv);
  return wrapPointer(_emscripten_bind_ascjson_bgetcondrels_1(self, vargv), rt);
};

/** @suppress {undefinedVars, duplicate} @this{Object} */
ascjson.prototype['bgetcondlogrels'] = ascjson.prototype.bgetcondlogrels = function(vargv) {
  var self = this.ptr;
  ensureCache.prepare();
  if (vargv && typeof vargv === 'object') vargv = vargv.ptr;
  else vargv = ensureString(vargv);
  return wrapPointer(_emscripten_bind_ascjson_bgetcondlogrels_1(self, vargv), rt);
};

/** @suppress {undefinedVars, duplicate} @this{Object} */
ascjson.prototype['x__brow_iswhen'] = ascjson.prototype.x__brow_iswhen = function(vargv) {
  var self = this.ptr;
  ensureCache.prepare();
  if (vargv && typeof vargv === 'object') vargv = vargv.ptr;
  else vargv = ensureString(vargv);
  return wrapPointer(_emscripten_bind_ascjson_x__brow_iswhen_1(self, vargv), rt);
};

/** @suppress {undefinedVars, duplicate} @this{Object} */
ascjson.prototype['x__brow_isinstanceinwhen'] = ascjson.prototype.x__brow_isinstanceinwhen = function(vargv) {
  var self = this.ptr;
  ensureCache.prepare();
  if (vargv && typeof vargv === 'object') vargv = vargv.ptr;
  else vargv = ensureString(vargv);
  return wrapPointer(_emscripten_bind_ascjson_x__brow_isinstanceinwhen_1(self, vargv), rt);
};

/** @suppress {undefinedVars, duplicate} @this{Object} */
ascjson.prototype['bgetwhens'] = ascjson.prototype.bgetwhens = function(vargv) {
  var self = this.ptr;
  ensureCache.prepare();
  if (vargv && typeof vargv === 'object') vargv = vargv.ptr;
  else vargv = ensureString(vargv);
  return wrapPointer(_emscripten_bind_ascjson_bgetwhens_1(self, vargv), rt);
};

/** @suppress {undefinedVars, duplicate} @this{Object} */
ascjson.prototype['x__brow_whensforinstance'] = ascjson.prototype.x__brow_whensforinstance = function(vargv) {
  var self = this.ptr;
  ensureCache.prepare();
  if (vargv && typeof vargv === 'object') vargv = vargv.ptr;
  else vargv = ensureString(vargv);
  return wrapPointer(_emscripten_bind_ascjson_x__brow_whensforinstance_1(self, vargv), rt);
};

/** @suppress {undefinedVars, duplicate} @this{Object} */
ascjson.prototype['bwritependings'] = ascjson.prototype.bwritependings = function(vargv) {
  var self = this.ptr;
  ensureCache.prepare();
  if (vargv && typeof vargv === 'object') vargv = vargv.ptr;
  else vargv = ensureString(vargv);
  return wrapPointer(_emscripten_bind_ascjson_bwritependings_1(self, vargv), rt);
};

/** @suppress {undefinedVars, duplicate} @this{Object} */
ascjson.prototype['bnumpendings'] = ascjson.prototype.bnumpendings = function(vargv) {
  var self = this.ptr;
  ensureCache.prepare();
  if (vargv && typeof vargv === 'object') vargv = vargv.ptr;
  else vargv = ensureString(vargv);
  return wrapPointer(_emscripten_bind_ascjson_bnumpendings_1(self, vargv), rt);
};

/** @suppress {undefinedVars, duplicate} @this{Object} */
ascjson.prototype['count_names'] = ascjson.prototype.count_names = function(vargv) {
  var self = this.ptr;
  ensureCache.prepare();
  if (vargv && typeof vargv === 'object') vargv = vargv.ptr;
  else vargv = ensureString(vargv);
  return wrapPointer(_emscripten_bind_ascjson_count_names_1(self, vargv), rt);
};

/** @suppress {undefinedVars, duplicate} @this{Object} */
ascjson.prototype['aliases'] = ascjson.prototype.aliases = function(vargv) {
  var self = this.ptr;
  ensureCache.prepare();
  if (vargv && typeof vargv === 'object') vargv = vargv.ptr;
  else vargv = ensureString(vargv);
  return wrapPointer(_emscripten_bind_ascjson_aliases_1(self, vargv), rt);
};

/** @suppress {undefinedVars, duplicate} @this{Object} */
ascjson.prototype['isas'] = ascjson.prototype.isas = function(vargv) {
  var self = this.ptr;
  ensureCache.prepare();
  if (vargv && typeof vargv === 'object') vargv = vargv.ptr;
  else vargv = ensureString(vargv);
  return wrapPointer(_emscripten_bind_ascjson_isas_1(self, vargv), rt);
};

/** @suppress {undefinedVars, duplicate} @this{Object} */
ascjson.prototype['cliques'] = ascjson.prototype.cliques = function(vargv) {
  var self = this.ptr;
  ensureCache.prepare();
  if (vargv && typeof vargv === 'object') vargv = vargv.ptr;
  else vargv = ensureString(vargv);
  return wrapPointer(_emscripten_bind_ascjson_cliques_1(self, vargv), rt);
};

/** @suppress {undefinedVars, duplicate} @this{Object} */
ascjson.prototype['brow_child_list'] = ascjson.prototype.brow_child_list = function(vargv) {
  var self = this.ptr;
  ensureCache.prepare();
  if (vargv && typeof vargv === 'object') vargv = vargv.ptr;
  else vargv = ensureString(vargv);
  return wrapPointer(_emscripten_bind_ascjson_brow_child_list_1(self, vargv), rt);
};

/** @suppress {undefinedVars, duplicate} @this{Object} */
ascjson.prototype['bmerge'] = ascjson.prototype.bmerge = function(vargv) {
  var self = this.ptr;
  ensureCache.prepare();
  if (vargv && typeof vargv === 'object') vargv = vargv.ptr;
  else vargv = ensureString(vargv);
  return wrapPointer(_emscripten_bind_ascjson_bmerge_1(self, vargv), rt);
};

/** @suppress {undefinedVars, duplicate} @this{Object} */
ascjson.prototype['brefine'] = ascjson.prototype.brefine = function(vargv) {
  var self = this.ptr;
  ensureCache.prepare();
  if (vargv && typeof vargv === 'object') vargv = vargv.ptr;
  else vargv = ensureString(vargv);
  return wrapPointer(_emscripten_bind_ascjson_brefine_1(self, vargv), rt);
};

/** @suppress {undefinedVars, duplicate} @this{Object} */
ascjson.prototype['bmakealike'] = ascjson.prototype.bmakealike = function(vargv) {
  var self = this.ptr;
  ensureCache.prepare();
  if (vargv && typeof vargv === 'object') vargv = vargv.ptr;
  else vargv = ensureString(vargv);
  return wrapPointer(_emscripten_bind_ascjson_bmakealike_1(self, vargv), rt);
};

/** @suppress {undefinedVars, duplicate} @this{Object} */
ascjson.prototype['b_isplottable'] = ascjson.prototype.b_isplottable = function(vargv) {
  var self = this.ptr;
  ensureCache.prepare();
  if (vargv && typeof vargv === 'object') vargv = vargv.ptr;
  else vargv = ensureString(vargv);
  return wrapPointer(_emscripten_bind_ascjson_b_isplottable_1(self, vargv), rt);
};

/** @suppress {undefinedVars, duplicate} @this{Object} */
ascjson.prototype['b_prepplotfile'] = ascjson.prototype.b_prepplotfile = function(vargv) {
  var self = this.ptr;
  ensureCache.prepare();
  if (vargv && typeof vargv === 'object') vargv = vargv.ptr;
  else vargv = ensureString(vargv);
  return wrapPointer(_emscripten_bind_ascjson_b_prepplotfile_1(self, vargv), rt);
};

/** @suppress {undefinedVars, duplicate} @this{Object} */
ascjson.prototype['free_all_vars'] = ascjson.prototype.free_all_vars = function(vargv) {
  var self = this.ptr;
  ensureCache.prepare();
  if (vargv && typeof vargv === 'object') vargv = vargv.ptr;
  else vargv = ensureString(vargv);
  return wrapPointer(_emscripten_bind_ascjson_free_all_vars_1(self, vargv), rt);
};

/** @suppress {undefinedVars, duplicate} @this{Object} */
ascjson.prototype['Asc_SimBinTokenSetOptionsHC'] = ascjson.prototype.Asc_SimBinTokenSetOptionsHC = function(vargv) {
  var self = this.ptr;
  ensureCache.prepare();
  if (vargv && typeof vargv === 'object') vargv = vargv.ptr;
  else vargv = ensureString(vargv);
  return wrapPointer(_emscripten_bind_ascjson_Asc_SimBinTokenSetOptionsHC_1(self, vargv), rt);
};

/** @suppress {undefinedVars, duplicate} @this{Object} */
ascjson.prototype['sims'] = ascjson.prototype.sims = function(vargv) {
  var self = this.ptr;
  ensureCache.prepare();
  if (vargv && typeof vargv === 'object') vargv = vargv.ptr;
  else vargv = ensureString(vargv);
  return wrapPointer(_emscripten_bind_ascjson_sims_1(self, vargv), rt);
};

/** @suppress {undefinedVars, duplicate} @this{Object} */
ascjson.prototype['sim_instantiate'] = ascjson.prototype.sim_instantiate = function(vargv) {
  var self = this.ptr;
  ensureCache.prepare();
  if (vargv && typeof vargv === 'object') vargv = vargv.ptr;
  else vargv = ensureString(vargv);
  return wrapPointer(_emscripten_bind_ascjson_sim_instantiate_1(self, vargv), rt);
};

/** @suppress {undefinedVars, duplicate} @this{Object} */
ascjson.prototype['sim_unique'] = ascjson.prototype.sim_unique = function(vargv) {
  var self = this.ptr;
  ensureCache.prepare();
  if (vargv && typeof vargv === 'object') vargv = vargv.ptr;
  else vargv = ensureString(vargv);
  return wrapPointer(_emscripten_bind_ascjson_sim_unique_1(self, vargv), rt);
};

/** @suppress {undefinedVars, duplicate} @this{Object} */
ascjson.prototype['sim_reinstantiate'] = ascjson.prototype.sim_reinstantiate = function(vargv) {
  var self = this.ptr;
  ensureCache.prepare();
  if (vargv && typeof vargv === 'object') vargv = vargv.ptr;
  else vargv = ensureString(vargv);
  return wrapPointer(_emscripten_bind_ascjson_sim_reinstantiate_1(self, vargv), rt);
};

/** @suppress {undefinedVars, duplicate} @this{Object} */
ascjson.prototype['x__sims_copy'] = ascjson.prototype.x__sims_copy = function(vargv) {
  var self = this.ptr;
  ensureCache.prepare();
  if (vargv && typeof vargv === 'object') vargv = vargv.ptr;
  else vargv = ensureString(vargv);
  return wrapPointer(_emscripten_bind_ascjson_x__sims_copy_1(self, vargv), rt);
};

/** @suppress {undefinedVars, duplicate} @this{Object} */
ascjson.prototype['x__sims_proto'] = ascjson.prototype.x__sims_proto = function(vargv) {
  var self = this.ptr;
  ensureCache.prepare();
  if (vargv && typeof vargv === 'object') vargv = vargv.ptr;
  else vargv = ensureString(vargv);
  return wrapPointer(_emscripten_bind_ascjson_x__sims_proto_1(self, vargv), rt);
};

/** @suppress {undefinedVars, duplicate} @this{Object} */
ascjson.prototype['x__sims_saveinst'] = ascjson.prototype.x__sims_saveinst = function(vargv) {
  var self = this.ptr;
  ensureCache.prepare();
  if (vargv && typeof vargv === 'object') vargv = vargv.ptr;
  else vargv = ensureString(vargv);
  return wrapPointer(_emscripten_bind_ascjson_x__sims_saveinst_1(self, vargv), rt);
};

/** @suppress {undefinedVars, duplicate} @this{Object} */
ascjson.prototype['sim_destroy'] = ascjson.prototype.sim_destroy = function(vargv) {
  var self = this.ptr;
  ensureCache.prepare();
  if (vargv && typeof vargv === 'object') vargv = vargv.ptr;
  else vargv = ensureString(vargv);
  return wrapPointer(_emscripten_bind_ascjson_sim_destroy_1(self, vargv), rt);
};

/** @suppress {undefinedVars, duplicate} @this{Object} */
ascjson.prototype['simlistpending'] = ascjson.prototype.simlistpending = function(vargv) {
  var self = this.ptr;
  ensureCache.prepare();
  if (vargv && typeof vargv === 'object') vargv = vargv.ptr;
  else vargv = ensureString(vargv);
  return wrapPointer(_emscripten_bind_ascjson_simlistpending_1(self, vargv), rt);
};

/** @suppress {undefinedVars, duplicate} @this{Object} */
ascjson.prototype['ddefine'] = ascjson.prototype.ddefine = function(vargv) {
  var self = this.ptr;
  ensureCache.prepare();
  if (vargv && typeof vargv === 'object') vargv = vargv.ptr;
  else vargv = ensureString(vargv);
  return wrapPointer(_emscripten_bind_ascjson_ddefine_1(self, vargv), rt);
};

/** @suppress {undefinedVars, duplicate} @this{Object} */
ascjson.prototype['ddiffdefine'] = ascjson.prototype.ddiffdefine = function(vargv) {
  var self = this.ptr;
  ensureCache.prepare();
  if (vargv && typeof vargv === 'object') vargv = vargv.ptr;
  else vargv = ensureString(vargv);
  return wrapPointer(_emscripten_bind_ascjson_ddiffdefine_1(self, vargv), rt);
};

/** @suppress {undefinedVars, duplicate} @this{Object} */
ascjson.prototype['disp'] = ascjson.prototype.disp = function(vargv) {
  var self = this.ptr;
  ensureCache.prepare();
  if (vargv && typeof vargv === 'object') vargv = vargv.ptr;
  else vargv = ensureString(vargv);
  return wrapPointer(_emscripten_bind_ascjson_disp_1(self, vargv), rt);
};

/** @suppress {undefinedVars, duplicate} @this{Object} */
ascjson.prototype['hier'] = ascjson.prototype.hier = function(vargv) {
  var self = this.ptr;
  ensureCache.prepare();
  if (vargv && typeof vargv === 'object') vargv = vargv.ptr;
  else vargv = ensureString(vargv);
  return wrapPointer(_emscripten_bind_ascjson_hier_1(self, vargv), rt);
};

/** @suppress {undefinedVars, duplicate} @this{Object} */
ascjson.prototype['file_by_type'] = ascjson.prototype.file_by_type = function(vargv) {
  var self = this.ptr;
  ensureCache.prepare();
  if (vargv && typeof vargv === 'object') vargv = vargv.ptr;
  else vargv = ensureString(vargv);
  return wrapPointer(_emscripten_bind_ascjson_file_by_type_1(self, vargv), rt);
};

/** @suppress {undefinedVars, duplicate} @this{Object} */
ascjson.prototype['dchild'] = ascjson.prototype.dchild = function(vargv) {
  var self = this.ptr;
  ensureCache.prepare();
  if (vargv && typeof vargv === 'object') vargv = vargv.ptr;
  else vargv = ensureString(vargv);
  return wrapPointer(_emscripten_bind_ascjson_dchild_1(self, vargv), rt);
};

/** @suppress {undefinedVars, duplicate} @this{Object} */
ascjson.prototype['drefines_me'] = ascjson.prototype.drefines_me = function(vargv) {
  var self = this.ptr;
  ensureCache.prepare();
  if (vargv && typeof vargv === 'object') vargv = vargv.ptr;
  else vargv = ensureString(vargv);
  return wrapPointer(_emscripten_bind_ascjson_drefines_me_1(self, vargv), rt);
};

/** @suppress {undefinedVars, duplicate} @this{Object} */
ascjson.prototype['drefines_meall'] = ascjson.prototype.drefines_meall = function(vargv) {
  var self = this.ptr;
  ensureCache.prepare();
  if (vargv && typeof vargv === 'object') vargv = vargv.ptr;
  else vargv = ensureString(vargv);
  return wrapPointer(_emscripten_bind_ascjson_drefines_meall_1(self, vargv), rt);
};

/** @suppress {undefinedVars, duplicate} @this{Object} */
ascjson.prototype['drefinement_tree'] = ascjson.prototype.drefinement_tree = function(vargv) {
  var self = this.ptr;
  ensureCache.prepare();
  if (vargv && typeof vargv === 'object') vargv = vargv.ptr;
  else vargv = ensureString(vargv);
  return wrapPointer(_emscripten_bind_ascjson_drefinement_tree_1(self, vargv), rt);
};

/** @suppress {undefinedVars, duplicate} @this{Object} */
ascjson.prototype['dgetparts'] = ascjson.prototype.dgetparts = function(vargv) {
  var self = this.ptr;
  ensureCache.prepare();
  if (vargv && typeof vargv === 'object') vargv = vargv.ptr;
  else vargv = ensureString(vargv);
  return wrapPointer(_emscripten_bind_ascjson_dgetparts_1(self, vargv), rt);
};

/** @suppress {undefinedVars, duplicate} @this{Object} */
ascjson.prototype['disroot_type'] = ascjson.prototype.disroot_type = function(vargv) {
  var self = this.ptr;
  ensureCache.prepare();
  if (vargv && typeof vargv === 'object') vargv = vargv.ptr;
  else vargv = ensureString(vargv);
  return wrapPointer(_emscripten_bind_ascjson_disroot_type_1(self, vargv), rt);
};

/** @suppress {undefinedVars, duplicate} @this{Object} */
ascjson.prototype['x__probe'] = ascjson.prototype.x__probe = function(vargv) {
  var self = this.ptr;
  ensureCache.prepare();
  if (vargv && typeof vargv === 'object') vargv = vargv.ptr;
  else vargv = ensureString(vargv);
  return wrapPointer(_emscripten_bind_ascjson_x__probe_1(self, vargv), rt);
};

/** @suppress {undefinedVars, duplicate} @this{Object} */
ascjson.prototype['x__var_analyze'] = ascjson.prototype.x__var_analyze = function(vargv) {
  var self = this.ptr;
  ensureCache.prepare();
  if (vargv && typeof vargv === 'object') vargv = vargv.ptr;
  else vargv = ensureString(vargv);
  return wrapPointer(_emscripten_bind_ascjson_x__var_analyze_1(self, vargv), rt);
};

/** @suppress {undefinedVars, duplicate} @this{Object} */
ascjson.prototype['x__rel_analyze'] = ascjson.prototype.x__rel_analyze = function(vargv) {
  var self = this.ptr;
  ensureCache.prepare();
  if (vargv && typeof vargv === 'object') vargv = vargv.ptr;
  else vargv = ensureString(vargv);
  return wrapPointer(_emscripten_bind_ascjson_x__rel_analyze_1(self, vargv), rt);
};

/** @suppress {undefinedVars, duplicate} @this{Object} */
ascjson.prototype['x__userdata_init'] = ascjson.prototype.x__userdata_init = function(vargv) {
  var self = this.ptr;
  ensureCache.prepare();
  if (vargv && typeof vargv === 'object') vargv = vargv.ptr;
  else vargv = ensureString(vargv);
  return wrapPointer(_emscripten_bind_ascjson_x__userdata_init_1(self, vargv), rt);
};

/** @suppress {undefinedVars, duplicate} @this{Object} */
ascjson.prototype['x__userdata_create'] = ascjson.prototype.x__userdata_create = function(vargv) {
  var self = this.ptr;
  ensureCache.prepare();
  if (vargv && typeof vargv === 'object') vargv = vargv.ptr;
  else vargv = ensureString(vargv);
  return wrapPointer(_emscripten_bind_ascjson_x__userdata_create_1(self, vargv), rt);
};

/** @suppress {undefinedVars, duplicate} @this{Object} */
ascjson.prototype['x__userdata_destroy'] = ascjson.prototype.x__userdata_destroy = function(vargv) {
  var self = this.ptr;
  ensureCache.prepare();
  if (vargv && typeof vargv === 'object') vargv = vargv.ptr;
  else vargv = ensureString(vargv);
  return wrapPointer(_emscripten_bind_ascjson_x__userdata_destroy_1(self, vargv), rt);
};

/** @suppress {undefinedVars, duplicate} @this{Object} */
ascjson.prototype['x__userdata_save'] = ascjson.prototype.x__userdata_save = function(vargv) {
  var self = this.ptr;
  ensureCache.prepare();
  if (vargv && typeof vargv === 'object') vargv = vargv.ptr;
  else vargv = ensureString(vargv);
  return wrapPointer(_emscripten_bind_ascjson_x__userdata_save_1(self, vargv), rt);
};

/** @suppress {undefinedVars, duplicate} @this{Object} */
ascjson.prototype['x__userdata_restore'] = ascjson.prototype.x__userdata_restore = function(vargv) {
  var self = this.ptr;
  ensureCache.prepare();
  if (vargv && typeof vargv === 'object') vargv = vargv.ptr;
  else vargv = ensureString(vargv);
  return wrapPointer(_emscripten_bind_ascjson_x__userdata_restore_1(self, vargv), rt);
};

/** @suppress {undefinedVars, duplicate} @this{Object} */
ascjson.prototype['x__userdata_query'] = ascjson.prototype.x__userdata_query = function(vargv) {
  var self = this.ptr;
  ensureCache.prepare();
  if (vargv && typeof vargv === 'object') vargv = vargv.ptr;
  else vargv = ensureString(vargv);
  return wrapPointer(_emscripten_bind_ascjson_x__userdata_query_1(self, vargv), rt);
};

/** @suppress {undefinedVars, duplicate} @this{Object} */
ascjson.prototype['x__userdata_print'] = ascjson.prototype.x__userdata_print = function(vargv) {
  var self = this.ptr;
  ensureCache.prepare();
  if (vargv && typeof vargv === 'object') vargv = vargv.ptr;
  else vargv = ensureString(vargv);
  return wrapPointer(_emscripten_bind_ascjson_x__userdata_print_1(self, vargv), rt);
};

/** @suppress {undefinedVars, duplicate} @this{Object} */
ascjson.prototype['get_model_children'] = ascjson.prototype.get_model_children = function(vargv) {
  var self = this.ptr;
  ensureCache.prepare();
  if (vargv && typeof vargv === 'object') vargv = vargv.ptr;
  else vargv = ensureString(vargv);
  return wrapPointer(_emscripten_bind_ascjson_get_model_children_1(self, vargv), rt);
};

/** @suppress {undefinedVars, duplicate} @this{Object} */
ascjson.prototype['slv_checksim'] = ascjson.prototype.slv_checksim = function(vargv) {
  var self = this.ptr;
  ensureCache.prepare();
  if (vargv && typeof vargv === 'object') vargv = vargv.ptr;
  else vargv = ensureString(vargv);
  return wrapPointer(_emscripten_bind_ascjson_slv_checksim_1(self, vargv), rt);
};

/** @suppress {undefinedVars, duplicate} @this{Object} */
ascjson.prototype['slv_checksys'] = ascjson.prototype.slv_checksys = function(vargv) {
  var self = this.ptr;
  ensureCache.prepare();
  if (vargv && typeof vargv === 'object') vargv = vargv.ptr;
  else vargv = ensureString(vargv);
  return wrapPointer(_emscripten_bind_ascjson_slv_checksys_1(self, vargv), rt);
};

/** @suppress {undefinedVars, duplicate} @this{Object} */
ascjson.prototype['slv_get_obj_list'] = ascjson.prototype.slv_get_obj_list = function(vargv) {
  var self = this.ptr;
  ensureCache.prepare();
  if (vargv && typeof vargv === 'object') vargv = vargv.ptr;
  else vargv = ensureString(vargv);
  return wrapPointer(_emscripten_bind_ascjson_slv_get_obj_list_1(self, vargv), rt);
};

/** @suppress {undefinedVars, duplicate} @this{Object} */
ascjson.prototype['slv_set_obj_by_num'] = ascjson.prototype.slv_set_obj_by_num = function(vargv) {
  var self = this.ptr;
  ensureCache.prepare();
  if (vargv && typeof vargv === 'object') vargv = vargv.ptr;
  else vargv = ensureString(vargv);
  return wrapPointer(_emscripten_bind_ascjson_slv_set_obj_by_num_1(self, vargv), rt);
};

/** @suppress {undefinedVars, duplicate} @this{Object} */
ascjson.prototype['slv_get_obj_num'] = ascjson.prototype.slv_get_obj_num = function(vargv) {
  var self = this.ptr;
  ensureCache.prepare();
  if (vargv && typeof vargv === 'object') vargv = vargv.ptr;
  else vargv = ensureString(vargv);
  return wrapPointer(_emscripten_bind_ascjson_slv_get_obj_num_1(self, vargv), rt);
};

/** @suppress {undefinedVars, duplicate} @this{Object} */
ascjson.prototype['slv_get_parms'] = ascjson.prototype.slv_get_parms = function(vargv) {
  var self = this.ptr;
  ensureCache.prepare();
  if (vargv && typeof vargv === 'object') vargv = vargv.ptr;
  else vargv = ensureString(vargv);
  return wrapPointer(_emscripten_bind_ascjson_slv_get_parms_1(self, vargv), rt);
};

/** @suppress {undefinedVars, duplicate} @this{Object} */
ascjson.prototype['set_slv_parms'] = ascjson.prototype.set_slv_parms = function(vargv) {
  var self = this.ptr;
  ensureCache.prepare();
  if (vargv && typeof vargv === 'object') vargv = vargv.ptr;
  else vargv = ensureString(vargv);
  return wrapPointer(_emscripten_bind_ascjson_set_slv_parms_1(self, vargv), rt);
};

/** @suppress {undefinedVars, duplicate} @this{Object} */
ascjson.prototype['set_slv_parmsnew'] = ascjson.prototype.set_slv_parmsnew = function(vargv) {
  var self = this.ptr;
  ensureCache.prepare();
  if (vargv && typeof vargv === 'object') vargv = vargv.ptr;
  else vargv = ensureString(vargv);
  return wrapPointer(_emscripten_bind_ascjson_set_slv_parmsnew_1(self, vargv), rt);
};

/** @suppress {undefinedVars, duplicate} @this{Object} */
ascjson.prototype['slv_get_parmsnew'] = ascjson.prototype.slv_get_parmsnew = function(vargv) {
  var self = this.ptr;
  ensureCache.prepare();
  if (vargv && typeof vargv === 'object') vargv = vargv.ptr;
  else vargv = ensureString(vargv);
  return wrapPointer(_emscripten_bind_ascjson_slv_get_parmsnew_1(self, vargv), rt);
};

/** @suppress {undefinedVars, duplicate} @this{Object} */
ascjson.prototype['slv_get_insttype'] = ascjson.prototype.slv_get_insttype = function(vargv) {
  var self = this.ptr;
  ensureCache.prepare();
  if (vargv && typeof vargv === 'object') vargv = vargv.ptr;
  else vargv = ensureString(vargv);
  return wrapPointer(_emscripten_bind_ascjson_slv_get_insttype_1(self, vargv), rt);
};

/** @suppress {undefinedVars, duplicate} @this{Object} */
ascjson.prototype['slv_get_cost_page'] = ascjson.prototype.slv_get_cost_page = function(vargv) {
  var self = this.ptr;
  ensureCache.prepare();
  if (vargv && typeof vargv === 'object') vargv = vargv.ptr;
  else vargv = ensureString(vargv);
  return wrapPointer(_emscripten_bind_ascjson_slv_get_cost_page_1(self, vargv), rt);
};

/** @suppress {undefinedVars, duplicate} @this{Object} */
ascjson.prototype['slv_get_stat_page'] = ascjson.prototype.slv_get_stat_page = function(vargv) {
  var self = this.ptr;
  ensureCache.prepare();
  if (vargv && typeof vargv === 'object') vargv = vargv.ptr;
  else vargv = ensureString(vargv);
  return wrapPointer(_emscripten_bind_ascjson_slv_get_stat_page_1(self, vargv), rt);
};

/** @suppress {undefinedVars, duplicate} @this{Object} */
ascjson.prototype['slv_get_objval'] = ascjson.prototype.slv_get_objval = function(vargv) {
  var self = this.ptr;
  ensureCache.prepare();
  if (vargv && typeof vargv === 'object') vargv = vargv.ptr;
  else vargv = ensureString(vargv);
  return wrapPointer(_emscripten_bind_ascjson_slv_get_objval_1(self, vargv), rt);
};

/** @suppress {undefinedVars, duplicate} @this{Object} */
ascjson.prototype['slv_get_instname'] = ascjson.prototype.slv_get_instname = function(vargv) {
  var self = this.ptr;
  ensureCache.prepare();
  if (vargv && typeof vargv === 'object') vargv = vargv.ptr;
  else vargv = ensureString(vargv);
  return wrapPointer(_emscripten_bind_ascjson_slv_get_instname_1(self, vargv), rt);
};

/** @suppress {undefinedVars, duplicate} @this{Object} */
ascjson.prototype['slv_get_pathname'] = ascjson.prototype.slv_get_pathname = function(vargv) {
  var self = this.ptr;
  ensureCache.prepare();
  if (vargv && typeof vargv === 'object') vargv = vargv.ptr;
  else vargv = ensureString(vargv);
  return wrapPointer(_emscripten_bind_ascjson_slv_get_pathname_1(self, vargv), rt);
};

/** @suppress {undefinedVars, duplicate} @this{Object} */
ascjson.prototype['slv_get_vr'] = ascjson.prototype.slv_get_vr = function(vargv) {
  var self = this.ptr;
  ensureCache.prepare();
  if (vargv && typeof vargv === 'object') vargv = vargv.ptr;
  else vargv = ensureString(vargv);
  return wrapPointer(_emscripten_bind_ascjson_slv_get_vr_1(self, vargv), rt);
};

/** @suppress {undefinedVars, duplicate} @this{Object} */
ascjson.prototype['slvdump'] = ascjson.prototype.slvdump = function(vargv) {
  var self = this.ptr;
  ensureCache.prepare();
  if (vargv && typeof vargv === 'object') vargv = vargv.ptr;
  else vargv = ensureString(vargv);
  return wrapPointer(_emscripten_bind_ascjson_slvdump_1(self, vargv), rt);
};

/** @suppress {undefinedVars, duplicate} @this{Object} */
ascjson.prototype['slv_presolve'] = ascjson.prototype.slv_presolve = function(vargv) {
  var self = this.ptr;
  ensureCache.prepare();
  if (vargv && typeof vargv === 'object') vargv = vargv.ptr;
  else vargv = ensureString(vargv);
  return wrapPointer(_emscripten_bind_ascjson_slv_presolve_1(self, vargv), rt);
};

/** @suppress {undefinedVars, duplicate} @this{Object} */
ascjson.prototype['slv_reanalyze'] = ascjson.prototype.slv_reanalyze = function(vargv) {
  var self = this.ptr;
  ensureCache.prepare();
  if (vargv && typeof vargv === 'object') vargv = vargv.ptr;
  else vargv = ensureString(vargv);
  return wrapPointer(_emscripten_bind_ascjson_slv_reanalyze_1(self, vargv), rt);
};

/** @suppress {undefinedVars, duplicate} @this{Object} */
ascjson.prototype['slv_check_and_reanalyze'] = ascjson.prototype.slv_check_and_reanalyze = function(vargv) {
  var self = this.ptr;
  ensureCache.prepare();
  if (vargv && typeof vargv === 'object') vargv = vargv.ptr;
  else vargv = ensureString(vargv);
  return wrapPointer(_emscripten_bind_ascjson_slv_check_and_reanalyze_1(self, vargv), rt);
};

/** @suppress {undefinedVars, duplicate} @this{Object} */
ascjson.prototype['slv_set_independent'] = ascjson.prototype.slv_set_independent = function(vargv) {
  var self = this.ptr;
  ensureCache.prepare();
  if (vargv && typeof vargv === 'object') vargv = vargv.ptr;
  else vargv = ensureString(vargv);
  return wrapPointer(_emscripten_bind_ascjson_slv_set_independent_1(self, vargv), rt);
};

/** @suppress {undefinedVars, duplicate} @this{Object} */
ascjson.prototype['slv_resolve'] = ascjson.prototype.slv_resolve = function(vargv) {
  var self = this.ptr;
  ensureCache.prepare();
  if (vargv && typeof vargv === 'object') vargv = vargv.ptr;
  else vargv = ensureString(vargv);
  return wrapPointer(_emscripten_bind_ascjson_slv_resolve_1(self, vargv), rt);
};

/** @suppress {undefinedVars, duplicate} @this{Object} */
ascjson.prototype['slv_solve'] = ascjson.prototype.slv_solve = function(vargv) {
  var self = this.ptr;
  ensureCache.prepare();
  if (vargv && typeof vargv === 'object') vargv = vargv.ptr;
  else vargv = ensureString(vargv);
  return wrapPointer(_emscripten_bind_ascjson_slv_solve_1(self, vargv), rt);
};

/** @suppress {undefinedVars, duplicate} @this{Object} */
ascjson.prototype['slv_iterate'] = ascjson.prototype.slv_iterate = function(vargv) {
  var self = this.ptr;
  ensureCache.prepare();
  if (vargv && typeof vargv === 'object') vargv = vargv.ptr;
  else vargv = ensureString(vargv);
  return wrapPointer(_emscripten_bind_ascjson_slv_iterate_1(self, vargv), rt);
};

/** @suppress {undefinedVars, duplicate} @this{Object} */
ascjson.prototype['slv_available'] = ascjson.prototype.slv_available = function(vargv) {
  var self = this.ptr;
  ensureCache.prepare();
  if (vargv && typeof vargv === 'object') vargv = vargv.ptr;
  else vargv = ensureString(vargv);
  return wrapPointer(_emscripten_bind_ascjson_slv_available_1(self, vargv), rt);
};

/** @suppress {undefinedVars, duplicate} @this{Object} */
ascjson.prototype['slv_number'] = ascjson.prototype.slv_number = function(vargv) {
  var self = this.ptr;
  ensureCache.prepare();
  if (vargv && typeof vargv === 'object') vargv = vargv.ptr;
  else vargv = ensureString(vargv);
  return wrapPointer(_emscripten_bind_ascjson_slv_number_1(self, vargv), rt);
};

/** @suppress {undefinedVars, duplicate} @this{Object} */
ascjson.prototype['slv_name'] = ascjson.prototype.slv_name = function(vargv) {
  var self = this.ptr;
  ensureCache.prepare();
  if (vargv && typeof vargv === 'object') vargv = vargv.ptr;
  else vargv = ensureString(vargv);
  return wrapPointer(_emscripten_bind_ascjson_slv_name_1(self, vargv), rt);
};

/** @suppress {undefinedVars, duplicate} @this{Object} */
ascjson.prototype['slv_linsol_names'] = ascjson.prototype.slv_linsol_names = function(vargv) {
  var self = this.ptr;
  ensureCache.prepare();
  if (vargv && typeof vargv === 'object') vargv = vargv.ptr;
  else vargv = ensureString(vargv);
  return wrapPointer(_emscripten_bind_ascjson_slv_linsol_names_1(self, vargv), rt);
};

/** @suppress {undefinedVars, duplicate} @this{Object} */
ascjson.prototype['slv_eligible_solver'] = ascjson.prototype.slv_eligible_solver = function(vargv) {
  var self = this.ptr;
  ensureCache.prepare();
  if (vargv && typeof vargv === 'object') vargv = vargv.ptr;
  else vargv = ensureString(vargv);
  return wrapPointer(_emscripten_bind_ascjson_slv_eligible_solver_1(self, vargv), rt);
};

/** @suppress {undefinedVars, duplicate} @this{Object} */
ascjson.prototype['slv_select_solver'] = ascjson.prototype.slv_select_solver = function(vargv) {
  var self = this.ptr;
  ensureCache.prepare();
  if (vargv && typeof vargv === 'object') vargv = vargv.ptr;
  else vargv = ensureString(vargv);
  return wrapPointer(_emscripten_bind_ascjson_slv_select_solver_1(self, vargv), rt);
};

/** @suppress {undefinedVars, duplicate} @this{Object} */
ascjson.prototype['slv_get_solver'] = ascjson.prototype.slv_get_solver = function(vargv) {
  var self = this.ptr;
  ensureCache.prepare();
  if (vargv && typeof vargv === 'object') vargv = vargv.ptr;
  else vargv = ensureString(vargv);
  return wrapPointer(_emscripten_bind_ascjson_slv_get_solver_1(self, vargv), rt);
};

/** @suppress {undefinedVars, duplicate} @this{Object} */
ascjson.prototype['slv_flush_solver'] = ascjson.prototype.slv_flush_solver = function(vargv) {
  var self = this.ptr;
  ensureCache.prepare();
  if (vargv && typeof vargv === 'object') vargv = vargv.ptr;
  else vargv = ensureString(vargv);
  return wrapPointer(_emscripten_bind_ascjson_slv_flush_solver_1(self, vargv), rt);
};

/** @suppress {undefinedVars, duplicate} @this{Object} */
ascjson.prototype['slv_import_qlfdid'] = ascjson.prototype.slv_import_qlfdid = function(vargv) {
  var self = this.ptr;
  ensureCache.prepare();
  if (vargv && typeof vargv === 'object') vargv = vargv.ptr;
  else vargv = ensureString(vargv);
  return wrapPointer(_emscripten_bind_ascjson_slv_import_qlfdid_1(self, vargv), rt);
};

/** @suppress {undefinedVars, duplicate} @this{Object} */
ascjson.prototype['slv_lnmget'] = ascjson.prototype.slv_lnmget = function(vargv) {
  var self = this.ptr;
  ensureCache.prepare();
  if (vargv && typeof vargv === 'object') vargv = vargv.ptr;
  else vargv = ensureString(vargv);
  return wrapPointer(_emscripten_bind_ascjson_slv_lnmget_1(self, vargv), rt);
};

/** @suppress {undefinedVars, duplicate} @this{Object} */
ascjson.prototype['slv_lnmset'] = ascjson.prototype.slv_lnmset = function(vargv) {
  var self = this.ptr;
  ensureCache.prepare();
  if (vargv && typeof vargv === 'object') vargv = vargv.ptr;
  else vargv = ensureString(vargv);
  return wrapPointer(_emscripten_bind_ascjson_slv_lnmset_1(self, vargv), rt);
};

/** @suppress {undefinedVars, duplicate} @this{Object} */
ascjson.prototype['slv_set_haltflag'] = ascjson.prototype.slv_set_haltflag = function(vargv) {
  var self = this.ptr;
  ensureCache.prepare();
  if (vargv && typeof vargv === 'object') vargv = vargv.ptr;
  else vargv = ensureString(vargv);
  return wrapPointer(_emscripten_bind_ascjson_slv_set_haltflag_1(self, vargv), rt);
};

/** @suppress {undefinedVars, duplicate} @this{Object} */
ascjson.prototype['slvhelp'] = ascjson.prototype.slvhelp = function(vargv) {
  var self = this.ptr;
  ensureCache.prepare();
  if (vargv && typeof vargv === 'object') vargv = vargv.ptr;
  else vargv = ensureString(vargv);
  return wrapPointer(_emscripten_bind_ascjson_slvhelp_1(self, vargv), rt);
};

/** @suppress {undefinedVars, duplicate} @this{Object} */
ascjson.prototype['slv_near_bounds'] = ascjson.prototype.slv_near_bounds = function(vargv) {
  var self = this.ptr;
  ensureCache.prepare();
  if (vargv && typeof vargv === 'object') vargv = vargv.ptr;
  else vargv = ensureString(vargv);
  return wrapPointer(_emscripten_bind_ascjson_slv_near_bounds_1(self, vargv), rt);
};

/** @suppress {undefinedVars, duplicate} @this{Object} */
ascjson.prototype['slv_far_from_nominals'] = ascjson.prototype.slv_far_from_nominals = function(vargv) {
  var self = this.ptr;
  ensureCache.prepare();
  if (vargv && typeof vargv === 'object') vargv = vargv.ptr;
  else vargv = ensureString(vargv);
  return wrapPointer(_emscripten_bind_ascjson_slv_far_from_nominals_1(self, vargv), rt);
};

/** @suppress {undefinedVars, duplicate} @this{Object} */
ascjson.prototype['slv_monitor'] = ascjson.prototype.slv_monitor = function(vargv) {
  var self = this.ptr;
  ensureCache.prepare();
  if (vargv && typeof vargv === 'object') vargv = vargv.ptr;
  else vargv = ensureString(vargv);
  return wrapPointer(_emscripten_bind_ascjson_slv_monitor_1(self, vargv), rt);
};

/** @suppress {undefinedVars, duplicate} @this{Object} */
ascjson.prototype['dbg_get_blk_of_var'] = ascjson.prototype.dbg_get_blk_of_var = function(vargv) {
  var self = this.ptr;
  ensureCache.prepare();
  if (vargv && typeof vargv === 'object') vargv = vargv.ptr;
  else vargv = ensureString(vargv);
  return wrapPointer(_emscripten_bind_ascjson_dbg_get_blk_of_var_1(self, vargv), rt);
};

/** @suppress {undefinedVars, duplicate} @this{Object} */
ascjson.prototype['dbg_get_blk_of_eqn'] = ascjson.prototype.dbg_get_blk_of_eqn = function(vargv) {
  var self = this.ptr;
  ensureCache.prepare();
  if (vargv && typeof vargv === 'object') vargv = vargv.ptr;
  else vargv = ensureString(vargv);
  return wrapPointer(_emscripten_bind_ascjson_dbg_get_blk_of_eqn_1(self, vargv), rt);
};

/** @suppress {undefinedVars, duplicate} @this{Object} */
ascjson.prototype['dbg_get_blk_coords'] = ascjson.prototype.dbg_get_blk_coords = function(vargv) {
  var self = this.ptr;
  ensureCache.prepare();
  if (vargv && typeof vargv === 'object') vargv = vargv.ptr;
  else vargv = ensureString(vargv);
  return wrapPointer(_emscripten_bind_ascjson_dbg_get_blk_coords_1(self, vargv), rt);
};

/** @suppress {undefinedVars, duplicate} @this{Object} */
ascjson.prototype['dbg_get_eqn_of_var'] = ascjson.prototype.dbg_get_eqn_of_var = function(vargv) {
  var self = this.ptr;
  ensureCache.prepare();
  if (vargv && typeof vargv === 'object') vargv = vargv.ptr;
  else vargv = ensureString(vargv);
  return wrapPointer(_emscripten_bind_ascjson_dbg_get_eqn_of_var_1(self, vargv), rt);
};

/** @suppress {undefinedVars, duplicate} @this{Object} */
ascjson.prototype['dbg_get_varpartition'] = ascjson.prototype.dbg_get_varpartition = function(vargv) {
  var self = this.ptr;
  ensureCache.prepare();
  if (vargv && typeof vargv === 'object') vargv = vargv.ptr;
  else vargv = ensureString(vargv);
  return wrapPointer(_emscripten_bind_ascjson_dbg_get_varpartition_1(self, vargv), rt);
};

/** @suppress {undefinedVars, duplicate} @this{Object} */
ascjson.prototype['dbg_get_eqnpartition'] = ascjson.prototype.dbg_get_eqnpartition = function(vargv) {
  var self = this.ptr;
  ensureCache.prepare();
  if (vargv && typeof vargv === 'object') vargv = vargv.ptr;
  else vargv = ensureString(vargv);
  return wrapPointer(_emscripten_bind_ascjson_dbg_get_eqnpartition_1(self, vargv), rt);
};

/** @suppress {undefinedVars, duplicate} @this{Object} */
ascjson.prototype['dbg_list_rels'] = ascjson.prototype.dbg_list_rels = function(vargv) {
  var self = this.ptr;
  ensureCache.prepare();
  if (vargv && typeof vargv === 'object') vargv = vargv.ptr;
  else vargv = ensureString(vargv);
  return wrapPointer(_emscripten_bind_ascjson_dbg_list_rels_1(self, vargv), rt);
};

/** @suppress {undefinedVars, duplicate} @this{Object} */
ascjson.prototype['dbg_list_vars'] = ascjson.prototype.dbg_list_vars = function(vargv) {
  var self = this.ptr;
  ensureCache.prepare();
  if (vargv && typeof vargv === 'object') vargv = vargv.ptr;
  else vargv = ensureString(vargv);
  return wrapPointer(_emscripten_bind_ascjson_dbg_list_vars_1(self, vargv), rt);
};

/** @suppress {undefinedVars, duplicate} @this{Object} */
ascjson.prototype['dbg_write_var'] = ascjson.prototype.dbg_write_var = function(vargv) {
  var self = this.ptr;
  ensureCache.prepare();
  if (vargv && typeof vargv === 'object') vargv = vargv.ptr;
  else vargv = ensureString(vargv);
  return wrapPointer(_emscripten_bind_ascjson_dbg_write_var_1(self, vargv), rt);
};

/** @suppress {undefinedVars, duplicate} @this{Object} */
ascjson.prototype['dbg_write_unattvar'] = ascjson.prototype.dbg_write_unattvar = function(vargv) {
  var self = this.ptr;
  ensureCache.prepare();
  if (vargv && typeof vargv === 'object') vargv = vargv.ptr;
  else vargv = ensureString(vargv);
  return wrapPointer(_emscripten_bind_ascjson_dbg_write_unattvar_1(self, vargv), rt);
};

/** @suppress {undefinedVars, duplicate} @this{Object} */
ascjson.prototype['brow_write_var'] = ascjson.prototype.brow_write_var = function(vargv) {
  var self = this.ptr;
  ensureCache.prepare();
  if (vargv && typeof vargv === 'object') vargv = vargv.ptr;
  else vargv = ensureString(vargv);
  return wrapPointer(_emscripten_bind_ascjson_brow_write_var_1(self, vargv), rt);
};

/** @suppress {undefinedVars, duplicate} @this{Object} */
ascjson.prototype['dbg_write_rel'] = ascjson.prototype.dbg_write_rel = function(vargv) {
  var self = this.ptr;
  ensureCache.prepare();
  if (vargv && typeof vargv === 'object') vargv = vargv.ptr;
  else vargv = ensureString(vargv);
  return wrapPointer(_emscripten_bind_ascjson_dbg_write_rel_1(self, vargv), rt);
};

/** @suppress {undefinedVars, duplicate} @this{Object} */
ascjson.prototype['brow_write_rel'] = ascjson.prototype.brow_write_rel = function(vargv) {
  var self = this.ptr;
  ensureCache.prepare();
  if (vargv && typeof vargv === 'object') vargv = vargv.ptr;
  else vargv = ensureString(vargv);
  return wrapPointer(_emscripten_bind_ascjson_brow_write_rel_1(self, vargv), rt);
};

/** @suppress {undefinedVars, duplicate} @this{Object} */
ascjson.prototype['dbg_write_obj'] = ascjson.prototype.dbg_write_obj = function(vargv) {
  var self = this.ptr;
  ensureCache.prepare();
  if (vargv && typeof vargv === 'object') vargv = vargv.ptr;
  else vargv = ensureString(vargv);
  return wrapPointer(_emscripten_bind_ascjson_dbg_write_obj_1(self, vargv), rt);
};

/** @suppress {undefinedVars, duplicate} @this{Object} */
ascjson.prototype['brow_write_obj'] = ascjson.prototype.brow_write_obj = function(vargv) {
  var self = this.ptr;
  ensureCache.prepare();
  if (vargv && typeof vargv === 'object') vargv = vargv.ptr;
  else vargv = ensureString(vargv);
  return wrapPointer(_emscripten_bind_ascjson_brow_write_obj_1(self, vargv), rt);
};

/** @suppress {undefinedVars, duplicate} @this{Object} */
ascjson.prototype['dbg_write_varattr'] = ascjson.prototype.dbg_write_varattr = function(vargv) {
  var self = this.ptr;
  ensureCache.prepare();
  if (vargv && typeof vargv === 'object') vargv = vargv.ptr;
  else vargv = ensureString(vargv);
  return wrapPointer(_emscripten_bind_ascjson_dbg_write_varattr_1(self, vargv), rt);
};

/** @suppress {undefinedVars, duplicate} @this{Object} */
ascjson.prototype['dbg_write_qlfattr'] = ascjson.prototype.dbg_write_qlfattr = function(vargv) {
  var self = this.ptr;
  ensureCache.prepare();
  if (vargv && typeof vargv === 'object') vargv = vargv.ptr;
  else vargv = ensureString(vargv);
  return wrapPointer(_emscripten_bind_ascjson_dbg_write_qlfattr_1(self, vargv), rt);
};

/** @suppress {undefinedVars, duplicate} @this{Object} */
ascjson.prototype['dbg_rel_included'] = ascjson.prototype.dbg_rel_included = function(vargv) {
  var self = this.ptr;
  ensureCache.prepare();
  if (vargv && typeof vargv === 'object') vargv = vargv.ptr;
  else vargv = ensureString(vargv);
  return wrapPointer(_emscripten_bind_ascjson_dbg_rel_included_1(self, vargv), rt);
};

/** @suppress {undefinedVars, duplicate} @this{Object} */
ascjson.prototype['dbg_var_fixed'] = ascjson.prototype.dbg_var_fixed = function(vargv) {
  var self = this.ptr;
  ensureCache.prepare();
  if (vargv && typeof vargv === 'object') vargv = vargv.ptr;
  else vargv = ensureString(vargv);
  return wrapPointer(_emscripten_bind_ascjson_dbg_var_fixed_1(self, vargv), rt);
};

/** @suppress {undefinedVars, duplicate} @this{Object} */
ascjson.prototype['dbg_get_incidence'] = ascjson.prototype.dbg_get_incidence = function(vargv) {
  var self = this.ptr;
  ensureCache.prepare();
  if (vargv && typeof vargv === 'object') vargv = vargv.ptr;
  else vargv = ensureString(vargv);
  return wrapPointer(_emscripten_bind_ascjson_dbg_get_incidence_1(self, vargv), rt);
};

/** @suppress {undefinedVars, duplicate} @this{Object} */
ascjson.prototype['dbg_get_order'] = ascjson.prototype.dbg_get_order = function(vargv) {
  var self = this.ptr;
  ensureCache.prepare();
  if (vargv && typeof vargv === 'object') vargv = vargv.ptr;
  else vargv = ensureString(vargv);
  return wrapPointer(_emscripten_bind_ascjson_dbg_get_order_1(self, vargv), rt);
};

/** @suppress {undefinedVars, duplicate} @this{Object} */
ascjson.prototype['dbg_write_incidence'] = ascjson.prototype.dbg_write_incidence = function(vargv) {
  var self = this.ptr;
  ensureCache.prepare();
  if (vargv && typeof vargv === 'object') vargv = vargv.ptr;
  else vargv = ensureString(vargv);
  return wrapPointer(_emscripten_bind_ascjson_dbg_write_incidence_1(self, vargv), rt);
};

/** @suppress {undefinedVars, duplicate} @this{Object} */
ascjson.prototype['dbg_find_eligible'] = ascjson.prototype.dbg_find_eligible = function(vargv) {
  var self = this.ptr;
  ensureCache.prepare();
  if (vargv && typeof vargv === 'object') vargv = vargv.ptr;
  else vargv = ensureString(vargv);
  return wrapPointer(_emscripten_bind_ascjson_dbg_find_eligible_1(self, vargv), rt);
};

/** @suppress {undefinedVars, duplicate} @this{Object} */
ascjson.prototype['brow_find_eligible'] = ascjson.prototype.brow_find_eligible = function(vargv) {
  var self = this.ptr;
  ensureCache.prepare();
  if (vargv && typeof vargv === 'object') vargv = vargv.ptr;
  else vargv = ensureString(vargv);
  return wrapPointer(_emscripten_bind_ascjson_brow_find_eligible_1(self, vargv), rt);
};

/** @suppress {undefinedVars, duplicate} @this{Object} */
ascjson.prototype['dbg_consistency_analysis'] = ascjson.prototype.dbg_consistency_analysis = function(vargv) {
  var self = this.ptr;
  ensureCache.prepare();
  if (vargv && typeof vargv === 'object') vargv = vargv.ptr;
  else vargv = ensureString(vargv);
  return wrapPointer(_emscripten_bind_ascjson_dbg_consistency_analysis_1(self, vargv), rt);
};

/** @suppress {undefinedVars, duplicate} @this{Object} */
ascjson.prototype['dbg_global_eligible'] = ascjson.prototype.dbg_global_eligible = function(vargv) {
  var self = this.ptr;
  ensureCache.prepare();
  if (vargv && typeof vargv === 'object') vargv = vargv.ptr;
  else vargv = ensureString(vargv);
  return wrapPointer(_emscripten_bind_ascjson_dbg_global_eligible_1(self, vargv), rt);
};

/** @suppress {undefinedVars, duplicate} @this{Object} */
ascjson.prototype['dbg_find_activerels'] = ascjson.prototype.dbg_find_activerels = function(vargv) {
  var self = this.ptr;
  ensureCache.prepare();
  if (vargv && typeof vargv === 'object') vargv = vargv.ptr;
  else vargv = ensureString(vargv);
  return wrapPointer(_emscripten_bind_ascjson_dbg_find_activerels_1(self, vargv), rt);
};

/** @suppress {undefinedVars, duplicate} @this{Object} */
ascjson.prototype['brow_find_activerels'] = ascjson.prototype.brow_find_activerels = function(vargv) {
  var self = this.ptr;
  ensureCache.prepare();
  if (vargv && typeof vargv === 'object') vargv = vargv.ptr;
  else vargv = ensureString(vargv);
  return wrapPointer(_emscripten_bind_ascjson_brow_find_activerels_1(self, vargv), rt);
};

/** @suppress {undefinedVars, duplicate} @this{Object} */
ascjson.prototype['dbg_struct_singular'] = ascjson.prototype.dbg_struct_singular = function(vargv) {
  var self = this.ptr;
  ensureCache.prepare();
  if (vargv && typeof vargv === 'object') vargv = vargv.ptr;
  else vargv = ensureString(vargv);
  return wrapPointer(_emscripten_bind_ascjson_dbg_struct_singular_1(self, vargv), rt);
};

/** @suppress {undefinedVars, duplicate} @this{Object} */
ascjson.prototype['dbg_num_block_singular'] = ascjson.prototype.dbg_num_block_singular = function(vargv) {
  var self = this.ptr;
  ensureCache.prepare();
  if (vargv && typeof vargv === 'object') vargv = vargv.ptr;
  else vargv = ensureString(vargv);
  return wrapPointer(_emscripten_bind_ascjson_dbg_num_block_singular_1(self, vargv), rt);
};

/** @suppress {undefinedVars, duplicate} @this{Object} */
ascjson.prototype['var_free2nom'] = ascjson.prototype.var_free2nom = function(vargv) {
  var self = this.ptr;
  ensureCache.prepare();
  if (vargv && typeof vargv === 'object') vargv = vargv.ptr;
  else vargv = ensureString(vargv);
  return wrapPointer(_emscripten_bind_ascjson_var_free2nom_1(self, vargv), rt);
};

/** @suppress {undefinedVars, duplicate} @this{Object} */
ascjson.prototype['var_nom2free'] = ascjson.prototype.var_nom2free = function(vargv) {
  var self = this.ptr;
  ensureCache.prepare();
  if (vargv && typeof vargv === 'object') vargv = vargv.ptr;
  else vargv = ensureString(vargv);
  return wrapPointer(_emscripten_bind_ascjson_var_nom2free_1(self, vargv), rt);
};

/** @suppress {undefinedVars, duplicate} @this{Object} */
ascjson.prototype['dbg_calc_relnoms'] = ascjson.prototype.dbg_calc_relnoms = function(vargv) {
  var self = this.ptr;
  ensureCache.prepare();
  if (vargv && typeof vargv === 'object') vargv = vargv.ptr;
  else vargv = ensureString(vargv);
  return wrapPointer(_emscripten_bind_ascjson_dbg_calc_relnoms_1(self, vargv), rt);
};

/** @suppress {undefinedVars, duplicate} @this{Object} */
ascjson.prototype['dbg_check_rels'] = ascjson.prototype.dbg_check_rels = function(vargv) {
  var self = this.ptr;
  ensureCache.prepare();
  if (vargv && typeof vargv === 'object') vargv = vargv.ptr;
  else vargv = ensureString(vargv);
  return wrapPointer(_emscripten_bind_ascjson_dbg_check_rels_1(self, vargv), rt);
};

/** @suppress {undefinedVars, duplicate} @this{Object} */
ascjson.prototype['dbg_write_slv0_xsys'] = ascjson.prototype.dbg_write_slv0_xsys = function(vargv) {
  var self = this.ptr;
  ensureCache.prepare();
  if (vargv && typeof vargv === 'object') vargv = vargv.ptr;
  else vargv = ensureString(vargv);
  return wrapPointer(_emscripten_bind_ascjson_dbg_write_slv0_xsys_1(self, vargv), rt);
};

/** @suppress {undefinedVars, duplicate} @this{Object} */
ascjson.prototype['dbg_write_slv0_sys'] = ascjson.prototype.dbg_write_slv0_sys = function(vargv) {
  var self = this.ptr;
  ensureCache.prepare();
  if (vargv && typeof vargv === 'object') vargv = vargv.ptr;
  else vargv = ensureString(vargv);
  return wrapPointer(_emscripten_bind_ascjson_dbg_write_slv0_sys_1(self, vargv), rt);
};

/** @suppress {undefinedVars, duplicate} @this{Object} */
ascjson.prototype['dbg_mtxwriteplot'] = ascjson.prototype.dbg_mtxwriteplot = function(vargv) {
  var self = this.ptr;
  ensureCache.prepare();
  if (vargv && typeof vargv === 'object') vargv = vargv.ptr;
  else vargv = ensureString(vargv);
  return wrapPointer(_emscripten_bind_ascjson_dbg_mtxwriteplot_1(self, vargv), rt);
};

/** @suppress {undefinedVars, duplicate} @this{Object} */
ascjson.prototype['dbg_calc_jacobian'] = ascjson.prototype.dbg_calc_jacobian = function(vargv) {
  var self = this.ptr;
  ensureCache.prepare();
  if (vargv && typeof vargv === 'object') vargv = vargv.ptr;
  else vargv = ensureString(vargv);
  return wrapPointer(_emscripten_bind_ascjson_dbg_calc_jacobian_1(self, vargv), rt);
};

/** @suppress {undefinedVars, duplicate} @this{Object} */
ascjson.prototype['dbghelp'] = ascjson.prototype.dbghelp = function(vargv) {
  var self = this.ptr;
  ensureCache.prepare();
  if (vargv && typeof vargv === 'object') vargv = vargv.ptr;
  else vargv = ensureString(vargv);
  return wrapPointer(_emscripten_bind_ascjson_dbghelp_1(self, vargv), rt);
};

/** @suppress {undefinedVars, duplicate} @this{Object} */
ascjson.prototype['integrate_setup'] = ascjson.prototype.integrate_setup = function(vargv) {
  var self = this.ptr;
  ensureCache.prepare();
  if (vargv && typeof vargv === 'object') vargv = vargv.ptr;
  else vargv = ensureString(vargv);
  return wrapPointer(_emscripten_bind_ascjson_integrate_setup_1(self, vargv), rt);
};

/** @suppress {undefinedVars, duplicate} @this{Object} */
ascjson.prototype['integrate_cleanup'] = ascjson.prototype.integrate_cleanup = function(vargv) {
  var self = this.ptr;
  ensureCache.prepare();
  if (vargv && typeof vargv === 'object') vargv = vargv.ptr;
  else vargv = ensureString(vargv);
  return wrapPointer(_emscripten_bind_ascjson_integrate_cleanup_1(self, vargv), rt);
};

/** @suppress {undefinedVars, duplicate} @this{Object} */
ascjson.prototype['integrate_set_y_file'] = ascjson.prototype.integrate_set_y_file = function(vargv) {
  var self = this.ptr;
  ensureCache.prepare();
  if (vargv && typeof vargv === 'object') vargv = vargv.ptr;
  else vargv = ensureString(vargv);
  return wrapPointer(_emscripten_bind_ascjson_integrate_set_y_file_1(self, vargv), rt);
};

/** @suppress {undefinedVars, duplicate} @this{Object} */
ascjson.prototype['integrate_set_obs_file'] = ascjson.prototype.integrate_set_obs_file = function(vargv) {
  var self = this.ptr;
  ensureCache.prepare();
  if (vargv && typeof vargv === 'object') vargv = vargv.ptr;
  else vargv = ensureString(vargv);
  return wrapPointer(_emscripten_bind_ascjson_integrate_set_obs_file_1(self, vargv), rt);
};

/** @suppress {undefinedVars, duplicate} @this{Object} */
ascjson.prototype['integrate_logunits'] = ascjson.prototype.integrate_logunits = function(vargv) {
  var self = this.ptr;
  ensureCache.prepare();
  if (vargv && typeof vargv === 'object') vargv = vargv.ptr;
  else vargv = ensureString(vargv);
  return wrapPointer(_emscripten_bind_ascjson_integrate_logunits_1(self, vargv), rt);
};

/** @suppress {undefinedVars, duplicate} @this{Object} */
ascjson.prototype['integrate_logformat'] = ascjson.prototype.integrate_logformat = function(vargv) {
  var self = this.ptr;
  ensureCache.prepare();
  if (vargv && typeof vargv === 'object') vargv = vargv.ptr;
  else vargv = ensureString(vargv);
  return wrapPointer(_emscripten_bind_ascjson_integrate_logformat_1(self, vargv), rt);
};

/** @suppress {undefinedVars, duplicate} @this{Object} */
ascjson.prototype['integrate_get_samples'] = ascjson.prototype.integrate_get_samples = function(vargv) {
  var self = this.ptr;
  ensureCache.prepare();
  if (vargv && typeof vargv === 'object') vargv = vargv.ptr;
  else vargv = ensureString(vargv);
  return wrapPointer(_emscripten_bind_ascjson_integrate_get_samples_1(self, vargv), rt);
};

/** @suppress {undefinedVars, duplicate} @this{Object} */
ascjson.prototype['integrate_set_samples'] = ascjson.prototype.integrate_set_samples = function(vargv) {
  var self = this.ptr;
  ensureCache.prepare();
  if (vargv && typeof vargv === 'object') vargv = vargv.ptr;
  else vargv = ensureString(vargv);
  return wrapPointer(_emscripten_bind_ascjson_integrate_set_samples_1(self, vargv), rt);
};

/** @suppress {undefinedVars, duplicate} @this{Object} */
ascjson.prototype['integrate_able'] = ascjson.prototype.integrate_able = function(vargv) {
  var self = this.ptr;
  ensureCache.prepare();
  if (vargv && typeof vargv === 'object') vargv = vargv.ptr;
  else vargv = ensureString(vargv);
  return wrapPointer(_emscripten_bind_ascjson_integrate_able_1(self, vargv), rt);
};

/** @suppress {undefinedVars, duplicate} @this{Object} */
ascjson.prototype['x__mtx_norms'] = ascjson.prototype.x__mtx_norms = function(vargv) {
  var self = this.ptr;
  ensureCache.prepare();
  if (vargv && typeof vargv === 'object') vargv = vargv.ptr;
  else vargv = ensureString(vargv);
  return wrapPointer(_emscripten_bind_ascjson_x__mtx_norms_1(self, vargv), rt);
};

/** @suppress {undefinedVars, duplicate} @this{Object} */
ascjson.prototype['mtx_gui_plot_incidence'] = ascjson.prototype.mtx_gui_plot_incidence = function(vargv) {
  var self = this.ptr;
  ensureCache.prepare();
  if (vargv && typeof vargv === 'object') vargv = vargv.ptr;
  else vargv = ensureString(vargv);
  return wrapPointer(_emscripten_bind_ascjson_mtx_gui_plot_incidence_1(self, vargv), rt);
};

/** @suppress {undefinedVars, duplicate} @this{Object} */
ascjson.prototype['mtxhelp'] = ascjson.prototype.mtxhelp = function(vargv) {
  var self = this.ptr;
  ensureCache.prepare();
  if (vargv && typeof vargv === 'object') vargv = vargv.ptr;
  else vargv = ensureString(vargv);
  return wrapPointer(_emscripten_bind_ascjson_mtxhelp_1(self, vargv), rt);
};

/** @suppress {undefinedVars, duplicate} @this{Object} */
ascjson.prototype['u_destroy_units'] = ascjson.prototype.u_destroy_units = function(vargv) {
  var self = this.ptr;
  ensureCache.prepare();
  if (vargv && typeof vargv === 'object') vargv = vargv.ptr;
  else vargv = ensureString(vargv);
  return wrapPointer(_emscripten_bind_ascjson_u_destroy_units_1(self, vargv), rt);
};

/** @suppress {undefinedVars, duplicate} @this{Object} */
ascjson.prototype['u_setSIdef'] = ascjson.prototype.u_setSIdef = function(vargv) {
  var self = this.ptr;
  ensureCache.prepare();
  if (vargv && typeof vargv === 'object') vargv = vargv.ptr;
  else vargv = ensureString(vargv);
  return wrapPointer(_emscripten_bind_ascjson_u_setSIdef_1(self, vargv), rt);
};

/** @suppress {undefinedVars, duplicate} @this{Object} */
ascjson.prototype['u_getbasedef'] = ascjson.prototype.u_getbasedef = function(vargv) {
  var self = this.ptr;
  ensureCache.prepare();
  if (vargv && typeof vargv === 'object') vargv = vargv.ptr;
  else vargv = ensureString(vargv);
  return wrapPointer(_emscripten_bind_ascjson_u_getbasedef_1(self, vargv), rt);
};

/** @suppress {undefinedVars, duplicate} @this{Object} */
ascjson.prototype['u_dump'] = ascjson.prototype.u_dump = function(vargv) {
  var self = this.ptr;
  ensureCache.prepare();
  if (vargv && typeof vargv === 'object') vargv = vargv.ptr;
  else vargv = ensureString(vargv);
  return wrapPointer(_emscripten_bind_ascjson_u_dump_1(self, vargv), rt);
};

/** @suppress {undefinedVars, duplicate} @this{Object} */
ascjson.prototype['u_dims'] = ascjson.prototype.u_dims = function(vargv) {
  var self = this.ptr;
  ensureCache.prepare();
  if (vargv && typeof vargv === 'object') vargv = vargv.ptr;
  else vargv = ensureString(vargv);
  return wrapPointer(_emscripten_bind_ascjson_u_dims_1(self, vargv), rt);
};

/** @suppress {undefinedVars, duplicate} @this{Object} */
ascjson.prototype['u_dim_setverify'] = ascjson.prototype.u_dim_setverify = function(vargv) {
  var self = this.ptr;
  ensureCache.prepare();
  if (vargv && typeof vargv === 'object') vargv = vargv.ptr;
  else vargv = ensureString(vargv);
  return wrapPointer(_emscripten_bind_ascjson_u_dim_setverify_1(self, vargv), rt);
};

/** @suppress {undefinedVars, duplicate} @this{Object} */
ascjson.prototype['u_dim2num'] = ascjson.prototype.u_dim2num = function(vargv) {
  var self = this.ptr;
  ensureCache.prepare();
  if (vargv && typeof vargv === 'object') vargv = vargv.ptr;
  else vargv = ensureString(vargv);
  return wrapPointer(_emscripten_bind_ascjson_u_dim2num_1(self, vargv), rt);
};

/** @suppress {undefinedVars, duplicate} @this{Object} */
ascjson.prototype['u_num2dim'] = ascjson.prototype.u_num2dim = function(vargv) {
  var self = this.ptr;
  ensureCache.prepare();
  if (vargv && typeof vargv === 'object') vargv = vargv.ptr;
  else vargv = ensureString(vargv);
  return wrapPointer(_emscripten_bind_ascjson_u_num2dim_1(self, vargv), rt);
};

/** @suppress {undefinedVars, duplicate} @this{Object} */
ascjson.prototype['u_frombasedim'] = ascjson.prototype.u_frombasedim = function(vargv) {
  var self = this.ptr;
  ensureCache.prepare();
  if (vargv && typeof vargv === 'object') vargv = vargv.ptr;
  else vargv = ensureString(vargv);
  return wrapPointer(_emscripten_bind_ascjson_u_frombasedim_1(self, vargv), rt);
};

/** @suppress {undefinedVars, duplicate} @this{Object} */
ascjson.prototype['u_fromatomdim'] = ascjson.prototype.u_fromatomdim = function(vargv) {
  var self = this.ptr;
  ensureCache.prepare();
  if (vargv && typeof vargv === 'object') vargv = vargv.ptr;
  else vargv = ensureString(vargv);
  return wrapPointer(_emscripten_bind_ascjson_u_fromatomdim_1(self, vargv), rt);
};

/** @suppress {undefinedVars, duplicate} @this{Object} */
ascjson.prototype['u_getdimatoms'] = ascjson.prototype.u_getdimatoms = function(vargv) {
  var self = this.ptr;
  ensureCache.prepare();
  if (vargv && typeof vargv === 'object') vargv = vargv.ptr;
  else vargv = ensureString(vargv);
  return wrapPointer(_emscripten_bind_ascjson_u_getdimatoms_1(self, vargv), rt);
};

/** @suppress {undefinedVars, duplicate} @this{Object} */
ascjson.prototype['u_change_baseunit'] = ascjson.prototype.u_change_baseunit = function(vargv) {
  var self = this.ptr;
  ensureCache.prepare();
  if (vargv && typeof vargv === 'object') vargv = vargv.ptr;
  else vargv = ensureString(vargv);
  return wrapPointer(_emscripten_bind_ascjson_u_change_baseunit_1(self, vargv), rt);
};

/** @suppress {undefinedVars, duplicate} @this{Object} */
ascjson.prototype['u_getprec'] = ascjson.prototype.u_getprec = function(vargv) {
  var self = this.ptr;
  ensureCache.prepare();
  if (vargv && typeof vargv === 'object') vargv = vargv.ptr;
  else vargv = ensureString(vargv);
  return wrapPointer(_emscripten_bind_ascjson_u_getprec_1(self, vargv), rt);
};

/** @suppress {undefinedVars, duplicate} @this{Object} */
ascjson.prototype['u_setprec'] = ascjson.prototype.u_setprec = function(vargv) {
  var self = this.ptr;
  ensureCache.prepare();
  if (vargv && typeof vargv === 'object') vargv = vargv.ptr;
  else vargv = ensureString(vargv);
  return wrapPointer(_emscripten_bind_ascjson_u_setprec_1(self, vargv), rt);
};

/** @suppress {undefinedVars, duplicate} @this{Object} */
ascjson.prototype['u_get_atoms'] = ascjson.prototype.u_get_atoms = function(vargv) {
  var self = this.ptr;
  ensureCache.prepare();
  if (vargv && typeof vargv === 'object') vargv = vargv.ptr;
  else vargv = ensureString(vargv);
  return wrapPointer(_emscripten_bind_ascjson_u_get_atoms_1(self, vargv), rt);
};

/** @suppress {undefinedVars, duplicate} @this{Object} */
ascjson.prototype['u_get_units'] = ascjson.prototype.u_get_units = function(vargv) {
  var self = this.ptr;
  ensureCache.prepare();
  if (vargv && typeof vargv === 'object') vargv = vargv.ptr;
  else vargv = ensureString(vargv);
  return wrapPointer(_emscripten_bind_ascjson_u_get_units_1(self, vargv), rt);
};

/** @suppress {undefinedVars, duplicate} @this{Object} */
ascjson.prototype['u_set_user'] = ascjson.prototype.u_set_user = function(vargv) {
  var self = this.ptr;
  ensureCache.prepare();
  if (vargv && typeof vargv === 'object') vargv = vargv.ptr;
  else vargv = ensureString(vargv);
  return wrapPointer(_emscripten_bind_ascjson_u_set_user_1(self, vargv), rt);
};

/** @suppress {undefinedVars, duplicate} @this{Object} */
ascjson.prototype['u_get_user'] = ascjson.prototype.u_get_user = function(vargv) {
  var self = this.ptr;
  ensureCache.prepare();
  if (vargv && typeof vargv === 'object') vargv = vargv.ptr;
  else vargv = ensureString(vargv);
  return wrapPointer(_emscripten_bind_ascjson_u_get_user_1(self, vargv), rt);
};

/** @suppress {undefinedVars, duplicate} @this{Object} */
ascjson.prototype['u_get_list'] = ascjson.prototype.u_get_list = function(vargv) {
  var self = this.ptr;
  ensureCache.prepare();
  if (vargv && typeof vargv === 'object') vargv = vargv.ptr;
  else vargv = ensureString(vargv);
  return wrapPointer(_emscripten_bind_ascjson_u_get_list_1(self, vargv), rt);
};

/** @suppress {undefinedVars, duplicate} @this{Object} */
ascjson.prototype['u_clear_user'] = ascjson.prototype.u_clear_user = function(vargv) {
  var self = this.ptr;
  ensureCache.prepare();
  if (vargv && typeof vargv === 'object') vargv = vargv.ptr;
  else vargv = ensureString(vargv);
  return wrapPointer(_emscripten_bind_ascjson_u_clear_user_1(self, vargv), rt);
};

/** @suppress {undefinedVars, duplicate} @this{Object} */
ascjson.prototype['u_getval'] = ascjson.prototype.u_getval = function(vargv) {
  var self = this.ptr;
  ensureCache.prepare();
  if (vargv && typeof vargv === 'object') vargv = vargv.ptr;
  else vargv = ensureString(vargv);
  return wrapPointer(_emscripten_bind_ascjson_u_getval_1(self, vargv), rt);
};

/** @suppress {undefinedVars, duplicate} @this{Object} */
ascjson.prototype['u_browgetval'] = ascjson.prototype.u_browgetval = function(vargv) {
  var self = this.ptr;
  ensureCache.prepare();
  if (vargv && typeof vargv === 'object') vargv = vargv.ptr;
  else vargv = ensureString(vargv);
  return wrapPointer(_emscripten_bind_ascjson_u_browgetval_1(self, vargv), rt);
};

/** @suppress {undefinedVars, duplicate} @this{Object} */
ascjson.prototype['u_slvgetrelval'] = ascjson.prototype.u_slvgetrelval = function(vargv) {
  var self = this.ptr;
  ensureCache.prepare();
  if (vargv && typeof vargv === 'object') vargv = vargv.ptr;
  else vargv = ensureString(vargv);
  return wrapPointer(_emscripten_bind_ascjson_u_slvgetrelval_1(self, vargv), rt);
};

/** @suppress {undefinedVars, duplicate} @this{Object} */
ascjson.prototype['u_slvgetvarval'] = ascjson.prototype.u_slvgetvarval = function(vargv) {
  var self = this.ptr;
  ensureCache.prepare();
  if (vargv && typeof vargv === 'object') vargv = vargv.ptr;
  else vargv = ensureString(vargv);
  return wrapPointer(_emscripten_bind_ascjson_u_slvgetvarval_1(self, vargv), rt);
};

/** @suppress {undefinedVars, duplicate} @this{Object} */
ascjson.prototype['u_slvgetobjval'] = ascjson.prototype.u_slvgetobjval = function(vargv) {
  var self = this.ptr;
  ensureCache.prepare();
  if (vargv && typeof vargv === 'object') vargv = vargv.ptr;
  else vargv = ensureString(vargv);
  return wrapPointer(_emscripten_bind_ascjson_u_slvgetobjval_1(self, vargv), rt);
};

/** @suppress {undefinedVars, duplicate} @this{Object} */
ascjson.prototype['uhelp'] = ascjson.prototype.uhelp = function(vargv) {
  var self = this.ptr;
  ensureCache.prepare();
  if (vargv && typeof vargv === 'object') vargv = vargv.ptr;
  else vargv = ensureString(vargv);
  return wrapPointer(_emscripten_bind_ascjson_uhelp_1(self, vargv), rt);
};

/** @suppress {undefinedVars, duplicate} @this{Object} */
ascjson.prototype['srefine'] = ascjson.prototype.srefine = function(vargv) {
  var self = this.ptr;
  ensureCache.prepare();
  if (vargv && typeof vargv === 'object') vargv = vargv.ptr;
  else vargv = ensureString(vargv);
  return wrapPointer(_emscripten_bind_ascjson_srefine_1(self, vargv), rt);
};

/** @suppress {undefinedVars, duplicate} @this{Object} */
ascjson.prototype['smerge'] = ascjson.prototype.smerge = function(vargv) {
  var self = this.ptr;
  ensureCache.prepare();
  if (vargv && typeof vargv === 'object') vargv = vargv.ptr;
  else vargv = ensureString(vargv);
  return wrapPointer(_emscripten_bind_ascjson_smerge_1(self, vargv), rt);
};

/** @suppress {undefinedVars, duplicate} @this{Object} */
ascjson.prototype['ascloadwin'] = ascjson.prototype.ascloadwin = function(vargv) {
  var self = this.ptr;
  ensureCache.prepare();
  if (vargv && typeof vargv === 'object') vargv = vargv.ptr;
  else vargv = ensureString(vargv);
  return wrapPointer(_emscripten_bind_ascjson_ascloadwin_1(self, vargv), rt);
};

/** @suppress {undefinedVars, duplicate} @this{Object} */
ascjson.prototype['gnutext'] = ascjson.prototype.gnutext = function(vargv) {
  var self = this.ptr;
  ensureCache.prepare();
  if (vargv && typeof vargv === 'object') vargv = vargv.ptr;
  else vargv = ensureString(vargv);
  return wrapPointer(_emscripten_bind_ascjson_gnutext_1(self, vargv), rt);
};

/** @suppress {undefinedVars, duplicate} @this{Object} */
ascjson.prototype['help'] = ascjson.prototype.help = function(vargv) {
  var self = this.ptr;
  ensureCache.prepare();
  if (vargv && typeof vargv === 'object') vargv = vargv.ptr;
  else vargv = ensureString(vargv);
  return wrapPointer(_emscripten_bind_ascjson_help_1(self, vargv), rt);
};


/** @suppress {undefinedVars, duplicate} @this{Object} */
ascjson.prototype['__destroy__'] = ascjson.prototype.__destroy__ = function() {
  var self = this.ptr;
  _emscripten_bind_ascjson___destroy___0(self);
};

(function() {
  function setupEnums() {
    
// $vtype

    Module['SVjson'] = _emscripten_enum_vtype_SVjson();

    Module['SVjson5'] = _emscripten_enum_vtype_SVjson5();

    Module['SVcstr'] = _emscripten_enum_vtype_SVcstr();

  }
  if (runtimeInitialized) setupEnums();
  else addOnInit(setupEnums);
})();
