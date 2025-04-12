/*
 *  by Ben Allan
 *  Created: 3/2025
 *  Version: 1
 *
 *  This file is part of the ASCEND json interface
 *
 *  Copyright 2025 Benjamin Andrew Allan
 *
 *  The ASCEND json nterface is free software; you can redistribute
 *  it and/or modify it under the terms of the GNU General Public License as
 *  published by the Free Software Foundation; either version 2 of the
 *  License, or (at your option) any later version.
 *
 *  The ASCEND json interface is distributed in hope that it will be
 *  useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#define KILLBDAG 0
#define REIMPLEMENT 0

extern "C" {
#include <ascend/utilities/config.h>
#include <ascend/utilities/ascSignal.h>
#include "config.h"
#include <ascend/general/ascMalloc.h>
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
#include <ascend/compiler/slist.h>
#include <ascend/compiler/child.h>
#include <ascend/compiler/childio.h>
#include <ascend/compiler/type_desc.h>
#include <ascend/compiler/typedef.h>
#include <ascend/compiler/extfunc.h>
#include <ascend/compiler/library.h>
#include <ascend/compiler/prototype.h>
#include <ascend/compiler/proc.h>
#include <ascend/compiler/nameio.h>
#include <ascend/compiler/parser.h>
#include <ascend/system/slv_types.h>

#include <ascend/compiler/instance_name.h>
#include <ascend/compiler/units.h>

#include <ascend/linear/mtx.h>
#include <ascend/system/slv_types.h>
#include <ascend/system/slv_client.h>
#include <ascend/solver/solver.h>
}

#include "ascjson.hpp"

#include <string>
#include <map>
#include <cerrno>

/* convert string to argv by splitting. must be called before ASCUSE */
#define INIT_ARGV(vargv) \
	if (bad) { \
		setc(__func__, SVcstr, " badness detected. restart needed.", -EPERM); \
		return getc(__func__); \
	} \
        int argc; \
        char **argv; \
        int err = toArgv(vargv, "\v", &argc, &argv); \
	if (err) setc(__func__, SVcstr, " vargv failed", -err)

/* clear data created by INIT_ARGV */
#define FREE_ARGV freeArgv(argv)

#define unimplemented \
	setc(__func__, SVcstr, "unimplemented", 0); \
	return getc(__func__)

static void banner()
{
  ASC_FPRINTF(stderr,"\nASCEND modeling environment\n");
  ASC_FPRINTF(stderr,"Copyright(C) 1997, 2006-2007 Carnegie Mellon University\n");
  ASC_FPRINTF(stderr,"Copyright(C) 1993-1996 Kirk Andre Abbott, Ben Allan\n");
  ASC_FPRINTF(stderr,"Copyright(C) 1990, 1993, 1994 Thomas Guthrie Epperly\n");
  // ASC_FPRINTF(stderr,"Built %s %s %s\n\n",__DATE__,__TIME__,build_name);
  ASC_FPRINTF(stderr,"ASCEND comes with ABSOLUTELY NO WARRANTY, and is free software that you may\n");
  ASC_FPRINTF(stderr,"redistribute within the conditions of the GNU General Public License. See the\n");
  ASC_FPRINTF(stderr,"included file 'LICENSE.txt' for full details.\n\n");
}

static
void AscTrap(int sig)
{
  putchar('\n');
  Asc_Panic(sig, "AscTrap", "Caught Signal: %d", sig);
}

int g_interface_simplify_relations = 0;

static void cInit()
{
#ifdef ASC_SIGNAL_TRAPS
	(void)SIGNAL(SIGINT, AscTrap);
#endif

  if ( Asc_CompilerInit(g_interface_simplify_relations) != 0 ) {
    Asc_Panic(2, "Asc_CompilerInit",
              "Insufficient memory to initialize compiler.");
  }

  SlvRegisterStandardClients();
}

static void cFinal()
{
//  Asc_UnitValue(NULL); fixme. need unitproc.ipp
//  Asc_SolvMemoryCleanup(); fixme need solverproc.ipp
  Asc_CompilerDestroy();
  // Asc_DestroyEnvironment(); fixme need utilities/ascenvvar.ipp
}

ascjson::ascjson()
{
#if 0
	bad = false;
        int argc; \
        char **argv; \
        int err = toArgv(vargv, "\v", &argc, &argv); \
	if (err) {
		setc(__func__, SVcstr, " vargv failed", -err);
		bad = true;
		return;
	}
#endif

	banner();
	cInit();
	if( Asc_HelpInit() == HELP_ERROR ) {
		bad = true;
			Asc_Panic(2, "Asc_HelpInit",
		"Insufficient memory to initialize help system.");
	}
	register_command_help();
	// freeArgv(argv);
}

