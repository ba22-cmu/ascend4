/*
 *  SolverProc.h
 *  by Kirk Abbott and Ben Allan
 *  Created: 1/94
 *  Version: $Revision: 1.20 $
 *  Version control file: $RCSfile: SolverProc.h,v $
 *  Date last modified: $Date: 2003/08/23 18:43:08 $
 *  Last modified by: $Author: ballan $
 *
 *  This file is part of the ASCEND Tcl/Tk interface
 *
 *  Copyright 1997, Carnegie Mellon University
 *
 *  The ASCEND Tcl/Tk interface is free software; you can redistribute
 *  it and/or modify it under the terms of the GNU General Public License as
 *  published by the Free Software Foundation; either version 2 of the
 *  License, or (at your option) any later version.
 *
 *  The ASCEND Tcl/Tk interface is distributed in hope that it will be
 *  useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef ASCTK_SOLVERPROC_HPP
#define ASCTK_SOLVERPROC_HPP

/** Marker for code to be removed */
#define DELETEME 0

static void Asc_SolvMemoryCleanup(void);
/**< Invoke system_free_reused_mem to clean up all memory solvers hide. */

#ifdef ASC_SIGNAL_TRAPS

int Asc_SolvTrapINT(Asc_DString *hptr, int argc, CONST84 char *argv[]);
/**< Activate floating point traps for slv/script callbacks  /slv_trapint/ */

int Asc_SolvTrapFP(Asc_DString *hptr, int argc, CONST84 char *argv[]);
/**< Activate floating point traps for slv callbacks  /slv_trapfp/ */

int Asc_SolvUnTrapINT(Asc_DString *hptr, int argc, CONST84 char *argv[]);
/**< Deactivate floating point traps for slv callbacks /slv_untrapfp/ */

int Asc_SolvUnTrapFP(Asc_DString *hptr, int argc, CONST84 char *argv[]);
/**< Deactivate floating point traps for slv callbacks /slv_untrapfp/ */

#endif


#if DELETEME /* going away */
int Asc_Sims2Solve(Asc_DString *hptr, int argc, CONST84 char *argv[]);
/**<
 *  Sends named simulation to solver context.
 *  Do this before calling solver window update. Flushes current sys.
 *  if exported instance and type are valid.
 *  Somebody elses job to update the solver window context list.
 *  Faster than Solv_ImportQlfdid.
 *  Does a presolve using slv0.
 */

int Asc_Brow2Solve(Asc_DString *hptr, int argc, CONST84 char *argv[]);
/**<
 *  Sends browser focused simulation(or subpart) to solver context.
 *  Do this before calling solver window update. Flushes current sys.
 *  If exported instance and type are valid.
 *  Somebody elses job to update the solver window context list.
 *  Faster than Solv_ImportQlfdid.
 *  Does a presolve using slv0.
 */

int Asc_SolvSimInst(Asc_DString *hptr, int argc, CONST84 char *argv[]);
/**<
 *  Solves top level simulation named with solver specified.
 *  This is for commandline use only.
 *  Just a wrapper of slv_interface.c Solve().
 */
#endif /* DELETEME */

#if 0
int Asc_SolvCurInst(Asc_DString *hptr,
                           int argc, CONST84 char *argv[]);
/**<
 *  Solves g_curinst with solver specified.
 *  This is for commandline use only.
 *  Just a wrapper of slv_interface.c Solve() for now.
 *  this function is Extremely buggy.
 */
#endif

int Asc_SolvGetModKids(Asc_DString *hptr, int argc, CONST84 char *argv[]);
/**<
 *  Returns a tcl list of the MODEL_INST children of the qlfdid passed
 *  in. Note this routine probably ought to be generalized to all INST
 *  types. Uses Asc_BrowQlfdidSearchCmd().
 */

int Asc_SolvIncompleteSim(Asc_DString *hptr, int argc, CONST84 char *argv[]);
/**< Checks whether named simulation is complete. */

int Asc_SolvCheckSys (Asc_DString *hptr, int argc, CONST84 char *argv[]);
/**< Checks whether g_solvsys_cur is non-NULL. */

int Asc_SolvGetObjList(Asc_DString *hptr, int argc, CONST84 char *argv[]);
/**<
 *  Output will be a list of indexes of the included objectives
 *  This list will be enclosed in {} so the output of the function is
 *  always a 1 item list whose first item is the list of indices.
 *  The list may come back empty if appropriate.
 *  Operates on g_solvsys_cur.<br><br>
 *
 *  Registered as:  slv_get_obj_list <dest> 0= stdout 1 = stderr 2= list
 */

int Asc_SolvSetObjByNum(Asc_DString *hptr, int argc, CONST84 char *argv[]);
/**<
 *  Sets the solvers objective to list[num] where
 *  list is the solvers list of objective relations.
 *  If num is -1 the ojective is set to NULL. <br><br>
 *
 *  Registered as:  slv_set_obj_by_num
 */

