/*	
fixme	
*/

//#define ASC_BUILDING_INTERFACE
#define ASC_BUILDING_LIBASCEND

#include <ascend/general/platform.h>

#include <stdarg.h>
#include <ctype.h>
#include <time.h>
extern "C"{
#include <ascend/utilities/config.h>
#include "config.h"
#include <ascend/general/panic.h>
#include <ascend/general/ospath.h>
#include <ascend/compiler/dimen.h>
#include <ascend/utilities/ascPrint.h>
#include <ascend/utilities/error.h>
#include <ascend/solver/solver.h>
#ifdef ASC_SIGNAL_TRAPS
# include <ascend/utilities/ascSignal.h>
#endif
};
#include "tcltk/json/ascjson.hpp"

#ifndef __WIN32__
# include <unistd.h>
#else

/* jds20041229 - windows.h now included in ascConfig.h. */
/* jp - i took it back out of ascConfig.h - Apr 2005 */
# define WIN32_LEAN_AND_MEAN
# include <windows.h>

# include <locale.h>
# undef WIN32_LEAN_AND_MEAN
#endif /* __WIN32__ */

extern "C"{
#include <ascend/utilities/config.h>
#include <ascend/general/ascMalloc.h>    /* for ascshutdown */
#include <ascend/general/panic.h>     /* for Asc_Panic */
#include <ascend/utilities/ascEnvVar.h>
#include <ascend/utilities/ascPrint.h>

#include <ascend/compiler/ascCompiler.h>
#include <ascend/compiler/instance_enum.h>
#include <ascend/compiler/units.h>
#include <ascend/compiler/simlist.h>

#include <ascend/linear/mtx.h>

#include <ascend/system/slv_client.h>
#include <ascend/system/slv_stdcalls.h>
}; 



/*
 *  EXPORTED VARIABLES
 */

/**
	Wrapper class for the app.  We need to make it global
	so that our signal/floating-porint traps can access it.
*/
ascjson *g_all;

/*
	Comes from the yacc file if yacc was built with debugging information
*/
#ifdef ZZ_DEBUG
extern int zz_debug;
#endif

#ifdef ASC_SIGNAL_TRAPS
static void AscTrap(int);
#endif

/*
	LOCALLY GLOBAL VARIABLES
	think global, act local :-)
*/

/**
	TRUE for compiler optimizations default is TRUE, set to FALSE by passing
	+s on the command line
*/
#if 0
static int g_interface_simplify_relations = TRUE;
#endif

/**
	who built this binary and when
 */
#if 0
#ifndef TIMESTAMP
static char build_name[]="by anonymous";
#else
static char build_name[]=TIMESTAMP;
#endif /* TIMESTAMP */
#endif

/*
	jp: Moved 'main' and 'WinMain' to separate 'main.c'
	so that ascend4.exe can be built without linkage to Tcl/Tk
*/