ascjson::~ascjson()
{
	Asc_HelpDestroy();
	cFinal();
	for ( auto i : m ) {
		delete i.second;
	}
	// delete iterate clear m
}

int ascjson::config( const char *argv )
{
	return 0;
}

void ascjson::setc(const char *idx, ENUM vtype t, std::string& v, int r)
{
	std::string msg = std::string(idx) + ": " + v;
	if (!m.count(idx)) {
		struct rc *c = new rc(msg, r, t);
		m[idx] = c;
	} else {
		m[idx]->e = r;
		m[idx]->s = msg;
		m[idx]->v = m[idx]->s.c_str();
		m[idx]->t = t;
	}
}

void ascjson::setc(const char* idx, ENUM vtype t, const char *v, int r)
{
	std::string msg = std::string(idx) + ": " + v;
	if (!m.count(idx)) {
		struct rc *c = new rc(msg, r, t);
		m[idx] = c;
	} else {
		m[idx]->e = r;
		m[idx]->s = msg;
		m[idx]->v = m[idx]->s.c_str();
		m[idx]->t = t;
	}
}

rcp ascjson::getc(const char *idx)
{
	return m[idx];
}

rcp ascjson::Asc_LibrQueryTypeCmdHC (const char *vargv) 
{
	wrap_dstring(Asc_LibrQueryTypeCmdDS, SVcstr);
}

rcp ascjson::Asc_LibrOptionsCmdHC (const char *vargv) 
{
	wrap_dstring(Asc_LibrOptionsCmdDS, SVcstr);
}

rcp ascjson::Asc_LibrParseCmdHC (const char *vargv) 
{
	wrap_dstring(Asc_LibrParseCmdDS, SVcstr);
}

rcp ascjson::Asc_LibrReadCmdHC (const char *vargv) 
{
	wrap_dstring(Asc_LibrReadCmdDS, SVcstr);
}

rcp ascjson::Asc_LibrModuleInfoCmdHC (const char *vargv) 
{
	wrap_dstring(Asc_LibrModuleInfoCmdDS, SVcstr);
}

// reduce these to wrap_dstring
rcp ascjson::Asc_LibrDestroyTypesCmdHC (const char *vargv) 
{
	// INIT_ARGV(vargv);
	if (bad) { 
                setc(__func__, SVcstr, " badness detected. restart needed.", -EPERM); 
                return getc(__func__); 
        } 
        int argc; 
        char **argv; 
        int err = toArgv(vargv, "\v", &argc, &argv); 
        if (err) setc(__func__, SVcstr, " vargv failed", -err);

	ASCUSE;
	err = Asc_LibrDestroyTypesCmdDS(&hds, argc, argv);
	setc(__func__, SVcstr, Asc_DStringValue(&hds), err);
	Asc_DStringFree(&hds);
	FREE_ARGV;
	return getc(__func__);
}

rcp ascjson::Asc_LibrHideTypeCmdHC (const char *vargv) 
{
	INIT_ARGV(vargv);
	ASCUSE;
	err = Asc_LibrHideTypeCmdDS(&hds, argc, argv);
	setc(__func__, SVcstr, Asc_DStringValue(&hds), err);
	Asc_DStringFree(&hds);
	FREE_ARGV;
	return getc(__func__);
}

rcp ascjson::Asc_LibrUnHideTypeCmdHC (const char *vargv) 
{
	INIT_ARGV(vargv);
	ASCUSE;
	err = Asc_LibrUnHideTypeCmdDS(&hds, argc, argv);
	setc(__func__, SVcstr, Asc_DStringValue(&hds), err);
	Asc_DStringFree(&hds);
	FREE_ARGV;
	return getc(__func__);
}

rcp ascjson::Asc_LibrTypeIsShownCmdHC (const char *vargv) 
{
	INIT_ARGV(vargv);
	ASCUSE;
	err = Asc_LibrTypeIsShownCmdDS(&hds, argc, argv);
	setc(__func__, SVcstr, Asc_DStringValue(&hds), err);
	Asc_DStringFree(&hds);
	FREE_ARGV;
	return getc(__func__);
}

rcp ascjson::Asc_LibrTypeListCmdHC (const char *vargv) 
{
	INIT_ARGV(vargv);
	ASCUSE;
	err = Asc_LibrTypeListCmdDS(&hds, argc, argv);
	setc(__func__, SVcstr, Asc_DStringValue(&hds), err);
	Asc_DStringFree(&hds);
	FREE_ARGV;
	return getc(__func__);
}

