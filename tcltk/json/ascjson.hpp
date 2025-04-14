#ifndef ascjson_h
#define ascjson_h

#include "tcltk/json/ascjson_util.hpp"
#include "tcltk/json/argv.h"

#include "tcltk/help-data/HelpProc.data.h"
#include "tcltk/json/HelpProcDS.hpp"
#include "tcltk/help-data/LibraryProc.data.h"

#include "BrowserMethod.json.h"
#include "BrowserProc.json.h"
#include "BrowserQuery.json.h"
#include "EnvVarProc.json.h"
#include "ProbeProc.json.h"
#include "SimsProc.json.h"
#include "SlvProc.json.h"
#include "SolverProc.json.h"
#include "typelex.json.h"
#include <string>
#include <map>

/** 
 * Ascend modeling environment context encapsulating the globals
 * and an api compatible with any language that can handle
 * json or vtab separated strings.
 *
 * Multiple instances of this should be able to coexist, where
 * each maintains its own library and its own set of UNIVERSAL
 * atom/model types. Need to update bison/flex under zz_parse to
 * avoid globals before this will be true.
 *
 * All rcp pointers returned from this class are
 * allocated at least until the next call to the 
 * same function or the destructor of ascjson is called.
 * They do not need to be freed by the caller.
 * Under this model, strings should be parsed to objects,
 * and immediately forgotten by the wrapping language.
 *
 */

class ascjson {

private:
	/* data and core utility functions */
	std::map<std::string, struct rc *> m;
	void setc(const char *func, enum vtype st, const char *c, int rc);
	void setc(const char *func, enum vtype st, std::string& val, int rc);
	rcp getc(const char *func);
	bool bad;
	
public:

	ascjson();

	~ascjson();

	int config(const char *vargv);

	/* Scripting interface functions */

	rcp Asc_LibrQueryTypeCmdHC (const char *vargv);

	rcp Asc_LibrOptionsCmdHC (const char *vargv) ;

	rcp Asc_LibrParseCmdHC (const char *vargv) ;

	rcp Asc_LibrReadCmdHC (const char *vargv) ;

	rcp Asc_LibrModuleInfoCmdHC (const char *vargv) ;

	rcp Asc_LibrDestroyTypesCmdHC (const char *vargv) ;

	rcp Asc_LibrHideTypeCmdHC (const char *vargv) ;

	rcp Asc_LibrUnHideTypeCmdHC (const char *vargv) ;

	rcp Asc_LibrTypeIsShownCmdHC (const char *vargv) ;

	rcp Asc_LibrTypeListCmdHC (const char *vargv) ;

	rcp Asc_ExtractTypeHC (const char *vargv) ;

	rcp qlfdid (const char *vargv) ;

	rcp bgetproc (const char *vargv) ;

	rcp Asc_BrowInitializeCmdHC (const char *vargv) ;

	rcp brow_assign (const char *vargv) ;

	rcp qassgn3 (const char *vargv) ;

	rcp x__brow_iname (const char *vargv) ;

	rcp x__brow_isrelation (const char *vargv) ;

	rcp x__brow_ismodel (const char *vargv) ;

	rcp bgetrels (const char *vargv) ;

	rcp bgetrelspf (const char *vargv) ;

	rcp x__brow_relsforatom (const char *vargv) ;

	rcp x__brow_islogrel (const char *vargv) ;

	rcp bgetlogrels (const char *vargv) ;

	rcp bgetlogrelspf (const char *vargv) ;

	rcp x__brow_logrelsforatom (const char *vargv) ;

	rcp bgetcondrels (const char *vargv) ;

	rcp bgetcondlogrels (const char *vargv) ;

	rcp x__brow_iswhen (const char *vargv) ;

	rcp x__brow_isinstanceinwhen (const char *vargv) ;

	rcp bgetwhens (const char *vargv) ;

	rcp x__brow_whensforinstance (const char *vargv) ;

	rcp bwritependings (const char *vargv) ;

	rcp bnumpendings (const char *vargv) ;

	rcp count_names (const char *vargv) ;

	rcp aliases (const char *vargv) ;