int Asc_SolvGetObjNumCmd(Asc_DString *hptr, int argc, CONST84 char *argv[]);

int Asc_SolvGetSlvParms(Asc_DString *hptr, int argc, CONST84 char *argv[]);
/**<
 *  Loads up hptr->result with strings from the C data. If solver not
 *  linked, returns slv0 parameters instead.
 *  g_solvsys_cur is used. specializes on slvI, I=solver number.
 *  This code should be updated if slv_parameters_structure
 *  slv_tolerance_data, or slv_output_data in slv.h are changed.
 *  If a specified solver is not linked, then slv0 parameters will be
 *  returned instead.<br><br>
 *
 *  Returns:
 *    - number of solver parameters were retrieved from.
 *    - time_limit
 *    - iteration_limit
 *    - tolerance.termination
 *    - tolerance.feasible
 *    - tolerance.pivot
 *    - tolerance.singular
 *    - tolerance.stationary
 *    - rho
 *    - partition
 *    - ignore_bounds
 *    - on(1)/off(0) of output file more_important
 *    - on(1)/off(0) of output file less_important
 *    - factor_option  (should be a sp)
 *
 *  Additional returns, if solver I has subparameters:
 *  This is one of those places where the interface and the solver have
 *  to make a deal. In particular, the TCL writer has to look at
 *  the slvI.h files to know the meaning of what is
 *  returned. Here we will return the parameters in the order they are
 *  stored in the arrays and the arrays will be printed in order:
 *  iap,rap,cap. The vap array will not be returned to TCL. vap is for
 *  use only in C.<br><br>
 *
 *  Registered as: slv_get_parms <solver number>
 */

int Asc_SolvSetSlvParms(Asc_DString *hptr, int argc, CONST84 char *argv[]);
/**<
 *  Loads up slv parameters struct from argv.
 *  This code should be updated if slv_parameters_structure
 *  slv_tolerance_data, or slv_output_data in slv.h are changed.
 *  If a specified solver is not linked, parameters will not be set.<br><br>
 *
 *  Input:
 *    - solver number
 *    - time_limit (seconds)
 *    - iteration_limit (integer)
 *    - tolerance.termination
 *    - tolerance.feasible
 *    - tolerance.pivot
 *    - tolerance.singular
 *    - tolerance.stationary
 *    - rho
 *    - partition  on(1)/off(0)
 *    - ignore_bounds 1->ignore, 0 ->not ignore
 *    - on(1)/off(0) of output file more_important
 *    - on(1)/off(0) of output file less_important
 *
 *  The exact number of values required to fill the subparameter arrays
 *  iap, rap must also be given and in the proper order.
 *  (set them just as you would be getting them.)
 *  String pointer and void pointer subparameters are set in a different
 *  call. Since no one has made use of string or void pointer parameters
 *  yet, their set call remains to be defined.<br><br>
 *
 *  Notes: Tcl_ExprBoolean is used on the binaries.<br><br>
 *
 *  Registered as:  set_slv_parms <items>
 */

int Asc_SolvGetSlvParmsNew(Asc_DString *hptr, int argc, CONST84 char *argv[]);
int Asc_SolvSetSlvParmsNew(Asc_DString *hptr, int argc, CONST84 char *argv[]);
/**<
 * extern int Asc_SolvGetSlvBoolParms(Asc_DString *hptr,
 *                             int argc, CONST84 char *argv[]);
 * extern int Asc_SolvGetSlvRealParms(Asc_DString *hptr,
 *                             int argc, CONST84 char *argv[]);
 */

int Asc_SolvGetInstType(Asc_DString *hptr, int argc, CONST84 char *argv[]);
/**<
 *  Returns a string ptr needed for the IS_A field.
 *  DO _ NOT _ FREE this pointer as compiler owns it.
 */

int Asc_SolvGetSlvStatPage(Asc_DString *hptr, int argc, CONST84 char *argv[]);
/**<
 *  Loads up hptr->result with strings from the slv status struct.
 *  g_solvsys_cur is used.
 *  This code should be updated if slv__status_structure or
 *  slv__block_status_structure in slv.h change.<br><br>
 *
 *  Returns:
 *    - ok   (boole)
 *    - over_defined   (boole)
 *    - under_defined   (boole)
 *    - struct_singular   (boole)
 *    - ready_to_solve   (boole)
 *    - converged    (boole)
 *    - diverged   (boole)
 *    - inconsistent   (boole)
 *    - calc_ok   (boole)
 *    - iteration_limit_exceeded   (boole)
 *    - time_limit_exceeded   (boole)
 *    - iteration (integer)
 *    - cpu_elapsed
 *    - (block) number_of
 *    - current_block
 *    - current_size
 *    - previous_total_size
 *    - iteration
 *    - cpu_elapsed
 *    - residual
 *
 *  Registered as:  slv_get_stat_page
 */

