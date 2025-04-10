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

#define OLD(...)

/**
 * \brief singleton class managing ascend environment with a json api
 */
class ascjson {
	ascjson();

	int/*str*?*/ cmdName (body-proc);

/* this can go to JS  */
#if 0
  ASCADDCOM(interp,"stringcompact", Asc_StringCompact,
    "miscellaneous",
    "stringcompact <string>",
    "returns a string constructed from the input by removing extra whitespace"
    "\nThis function reduces all extra tabs, newlines and blanks to a space, \
     and ascplot depends on it heavily to insure sanity",
    NULL
    /* the stuff above should be replaced with header macros. */
  );
#endif //0

  int  Asc_LibrQueryTypeCmdHC ();
  OLD(, Asc_LibrQueryTypeCmd,
    "library", Asc_LibrQueryTypeCmdHU, Asc_LibrQueryTypeCmdHS,
    Asc_LibrQueryTypeCmdHLF);

  int  Asc_LibrOptionsCmdHC ()
	  OLD(, Asc_LibrOptionsCmd,
    "library", Asc_LibrOptionsCmdHU, Asc_LibrOptionsCmdHS,
    Asc_LibrOptionsCmdHLF);

  int  Asc_LibrParseCmdHC ()
	  OLD(, Asc_LibrParseCmd,
    "library", Asc_LibrParseCmdHU, Asc_LibrParseCmdHS,
    Asc_LibrParseCmdHLF);

  int  Asc_LibrReadCmdHC ()
	  OLD(, Asc_LibrReadCmd,
    "library", Asc_LibrReadCmdHU, Asc_LibrReadCmdHS,
    Asc_LibrReadCmdHLF);

  int  Asc_LibrModuleInfoCmdHC ()
	  OLD(, Asc_LibrModuleInfoCmd,
    "library", Asc_LibrModuleInfoCmdHU, Asc_LibrModuleInfoCmdHS,
    Asc_LibrModuleInfoCmdHLF);

  int  Asc_LibrDestroyTypesCmdHC ()
	  OLD(, Asc_LibrDestroyTypesCmd,
    "library",  Asc_LibrDestroyTypesCmdHU,  Asc_LibrDestroyTypesCmdHS,
     Asc_LibrDestroyTypesCmdHLF);

  int  Asc_LibrHideTypeCmdHC ()
	  OLD(, Asc_LibrHideTypeCmd,
    "library", Asc_LibrHideTypeCmdHU, Asc_LibrHideTypeCmdHS,
    Asc_LibrHideTypeCmdHLF);

  int  Asc_LibrUnHideTypeCmdHC ()
	  OLD(, Asc_LibrUnHideTypeCmd,
    "library", Asc_LibrUnHideTypeCmdHU, Asc_LibrUnHideTypeCmdHS,
    Asc_LibrUnHideTypeCmdHLF);

  int  Asc_LibrTypeIsShownCmdHC ()
	  OLD(, Asc_LibrTypeIsShownCmd,
    "library", Asc_LibrTypeIsShownCmdHU, Asc_LibrTypeIsShownCmdHS,
    Asc_LibrTypeIsShownCmdHLF);

  int  Asc_LibrTypeListCmdHC ()
	  OLD(, Asc_LibrTypeListCmd,
    "library", Asc_LibrTypeListCmdHU, Asc_LibrTypeListCmdHS,
    Asc_LibrTypeListCmdHLF);

  int  Asc_ExtractTypeHC ()
	  OLD(, Asc_ExtractType,
    "library", Asc_ExtractTypeHU,  Asc_ExtractTypeHS,  Asc_ExtractTypeHLF);


  /* Browser Routines */
  int qlfdid ()
	  OLD(, ()
	  OLD(Asc_BrowQlfdidSearchCmd,
    "miscellaneous",
    NULL,
    "qlfdid     -- searches for the inst of a qualified name",
    NULL
    /* the stuff above should be replaced with header macros. */
  );

#if 0
  Asc_HelpDefineGroup("fileio",
    "Explanation: functions that read/write files");
  int "bwritevalues", Asc_BrowWriteValues,
    "fileio",
    NULL,
    NULL,
    NULL
    /* the stuff above should be replaced with header macros. */
  );
#endif

  int __brow_find_type ()
	  OLD(, Asc_BrowFindTypeCmd,
    "browser",
    NULL,
    NULL,
    NULL
    /* the stuff above should be replaced with header macros. */
  );

  int __brow_reln_relop ()
	  OLD(, Asc_BrowRelationRelopCmd,
    "browser",
    NULL,
    NULL,
    NULL
    /* the stuff above should be replaced with header macros. */
  );

  /*   Future work
  int __brow_lrel_relop ()OLD(, BrowLogRelRelopCmd,
                    (ClientData) NULL,(Tcl_CmdDeleteProc *) NULL);  */

  int btransfer ()
	  OLD(, Asc_BrowTransferCmd,
    "browser",
    NULL,
    NULL,
    NULL
    /* the stuff above should be replaced with header macros. */
  );

  int rootinit ()
	  OLD(, Asc_BrowRootInitCmd,
    "browser",
    NULL,
    NULL,
    NULL
    /* the stuff above should be replaced with header macros. */
  );

  int root ()
	  OLD(, Asc_BrowRootCmd,
    "browser",
    NULL,
    "root       -- sets the root instance for queries",
    NULL
    /* the stuff above should be replaced with header macros. */
  );

  int oldinst ()
	  OLD(, Asc_BrowRootBackupCmd,
    "browser",
    NULL,
    "oldinst    -- backup to parent instance",
    NULL
    /* the stuff above should be replaced with header macros. */
  );

  int rootn ()
	  OLD(, Asc_BrowRootNCmd,
    "browser",
    NULL,
    "rootn      -- backup to ancestor instance",
    NULL
    /* the stuff above should be replaced with header macros. */
  );

  int slist ()
	  OLD(, Asc_BrowSimListCmd,
    "simulations",
    NULL,
    "slist      -- prints the list of all simulations",
    NULL
    /* the stuff above should be replaced with header macros. */
  );

  int simtype ()
	  OLD(, Asc_BrowSimTypeCmd,
    "simulations",
    NULL,
    NULL,
    NULL
    /* the stuff above should be replaced with header macros. */
  );

  int bstatistics ()
	  OLD(,Asc_BrowInstStatCmd,
    "browser",
    NULL,
    "bstatistics - inst tree stats",
    NULL
    /* the stuff above should be replaced with header macros. */
  );

  int ilist ()
	  OLD(,Asc_BrowInstListCmd,
    "miscellaneous",
    NULL,
    "ilist      -- prints the list of all working instances",
    NULL
    /* the stuff above should be replaced with header macros. */
  );

  int is_type_refined ()
	  OLD(,Asc_BrowRefinesMeCmd,
    "miscellaneous",
    NULL,
    NULL,
    NULL
    /* the stuff above should be replaced with header macros. */
  );