	rcp isas (const char *vargv) ;

	rcp cliques (const char *vargv) ;

	rcp Asc_BrowWriteInstanceCmdHC (const char *vargv) ;

	rcp bmerge (const char *vargv) ;

	rcp brefine (const char *vargv) ;

	rcp bmakealike (const char *vargv) ;

	rcp b_isplottable (const char *vargv) ;

	rcp b_prepplotfile (const char *vargv) ;

	rcp free_all_vars (const char *vargv) ;

	rcp Asc_SimBinTokenSetOptionsHC (const char *vargv) ;

	rcp sims (const char *vargv) ;

	rcp sim_instantiate (const char *vargv) ;

	rcp sim_unique (const char *vargv) ;

	rcp sim_reinstantiate (const char *vargv) ;

	rcp x__sims_copy (const char *vargv) ;

	rcp x__sims_proto (const char *vargv) ;

	rcp x__sims_saveinst (const char *vargv) ;

	rcp sim_destroy (const char *vargv) ;

	rcp simlistpending (const char *vargv) ;

	rcp ddefine (const char *vargv) ;

	rcp ddiffdefine (const char *vargv) ;

	rcp disp (const char *vargv) ;

	rcp hier (const char *vargv) ;

	rcp file_by_type (const char *vargv) ;

	rcp dchild (const char *vargv) ;

	rcp drefines_me (const char *vargv) ;

	rcp drefines_meall (const char *vargv) ;

	rcp drefinement_tree (const char *vargv) ;

	rcp dgetparts (const char *vargv) ;

	rcp disroot_type (const char *vargv) ;

	rcp Asc_ProbeCmdHC (const char *vargv) ;

	rcp x__var_analyze (const char *vargv) ;

	rcp x__rel_analyze (const char *vargv) ;

	rcp x__userdata_init (const char *vargv) ;

	rcp x__userdata_create (const char *vargv) ;

	rcp x__userdata_destroy (const char *vargv) ;

	rcp x__userdata_save (const char *vargv) ;

	rcp x__userdata_restore (const char *vargv) ;

	rcp x__userdata_query (const char *vargv) ;

	rcp x__userdata_print (const char *vargv) ;

	rcp get_model_children (const char *vargv) ;

	rcp slv_checksim (const char *vargv) ;

	rcp slv_checksys (const char *vargv) ;

	rcp slv_get_obj_list (const char *vargv) ;

	rcp slv_set_obj_by_num (const char *vargv) ;

	rcp Asc_SolvGetObjNumCmdHC (const char *vargv) ;

	rcp slv_get_parms (const char *vargv) ;

	rcp set_slv_parms (const char *vargv) ;

	rcp set_slv_parmsnew (const char *vargv) ;

	rcp slv_get_parmsnew (const char *vargv) ;

	rcp slv_get_insttype (const char *vargv) ;

	rcp slv_get_cost_page (const char *vargv) ;

	rcp slv_get_stat_page (const char *vargv) ;

	rcp slv_get_objval (const char *vargv) ;

	rcp slv_get_instname (const char *vargv) ;

	rcp slv_get_pathname (const char *vargv) ;

	rcp slv_get_vr (const char *vargv) ;

	rcp slvdump (const char *vargv) ;

	rcp slv_presolve (const char *vargv) ;

	rcp slv_reanalyze (const char *vargv) ;

	rcp slv_check_and_reanalyze (const char *vargv) ;

	rcp slv_set_independent (const char *vargv) ;

	rcp slv_resolve (const char *vargv) ;

	rcp slv_solve (const char *vargv) ;

	rcp slv_iterate (const char *vargv) ;

	rcp slv_available (const char *vargv) ;

	rcp slv_number (const char *vargv) ;

	rcp slv_name (const char *vargv) ;

	rcp slv_linsol_names (const char *vargv) ;

	rcp slv_eligible_solver (const char *vargv) ;

	rcp slv_select_solver (const char *vargv) ;

	rcp slv_get_solver (const char *vargv) ;

	rcp slv_flush_solver (const char *vargv) ;

	rcp slv_import_qlfdid (const char *vargv) ;