rcp ascjson::Asc_ExtractTypeHC (const char *vargv) 
{
#if 0
	ASCUSE;  /* see if first arg is -help */
	err = Asc_ExtractTypeDS(&hds, argc, argv);
	setc(__func__, SVcstr, Asc_DStringValue(&hds), err);
	Asc_DStringFree(&hds);
	FREE_ARGV;
	return getc(__func__);
#else
	unimplemented;
#endif
}

// browser

rcp ascjson::qlfdid (const char *vargv) 
{
	unimplemented;
	//Asc_BrowQlfdidSearchCmd
}

rcp ascjson::bgetproc (const char *vargv) 
{
	unimplemented;
	// Asc_BrowWriteProcedure
}

rcp ascjson::Asc_BrowInitializeCmdHC (const char *vargv) 
{
	unimplemented;
	// Asc_BrowInitializeCmd
}

rcp ascjson::brow_assign (const char *vargv) 
{
	unimplemented;
	// Asc_BrowRunAssignmentCmd
}

rcp ascjson::qassgn3 (const char *vargv) 
{
	unimplemented;
	// Asc_BrowRunAssignQlfdidCmd3
}

rcp ascjson::x__brow_iname (const char *vargv) 
{
	unimplemented;
	// Asc_BrowWriteInstanceNameCmd
}

rcp ascjson::x__brow_isrelation (const char *vargv) 
{
	unimplemented;
	// Asc_BrowIsRelationCmd
}

rcp ascjson::x__brow_ismodel (const char *vargv) 
{
	unimplemented;
	// Asc_BrowIsModelCmd
}

rcp ascjson::bgetrels (const char *vargv) 
{
	unimplemented;
	// Asc_BrowWriteRelListCmd
}

rcp ascjson::bgetrelspf (const char *vargv) 
{
	unimplemented;
	// Asc_BrowWriteRelListPostfixCmd
}

rcp ascjson::x__brow_relsforatom (const char *vargv) 
{
	unimplemented;
	// Asc_BrowWriteRelsForAtomCmd
}

rcp ascjson::x__brow_islogrel (const char *vargv) 
{
	unimplemented;
	// Asc_BrowIsLogRelCmd
}

rcp ascjson::bgetlogrels (const char *vargv) 
{
	unimplemented;
	// Asc_BrowWriteLogRelListCmd
}

rcp ascjson::bgetlogrelspf (const char *vargv) 
{
	unimplemented;
	// Asc_BrowWriteLogRelListPostfixCmd
}

rcp ascjson::x__brow_logrelsforatom (const char *vargv) 
{
	unimplemented;
	// Asc_BrowWriteLogRelsForAtomCmd
}

rcp ascjson::bgetcondrels (const char *vargv) 
{
	unimplemented;
	// Asc_BrowWriteCondRelListCmd
}

rcp ascjson::bgetcondlogrels (const char *vargv) 
{
	unimplemented;
	// Asc_BrowWriteCondLogRelListCmd
}

rcp ascjson::x__brow_iswhen (const char *vargv) 
{
	unimplemented;
	// Asc_BrowIsWhenCmd
}

rcp ascjson::x__brow_isinstanceinwhen (const char *vargv) 
{
	unimplemented;
	// Asc_BrowIsInstanceInWhenCmd
}

rcp ascjson::bgetwhens (const char *vargv) 
{
	unimplemented;
	// Asc_BrowWriteWhenListCmd
}

rcp ascjson::x__brow_whensforinstance (const char *vargv) 
{
	unimplemented;
	// Asc_BrowWriteWhensForInstanceCmd
}

rcp ascjson::bwritependings (const char *vargv) 
{
	unimplemented;
	// Asc_BrowWritePendingsSTDOUT
}

rcp ascjson::bnumpendings (const char *vargv) 
{
	unimplemented;
	// Asc_BrowShowPendings
}

rcp ascjson::count_names (const char *vargv) 
{
	unimplemented;
	// Asc_BrowCountNamesCmd
}

rcp ascjson::aliases (const char *vargv) 
{
	unimplemented;
	// Asc_BrowWriteAliasesCmd
}

rcp ascjson::isas (const char *vargv) 
{
	unimplemented;
	// Asc_BrowWriteISAsCmd
}

rcp ascjson::cliques (const char *vargv) 
{
	unimplemented;
	// Asc_BrowWriteCliqueCmd
}

