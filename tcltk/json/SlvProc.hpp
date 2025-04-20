/*
 *  SlvProc.h
 *  by Ken Tyner and Ben Allan
 *  Created: 6/97
 *  Version: $Revision: 1.4 $
 *  Version control file: $RCSfile: SlvProc.h,v $
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
 *
 *  This probe implementation constitutes a total rework of the probe
 *  first created by Abbott, 1994.
 */

#ifndef ASCTK_SLVPROC_HPP
#define ASCTK_SLVPROC_HPP

#define SLVMONITORPREFIX "slvmon"

int Asc_VarAnalyzeCmd(Asc_DString *hptr,
                             int argc, CONST84 char *argv[]);
/**<
 *  <!--  int Asc_VarAnalyzeCmd;                                       -->
 *  This function will analyze the elements in the current slv_system to see
 *  whether they satisfy the proximity queries. The variable type supported is
 *  the "solver_var". It can be used to analyse variables or relations for
 *  proximity to a "othervalue". rel?abs will determine whether
 *  the queries are done relatively or using absolute values. tolerance must
 *  be set appropriately based on rel?abs. The queries ?scaling?lower?upper
 *  pertain to the nominal,lower_bound, and upper_bound of a solver_var with
 *  respect to the value of the solver_var. "residual" pertains to the residual
 *  of a relation as compared to 0.0. If for any of the queries,
 *  the values are too close to zero, then "absolute" queries are made.
 *  Internal units are used for all queries.<br><br>
 *
 *  Examples:
 *   -# __var_analyze 20 46 lower rel 0.2;
 *      will analyse (indexes 20 through 46 inclusive) all solver_vars
 *      that have values which are within 20% of their lower_bounds.
 *   -# __var_analyze 20 46 lower abs 0.2;
 *      will analyse (indexes 20 through 46 inclusive) all solver_vars
 *      that have values which are within 0.2 of their lower_bounds.
 *   -# __var_analyze 20 46 other rel 0.2 15.0;
 *      will analyse (indexes 20 through 46 inclusive) all solver_vars
 *      that have values which are within 20% of 15.0.
 *
 *  Will be return a formatted string with of the form:
 *  {index b_close} {index b_far} where index is the original probe list index.
 *  See the source for more details.<br><br>
 *
 *  Registered as \"__var_analyze low high scaling?lower?upper?other rel?abs tolerance <othervalue>.
 */
int Asc_RelAnalyzeCmd(Asc_DString *hptr, int argc, CONST84 char *argv[]);
/**<
 *  <!--  int Asc_RelAnalyzeCmd;                                       -->
 *  See Asc_VarAnalyzeCmd().
 *
 *  Registered as \"__rel_analyze low high residual?other rel?abs tolerance <othervalue>.
 */

int Asc_SolveMonitorCmd(Asc_DString *, int, CONST84 char **);
/**< status = Asc_SolveMonitorCmd((cdata,hptr, argc,argv); */

int SolveMonitor(Asc_DString *hptr, int argc, const char *argv[]);
/**< callback for instances. */

#endif  /* ASCTK_SLVPROC_HPP */