	rcp slv_lnmget (const char *vargv) ;

	rcp slv_lnmset (const char *vargv) ;

	rcp slv_set_haltflag (const char *vargv) ;

	rcp slvhelp (const char *vargv) ;

	rcp slv_near_bounds (const char *vargv) ;

	rcp slv_far_from_nominals (const char *vargv) ;

	rcp Asc_SolveMonitorCmdHC (const char *vargv) ;

	rcp dbg_get_blk_of_var (const char *vargv) ;

	rcp dbg_get_blk_of_eqn (const char *vargv) ;

	rcp dbg_get_blk_coords (const char *vargv) ;

	rcp dbg_get_eqn_of_var (const char *vargv) ;

	rcp dbg_get_varpartition (const char *vargv) ;

	rcp dbg_get_eqnpartition (const char *vargv) ;

	rcp dbg_list_rels (const char *vargv) ;

	rcp dbg_list_vars (const char *vargv) ;

	rcp dbg_write_var (const char *vargv) ;

	rcp dbg_write_unattvar (const char *vargv) ;

	rcp brow_write_var (const char *vargv) ;

	rcp dbg_write_rel (const char *vargv) ;

	rcp brow_write_rel (const char *vargv) ;

	rcp dbg_write_obj (const char *vargv) ;

	rcp brow_write_obj (const char *vargv) ;

	rcp dbg_write_varattr (const char *vargv) ;

	rcp dbg_write_qlfattr (const char *vargv) ;

	rcp dbg_rel_included (const char *vargv) ;

	rcp dbg_var_fixed (const char *vargv) ;

	rcp dbg_get_incidence (const char *vargv) ;

	rcp dbg_get_order (const char *vargv) ;

	rcp dbg_write_incidence (const char *vargv) ;

	rcp dbg_find_eligible (const char *vargv) ;

	rcp brow_find_eligible (const char *vargv) ;

	rcp dbg_consistency_analysis (const char *vargv) ;

	rcp dbg_global_eligible (const char *vargv) ;

	rcp dbg_find_activerels (const char *vargv) ;

	rcp brow_find_activerels (const char *vargv) ;

	rcp dbg_struct_singular (const char *vargv) ;

	rcp dbg_num_block_singular (const char *vargv) ;

	rcp var_free2nom (const char *vargv) ;

	rcp var_nom2free (const char *vargv) ;

	rcp dbg_calc_relnoms (const char *vargv) ;

	rcp dbg_check_rels (const char *vargv) ;

	rcp dbg_write_slv0_xsys (const char *vargv) ;

	rcp dbg_write_slv0_sys (const char *vargv) ;

	rcp dbg_mtxwriteplot (const char *vargv) ;

	rcp dbg_calc_jacobian (const char *vargv) ;

	rcp dbghelp (const char *vargv) ;

	rcp integrate_setup (const char *vargv) ;

	rcp integrate_cleanup (const char *vargv) ;

	rcp integrate_set_y_file (const char *vargv) ;

	rcp integrate_set_obs_file (const char *vargv) ;

	rcp integrate_logunits (const char *vargv) ;

	rcp integrate_logformat (const char *vargv) ;

	rcp integrate_get_samples (const char *vargv) ;

	rcp integrate_set_samples (const char *vargv) ;

	rcp integrate_able (const char *vargv) ;

	rcp x__mtx_norms (const char *vargv) ;

	rcp mtx_gui_plot_incidence (const char *vargv) ;

	rcp mtxhelp (const char *vargv) ;

	rcp u_destroy_units (const char *vargv) ;

	rcp u_setSIdef (const char *vargv) ;

	rcp u_getbasedef (const char *vargv) ;

	rcp u_dump (const char *vargv) ;

	rcp u_dims (const char *vargv) ;

	rcp u_dim_setverify (const char *vargv) ;

	rcp u_dim2num (const char *vargv) ;

	rcp u_num2dim (const char *vargv) ;

	rcp u_frombasedim (const char *vargv) ;

	rcp u_fromatomdim (const char *vargv) ;

	rcp u_getdimatoms (const char *vargv) ;