rcp ascjson::Asc_BrowWriteInstanceCmdHC (const char *vargv) 
{
	unimplemented;
	// Asc_BrowWriteInstanceCmd
}

rcp ascjson::bmerge (const char *vargv) 
{
	unimplemented;
	// Asc_BrowInstanceMergeCmd
}

rcp ascjson::brefine (const char *vargv) 
{
	unimplemented;
	// Asc_BrowInstanceRefineCmd
}

rcp ascjson::bmakealike (const char *vargv) 
{
	unimplemented;
	// Asc_BrowMakeAlikeCmd
}

rcp ascjson::b_isplottable (const char *vargv) 
{
	unimplemented;
	// Asc_BrowIsPlotAllowedCmd
}

rcp ascjson::b_prepplotfile (const char *vargv) 
{
	unimplemented;
	// Asc_BrowPreparePlotFileCmd
}

rcp ascjson::free_all_vars (const char *vargv) 
{
	unimplemented;
	// Asc_BrowClearVarsCmd
}

rcp ascjson::Asc_SimBinTokenSetOptionsHC (const char *vargv) 
{
	unimplemented;
	// Asc_SimBinTokenSetOptions
}

rcp ascjson::sims (const char *vargv) 
{
	unimplemented;
	// Asc_SimsQueryCmd
}

rcp ascjson::sim_instantiate (const char *vargv) 
{
	unimplemented;
	// Asc_SimsCreateInstanceCmd
}

rcp ascjson::sim_unique (const char *vargv) 
{
	unimplemented;
	// Asc_SimsUniqueNameCmd
}

rcp ascjson::sim_reinstantiate (const char *vargv) 
{
	unimplemented;
	// Asc_SimsResumeInstantiateCmd
}

rcp ascjson::x__sims_copy (const char *vargv) 
{
	unimplemented;
	// Asc_SimsCopyInstanceCmd
}

rcp ascjson::x__sims_proto (const char *vargv) 
{
	unimplemented;
	// Asc_SimsProtoTypeInstanceCmd
}

rcp ascjson::x__sims_saveinst (const char *vargv) 
{
	unimplemented;
	// Asc_SimsSaveInstanceCmd
}

rcp ascjson::sim_destroy (const char *vargv) 
{
	unimplemented;
	// Asc_SimsDestroySimulationCmd
}

rcp ascjson::simlistpending (const char *vargv) 
{
	unimplemented;
	// Asc_SimListPending
}

rcp ascjson::ddefine (const char *vargv) 
{
	unimplemented;
	// Asc_DispDefineCmd
}

rcp ascjson::ddiffdefine (const char *vargv) 
{
	unimplemented;
	// Asc_DispDiffDefineCmd
}

rcp ascjson::disp (const char *vargv) 
{
	unimplemented;
	// Asc_DispQueryCmd
}

rcp ascjson::hier (const char *vargv) 
{
	unimplemented;
	// Asc_DispHierarchyCmd
}

rcp ascjson::file_by_type (const char *vargv) 
{
	unimplemented;
	// Asc_DispFileByTypeCmd
}

rcp ascjson::dchild (const char *vargv) 
{
	unimplemented;
	// Asc_DispChildOneCmd
}

rcp ascjson::drefines_me (const char *vargv) 
{
	unimplemented;
	// Asc_DispRefinesMeCmd
}

rcp ascjson::drefines_meall (const char *vargv) 
{
	unimplemented;
	// Asc_DispRefinesMeCmd cd=1
}

rcp ascjson::drefinement_tree (const char *vargv) 
{
	unimplemented;
	// Asc_DispRefinesMeTreeCmd
}

rcp ascjson::dgetparts (const char *vargv) 
{
	unimplemented;
	// Asc_DispTypePartsCmd
}

rcp ascjson::disroot_type (const char *vargv) 
{
	unimplemented;
	// Asc_DispIsRootTypeCmd
}

rcp ascjson::Asc_ProbeCmdHC (const char *vargv) 
{
	unimplemented;
	// Asc_ProbeCmd
}

rcp ascjson::x__var_analyze (const char *vargv) 
{
	unimplemented;
	// Asc_VarAnalyzeCmd
}

rcp ascjson::x__rel_analyze (const char *vargv) 
{
	unimplemented;
	// Asc_RelAnalyzeCmd
}

rcp ascjson::x__userdata_init (const char *vargv) 
{
	unimplemented;
	// Asc_UserDataInitializeCmd
}

rcp ascjson::x__userdata_create (const char *vargv) 
{
	unimplemented;
	// Asc_UserDataCreateCmd
}