int Asc_SolvGetSlvCostPage(Asc_DString *hptr, int argc, CONST84 char *argv[]);
/**<
 *  Loads up hptr->result with strings from the slv status struct
 *  cost list. It is likely that this data is only valid after the
 *  solver has converged or otherwise finished.
 *  Returns a list (in block order) of block costs from the current
 *  solver. Some solvers may not have one. <br><br>
 *
 *  Each list element is:
 *    - block size
 *    - iterations in block
 *    - function calls
 *    - jacobian calls
 *    - cpu time
 *    - residual
 *    - function time
 *    - jacobian time
 *
 *  Registered as:  slv_get_cost_page
 */

int Asc_SolvGetObjectiveVal(Asc_DString *hptr, int argc, CONST84 char *argv[]);
/**<
 *  Evaluates and returns the objective function, or "none" if there
 *  isn't one.
 */

int Asc_SolvGetInstName(Asc_DString *hptr, int argc, CONST84 char *argv[]);
/**<
 *  Returns the name from solvinstroot to solvinstcur.
 *  Returns \none\ if either inst is null.
 */

int Asc_SolvGetPathName(Asc_DString *hptr, int argc, CONST84 char *argv[]);
/**<
 *  Returns the name from solvinstroot to solvinstcur including sim.
 *  Returns \none\ if either inst is null.
 */

int Asc_SolvGetVRCounts(Asc_DString *hptr, int argc, CONST84 char *argv[]);
/**<
 *  Returns the counts of variables and relations from g_solvsys_cur.
 *  Call presolve before calling this.
 *  Return order is:
 *    - total relations,
 *    - active relations,
 *    - included relations,
 *    - total vars,
 *    - active vars,
 *    - free vars,
 *    - equalities,
 *    - included equalities
 *    - inequalities,
 *    - inactive included equalities,
 *    - inactive included inequalities,
 *    - unincluded relations,
 *    - fixed vars,
 *    - inactive free vars,
 *    - inactive fixed vars,
 *    - active unattached vars
 *
 *  Modified for including the notion of active variable and active
 *  relation. VRR
 */

/* Hereunder we do some lowlevel operations.                         */

int Asc_SolvSlvDumpInt(Asc_DString *hptr, int argc, CONST84 char *argv[]);
/**< Calls slv_dump_internals on g_solvsys_cur. */

int Asc_SolvSlvPresolve(Asc_DString *hptr, int argc, CONST84 char *argv[]);
/**< Presolves the g_solvsys_cur, or tries to. */

int Asc_SolvReanalyze(Asc_DString *hptr, int argc, CONST84 char *argv[]);
/**< Reanalyze the g_solvsys_cur solver lists after running a procedure. */

int Asc_SolvCheckAndReanalyze(Asc_DString *hptr, int argc, CONST84 char *argv[]);
/**<
 *  Checks if the instance modified in the browser is a member of a
 *  whenvarlist and, if it is, reanalyze the g_solvsys_cur solver lists.
 */

int Asc_SolvSlvResolve(Asc_DString *hptr, int argc, CONST84 char *argv[]);
/**< Resolves the g_solvsys_cur, or tries to. */

int Asc_SolvSlvSolve(Asc_DString *hptr, int argc, CONST84 char *argv[]);
/**< Solves the g_solvsys_cur, or tries to. */

int Asc_SolvSlvIterate(Asc_DString *hptr, int argc, CONST84 char *argv[]);
/**<
 *  Iterates the g_solvsys_cur up to n times for t seconds or until some*
 *  error or convergence occurs. At least 1 iteration in any case.
 *  If t not given, 5 cpu seconds assumed.<br><br>
 *
 *  Registered as:  slv_iterate n t
 */

int Asc_SolvAvailSolver(Asc_DString *hptr, int argc, CONST84 char *argv[]);
/**<
 *  Returns the list of linked solver names.
 *  If a solver in not linked, the first three char of its name
 *  will be no_.<br><br>
 *
 *  Registered as:  slv_available <no args>
 */

int Asc_SolvSolverNum(Asc_DString *hptr, int argc, CONST84 char *argv[]);
/**<
	Returns solver number for given name.
	Registered as: slv_number <name>
*/

int Asc_SolvSolverName(Asc_DString *hptr, int argc, CONST84 char *argv[]);
/**<
	Returns solver name for given number.
	Registered as: slv_name <name>
*/

int Asc_SolvLinsolNames(Asc_DString *hptr, int argc, CONST84 char *argv[]);
/**<
 *  Returns the list of linsolqr factor methods separated
 *  by commas.  Split at commas to extract names.<br><br>
 *
 *  Registered as:  slv_linsol_names <no args>
 */