  int bprint ()
	  OLD(, Asc_BrowPrintCmd,
    "miscellaneous",
    NULL,
    "bprint     -- prints the contents of an instance",
    NULL
    /* the stuff above should be replaced with header macros. */
  );

  int inst, Asc_BrowInstQueryCmd,
    "browser",
    NULL,
    "inst       -- general inst query routines",
    NULL
    /* the stuff above should be replaced with header macros. */
  );

  int bgetproc ()
	  OLD(, Asc_BrowWriteProcedure,
    "browser",
    NULL,
    NULL,
    NULL
    /* the stuff above should be replaced with header macros. */
  );

  int  Asc_BrowInitializeCmdHC ()
	  OLD(, Asc_BrowInitializeCmd,
    "browser", Asc_BrowInitializeCmdHU, Asc_BrowInitializeCmdHS,
    Asc_BrowInitializeCmdHLF
  );

  int brow_assign ()
	  OLD(, Asc_BrowRunAssignmentCmd,
    "browser",
    NULL,
    "assigns a value to the current or search atomic instance",
    NULL
    /* the stuff above should be replaced with header macros. */
  );

#if 0
  ASCADDCOM(interp,"qassgn2", Asc_BrowRunAssignQlfdidCmd2,
    "miscellaneous",
    NULL,
    "qassgn     -- assigns a value to a qlfdid atomic instance",
    NULL
    /* the stuff above should be replaced with header macros. */
  );
#endif

  int qassgn3 ()
	  OLD(, Asc_BrowRunAssignQlfdidCmd3,
    "miscellaneous",
    NULL,
    "qassgn3    -- assigns a value to a qlfdid atomic instance",
    NULL
    /* the stuff above should be replaced with header macros. */
  );


  int __brow_iname ()
	  OLD(, Asc_BrowWriteInstanceNameCmd,
    "browser",
    NULL,
    "iname      -- returns the name of the working instance",
    NULL
    /* the stuff above should be replaced with header macros. */
  );

  int __brow_isrelation ()
	  OLD(, Asc_BrowIsRelationCmd,
    "browser",
    NULL,
    NULL,
    NULL
    /* the stuff above should be replaced with header macros. */
  );

  int __brow_ismodel ()
	  OLD(, Asc_BrowIsModelCmd,
    "browser",
    NULL,
    NULL,
    NULL
    /* the stuff above should be replaced with header macros. */
  );

  int bgetrels ()
	  OLD(, Asc_BrowWriteRelListCmd,
    "browser",
    NULL,
    NULL,
    NULL
    /* the stuff above should be replaced with header macros. */
  );

  int bgetrelspf ()
	  OLD(, Asc_BrowWriteRelListPostfixCmd,
    "browser",
    NULL,
    NULL,
    NULL
    /* the stuff above should be replaced with header macros. */
  );

  int __brow_relsforatom ()
	  OLD(, Asc_BrowWriteRelsForAtomCmd,
    "browser",
    NULL,
    NULL,
    NULL
    /* the stuff above should be replaced with header macros. */
  );


  int__brow_islogrel ()
	  OLD(, Asc_BrowIsLogRelCmd,
    "browser",
    NULL,
    NULL,
    NULL
    /* the stuff above should be replaced with header macros. */
  );

  int bgetlogrels ()
	  OLD(, Asc_BrowWriteLogRelListCmd,
    "browser",
    NULL,
    NULL,
    NULL
    /* the stuff above should be replaced with header macros. */
  );

  int bgetlogrelspf ()
	  OLD(, Asc_BrowWriteLogRelListPostfixCmd,
    "browser",
    NULL,
    NULL,
    NULL
    /* the stuff above should be replaced with header macros. */
  );

  int __brow_logrelsforatom ()
	  OLD(, Asc_BrowWriteLogRelsForAtomCmd,
    "browser",
    NULL,
    NULL,
    NULL
    /* the stuff above should be replaced with header macros. */
  );


  int bgetcondrels ()
	  OLD(, Asc_BrowWriteCondRelListCmd,
    "browser",
    NULL,
    NULL,
    NULL
    /* the stuff above should be replaced with header macros. */
  );

  int bgetcondlogrels ()
	  OLD(, Asc_BrowWriteCondLogRelListCmd,
    "browser",
    NULL,
    NULL,
    NULL
    /* the stuff above should be replaced with header macros. */
  );


  int __brow_iswhen ()
	  OLD(, Asc_BrowIsWhenCmd,
    "browser",
    NULL,
    NULL,
    NULL
    /* the stuff above should be replaced with header macros. */
  );

  int __brow_isinstanceinwhen ()
	  OLD(,
                    Asc_BrowIsInstanceInWhenCmd,
    "browser",
    NULL,
    NULL,
    NULL
    /* the stuff above should be replaced with header macros. */
  );

  int bgetwhens ()
	  OLD(, Asc_BrowWriteWhenListCmd,
    "browser",
    NULL,
    NULL,
    NULL
    /* the stuff above should be replaced with header macros. */
  );

  int __brow_whensforinstance ()
	  OLD(,
                    Asc_BrowWriteWhensForInstanceCmd,
    "browser",
    NULL,
    NULL,
    NULL
    /* the stuff above should be replaced with header macros. */
  );

  int bwritependings ()
	  OLD(, Asc_BrowWritePendingsSTDOUT,
    "browser",
    NULL,
    NULL,
    NULL
    /* the stuff above should be replaced with header macros. */
  );

  int bnumpendings ()
	  OLD(, Asc_BrowShowPendings,
    "browser",
    NULL,
    NULL,
    NULL
    /* the stuff above should be replaced with header macros. */
  );

  int count_names ()
	  OLD(, Asc_BrowCountNamesCmd,
    "browser",
    NULL,
    "count_names-- counts things slowly. see output for details",
    NULL
    /* the stuff above should be replaced with header macros. */
  );

  int aliases ()
	  OLD(, Asc_BrowWriteAliasesCmd,
    "browser",
    NULL,
   "aliases    -- returns the aliases of an instance",
    NULL
    /* the stuff above should be replaced with header macros. */
  );

  int isas ()
	  OLD(, Asc_BrowWriteISAsCmd,
    "browser",
    NULL,
   "returns the constructions of an instance",
    NULL
    /* the stuff above should be replaced with header macros. */
  );

  int cliques ()
	  OLD(, Asc_BrowWriteCliqueCmd,
    "browser",
    NULL,
    "returns all the members in the same ARE_ALIKE clique",
    NULL
    /* the stuff above should be replaced with header macros. */
  );

  int Asc_BrowWriteInstanceCmdHC ()
	  OLD(, Asc_BrowWriteInstanceCmd,
    "browser",
    Asc_BrowWriteInstanceCmdHU,
    "returns formatted info for the Browser",
    NULL
    /* the stuff above should be replaced with header macros. */
  );

  int bmerge ()
	  OLD(, Asc_BrowInstanceMergeCmd,
    "browser",
    NULL,
    "merge current and search instances",
    NULL
    /* the stuff above should be replaced with header macros. */
  );

  int brefine ()
	  OLD(,Asc_BrowInstanceRefineCmd,
    "browser",
    NULL,
    "refine current or search inst to type given",
    NULL
    /* the stuff above should be replaced with header macros. */
  );

  int bmakealike ()
	  OLD(,Asc_BrowMakeAlikeCmd,
    "browser",
    NULL,
    "ARE_ALIKE current and search instances",
    NULL
    /* the stuff above should be replaced with header macros. */
  );

  int b_isplottable ()
	  OLD(, Asc_BrowIsPlotAllowedCmd,
    "browser",
    NULL,
    "return boolean value TRUE if instance is a plot",
    NULL
    /* the stuff above should be replaced with header macros. */
  );

  int b_prepplotfile ()
	  OLD(, Asc_BrowPreparePlotFileCmd,
    "browser",
    NULL,
    NULL,
    NULL
    /* the stuff above should be replaced with header macros. */
  );

  int free_all_vars ()
	  OLD(,Asc_BrowClearVarsCmd,
    "browser",
    NULL,
    NULL,
    NULL
    /* the stuff above should be replaced with header macros. */
  );


  /* Simulation Routines */
  int  Asc_SimBinTokenSetOptionsHC ()
	  OLD(, Asc_SimBinTokenSetOptions,
    "library", Asc_SimBinTokenSetOptionsHU, Asc_SimBinTokenSetOptionsHS,
    Asc_SimBinTokenSetOptionsHLF);

  int sims ()
	  OLD(, Asc_SimsQueryCmd,
    "simulations",
    NULL,
    "return list of simulation names in the instance universe",
    NULL
    /* the stuff above should be replaced with header macros. */
  );

  int sim_instantiate ()
	  OLD(,Asc_SimsCreateInstanceCmd,
    "simulations",
    NULL,
    "create a simulation in the instance universe",
    NULL
    /* the stuff above should be replaced with header macros. */
  );

  int sim_unique ()
	  OLD(,Asc_SimsUniqueNameCmd,
    "simulations",
    NULL,
    "boolean check for potential simulation name not currently in use",
    NULL
    /* the stuff above should be replaced with header macros. */
  );

  int sim_reinstantiate ()
	  OLD(,Asc_SimsResumeInstantiateCmd,
    "simulations",
    NULL,
    "sim_reinstantiate - resume compiling a simulation specified",
    NULL
    /* the stuff above should be replaced with header macros. */
  );

  int __sims_copy ()
	  OLD(,Asc_SimsCopyInstanceCmd,
    "broken",
    NULL,
    NULL,
    NULL
    /* the stuff above should be replaced with header macros. */
  );

  int __sims_proto ()
	  OLD(,Asc_SimsProtoTypeInstanceCmd,
    "broken",
    NULL,
    NULL,
    NULL
    /* the stuff above should be replaced with header macros. */
  );

  int __sims_saveinst ()
	  OLD(,Asc_SimsSaveInstanceCmd,
    "broken",
    NULL,
    NULL,
    NULL
    /* the stuff above should be replaced with header macros. */
  );