rcp ascjson::x__userdata_destroy (const char *vargv) 
{
	unimplemented;
	// Asc_UserDataDestroyCmd
}

rcp ascjson::x__userdata_save (const char *vargv) 
{
	unimplemented;
	// Asc_UserDataSaveValuesCmd
}

rcp ascjson::x__userdata_restore (const char *vargv) 
{
	unimplemented;
	// Asc_UserDataRestoreValuesCmd
}

rcp ascjson::x__userdata_query (const char *vargv) 
{
	unimplemented;
	// Asc_UserDataQueryCmd
}

rcp ascjson::x__userdata_print (const char *vargv) 
{
	unimplemented;
	// Asc_UserDataPrintLibrary
}

rcp ascjson::get_model_children (const char *vargv) 
{
	unimplemented;
	// Asc_SolvGetModKids
}

rcp ascjson::slv_checksim (const char *vargv) 
{
	unimplemented;
	// Asc_SolvIncompleteSim
}

rcp ascjson::slv_checksys (const char *vargv) 
{
	unimplemented;
	// Asc_SolvCheckSys
}

rcp ascjson::slv_get_obj_list (const char *vargv) 
{
	unimplemented;
	// Asc_SolvGetObjList
}

rcp ascjson::slv_set_obj_by_num (const char *vargv) 
{
	unimplemented;
	// Asc_SolvSetObjByNum
}

rcp ascjson::Asc_SolvGetObjNumCmdHC (const char *vargv) 
{
	unimplemented;
	// Asc_SolvGetObjNumCmd
}
#if 1
rcp ascjson::slv_get_parms (const char *vargv) 
{
	unimplemented;
	// Asc_SolvGetSlvParms
}

rcp ascjson::set_slv_parms (const char *vargv) 
{
	unimplemented;
	// Asc_SolvSetSlvParms
}
#endif

rcp ascjson::set_slv_parmsnew (const char *vargv) 
{
	unimplemented;
	// Asc_SolvSetSlvParmsNew
}

rcp ascjson::slv_get_parmsnew (const char *vargv) 
{
	unimplemented;
	// Asc_SolvGetSlvParmsNew
}

rcp ascjson::slv_get_insttype (const char *vargv) 
{
	unimplemented;
	// Asc_SolvGetInstType
}

rcp ascjson::slv_get_cost_page (const char *vargv) 
{
	unimplemented;
	// Asc_SolvGetSlvCostPage
}

rcp ascjson::slv_get_stat_page (const char *vargv) 
{
	unimplemented;
	// Asc_SolvGetSlvStatPage
}

rcp ascjson::slv_get_objval (const char *vargv) 
{
	unimplemented;
	// Asc_SolvGetObjectiveVal
}

rcp ascjson::slv_get_instname (const char *vargv) 
{
	unimplemented;
	// Asc_SolvGetInstName
}

rcp ascjson::slv_get_pathname (const char *vargv) 
{
	unimplemented;
	// Asc_SolvGetPathName
}

rcp ascjson::slv_get_vr (const char *vargv) 
{
	unimplemented;
	// Asc_SolvGetVRCounts
}

rcp ascjson::slvdump (const char *vargv) 
{
	unimplemented;
	// Asc_SolvSlvDumpInt
}

rcp ascjson::slv_presolve (const char *vargv) 
{
	unimplemented;
	// Asc_SolvSlvPresolve
}

rcp ascjson::slv_reanalyze (const char *vargv) 
{
	unimplemented;
	// Asc_SolvReanalyze
}

rcp ascjson::slv_check_and_reanalyze (const char *vargv) 
{
	unimplemented;
	// Asc_SolvCheckAndReanalyze
}

rcp ascjson::slv_set_independent (const char *vargv) 
{
	unimplemented;
	// Asc_SolvMakeIndependent
}

rcp ascjson::slv_resolve (const char *vargv) 
{
	unimplemented;
	// Asc_SolvSlvResolve
}

rcp ascjson::slv_solve (const char *vargv) 
{
	unimplemented;
	// Asc_SolvSlvSolve
}

rcp ascjson::slv_iterate (const char *vargv) 
{
	unimplemented;
	// Asc_SolvSlvIterate
}

rcp ascjson::slv_available (const char *vargv) 
{
	unimplemented;
	// Asc_SolvAvailSolver
}

rcp ascjson::slv_number (const char *vargv) 
{
	unimplemented;
	// Asc_SolvSolverNum
}

rcp ascjson::slv_name (const char *vargv) 
{
	unimplemented;
	// Asc_SolvSolverName
}

rcp ascjson::slv_linsol_names (const char *vargv) 
{
	unimplemented;
	// Asc_SolvLinsolNames
}

