#ifndef ascjson_util_h
#define ascjson_util_h
#include <string>
#include <cstdlib>


enum vtype {
	SVjson,
	SVjson5,
	SVcstr
};


struct rc {
	rc() {};
	rc(std::string msg, int e, enum vtype t): s(msg), e(e), v(s.c_str()), t(t) {};
	std::string s;
	int e; // possibly a -ERRNO value, or see call documentation.
	const char *v; 
	enum vtype t;
};

typedef const struct rc * rcp;


/* All strings returned via rcp from this class are
 * allocated at least until the next call to the 
 * same function or the destructor of ascjson is called.
 *
 * Under this model, strings should be parsed to objects,
 * and immediately forgotten by the wrapping language.
 *
 * Some of the function names and other strings used here are macros
 * extracted from the tcl binding to maintain consistency.
 */
struct jcache;

#define VQUIET(a) a.c_str()
#define QUIET(a) a
#define QUIET2(a) a
#ifndef STR
#define STR(x) #x
#define LHN(NAME) LHN1(NAME, HLF)
#define LHN1(NAME, SUFFIX) LHN2(NAME, SUFFIX)
#define LHN2(NAME, SUFFIX) NAME ##  SUFFIX
#endif
/** add vtab separated entry to string */
#define VTcl_AppendElement(a, b) \
	Asc_DStringAppend(a, b, HALL); \
	Asc_DStringAppend(a, "\v", 1)

// DTcl_GetInt doesn't report errors; we need to improve it. FIXME (null, range checks)
#define JTcl_GetInt(i, str, iptr) \
	*iptr = atoi(str)
	

#define wrap_dstring(call, stype) \
        INIT_ARGV(vargv); \
        ASCUSE; \
        err = call(&hds, argc, argv); \
        setc(__func__, stype, Asc_DStringValue(&hds), err); \
        Asc_DStringFree(&hds); \
        FREE_ARGV; \
        return getc(__func__)

#endif // ascjson_util_h