#if 0
   ASCADDCOM(interp,"__sims_update",Asc_SimsUpdateInstanceCmd,
    "broken",
    NULL,
    NULL,
    NULL
    /* the stuff above should be replaced with header macros. */
  );
#endif

  int sim_destroy ()
	  OLD(,Asc_SimsDestroySimulationCmd,
    "simulations",
    NULL,
    "sim_destroy   -- destroy a simulation",
    NULL
    /* the stuff above should be replaced with header macros. */
  );

  int simlistpending ()
	  OLD(,Asc_SimListPending,
    "simulations",
    NULL,
    NULL,
    NULL
    /* the stuff above should be replaced with header macros. */
  );


  /* Display and Type query */
  int ddefine ()
	  OLD(, Asc_DispDefineCmd,
    "library",
    NULL,
    "ddefine    -- list all the types, or details of a type",
    NULL
    /* the stuff above should be replaced with header macros. */
  );

  int ddiffdefine ()
	  OLD(, Asc_DispDiffDefineCmd,
    "library",
    NULL,
    "ddiffdefine -- list the details of a type differing from parent",
    NULL
    /* the stuff above should be replaced with header macros. */
  );

  int disp ()
	  OLD(, Asc_DispQueryCmd,
    "miscellaneous",
    NULL,
    NULL,
    NULL
    /* the stuff above should be replaced with header macros. */
  );

  int hier ()
	  OLD(, Asc_DispHierarchyCmd,
    "library",
    NULL,
    "hier       -- list the hierarchy of a given type",
    NULL
    /* the stuff above should be replaced with header macros. */
  );

  int file_by_type ()
	  OLD(, Asc_DispFileByTypeCmd,
    "fileio",
    NULL,
    NULL,
    NULL
    /* the stuff above should be replaced with header macros. */
  );

  int dchild ()
	  OLD(, Asc_DispChildOneCmd,
    "miscellaneous",
    NULL,
    NULL,
    NULL
    /* the stuff above should be replaced with header macros. */
  );

  int drefines_me ()
	  OLD(, Asc_DispRefinesMeCmd,
    "library",
    NULL,
    "drefines_me - list the immediate refinements of a given type",
    NULL
    /* the stuff above should be replaced with header macros. */
  );


  int drefines_meall ()
		  OLD(, Asc_DispRefinesMeCmd,
    (ClientData) 1, (Tcl_CmdDeleteProc *)NULL,
    "library",
    NULL,
    "drefines_meall - list all refinements of a given type",
    NULL
    /* the stuff above should be replaced with header macros. */
  );

  int drefinement_tree ()
	  OLD(, Asc_DispRefinesMeTreeCmd,
    "library",
    NULL,
    "drefinement_tree - list the refinement hierarchy based on type",
    NULL
    /* the stuff above should be replaced with header macros. */
  );

  int dgetparts ()
	  OLD(, Asc_DispTypePartsCmd,
    "library",
    NULL,
    "dgetparts  -- list the types of parts IN a type.",
    NULL
    /* the stuff above should be replaced with header macros. */
  );

  int disroot_type ()
	  OLD(, Asc_DispIsRootTypeCmd,
    "library",
    NULL,
    NULL,
    NULL
    /* the stuff above should be replaced with header macros. */
  );

  /* Probe Routine */
  int Asc_ProbeCmdHC ()
	  OLD(, Asc_ProbeCmd,
    "probe-list",Asc_ProbeCmdHU,Asc_ProbeCmdHS,Asc_ProbeCmdHLF);

  /* slv query routines */
  int __var_analyze ()
	  OLD(, Asc_VarAnalyzeCmd,
    "miscellaneous",
    NULL,
    NULL,
    NULL
    /* the stuff above should be replaced with header macros. */
  );

  int __rel_analyze ()
	  OLD(, Asc_RelAnalyzeCmd,
    "miscellaneous",
    NULL,
    NULL,
    NULL
    /* the stuff above should be replaced with header macros. */
  );

  /* UserData Routines */
  int __userdata_init ()
	  OLD(, Asc_UserDataInitializeCmd,
    "user-list",
    NULL,
    NULL,
    NULL
    /* the stuff above should be replaced with header macros. */
  );

  int __userdata_create ()
	  OLD(, Asc_UserDataCreateCmd,
    "user-list",
    NULL,
    NULL,
    NULL
    /* the stuff above should be replaced with header macros. */
  );

  int __userdata_destroy ()
	  OLD(, Asc_UserDataDestroyCmd,
    "user-list",
    NULL,
    NULL,
    NULL
    /* the stuff above should be replaced with header macros. */
  );

  int __userdata_save ()
	  OLD(, Asc_UserDataSaveValuesCmd,
    "user-list",
    NULL,
    NULL,
    NULL
    /* the stuff above should be replaced with header macros. */
  );

  int __userdata_restore ()
	  OLD(, Asc_UserDataRestoreValuesCmd,
    "user-list",
    NULL,
    NULL,
    NULL
    /* the stuff above should be replaced with header macros. */
  );

  int __userdata_query ()
	  OLD(, Asc_UserDataQueryCmd,
    "user-list",
    NULL,
    NULL,
    NULL
    /* the stuff above should be replaced with header macros. */
  );

  int __userdata_print ()
	  OLD(, Asc_UserDataPrintLibrary,
    "user-list",
    NULL,
    NULL,
    NULL
    /* the stuff above should be replaced with header macros. */
  );

  /* Solver Routines */
    /* Solve_* are routines calling the commandline solver */
    /* Solv_[SOLVERNAME_]<cfunction> are commands for the windowed solvers */