rcp ascjson::slv_eligible_solver (const char *vargv) 
{
	unimplemented;
	// Asc_SolvEligSolver
}

rcp ascjson::slv_select_solver (const char *vargv) 
{
	unimplemented;
	// Asc_SolvSelectSolver
}

rcp ascjson::slv_get_solver (const char *vargv) 
{
	unimplemented;
	// Asc_SolvGetSelectedSolver
}

rcp ascjson::slv_flush_solver (const char *vargv) 
{
	unimplemented;
	// Asc_SolvFlushSolver
}

rcp ascjson::slv_import_qlfdid (const char *vargv) 
{
	unimplemented;
	// Asc_SolvImportQlfdid
}

rcp ascjson::slv_lnmget (const char *vargv) 
{
	unimplemented;
	// Asc_SolvGetLnmEpsilon
}

rcp ascjson::slv_lnmset (const char *vargv) 
{
	unimplemented;
	// Asc_SolvSetLnmEpsilon
}

rcp ascjson::slv_set_haltflag (const char *vargv) 
{
	unimplemented;
	// Asc_SolvSetCHaltFlag
}

rcp ascjson::slvhelp (const char *vargv) 
{
	unimplemented;
	// Asc_SolvHelpList
}

rcp ascjson::slv_near_bounds (const char *vargv) 
{
	unimplemented;
	// Asc_SolvNearBounds
}

rcp ascjson::slv_far_from_nominals (const char *vargv) 
{
	unimplemented;
	// Asc_SolvFarFromNominal
}

rcp ascjson::Asc_SolveMonitorCmdHC (const char *vargv) 
{
	unimplemented;
	// Asc_SolveMonitorCmd
}

rcp ascjson::dbg_get_blk_of_var (const char *vargv) 
{
	unimplemented;
	// Asc_DebuGetBlkOfVar
}

rcp ascjson::dbg_get_blk_of_eqn (const char *vargv) 
{
	unimplemented;
	// Asc_DebuGetBlkOfEqn
}

rcp ascjson::dbg_get_blk_coords (const char *vargv) 
{
	unimplemented;
	// Asc_DebuGetBlkCoords
}

rcp ascjson::dbg_get_eqn_of_var (const char *vargv) 
{
	unimplemented;
	// Asc_DebuGetEqnOfVar
}

rcp ascjson::dbg_get_varpartition (const char *vargv) 
{
	unimplemented;
	// Asc_DebuGetVarPartition
}

rcp ascjson::dbg_get_eqnpartition (const char *vargv) 
{
	unimplemented;
	// Asc_DebuGetEqnPartition
}

rcp ascjson::dbg_list_rels (const char *vargv) 
{
	unimplemented;
	// Asc_DebuListRels
}

rcp ascjson::dbg_list_vars (const char *vargv) 
{
	unimplemented;
	// Asc_DebuListVars
}

rcp ascjson::dbg_write_var (const char *vargv) 
{
	unimplemented;
	// Asc_DebuWriteVar
}

rcp ascjson::dbg_write_unattvar (const char *vargv) 
{
	unimplemented;
	// Asc_DebuWriteUnattachedVar
}

rcp ascjson::brow_write_var (const char *vargv) 
{
	unimplemented;
	// Asc_DebuWriteVar cd=1
}


rcp ascjson::dbg_write_rel (const char *vargv) 
{
	unimplemented;
	// Asc_DebuWriteRel cd=NULL
}

rcp ascjson::brow_write_rel (const char *vargv) 
{
	unimplemented;
	// Asc_DebuWriteRel cd=1
}

rcp ascjson::dbg_write_obj (const char *vargv) 
{
	unimplemented;
	// Asc_DebuWriteObj cd=NULL
}

rcp ascjson::brow_write_obj (const char *vargv) 
{
	unimplemented;
	// Asc_DebuWriteObj cd=1
}

rcp ascjson::dbg_write_varattr (const char *vargv) 
{
	unimplemented;
	// Asc_DebuWriteVarAttr cd=NULL
}

rcp ascjson::dbg_write_qlfattr (const char *vargv) 
{
	unimplemented;
	// Asc_DebuWriteVarAttr cd=1
}

rcp ascjson::dbg_rel_included (const char *vargv) 
{
	unimplemented;
	// Asc_DebuRelIncluded
}

rcp ascjson::dbg_var_fixed (const char *vargv) 
{
	unimplemented;
	// Asc_DebuVarFixed
}

