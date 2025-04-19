#ifndef ascjson_util_h
#define ascjson_util_h
#include <string>
#include <cstdlib>

enum vtype {
	SVjson,
	SVjson5,
	SVcstr,
	SVvtab
};

static const char *vnames[4] = { "SVjson", "SVjson5", "SVcstr", "SVvtab" };

struct rc {
	rc() {};
	rc(std::string msg, int e, enum vtype t): s(msg), e(e), v(s.c_str()), t(t) {};
	std::string s;
	int e; // possibly a -ERRNO value, or see call documentation.
	const char *v; 
	enum vtype t;
	const char* ts() {
		switch (e) {
		case SVjson: return vnames[0];
		case SVjson5: return vnames[1];
		case SVcstr: return vnames[2];
		case SVvtab: return vnames[3];
		default: return "";
		}
	}
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

int JTcl_GetInt(void *i, const char* str, int *iptr);
int JTcl_GetLong(void *i, const char* str, long *lptr);
	
#define Tcl_GetInt JTcl_GetInt
#define Tcl_ExprLong JTcl_GetLong
#define Tcl_ResetResult Asc_DStringFree
#define TCL_VOLATILE
#define TCL_STATIC
#define TCL_OK hELP_OK
#define TCL_ERROR HELP_ERROR
#define Tcl_AppendResult Asc_DStringAppend

/// invoke call with dstr/argc/argv and set return data with stype
/// if call returns 0, otherwise assume return is an error string.
#define wrap_dstring(call, stype) \
        INIT_ARGV(vargv); \
        ASCUSE; \
        err = call(&hds, argc, argv); \
        setc(__func__, (err ? SVcstr : stype), Asc_DStringValue(&hds), err); \
        Asc_DStringFree(&hds); \
        FREE_ARGV; \
        return getc(__func__)

#define Asc_DStringAppend2(hptr,a,b,HALL) \
	Asc_DStringAppend(hptr,a,HALL); \
	Asc_DStringAppend(hptr,b,HALL)

#define Asc_DStringAppend3(hptr,a,b,c,HALL) \
	Asc_DStringAppend(hptr,a,HALL); \
	Asc_DStringAppend(hptr,b,HALL); \
	Asc_DStringAppend(hptr,c,HALL);

#define Asc_DStringAppend4(hptr,a,b,c,d,HALL) \
	Asc_DStringAppend(hptr,a,HALL); \
	Asc_DStringAppend(hptr,b,HALL); \
	Asc_DStringAppend(hptr,c,HALL); \
	Asc_DStringAppend(hptr,d,HALL)

#define Asc_DStringAppend5(hptr,a,b,c,d,e,HALL) \
	Asc_DStringAppend(hptr,a,HALL); \
	Asc_DStringAppend(hptr,b,HALL); \
	Asc_DStringAppend(hptr,c,HALL); \
	Asc_DStringAppend(hptr,d,HALL); \
	Asc_DStringAppend(hptr,e,HALL)

#define Asc_DStringAppend6(hptr,a,b,c,d,e,f,HALL) \
	Asc_DStringAppend(hptr,a,HALL); \
	Asc_DStringAppend(hptr,b,HALL); \
	Asc_DStringAppend(hptr,c,HALL); \
	Asc_DStringAppend(hptr,d,HALL); \
	Asc_DStringAppend(hptr,e,HALL); \
	Asc_DStringAppend(hptr,f,HALL)

// cast slvbackendtoken to instance
#define T2I(t) ((struct Instance *)t)

#endif // ascjson_util_h