/**
	A common entry point for Windows and Unix.  The corresponding
	WinMain() and main() functions just call this function.

	This function creates a Tcl interpreter, initializes Tcl and Tk,
	initializes the Ascend data structures, sets up the user's
	environment, sources ASCEND's startup script, and calls Tk_MainLoop
	so the user can interact with ASCEND.  Cleans up and exits the
	program when Tk_MainLoop returns.

	This function is based on the functions Tk_Main and Tcl_AppInit
	from the Tk8.0 distribution.  See the files tkMain.c and tkAppInit.c
	in the Tk sources.
*/
extern "C" {
int AscDriver(int argc, CONST char **argv)
{

  // Tcl_FindExecutable(argv[0]); // /proc/self/exe readlink under linux
	Asc_DString d;
	Asc_DStringInit(&d);
	Asc_DStringSet(&d,"bob\t\v");
	printf("%d %s\n",Asc_DStringLength(&d),Asc_DStringValue(&d));
	Asc_DStringStrip(&d,"\t\v");
	printf("%d %s\n",Asc_DStringLength(&d),Asc_DStringValue(&d));

  /*
   *  Create the interpreter
   */
	ascjson	*world = new ascjson();
	world->config("foo");
	world->u_setSIdef("u_setSIdef");

	const char *libdestroy = "a\v" Asc_LibrDestroyTypesCmdHN;
	auto r1 = world->Asc_LibrDestroyTypesCmdHC(libdestroy);
	printf("%d\n", r1->e);

	// next line depends on ASCEND_LIBRARY env var defaulted right in ascjson.cpp
	// or there being something in user env that overrides it.
	const char *read_file = "librread\vvessel.a4c";
	auto r2 = world->Asc_LibrReadCmdHC(read_file);
	printf("%d\n", r2->e);
	printf("%s\n", r2->v);
	

	const char *typelist = Asc_LibrTypeListCmdHN "\vbasemodel.a4l<0>";
	auto r3 = world->Asc_LibrTypeListCmdHC(typelist);
	printf("%d\n", r3->e);
	printf("%s\n", r3->v);
	// should return testcmumodel cmumodel your_site_models catch_Word_model as a vtab list

	const char *minfo = Asc_LibrModuleInfoCmdHN "\vbasemodel.a4l<0>\vvessel.a4c<0>";
	auto r4 = world->Asc_LibrModuleInfoCmdHC(minfo);
	printf("%d\n", r4->e);
	printf("%s\n", r4->v);

	auto rsa = world->slv_available("slv_available");
	int sargc=0; char **sargv = NULL;
	VTcl_SplitList(NULL, rsa->v, &sargc, &sargv);
	for (int k = 0; k < sargc; k++ ) {
		printf("solver: %s\n", sargv[k]);
	}
	freeArgv(sargv);

	world->asc_compiler_option("asc_compiler_option\v-useCopyAnon\v0");
	world->sim_instantiate("sim_instantiate\vvp\vvessel");
	world->qlfdid("qlfdid\vvp");
	world->btransfer("btransfer\vvp");
	world->qlfdid("qlfdid\vvp");
	auto bgr = world->bgetrels("bgetrels\vsearch");
	printf("bgr err: %d\n", bgr->e);
	printf("bgr str: %s\n", bgr->v);

	bgr = world->bgetrels("bgetrels\vsearch\vrelio_tex");
	printf("bgr err: %d\n", bgr->e);
	printf("bgr str: %s\n", bgr->v);

	auto lxt = world->libr_extract_type("libr_extract_type\vcmumodel\v../../models/basemodel.a4l\v-s");
	if (lxt->e == 0) {
		printf("code for cmumodel is\n");
		printf("%s\n",lxt->v);
	} else {
		printf("(err %d): %s\n",lxt->e, lxt->v);
		printf("%s\n",lxt->v);
	}
	auto mr = world->Asc_BrowInitializeCmdHC(Asc_BrowInitializeCmdHS
		       	"\v-method\von_load\v-qlfdid\vvp");
	printf("onload: %s\n",mr->v);

	auto bcl = world->brow_child_list("brow_child_list\vsearch\vall\vVALUE\vATOMS");
		//  <current,search> <all,N> ["TYPE","VALUE"] ["ATOMS"] ["PASSED"]
	if (bcl->e == 0) {
		printf("brow_child_list\n%s\n",bcl->v);
	} else {
		printf("brow_child_list failed\n");
	}
	
	auto siq = world->slv_import_qlfdid("slv_import_qlfdid\vvp\vtest");
	if (siq->e) {
		printf("slv_import_qlfdid test failed\n");
	} else {
		world->slv_import_qlfdid("slv_import_qlfdid\vvp");
		auto sn = world->slv_get_pathname("slv_get_pathname");
		printf("slv using instance %s\n",sn->v);
		world->slv_presolve("slv_presolve");
		auto sp =  world->slv_get_stat_page("slv_get_stat_page");
		printf("before stats: %s\n", sp->v);
		world->slv_solve("slv_solve");
		sp =  world->slv_get_stat_page("slv_get_stat_page");
		printf("after stats: %s\n", sp->v);
	}

	world->slv_flush_solver("slv_flush_solver");
	world->sim_destroy("sim_destroy\vvp");
	
	if (argc > 6) {
		int count = world->call_all_force_link();
		printf("force count was: %d", count);
	}
	
	world->u_destroy_units("u_destroy_units");
	delete world;

	return 0;
}
}

#ifdef ASC_SIGNAL_TRAPS
/**
	Function to call when we receive an interrupt.
 */
static
void AscTrap(int sig)
{
  putchar('\n');
  Asc_Panic(sig, "AscTrap", "Caught Signal: %d", sig);
}
#endif