rcp ascjson::dbg_get_incidence (const char *vargv) 
{
	unimplemented;
	// Asc_DebuGetIncidence
}

rcp ascjson::dbg_get_order (const char *vargv) 
{
	unimplemented;
	// Asc_DebuGetOrder
}

rcp ascjson::dbg_write_incidence (const char *vargv) 
{
	unimplemented;
	// Asc_DebuWriteIncidence
}

rcp ascjson::dbg_find_eligible (const char *vargv) 
{
	unimplemented;
	// Asc_DebuFindEligible
}

rcp ascjson::brow_find_eligible (const char *vargv) 
{
	unimplemented;
	// Asc_DebuInstEligible
}

rcp ascjson::dbg_consistency_analysis (const char *vargv) 
{
	unimplemented;
	// Asc_DebuConsistencyAnalysis
}

rcp ascjson::dbg_global_eligible (const char *vargv) 
{
	unimplemented;
	// Asc_DebuFindGlobalEligible
}

rcp ascjson::dbg_find_activerels (const char *vargv) 
{
	unimplemented;
	// Asc_DebuFindActive
}

rcp ascjson::brow_find_activerels (const char *vargv) 
{
	unimplemented;
	// Asc_DebuInstActive
}

rcp ascjson::dbg_struct_singular (const char *vargv) 
{
	unimplemented;
	// Asc_DebuStructSing
}

rcp ascjson::dbg_num_block_singular (const char *vargv) 
{
	unimplemented;
	// Asc_DebuNumBlockSing
}

rcp ascjson::var_free2nom (const char *vargv) 
{
	unimplemented;
	// Asc_DebuVarFree2Nom
}

rcp ascjson::var_nom2free (const char *vargv) 
{
	unimplemented;
	// Asc_DebuVarNom2Free
}

rcp ascjson::dbg_calc_relnoms (const char *vargv) 
{
	unimplemented;
	// Asc_DebuCalcRelNominals
}

rcp ascjson::dbg_check_rels (const char *vargv) 
{
	unimplemented;
	// Asc_DebuCheckRelFp
}

rcp ascjson::dbg_write_slv0_xsys (const char *vargv) 
{
	unimplemented;
	// Asc_DebuWriteSystem cd=NULL
}

rcp ascjson::dbg_write_slv0_sys (const char *vargv) 
{
	unimplemented;
	// Asc_DebuWriteSystem cd=1
}

rcp ascjson::dbg_mtxwriteplot (const char *vargv) 
{
	unimplemented;
	// Asc_DebuMtxWritePlotCmd cd=1
}

rcp ascjson::dbg_calc_jacobian (const char *vargv) 
{
	unimplemented;
	// Asc_DebuMtxCalcJacobianCmd cd=1
}

rcp ascjson::dbghelp (const char *vargv) 
{
	unimplemented;
	// Asc_DebuHelpList
}

rcp ascjson::integrate_setup (const char *vargv) 
{
	unimplemented;
	// Asc_IntegSetupCmd
}

rcp ascjson::integrate_cleanup (const char *vargv) 
{
	unimplemented;
	// Asc_IntegCleanupCmd
}

rcp ascjson::integrate_set_y_file (const char *vargv) 
{
	unimplemented;
	// Asc_IntegSetYFileCmd
}

rcp ascjson::integrate_set_obs_file (const char *vargv) 
{
	unimplemented;
	// Asc_IntegSetObsFileCmd
}

rcp ascjson::integrate_logunits (const char *vargv) 
{
	unimplemented;
	// Asc_IntegSetFileUnitsCmd
}

rcp ascjson::integrate_logformat (const char *vargv) 
{
	unimplemented;
	// Asc_IntegSetFileFormatCmd
}

rcp ascjson::integrate_get_samples (const char *vargv) 
{
	unimplemented;
	// Asc_IntegGetXSamplesCmd
}

rcp ascjson::integrate_set_samples (const char *vargv) 
{
	unimplemented;
	// Asc_IntegSetXSamplesCmd
}

rcp ascjson::integrate_able (const char *vargv) 
{
	unimplemented;
	// Asc_IntegInstIntegrableCmd
}

rcp ascjson::x__mtx_norms (const char *vargv) 
{
	unimplemented;
	// Asc_MtxNormsCmd
}

rcp ascjson::mtx_gui_plot_incidence (const char *vargv) 
{
	unimplemented;
	// Asc_MtxGUIPlotIncidence
}

rcp ascjson::mtxhelp (const char *vargv) 
{
	unimplemented;
	// Asc_MtxHelpList
}

rcp ascjson::u_destroy_units (const char *vargv) 
{
	unimplemented;
	// Asc_UnitDestroyDisplayList
}