int Asc_SolvEligSolver(Asc_DString *hptr, int argc, CONST84 char *argv[]);
/**<
 *  Returns a boolean of whether the current solver is eligible on
 *  current problem.
 *  If any third argument is present, displays eligibility of all
 *  solvers on stderr for current problem.
 *  Swiped Karl code in slv_interface
 */

int Asc_SolvSelectSolver(Asc_DString *hptr, int argc, CONST84 char *argv[]);
/**<
 *  Sets solver to N, if N available.
 *  On return the Tcl hptrreter result is set to the selected solver number.
 *
 *  @param cdata  Not used.
 *  @param hptr The Tcl hptrreter.
 *  @param argc   The number of argv elements - must be 2.
 *  @param argv   argv[1] should contain the number of the solver
 *                to select as a string.
 *  @return HELP_OK if the solver was selected, HELP_ERROR otherwise.
 */

int Asc_SolvGetSelectedSolver(Asc_DString *hptr, int argc, CONST84 char *argv[]);
/**<
 *  Gets the number of the selected solver.
 *  On return the Tcl hptrreter result is set to the selected solver number.
 *
 *  @param cdata  Not used.
 *  @param hptr The Tcl hptrreter.
 *  @param argc   The number of argv elements - must be 1.
 *  @param argv   Not used.
 *  @return HELP_OK if the solver was selected, HELP_ERROR otherwise.
 */

int Asc_SolvImportQlfdid(Asc_DString *hptr, int argc, CONST84 char *argv[]);
/**<
 *  Import qlfdid to solver context.
 *  Will take a fully qualified ascend name and will search for the
 *  instance with that name. Will leave g_solvsys_cur looking at the
 *  named instance if successful.
 *  Uses Asc_BrowQlfdidSearchCmd().
 *  If a called with an id and a third argument, will return 0 if an
 *  import is allowable, or 1 if it is not, without actually importing.
 *  e.g. set notok [slv_import_qlfdid fred.model test]
 *  Does NOT presolve. leaves solver number set to previous number if
 *  solver previously occupied.<br><br>
 *
 *  Registered as : slv_import_qlfdid
 */

int Asc_SolvFlushSolver(Asc_DString *hptr, int argc, CONST84 char *argv[]);
/**<
 *  Clear out the solver.
 *  Free all memory associated with all solver engines.
 */

int Asc_SolvMakeIndependent(Asc_DString *hptr, int argc, CONST84 char *argv[]);
/**<
 *  Move vars (given by index #) to the unassigned region of the slv0
 *  matrix if possible.  Returns indices of vars that can't be moved
 *  because structural infeasibility would occur.
 *  Returns tcl_Error if too many vars are specified or slv0 otherwise
 *  not ready.
 *  May have the side effect of repartitioning if needed.
 *  Will fail if the mtx does not have the incidence pattern in it,
 *  which is generally only the case at the end of slv_presolve.
 *  The cost data will also be updated if necessary.<br><br>
 *
 *  Registered as:  slv_set_independent <index> [index ...]
 */

int Asc_SolvGetLnmEpsilon(Asc_DString *hptr, int argc, CONST84 char *argv[]);
/**< Get epsilon used in lnm (modified log ) calculation. */

int Asc_SolvSetLnmEpsilon(Asc_DString *hptr, int argc, CONST84 char *argv[]);
/**< Set epsilon used in lnm (modified log ) calculation. 0.5 > eps > 0.0. */

int Asc_SolvSetCHaltFlag(Asc_DString *hptr, int argc, CONST84 char *argv[]);
/**<
 *  Tcl function for setting the halt flag for a solver.
 *  integer is an integer value, 0 or nonzero. If 0 will turn the
 *  flag off.  Otherwise will turn the flag on.
 *
 *  Registered as:  slv_set_haltflag integer.
 */

int Asc_SolvHelpList(Asc_DString *hptr, int argc, CONST84 char *argv[]);
/**<
 *  slvhelp command for tcl.
 *  No arg or "s" -> list names only. "l" -> short explanations also.<br><br>
 *
 *  Registered as:  slvhelp [s,l]
 */

int Asc_SolvNearBounds(Asc_DString *hptr, int argc, CONST84 char *argv[]);
/**<
 *  See function of same name in c for documentation.<br><br>
 *
 *  Registered as:  slv_near_bounds
 */

int Asc_SolvFarFromNominal(Asc_DString *hptr, int argc, CONST84 char *argv[]);
/**<
 *  See function of same name in c for documentation.<br><br>
 *
 *  Registered as:  slv_far_from_nominals
 */

#endif  /* ASCTK_SOLVERPROC_HPP */