	rcp u_change_baseunit (const char *vargv) ;

	rcp u_getprec (const char *vargv) ;

	rcp u_setprec (const char *vargv) ;

	rcp u_get_atoms (const char *vargv) ;

	rcp u_get_units (const char *vargv) ;

	rcp u_set_user (const char *vargv) ;

	rcp u_get_user (const char *vargv) ;

	rcp u_get_list (const char *vargv) ;

	rcp u_clear_user (const char *vargv) ;

	rcp u_getval (const char *vargv) ;

	rcp u_browgetval (const char *vargv) ;

	rcp u_slvgetrelval (const char *vargv) ;

	rcp u_slvgetvarval (const char *vargv) ;

	rcp u_slvgetobjval (const char *vargv) ;

	rcp uhelp (const char *vargv) ;

	rcp srefine (const char *vargv) ;

	rcp smerge (const char *vargv) ;

	rcp ascloadwin (const char *vargv) ;

	rcp gnutext (const char *vargv) ;

	rcp Asc_HelpCmdHC (const char *vargv);

	int call_all_force_link();

private:
	/* processing for internals, grouped by implementation file. */

	void register_command_help();

	/* HelpProcDS.ipp */
	// macros for simplifying food of the help system.
	
	/* forward declaration of a scripting function's long help fetcher */ 
#define STDHLF_H(baseName) static char * LHN(baseName )(void)

	/**
	 * Defines a help function using an arbitrary number of strings.
	 *
	 * To get past the string literal limits of compilers,
	 * use the following macro in your .ipp file like so:
	 * STDHLF(yourCmd,(your,list,of,macro,strings,HLFSTOP)); 
	 * Then you can pass yourCmdHLF into the registration of your command.
	 * Generally, each option of a command with many options should get
	 * its own macro string. The ANSI C limit on such strings used to be 509 chars.
	 * Use STDHLF_H(yourCmd); in your header file to define your function.<br><br>
	 */
#define STDHLF(baseName,list_of_strings_in_parens_end_in_null) \
	char * ascjson:: LHN(baseName )(void) \
	{ \
	  char *result; \
	  result = HelpBuildString list_of_strings_in_parens_end_in_null ; \
	  return result; \
	} extern int g_does_not_exist_so_do_not_use /* hack to use a ; after\} */


	/**
	 * Builds a long string from an arbitrary number of input strings
	 * so long as the last string is NULL and the first is not.
	 * All arguments must be strings (char *).
	 * The string returned is the callers responsibility to free.
	 * Up to the first MAXHELPARGS are used in the returned string.
	 *
	 * HelpBuildString(char *, variable_number_of_args,NULL);
	 */
	static char *HelpBuildString(const char *first, ...);

	/**
	 * Returns HELP_BREAK if a help message is printed to the interpreter
	 * or HELP_OK if not.
	 * If this functions returns HELP_BREAK, the caller should return.
	 */
	int Asc_HelpCheckDS(Asc_DString *hPtr, int argc, char **argv);

	/**
	 *  Initializes the help data structures so that commands can then
	 *  be registered.
	 */
	int Asc_HelpInit(void);

	/**
	 *  Destroys the help data structures. This should only be called at
	 *  shutdown.
	 */
	void Asc_HelpDestroy(void);

	/**
	 *  Defines the generic description for the command group given.
	 *  explanation should start with Explanation:
	 *  group should not contain whitespace.
	 *  Follows tcl return conventions.
	 */
	int Asc_HelpDefineGroup(const char *froup, const char *explanation);

	/**
	 *  Defines the help strings for the command name given.
	 *
	 *  All the strings given should be static strings, because we may
	 *  store pointers to them.
	 *  HLFunc should return a string that we become the owner of, or NULL.
	 *  All 5 parameters should be coming via defines in the
	 *  C header files for the function being documented.
	 *  See Asc_HelpCmd() for an example of same.<br><br>
	 *
	 *  If you have not written up someplace else the information that
	 *  is required here, you obviously have not thought the command you
	 *  are registering through and you should not be registering it.
	 *
	 *  @param name     The tcl registration string.
	 *  @param group    The command group name.  It should not contain whitespace.
	 *  @param usage    The command syntax by example or in man page style.
	 *  @param desc     The basic one-line description of the command. (<= 70 char)
	 *  @param longfunc Function returning the detailed explanation of the command.
	 */
	int Asc_HelpDefine(const char *name, const char *group, const char *usage, const char *desc, HLFunc longfunc);