rcp ascjson::u_setSIdef (const char *vargv) 
{
	unimplemented;
	// Asc_UnitDefaultBaseUnits
}

rcp ascjson::u_getbasedef (const char *vargv) 
{
	unimplemented;
	// Asc_UnitGetBaseUnits
}

rcp ascjson::u_dump (const char *vargv) 
{
	unimplemented;
	// Asc_UnitDump
}

rcp ascjson::u_dims (const char *vargv) 
{
	unimplemented;
	// Asc_DimenDump
}

rcp ascjson::u_dim_setverify (const char *vargv) 
{
	unimplemented;
	// Asc_DimenRelCheck
}

rcp ascjson::u_dim2num (const char *vargv) 
{
	unimplemented;
	// Asc_UnitBaseDimToNum
}

rcp ascjson::u_num2dim (const char *vargv) 
{
	unimplemented;
	// Asc_UnitNumToBaseDim
}

rcp ascjson::u_frombasedim (const char *vargv) 
{
	unimplemented;
	// Asc_UnitMatchBaseDim
}

rcp ascjson::u_fromatomdim (const char *vargv) 
{
	unimplemented;
	// Asc_UnitMatchAtomDim
}

rcp ascjson::u_getdimatoms (const char *vargv) 
{
	unimplemented;
	// Asc_UnitGetAtomList
}

rcp ascjson::u_change_baseunit (const char *vargv) 
{
	unimplemented;
	// Asc_UnitChangeBaseUnit
}

rcp ascjson::u_getprec (const char *vargv) 
{
	unimplemented;
	// Asc_UnitGetPrec
}

rcp ascjson::u_setprec (const char *vargv) 
{
	unimplemented;
	// Asc_UnitSetPrec
}

rcp ascjson::u_get_atoms (const char *vargv) 
{
	unimplemented;
	// Asc_UnitGetAtomsForUnit
}

rcp ascjson::u_get_units (const char *vargv) 
{
	unimplemented;
	// Asc_UnitGetUnits
}

rcp ascjson::u_set_user (const char *vargv) 
{
	unimplemented;
	// Asc_UnitSetUser
}

rcp ascjson::u_get_user (const char *vargv) 
{
	unimplemented;
	// Asc_UnitGetUser
}

rcp ascjson::u_get_list (const char *vargv) 
{
	unimplemented;
	// Asc_UnitGetList
}

rcp ascjson::u_clear_user (const char *vargv) 
{
	unimplemented;
	// Asc_UnitClearUser
}

rcp ascjson::u_getval (const char *vargv) 
{
	unimplemented;
	// Asc_UnitGetVal
}

rcp ascjson::u_browgetval (const char *vargv) 
{
	unimplemented;
	// Asc_UnitBrowGetVal
}

rcp ascjson::u_slvgetrelval (const char *vargv) 
{
	unimplemented;
	// Asc_UnitSlvGetRelVal
}

rcp ascjson::u_slvgetvarval (const char *vargv) 
{
	unimplemented;
	// Asc_UnitSlvGetVarVal
}

rcp ascjson::u_slvgetobjval (const char *vargv) 
{
	unimplemented;
	// Asc_UnitSlvGetObjVal
}

rcp ascjson::uhelp (const char *vargv) 
{
	unimplemented;
	// Asc_UnitHelpList
}

rcp ascjson::srefine (const char *vargv) 
{
	unimplemented;
	// Asc_ScriptRefineCmd
}

rcp ascjson::smerge (const char *vargv) 
{
	unimplemented;
	// Asc_ScriptMergeCmd
}

#if 0
rcp ascjson::ascloadwin (const char *vargv) 
{
	setc("", 0);
	return getc(__func__);
}
#endif

rcp ascjson::gnutext (const char * vargv ) 
{
	wrap_dstring(Asc_GNUTextCmdDS, SVcstr);
}

rcp ascjson::Asc_HelpCmdHC (const char *vargv)
{
	INIT_ARGV(vargv);
	ASCUSE;  /* see if first arg is -help */
	err = Asc_HelpCmdDS(&hds, argc, argv);
	setc(__func__, SVcstr, Asc_DStringValue(&hds), err);
	Asc_DStringFree(&hds);
	FREE_ARGV;
	return getc(__func__);
}

// -------------------------------------------------------------

#include "tcltk/json/HelpProcDS.ipp"
#include "tcltk/json/CommandsDS.ipp"
#include "tcltk/json/LibraryProc.ipp"
#include "tcltk/json/all_call.ipp"