#if DELETEME
  ASCADDCOM(interp,"bexp_s" ()
		  OLD(,Asc_Brow2Solve,
    "miscellaneous",
    NULL,
    "bexp_s     -- export browser instance to solver context",
    NULL
    /* the stuff above should be replaced with header macros. */
  );
#endif /* DELETEME */

  int get_model_children ()
  OLD(,Asc_SolvGetModKids,
    "solver",
    NULL,
    NULL,
    NULL
    /* the stuff above should be replaced with header macros. */
  );

#if DELETEME
  int slv_import_sim () OLD(,Asc_Sims2Solve,
    "solver",
    NULL,
    NULL,
    NULL
    /* the stuff above should be replaced with header macros. */
  );

  int ssolve () OLD(,Asc_SolvSimInst,
    "solver",
    NULL,
    "ssolve     -- fire up commandline solver <n> on <name>. DON'T.",
    NULL
    /* the stuff above should be replaced with header macros. */
  );
#endif /* DELETEME */

#if 0
  ASCADDCOM(interp,"solve",Asc_SolvCurInst,
    "solver",
    NULL,
    "solve      -- fire up commandline solver <n>. on g_curinst. DON'T.",
    NULL
    /* the stuff above should be replaced with header macros. */
  );
#endif

#ifdef ASC_SIGNAL_TRAPS
  int slv_trapfp ()
	  OLD(,Asc_SolvTrapFP,
    "solver",
    NULL,
    NULL,
    NULL
    /* the stuff above should be replaced with header macros. */
  );

  int slv_untrapfp ()
	  OLD(,Asc_SolvUnTrapFP,
    "solver",
    NULL,
    NULL,
    NULL
    /* the stuff above should be replaced with header macros. */
  );

  int slv_trapint ()
	  OLD(,Asc_SolvTrapINT,
    "solver",
    NULL,
    NULL,
    NULL
    /* the stuff above should be replaced with header macros. */
  );

  int slv_untrapint ()
	  OLD(,Asc_SolvUnTrapINT,
    "solver",
    NULL,
    NULL,
    NULL
    /* the stuff above should be replaced with header macros. */
  );
#endif

  int slv_checksim ()
	  OLD(,Asc_SolvIncompleteSim,
    "solver",
    NULL,
    NULL,
    NULL
    /* the stuff above should be replaced with header macros. */
  );

  int slv_checksys ()
	  OLD(,Asc_SolvCheckSys,
    "solver",
    NULL,
    NULL,
    NULL
    /* the stuff above should be replaced with header macros. */
  );

  int slv_get_obj_list ()
	  OLD(,Asc_SolvGetObjList,
    "solver",
    NULL,
    NULL,
    NULL
    /* the stuff above should be replaced with header macros. */
  );

  int slv_set_obj_by_num ()
	  OLD(,Asc_SolvSetObjByNum,
    "solver",
    NULL,
    NULL,
    NULL
    /* the stuff above should be replaced with header macros. */
  );

  int Asc_SolvGetObjNumCmdHC ()
	  OLD(,Asc_SolvGetObjNumCmd,
    "solver",Asc_SolvGetObjNumCmdHU,Asc_SolvGetObjNumCmdHS,
    Asc_SolvGetObjNumCmdHLF);

  int slv_get_parms ()
	  OLD(,Asc_SolvGetSlvParms,
    "solver",
    NULL,
    NULL,
    NULL
    /* the stuff above should be replaced with header macros. */
  );

  int set_slv_parms ()
	  OLD(,Asc_SolvSetSlvParms,
    "solver",
    NULL,
    NULL,
    NULL
    /* the stuff above should be replaced with header macros. */
  );

  int set_slv_parmsnew ()
	  OLD(,Asc_SolvSetSlvParmsNew,
    "solver",
    NULL,
    NULL,
    NULL
    /* the stuff above should be replaced with header macros. */
  );

  int slv_get_parmsnew ()
	  OLD(,Asc_SolvGetSlvParmsNew,
    "solver",
    NULL,
    NULL,
    NULL
    /* the stuff above should be replaced with header macros. */
  );

/* what's up with this? */
/*  int slv_get_bool_parms ()
 *  OLD(Asc_SolvGetSlvBoolParms,
 *                   (ClientData) NULL, (Tcl_CmdDeleteProc *) NULL);
 *  int slv_get_real_parms ()
 *  OLD(Asc_SolvGetSlvRealParms,
 *                   (ClientData) NULL, (Tcl_CmdDeleteProc *) NULL);
 */

  int slv_get_insttype ()
	  OLD(,Asc_SolvGetInstType,
    "solver",
    NULL,
    NULL,
    NULL
    /* the stuff above should be replaced with header macros. */
  );

  int slv_get_cost_page ()
	  OLD(,Asc_SolvGetSlvCostPage,
    "solver",
    NULL,
    NULL,
    NULL
    /* the stuff above should be replaced with header macros. */
  );

  int slv_get_stat_page ()
	  OLD(,Asc_SolvGetSlvStatPage,
    "solver",
    NULL,
    NULL,
    NULL
    /* the stuff above should be replaced with header macros. */
  );

  int slv_get_objval ()
	  OLD(,Asc_SolvGetObjectiveVal,
    "solver",
    NULL,
    NULL,
    NULL
    /* the stuff above should be replaced with header macros. */
  );

  int slv_get_instname ()
	  OLD(,Asc_SolvGetInstName,
    "solver",
    NULL,
    NULL,
    NULL
    /* the stuff above should be replaced with header macros. */
  );

  int slv_get_pathname ()
	  OLD(,Asc_SolvGetPathName,
    "solver",
    NULL,
    NULL,
    NULL
    /* the stuff above should be replaced with header macros. */
  );

  int slv_get_vr ()
	  OLD(,Asc_SolvGetVRCounts,
    "solver",
    NULL,
    NULL,
    NULL
    /* the stuff above should be replaced with header macros. */
  );

  int slvdump ()
	  OLD(,Asc_SolvSlvDumpInt,
    "solver",
    NULL,
    NULL,
    NULL
    /* the stuff above should be replaced with header macros. */
  );

  int slv_presolve ()
	  OLD(,Asc_SolvSlvPresolve,
    "solver",
    NULL,
    NULL,
    NULL
    /* the stuff above should be replaced with header macros. */
  );

  int slv_reanalyze ()
	  OLD(,Asc_SolvReanalyze,
    "solver",
    NULL,
    NULL,
    NULL
    /* the stuff above should be replaced with header macros. */
  );

  int slv_check_and_reanalyze ()
	  OLD(,Asc_SolvCheckAndReanalyze,
    "solver",
    NULL,
    NULL,
    NULL
    /* the stuff above should be replaced with header macros. */
  );

  int slv_set_independent ()
	  OLD(,Asc_SolvMakeIndependent,
    "solver",
    NULL,
    NULL,
    NULL
    /* the stuff above should be replaced with header macros. */
  );

  int slv_resolve ()
	  OLD(,Asc_SolvSlvResolve,
    "solver",
    NULL,
    NULL,
    NULL
    /* the stuff above should be replaced with header macros. */
  );

  int slv_solve ()
	  OLD(,Asc_SolvSlvSolve,
    "solver",
    NULL,
    NULL,
    NULL
    /* the stuff above should be replaced with header macros. */
  );

  int slv_iterate ()
	  OLD(,Asc_SolvSlvIterate,
    "solver",
    NULL,
    NULL,
    NULL
    /* the stuff above should be replaced with header macros. */
  );

  int slv_available ()
	  OLD(,Asc_SolvAvailSolver,
    "solver",
    NULL,
    NULL,
    NULL
    /* the stuff above should be replaced with header macros. */
  );

  int slv_number ()
	  OLD(,Asc_SolvSolverNum,
    "solver",
    NULL,
    NULL,
    NULL
    /* the stuff above should be replaced with header macros. */
  );

  int slv_name ()
	  OLD(,Asc_SolvSolverName,
    "solver",
    NULL,
    NULL,
    NULL
    /* the stuff above should be replaced with header macros. */
  );

  int slv_linsol_names ()
	  OLD(,Asc_SolvLinsolNames,
    "solver",
    NULL,
    NULL,
    NULL
    /* the stuff above should be replaced with header macros. */
  );

  int slv_eligible_solver ()
	  OLD(,Asc_SolvEligSolver,
    "solver",
    NULL,
    NULL,
    NULL
    /* the stuff above should be replaced with header macros. */
  );

  int slv_select_solver ()
	  OLD(,Asc_SolvSelectSolver,
    "solver",
    NULL,
    NULL,
    NULL
    /* the stuff above should be replaced with header macros. */
  );

  int slv_get_solver ()
	  OLD(,Asc_SolvGetSelectedSolver,
    "solver",
    NULL,
    NULL,
    NULL
    /* the stuff above should be replaced with header macros. */
  );

  int slv_flush_solver ()
	  OLD(,Asc_SolvFlushSolver,
    "solver",
    NULL,
    NULL,
    NULL
    /* the stuff above should be replaced with header macros. */
  );

  int slv_import_qlfdid ()
	  OLD(,Asc_SolvImportQlfdid,
    "solver",
    NULL,
    NULL,
    NULL
    /* the stuff above should be replaced with header macros. */
  );

  int slv_lnmget ()
	  OLD(,Asc_SolvGetLnmEpsilon,
    "solver",
    NULL,
    NULL,
    NULL
    /* the stuff above should be replaced with header macros. */
  );

  int slv_lnmset ()
	  OLD(,Asc_SolvSetLnmEpsilon,
    "solver",
    NULL,
    NULL,
    NULL
    /* the stuff above should be replaced with header macros. */
  );

  int slv_set_haltflag ()
	  OLD(,Asc_SolvSetCHaltFlag,
    "solver",
    NULL,
    NULL,
    NULL
    /* the stuff above should be replaced with header macros. */
  );

  int slvhelp ()
	  OLD(,Asc_SolvHelpList,
    "solver",
    NULL,
    "slvhelp    -- give tcl/[s]hort/[l]ong solver primitive list",
    NULL
    /* the stuff above should be replaced with header macros. */
  );

  int slv_near_bounds ()
	  OLD(,Asc_SolvNearBounds,
    "solver",
    NULL,
    NULL,
    NULL
    /* the stuff above should be replaced with header macros. */
  );

  int slv_far_from_nominals ()
	  OLD(,Asc_SolvFarFromNominal,
    "solver",
    NULL,
    NULL,
    NULL
    /* the stuff above should be replaced with header macros. */
  );

  int Asc_SolveMonitorCmdHC ()
	  OLD(,Asc_SolveMonitorCmd,"solver",
    Asc_SolveMonitorCmdHU, Asc_SolveMonitorCmdHS, Asc_SolveMonitorCmdHLF);




  /* solver Debugger commands */
  int dbg_get_blk_of_var ()
	  OLD(,Asc_DebuGetBlkOfVar,
    "debugger",
    NULL,
    NULL,
    NULL
    /* the stuff above should be replaced with header macros. */
  );

  int dbg_get_blk_of_eqn ()
	  OLD(,Asc_DebuGetBlkOfEqn,
    "debugger",
    NULL,
    NULL,
    NULL
    /* the stuff above should be replaced with header macros. */
  );

  int dbg_get_blk_coords ()
	  OLD(,Asc_DebuGetBlkCoords,
    "debugger",
    NULL,
    NULL,
    NULL
    /* the stuff above should be replaced with header macros. */
  );

  int dbg_get_eqn_of_var ()
	  OLD(,Asc_DebuGetEqnOfVar,
    "debugger",
    NULL,
    NULL,
    NULL
    /* the stuff above should be replaced with header macros. */
  );

  int dbg_get_varpartition ()
	  OLD(,Asc_DebuGetVarPartition,
    "debugger",
    NULL,
    NULL,
    NULL
    /* the stuff above should be replaced with header macros. */
  );

  int dbg_get_eqnpartition ()
	  OLD(,Asc_DebuGetEqnPartition,
    "debugger",
    NULL,
    NULL,
    NULL
    /* the stuff above should be replaced with header macros. */
  );

  int dbg_list_rels ()
	  OLD(,Asc_DebuListRels,
    "debugger",
    NULL,
    NULL,
    NULL
    /* the stuff above should be replaced with header macros. */
  );

  int dbg_list_vars ()
	  OLD(,Asc_DebuListVars,
    "debugger",
    NULL,
    NULL,
    NULL
    /* the stuff above should be replaced with header macros. */
  );

  int dbg_write_var ()
	  OLD(,Asc_DebuWriteVar,
    "debugger",
    NULL,
    NULL,
    NULL
    /* the stuff above should be replaced with header macros. */
  );

  int dbg_write_unattvar ()
	  OLD(,Asc_DebuWriteUnattachedVar,
    "debugger",
    NULL,
    NULL,
    NULL
    /* the stuff above should be replaced with header macros. */
  );

  int brow_write_var ()
		  OLD(,Asc_DebuWriteVar,
                 (ClientData) 1, (Tcl_CmdDeleteProc *) NULL,
    "miscellaneous",
    NULL,
    NULL,
    NULL
    /* the stuff above should be replaced with header macros. */
  );


  int dbg_write_rel ()
	  OLD(,Asc_DebuWriteRel,
    "debugger",
    NULL,
    NULL,
    NULL
    /* the stuff above should be replaced with header macros. */
  );

  /* do we need this function? */
  int brow_write_rel ()
		  OLD(,Asc_DebuWriteRel,
                 (ClientData) 1, (Tcl_CmdDeleteProc *) NULL,
    "miscellaneous",
    NULL,
    NULL,
    NULL
    /* the stuff above should be replaced with header macros. */
  );

  int dbg_write_obj ()
	  OLD(,Asc_DebuWriteObj,
    "debugger",
    NULL,
    NULL,
    NULL
    /* the stuff above should be replaced with header macros. */
  );

  /* do we need this function? */
  int brow_write_obj ()
		  OLD(,Asc_DebuWriteObj,
                 (ClientData) 1, (Tcl_CmdDeleteProc *) NULL,
    "miscellaneous",
    NULL,
    NULL,
    NULL
    /* the stuff above should be replaced with header macros. */
  );

  int dbg_write_varattr ()
  OLD(,Asc_DebuWriteVarAttr,
    "debugger",
    NULL,
    NULL,
    NULL
    /* the stuff above should be replaced with header macros. */
  );

  int dbg_write_qlfattr ()
		  OLD(,Asc_DebuWriteVarAttr,
                     (ClientData)1, (Tcl_CmdDeleteProc *) NULL,
    "debugger",
    NULL,
    NULL,
    NULL
    /* the stuff above should be replaced with header macros. */
  );

  int dbg_rel_included ()
	  OLD(,Asc_DebuRelIncluded,
    "debugger",
    NULL,
    NULL,
    NULL
    /* the stuff above should be replaced with header macros. */
  );

  int dbg_var_fixed ()
	  OLD(,Asc_DebuVarFixed,
    "debugger",
    NULL,
    NULL,
    NULL
    /* the stuff above should be replaced with header macros. */
  );

  int dbg_get_incidence ()
	  OLD(,Asc_DebuGetIncidence,
    "debugger",
    NULL,
    NULL,
    NULL
    /* the stuff above should be replaced with header macros. */
  );

  int dbg_get_order ()
	  OLD(,Asc_DebuGetOrder,
    "debugger",
    NULL,
    NULL,
    NULL
    /* the stuff above should be replaced with header macros. */
  );

  int dbg_write_incidence ()
	  OLD(,Asc_DebuWriteIncidence,
    "debugger",
    NULL,
    NULL,
    NULL
    /* the stuff above should be replaced with header macros. */
  );

  int dbg_find_eligible ()
	  OLD(,Asc_DebuFindEligible,
    "debugger",
    NULL,
    NULL,
    NULL
    /* the stuff above should be replaced with header macros. */
  );

  int brow_find_eligible ()
	  OLD(,Asc_DebuInstEligible,
    "debugger",
    NULL,
    NULL,
    NULL
    /* the stuff above should be replaced with header macros. */
  );

  int dbg_consistency_analysis ()
	  OLD(,Asc_DebuConsistencyAnalysis,
    "debugger",
    NULL,
    NULL,
    NULL
    /* the stuff above should be replaced with header macros. */
  );

  int dbg_global_eligible ()
	  OLD(,Asc_DebuFindGlobalEligible,
    "debugger",
    NULL,
    NULL,
    NULL
    /* the stuff above should be replaced with header macros. */
  );


  int dbg_find_activerels ()
	  OLD(,Asc_DebuFindActive,
    "debugger",
    NULL,
    NULL,
    NULL
    /* the stuff above should be replaced with header macros. */
  );

  int brow_find_activerels ()
	  OLD(,Asc_DebuInstActive,
    "debugger",
    NULL,
    NULL,
    NULL
    /* the stuff above should be replaced with header macros. */
  );

  int dbg_struct_singular ()
	  OLD(,Asc_DebuStructSing,
    "debugger",
    NULL,
    NULL,
    NULL
    /* the stuff above should be replaced with header macros. */
  );

  int dbg_num_block_singular ()
	  OLD(,Asc_DebuNumBlockSing,
    "debugger",
    NULL,
    NULL,
    NULL
    /* the stuff above should be replaced with header macros. */
  );

  int var_free2nom ()
	  OLD(,Asc_DebuVarFree2Nom,
    "debugger",
    NULL,
    NULL,
    NULL
    /* the stuff above should be replaced with header macros. */
  );

  int var_nom2free ()
	  OLD(,Asc_DebuVarNom2Free,
    "debugger",
    NULL,
    NULL,
    NULL
    /* the stuff above should be replaced with header macros. */
  );

  int dbg_calc_relnoms ()
	  OLD(,Asc_DebuCalcRelNominals,
    "debugger",
    NULL,
    NULL,
    NULL
    /* the stuff above should be replaced with header macros. */
  );

  int dbg_check_rels ()
	  OLD(,Asc_DebuCheckRelFp,
    "debugger",
    NULL,
    NULL,
    NULL
    /* the stuff above should be replaced with header macros. */
  );

  int dbg_write_slv0_xsys ()
	  OLD(,Asc_DebuWriteSystem,
    "fileio",
    NULL,
    NULL,
    NULL
    /* the stuff above should be replaced with header macros. */
  );


  int dbg_write_slv0_sys ()
		  OLD(,Asc_DebuWriteSystem,
                 (ClientData) 1, (Tcl_CmdDeleteProc *) NULL,
    "debugger",
    NULL,
    NULL,
    NULL
    /* the stuff above should be replaced with header macros. */
  );

  int dbg_mtxwriteplot ()
		  OLD(,Asc_DebuMtxWritePlotCmd,
                 (ClientData) 1, (Tcl_CmdDeleteProc *) NULL,
    "debugger",
    NULL,
    NULL,
    NULL
    /* the stuff above should be replaced with header macros. */
  );

  int dbg_calc_jacobian ()
		  OLD(,Asc_DebuMtxCalcJacobianCmd,
                     (ClientData) 1, (Tcl_CmdDeleteProc *) NULL,
    "debugger",
    NULL,
    NULL,
    NULL
    /* the stuff above should be replaced with header macros. */
  );

  int dbghelp ()
  OLD(,Asc_DebuHelpList,
    "debugger",
    NULL,
    "dbghelp    -- give tcl/[s]hort/[l]ong debugger primitives list",
    NULL
    /* the stuff above should be replaced with header macros. */
  );


  /* ivp solver commands */
  int integrate_setup ()
	  OLD(, Asc_IntegSetupCmd,
    "solver",
    NULL,
    NULL,
    NULL
    /* the stuff above should be replaced with header macros. */
  );

  int integrate_cleanup ()
	  OLD(, Asc_IntegCleanupCmd,
    "solver",
    NULL,
    NULL,
    NULL
    /* the stuff above should be replaced with header macros. */
  );

  int integrate_set_y_file ()
	  OLD(, Asc_IntegSetYFileCmd,
    "solver",
    NULL,
    NULL,
    NULL
    /* the stuff above should be replaced with header macros. */
  );

  int integrate_set_obs_file ()
	  OLD(, Asc_IntegSetObsFileCmd,
    "solver",
    NULL,
    NULL,
    NULL
    /* the stuff above should be replaced with header macros. */
  );

  int integrate_logunits ()
	  OLD(, Asc_IntegSetFileUnitsCmd,
    "solver",
    NULL,
    NULL,
    NULL
    /* the stuff above should be replaced with header macros. */
  );

  int integrate_logformat ()
	  OLD(, Asc_IntegSetFileFormatCmd,
    "solver",
    NULL,
    NULL,
    NULL
    /* the stuff above should be replaced with header macros. */
  );

  int integrate_get_samples ()
	  OLD(, Asc_IntegGetXSamplesCmd,
    "solver",
    NULL,
    NULL,
    NULL
    /* the stuff above should be replaced with header macros. */
  );

  int integrate_set_samples ()
	  OLD(, Asc_IntegSetXSamplesCmd,
    "solver",
    NULL,
    NULL,
    NULL
    /* the stuff above should be replaced with header macros. */
  );

  int integrate_able ()
	  OLD(, Asc_IntegInstIntegrableCmd,
    "solver",
    NULL,
    NULL,
    NULL
    /* the stuff above should be replaced with header macros. */
  );

  /* This code is in Sensitivity.[ch] but needs a home */
  /* Also need a different name as the Mtx name space is already taken. */
  int __mtx_norms ()
	  OLD(, Asc_MtxNormsCmd,
    "solver",
    NULL,
    NULL,
    NULL
    /* the stuff above should be replaced with header macros. */
  );

  /* Working very hard to avoid direct X code, here we make a command
     set for handling incidence matrices via Tcl/tk without the
     amazing overhead of a tcl interpreter getting in the way.
     Avoids a _lot_ of list operations.
  */
  /* mtx incidence commands */
  int mtx_gui_plot_incidence ()
	  OLD(,Asc_MtxGUIPlotIncidence,
    "mtxplot",
    NULL,
    NULL,
    NULL
    /* the stuff above should be replaced with header macros. */
  );

  int mtxhelp ()
	  OLD(,Asc_MtxHelpList,
    "mtxplot",
    NULL,
    "mtxhelp    -- give tcl/[s]hort/[l]ong incid matrix primitives.",
    NULL
    /* the stuff above should be replaced with header macros. */
  );

  /* Units window stuff */
  int u_destroy_units ()
	  OLD(,Asc_UnitDestroyDisplayList,
    "units",
    NULL,
    NULL,
    NULL
    /* the stuff above should be replaced with header macros. */
  );

  int u_setSIdef ()
	  OLD(,Asc_UnitDefaultBaseUnits,
    "units",
    NULL,
    NULL,
    NULL
    /* the stuff above should be replaced with header macros. */
  );

  int u_getbasedef ()
	  OLD(,Asc_UnitGetBaseUnits,
    "units",
    NULL,
    NULL,
    NULL
    /* the stuff above should be replaced with header macros. */
  );

  int u_dump ()
	  OLD(,Asc_UnitDump,
    "units",
    NULL,
    NULL,
    NULL
    /* the stuff above should be replaced with header macros. */
  );

  int u_dims ()
	  OLD(,Asc_DimenDump,
    "units",
    NULL,
    NULL,
    NULL
    /* the stuff above should be replaced with header macros. */
  );

  int u_dim_setverify ()
	  OLD(,Asc_DimenRelCheck,
    "units",
    NULL,
    NULL,
    NULL
    /* the stuff above should be replaced with header macros. */
  );

  int u_dim2num ()
	  OLD(,Asc_UnitBaseDimToNum,
    "units",
    NULL,
    NULL,
    NULL
    /* the stuff above should be replaced with header macros. */
  );

  int u_num2dim ()
	  OLD(,Asc_UnitNumToBaseDim,
    "units",
    NULL,
    NULL,
    NULL
    /* the stuff above should be replaced with header macros. */
  );

  int u_frombasedim ()
	  OLD(,Asc_UnitMatchBaseDim,
    "units",
    NULL,
    NULL,
    NULL
    /* the stuff above should be replaced with header macros. */
  );

  int u_fromatomdim ()
	  OLD(,Asc_UnitMatchAtomDim,
    "units",
    NULL,
    NULL,
    NULL
    /* the stuff above should be replaced with header macros. */
  );

  int u_getdimatoms ()
	  OLD(,Asc_UnitGetAtomList,
    "units",
    NULL,
    NULL,
    NULL
    /* the stuff above should be replaced with header macros. */
  );

  int u_change_baseunit ()
	  OLD(,Asc_UnitChangeBaseUnit,
    "units",
    NULL,
    NULL,
    NULL
    /* the stuff above should be replaced with header macros. */
  );

  int u_getprec ()
	  OLD(,Asc_UnitGetPrec,
    "units",
    NULL,
    NULL,
    NULL
    /* the stuff above should be replaced with header macros. */
  );

  int u_setprec ()
	  OLD(,Asc_UnitSetPrec,
    "units",
    NULL,
    NULL,
    NULL
    /* the stuff above should be replaced with header macros. */
  );

  int u_get_atoms ()
	  OLD(,Asc_UnitGetAtomsForUnit,
    "units",
    NULL,
    NULL,
    NULL
    /* the stuff above should be replaced with header macros. */
  );


  int u_get_units ()
	  OLD(,Asc_UnitGetUnits,
    "units",
    NULL,
    NULL,
    NULL
    /* the stuff above should be replaced with header macros. */
  );

  int u_set_user ()
	  OLD(,Asc_UnitSetUser,
    "units",
    NULL,
    NULL,
    NULL
    /* the stuff above should be replaced with header macros. */
  );

  int u_get_user ()
	  OLD(,Asc_UnitGetUser,
    "units",
    NULL,
    NULL,
    NULL
    /* the stuff above should be replaced with header macros. */
  );

  int u_get_list ()
	  OLD(,Asc_UnitGetList,
    "units",
    NULL,
    NULL,
    NULL
    /* the stuff above should be replaced with header macros. */
  );

  int u_clear_user ()
	  OLD(,Asc_UnitClearUser,
    "units",
    NULL,
    NULL,
    NULL
    /* the stuff above should be replaced with header macros. */
  );

  int u_getval ()
	  OLD(,Asc_UnitGetVal,
    "units",
    NULL,
    NULL,
    NULL
    /* the stuff above should be replaced with header macros. */
  );

  int u_browgetval ()
	  OLD(,Asc_UnitBrowGetVal,
    "units",
    NULL,
    NULL,
    NULL
    /* the stuff above should be replaced with header macros. */
  );

  int u_slvgetrelval ()
	  OLD(,Asc_UnitSlvGetRelVal,
    "units",
    NULL,
    NULL,
    NULL
    /* the stuff above should be replaced with header macros. */
  );

  int u_slvgetvarval ()
	  OLD(,Asc_UnitSlvGetVarVal,
    "units",
    NULL,
    NULL,
    NULL
    /* the stuff above should be replaced with header macros. */
  );

  int u_slvgetobjval ()
	  OLD(,Asc_UnitSlvGetObjVal,
    "units",
    NULL,
    NULL,
    NULL
    /* the stuff above should be replaced with header macros. */
  );

  int uhelp ()
	  OLD(,Asc_UnitHelpList,
    "units",
    NULL,
    "uhelp      -- give tcl/[s]hort/[l]ong units primitives list.",
    NULL
    /* the stuff above should be replaced with header macros. */
  );

  /*Script callbacks */
  int srefine ()
	  OLD(, Asc_ScriptRefineCmd,
    "scripting",
    NULL,
    "srefine    -- refine qlfdid to type given",
    NULL
    /* the stuff above should be replaced with header macros. */
  );

  int smerge ()
	  OLD(, Asc_ScriptMergeCmd,
    "scripting",
    NULL,
    "smerge     -- merge 2 qlfdids",
    NULL
    /* the stuff above should be replaced with header macros. */
  );

  /* Auxiliaries and Window Management */
  int ascloadwin ()
	  OLD(,Asc_LoadWin,
    "miscellaneous",
    NULL,
    NULL,
    NULL
    /* the stuff above should be replaced with header macros. */
  );

  int gnutext ()
	  OLD(,Asc_GNUTextCmd,
    "miscellaneous",
    NULL,
    "gnutext    -- give compiler license [l] or warranty [w]",
    NULL
    /* the stuff above should be replaced with header macros. */
  );


#if REIMPLEMENT
  int dbg_write_kirk_xsys () OLD(,Asc_DebuWriteKirkSystem,
                    (ClientData) NULL, (Tcl_CmdDeleteProc *) NULL);
  int dbg_write_gams_xsys () OLD(,Asc_DebuWriteGAMSSystem,
                    (ClientData) NULL, (Tcl_CmdDeleteProc *) NULL);
#endif
#if REIMPLEMENT
  /* code generation commands */
  int __codegen_parsedata () OLD(,Asc_CodeGenParseDataCmd,
                    (ClientData) NULL, (Tcl_CmdDeleteProc *) NULL);
  int __codegen_general () OLD(,Asc_CodeGenGeneralCmd,
                    (ClientData) NULL, (Tcl_CmdDeleteProc *) NULL);
  int __codegen_c () OLD(,Asc_CodeGenCCmd,
                    (ClientData) NULL, (Tcl_CmdDeleteProc *) NULL);
  int __codegen_gams () OLD(,Asc_CodeGenGamsCmd,
                    (ClientData) NULL, (Tcl_CmdDeleteProc *) NULL);
  int __codegen_write () OLD(,Asc_CodeGenWriteCmd,
                    (ClientData) NULL, (Tcl_CmdDeleteProc *) NULL);
  int __codegen_read () OLD(,Asc_CodeGenReadCmd,
                    (ClientData) NULL, (Tcl_CmdDeleteProc *) NULL);
#endif
  /* some experimental stuff */
#if 0 && defined(DISUSED)
  int __codegen_types () OLD(,Asc_CodeGenTypesCmd,
                    (ClientData) NULL, (Tcl_CmdDeleteProc *) NULL);
#endif
  /* Dag Routines */
#if KILLBDAG
  int __brow_tree_list () OLD(, Asc_BrowTreeListCmd,
                    (ClientData) NULL,(Tcl_CmdDeleteProc *) NULL);

  int __dag_write_instdag () OLD(, Asc_DagWriteInstDagCmd,
                    (ClientData) NULL,(Tcl_CmdDeleteProc *) NULL);
  int __dag_write_modeldag () OLD(, Asc_DagWriteModelDagCmd,
                    (ClientData) NULL,(Tcl_CmdDeleteProc *) NULL);
  int __dag_partition () OLD(, Asc_DagPartitionCmd,
                    (ClientData) NULL,(Tcl_CmdDeleteProc *) NULL);
  int __dag_countrelns () OLD(, Asc_DagCountRelnsCmd,
                    (ClientData) NULL,(Tcl_CmdDeleteProc *) NULL);

  int __dag_coupling_relns () OLD(, Asc_DagCouplingRelnsCmd,
                    (ClientData) NULL,(Tcl_CmdDeleteProc *) NULL);
  int __dag_model_relns () OLD(, Asc_DagModelRelnsCmd,
                    (ClientData) NULL,(Tcl_CmdDeleteProc *) NULL);

  int __dag_build () OLD(, Asc_DagBuildDagCmd,
                    (ClientData) NULL,(Tcl_CmdDeleteProc *) NULL);
  int __dag_prepare () OLD(, Asc_DagPrepareCmd,
                    (ClientData) NULL,(Tcl_CmdDeleteProc *) NULL);
  int __dag_shutdown () OLD(, Asc_DagShutdownCmd,
                    (ClientData) NULL,(Tcl_CmdDeleteProc *) NULL);
#endif /* killbdag */

#ifdef DEBUG_MALLOC
  /* See GUIinit.[ch] for definition of Asc_DebugMallocCmd */
  int __dbmalloc ()
		  OLD(,Asc_DebugMallocCmd,
                    (ClientData) NULL, (Tcl_CmdDeleteProc *) NULL);
#endif

#ifdef BALLAN /* testing crap not checked in */

  int  Asc_LibrAnonTypesCmdHC ()
  OLD(, Asc_LibrAnonTypesCmd,
    "library", Asc_LibrAnonTypesCmdHU,  Asc_LibrAnonTypesCmdHS,
    Asc_LibrAnonTypesCmdHLF);

  int  Asc_DotCmdHC ()
  OLD(, Asc_DotCmd, "browser",
    Asc_DotCmdHU,  Asc_DotCmdHS, Asc_DotCmdHLF);

#endif /* ballan */

};