	const char *Asc_HelpGetShortDS(Asc_DString *hPtr, const char *commandname);

	/**
	 *  Returns the long string for the command given, if there is one.
	 *  If interp is not NULL, also appends the returned string to the
	 *  interpreter's result string.
	 */
	const char *Asc_HelpGetLongDS(Asc_DString *hPtr, const char *commandname);

	/**
	 *  Returns the usage string for the command given, if there is one.
	 *  If hPtr is not NULL, also appends the returned string to the
	 *  interpreter's result string.
	 */
	const char *Asc_HelpGetUsageDS(Asc_DString *hPtr, const char *commandname);

	/**
	 *  Returns the members of a group, and the group explanation
	 *  if there is one, in the interpreter.
	 *  All is in a vtab  list format and the explanation is first
	 *  if it exists. If group does not exist, returns an error message.
	 *  By use convention: the group explanation is a list element which
	 *  starts with "Explanation".
	 *  The return code follows TCL conventions.
	 */
	int Asc_HelpGetGroupDS(Asc_DString *hPtr, const char *groupname);

	/**
	 *  Appends the complete vtab list of groups to the interpreter.
	 *  Commands are sorted in some reasonable fashion.
	 *  The return code follows HELP_ conventions.
	 */
	int Asc_HelpCommandGroupsDS(Asc_DString *hPtr);

	/**
	 *  Appends the complete vtab list of commands to the interpreter.
	 *  Commands are sorted in alphabetical order.
	 *  The return code follows HELP_  conventions.
	 */
	int Asc_HelpCommandListDS(Asc_DString *hPtr);

	/**
	 *  Appends the complete vtab list of commands to the interpreter.
	 *  Groups are sorted and then commands are sorted in alphabetical order.
	 *  The return code follows HELP_ conventions.
	 */
	int Asc_HelpCommandsByGroupsDS(Asc_DString *hPtr);

	/**
	 * This is the dstring callback for our commandline help facility.
	 */
	int Asc_HelpCmdDS(Asc_DString *hPtr, int arcg, char **argv);
	STDHLF_H(Asc_HelpCmdHC);

#include "tcltk/json/LibraryProcDS.hpp"

	STDHLF_H(Asc_EnvVarCmdHC);
	STDHLF_H(Asc_TimeCmd);
	STDHLF_H(Asc_LibrQueryTypeCmdHC);
	STDHLF_H(Asc_LibrOptionsCmdHC);
	STDHLF_H(Asc_LibrParseCmdHC);
	STDHLF_H(Asc_LibrReadCmdHC);
	STDHLF_H(Asc_LibrModuleInfoCmdHC);
	STDHLF_H(Asc_LibrDestroyTypesCmdHC);
	STDHLF_H(Asc_LibrHideTypeCmdHC);
	STDHLF_H(Asc_LibrUnHideTypeCmdHC);
	STDHLF_H(Asc_LibrTypeIsShownCmdHC);
	STDHLF_H(Asc_LibrTypeListCmdHC);
	STDHLF_H(Asc_ExtractType);
	STDHLF_H(Asc_BrowInitializeCmd);
	STDHLF_H(Asc_SimBinTokenSetOptions);
	STDHLF_H(Asc_ProbeCmd);
	STDHLF_H(Asc_SolvGetObjNumCmd);
	STDHLF_H(Asc_SolveMonitorCmd);
#if 0
	STDHLF_H();
	STDHLF_H();
	STDHLF_H();
	STDHLF_H();
	STDHLF_H();
	STDHLF_H();
	STDHLF_H();
	STDHLF_H();
	STDHLF_H();
	STDHLF_H();
	STDHLF_H();
	STDHLF_H();
#endif

};
#endif // ascjson_h
