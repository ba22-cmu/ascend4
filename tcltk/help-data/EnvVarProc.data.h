#ifndef EnvVarProc_json_h
#define EnvVarProc_json_h


#define Asc_EnvVarCmdHN "asc_env"
#define Asc_EnvVarCmdHC  asc_env 
/** Usage */
#define Asc_EnvVarCmdHU \
 "asc_env option args\n\
  options are append, export, get, import, list, names, put, and set."

#define Asc_EnvVarCmdHS \
 "asc_env manipulates a database of strings for the ascend application"

#define Asc_EnvVarCmdHL1 \
"\
 * These strings are only loosely tied to either the C or Tcl environment\n\
 * on flakey non-UNIX platforms.\n\
 * The options are as follows:\n\
"

#define Asc_EnvVarCmdHL2 \
"\
 * 	append	var pathelement\n\
 *		Adds pathelement to the ascend environment string var.\n\
 *		Path elements may contain spaces, but should not\n\
 *		contain path dividers.\n\
"

#define Asc_EnvVarCmdHL3 \
"\
 * 	export	var\n\
 *		Adds the definition of var to the Tcl global env array.\n\
 *		This does not LINK the var and Tcl env, so changes\n\
 *		with asc_env may need to be followed by reexporting.\n\
 *		Whether or not the C environment sees changes to Tcl\n\
 *		env depends entirely on the Tcl core implementation.\n\
"

#define Asc_EnvVarCmdHL4 \
"\
 * 	get 	var\n\
 *		Returns the ascend environment string var. If var has\n\
 *		more than one element, then the elements are glued\n\
 *		together into a single string using the pathdiv separator.\n\
 *		UNIX pathdiv is : and MS pathdiv is ; .\n\
"

#define Asc_EnvVarCmdHL5 \
"\
 * 	import	var\n\
 *		Sets the ascend environment var using the result of a \n\
 *		call to C getenv(). Tcl env may or may not match.\n\
 * 	list	var\n\
 *		Returns a Tcl list of the elements stored for ascend var.\n\
"

#define Asc_EnvVarCmdHL6 \
"\
 * 	names	\n\
 *		Takes no arguments and returns the names of known\n\
 *		ascend environment variables as a Tcl list.  \n\
"

#define Asc_EnvVarCmdHL7 \
"\
 * 	put	input_string\n\
 *		Parses the input string using the form %s = %s to extract\n\
 *		a varname (before the =) and path value. The path value is\n\
 *		assumed to be separated by the platform value of pathdiv\n\
 *		as defined for getenv above and split accordingly into\n\
 *		pathelements.\n\
"

#define Asc_EnvVarCmdHL8 \
"\
 * 	set	var path \n\
 *		Like putenv, but you have done the split on = for us.\n\
 *  \n\
"

#define Asc_EnvVarCmdHL9 \
"\
 * These commands manage a global array of strings in the process. \n\
 * This global array is somewhat more intelligent than the average\n\
 * UNIX environment variable implementation for our purposes. \n\
 * These strings are accessible to other clients through the C api \n\
"

#define Asc_EnvVarCmdHL10 \
"\
 * in utilities/ascEnvVar.h. We do this because we don't want our \n\
 * application core including Tcl code, and because Windoze doesn't\n\
 * handle environment variables reliably. \n\
 * These functi/ns will not work until the C api has been initialized \n\
 * elsewhere.\n\
"

#endif // EnvVarProc_json_h
