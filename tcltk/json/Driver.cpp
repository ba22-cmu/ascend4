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

  /*
   *  Create the interpreter
   */
	ascjson	*world = new ascjson();
	world->config("foo");

	const char *libdestroy = "a\v" Asc_LibrDestroyTypesCmdHN;
	auto r1 = world->Asc_LibrDestroyTypesCmdHC(libdestroy);
	printf("%d\n", r1->e);

	// next line depends on ASCEND_LIBRARY env var defaulted right in ascjson.cpp
	// or there being something in user env that overrides it.
	const char *read_file = "librread\vbasemodel.a4l";
	auto r2 = world->Asc_LibrReadCmdHC(read_file);
	printf("%d\n", r2->e);
	printf("%s\n", r2->v);
	

	const char *typelist = Asc_LibrTypeListCmdHN "\vbasemodel.a4l<0>";
	auto r3 = world->Asc_LibrTypeListCmdHC(typelist);
	printf("%d\n", r3->e);
	printf("%s\n", r3->v);
	// should return testcmumodel cmumodel your_site_models catch_Word_model as a vtab list
	
	if (argc > 6) {
		int count = world->call_all_force_link();
		printf("force count was: %d", count);
	}
	
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

