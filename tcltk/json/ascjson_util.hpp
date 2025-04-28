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

// free the result
char *file_to_string(FILE *f);

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
	Asc_DStringAppend(a, (b ? b : ""), HALL); \
	Asc_DStringAppend(a, "\v", 1)
#define VAEstrip(a) Asc_DStringStrip(a,"\v")

/// split str on vtab and return data in *argvP. call must free *argvP.
int VTcl_SplitList(void *i, const char *str, int *argcP, char ***argvP);

int JTcl_GetInt(void *i, const char* str, int *iptr);
int JTcl_GetLong(void *i, const char* str, long *lptr);
/* convert any string starting t,T,y,Y,nonzero decimal digit to true, else false. */
int JTcl_GetBool(void *i, const char* str, int *lptr);
int JTcl_GetDouble(void *i, const char* str, double *lptr);
	
#define Tcl_GetDouble JTcl_GetDouble
#define Tcl_GetInt JTcl_GetInt
#define Tcl_ExprLong JTcl_GetLong
#define Tcl_ExprBoolean JTcl_GetBool
#define Tcl_ResetResult Asc_DStringFree
#define TCL_VOLATILE
#define TCL_STATIC
#define TCL_OK hELP_OK
#define TCL_ERROR HELP_ERROR
#define Tcl_AppendResult Asc_DStringAppend
#define MAXIMUM_INST_DEPTH 40

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

#define Asc_DStringAppend2(hptr,a,b,x) \
	Asc_DStringAppend(hptr,a,HALL); \
	Asc_DStringAppend(hptr,b,HALL)

#define Asc_DStringAppend3(hptr,a,b,c,x) \
	Asc_DStringAppend(hptr,a,HALL); \
	Asc_DStringAppend(hptr,b,HALL); \
	Asc_DStringAppend(hptr,c,HALL);

#define Asc_DStringAppend4(hptr,a,b,c,d,x) \
	Asc_DStringAppend(hptr,a,HALL); \
	Asc_DStringAppend(hptr,b,HALL); \
	Asc_DStringAppend(hptr,c,HALL); \
	Asc_DStringAppend(hptr,d,HALL)

#define Asc_DStringAppend5(hptr,a,b,c,d,e,x) \
	Asc_DStringAppend(hptr,a,HALL); \
	Asc_DStringAppend(hptr,b,HALL); \
	Asc_DStringAppend(hptr,c,HALL); \
	Asc_DStringAppend(hptr,d,HALL); \
	Asc_DStringAppend(hptr,e,HALL)

#define Asc_DStringAppend6(hptr,a,b,c,d,e,f,x) \
	Asc_DStringAppend(hptr,a,HALL); \
	Asc_DStringAppend(hptr,b,HALL); \
	Asc_DStringAppend(hptr,c,HALL); \
	Asc_DStringAppend(hptr,d,HALL); \
	Asc_DStringAppend(hptr,e,HALL); \
	Asc_DStringAppend(hptr,f,HALL)

#define Asc_DStringAppend7(hptr,a,b,c,d,e,f,g,x) \
	Asc_DStringAppend(hptr,a,HALL); \
	Asc_DStringAppend(hptr,b,HALL); \
	Asc_DStringAppend(hptr,c,HALL); \
	Asc_DStringAppend(hptr,d,HALL); \
	Asc_DStringAppend(hptr,e,HALL); \
	Asc_DStringAppend(hptr,f,HALL); \
	Asc_DStringAppend(hptr,g,HALL)

#define Asc_DStringAppend8(hptr,a,b,c,d,e,f,g,h,x) \
	Asc_DStringAppend(hptr,a,HALL); \
	Asc_DStringAppend(hptr,b,HALL); \
	Asc_DStringAppend(hptr,c,HALL); \
	Asc_DStringAppend(hptr,d,HALL); \
	Asc_DStringAppend(hptr,e,HALL); \
	Asc_DStringAppend(hptr,f,HALL); \
	Asc_DStringAppend(hptr,g,HALL); \
	Asc_DStringAppend(hptr,h,HALL)

#define Asc_DStringAppend9(hptr,a,b,c,d,e,f,g,h,i,x) \
	Asc_DStringAppend(hptr,a,HALL); \
	Asc_DStringAppend(hptr,b,HALL); \
	Asc_DStringAppend(hptr,c,HALL); \
	Asc_DStringAppend(hptr,d,HALL); \
	Asc_DStringAppend(hptr,e,HALL); \
	Asc_DStringAppend(hptr,f,HALL); \
	Asc_DStringAppend(hptr,g,HALL); \
	Asc_DStringAppend(hptr,h,HALL); \
	Asc_DStringAppend(hptr,i,HALL)

// cast slvbackendtoken to instance
#define T2I(t) ((struct Instance *)t)

extern "C" {
#include <ascend/general/platform.h>
// #include "config.h"
#include <ascend/utilities/config.h>
#include <ascend/utilities/ascSignal.h>
#include <ascend/utilities/ascEnvVar.h>
#include <ascend/utilities/ascPrint.h>
#include <ascend/utilities/error.h>

#include <ascend/general/ascMalloc.h>
#include <ascend/general/ospath.h>
#include <ascend/general/dstring.h>
#include <ascend/general/list.h>
#include <ascend/general/panic.h>

#include <ascend/compiler/ascCompiler.h>
#include <ascend/compiler/symtab.h>
#include <ascend/compiler/braced.h>
#include <ascend/compiler/notate.h>
#include <ascend/compiler/expr_types.h>
#include <ascend/compiler/syntax.h>
#include <ascend/compiler/module.h>
#include <ascend/compiler/instance_enum.h>
#include <ascend/compiler/dump.h>
#include <ascend/compiler/stattypes.h>
#include <ascend/compiler/simlist.h>
#include <ascend/compiler/copyinst.h>
#include <ascend/compiler/instquery.h>
#include <ascend/compiler/instantiate.h>
#include <ascend/compiler/qlfdid.h>
#include <ascend/compiler/pending.h>
#include <ascend/compiler/check.h>
#include <ascend/compiler/statement.h>
#include <ascend/compiler/statio.h>
#include <ascend/compiler/bintoken.h>
#include <ascend/compiler/instance_io.h>
#include <ascend/compiler/destroyinst.h>
#include <ascend/general/tm_time.h>
#include <ascend/compiler/slist.h>
#include <ascend/compiler/child.h>
#include <ascend/compiler/childio.h>
#include <ascend/compiler/type_desc.h>
#include <ascend/compiler/type_descio.h>
#include <ascend/compiler/typedef.h>
#include <ascend/compiler/extfunc.h>
#include <ascend/compiler/library.h>
#include <ascend/compiler/prototype.h>
#include <ascend/compiler/proc.h>
#include <ascend/compiler/procio.h>
#include <ascend/compiler/name.h>
#include <ascend/compiler/parentchild.h>
#include <ascend/compiler/setinstval.h>
#include <ascend/compiler/nameio.h>
#include <ascend/compiler/parser.h>
#include <ascend/compiler/visitinst.h>
#include <ascend/compiler/visitlink.h>
#include <ascend/compiler/plot.h>
#include <ascend/compiler/instance_name.h>
#include <ascend/compiler/dimen.h>
#include <ascend/compiler/units.h>
#include <ascend/compiler/mathinst.h>
#include <ascend/compiler/atomvalue.h>
#include <ascend/compiler/instance_enum.h>
#include <ascend/compiler/cmpfunc.h>
#include <ascend/compiler/dimen_io.h>
#include <ascend/compiler/child.h>
#include <ascend/compiler/type_desc.h>
#include <ascend/compiler/module.h>
#include <ascend/compiler/library.h>
#include <ascend/compiler/symtab.h>
#include <ascend/compiler/instance_io.h>
#include <ascend/compiler/atomvalue.h>
#include <ascend/compiler/instquery.h>
#include <ascend/compiler/mathinst.h>
#include <ascend/compiler/instance_name.h>
#include <ascend/compiler/fractions.h>
#include <ascend/compiler/compiler.h>
#include <ascend/compiler/exprs.h>
#include <ascend/compiler/value_type.h>
#include <ascend/compiler/find.h>
#include <ascend/compiler/rel_blackbox.h>
#include <ascend/compiler/logrel_util.h>
#include <ascend/compiler/vlist.h>
#include <ascend/compiler/relation.h>
#include <ascend/compiler/relation_io.h>
#include <ascend/compiler/functype.h>
#include <ascend/compiler/safe.h>
#include <ascend/compiler/relation_util.h>
#include <ascend/compiler/initialize.h>
#include <ascend/compiler/watchpt.h>
#include <ascend/compiler/watchptio.h>
#include <ascend/compiler/mergeinst.h>
#include <ascend/compiler/refineinst.h>
#include <ascend/compiler/anontype.h>

#include <ascend/linear/mtx.h>
#include <ascend/system/slv_types.h>
#include <ascend/system/slv_client.h>
#include <ascend/system/slv_stdcalls.h>
#include <ascend/solver/solver.h>
#include <ascend/packages/ascFreeAllVars.h>

#include <ascend/system/slv_server.h>   /* KHACK: not sure if this should be here */
#include <ascend/system/calc.h>
#include <ascend/system/system.h>
#include <ascend/system/cond_config.h>
#include <ascend/system/relman.h>
#include <ascend/solver/slv_interface.h>
#include <ascend/solver/slvDOF.h>
#include <ascend/integrator/integrator.h>
#include <ascend/integrator/samplelist.h>

#include <solvers/static_solvers.h>

}

/// remove all characters in stripchars from end of dstring.
void Asc_DStringStrip(Asc_DString *ds, const char * stripchars);

#endif // ascjson_util_h
