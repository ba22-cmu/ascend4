/*
 *  BrowserRel_io.h
 *  by Kirk Abbott and Ben Allan
 *  Created: 1/94
 *  Version: $Revision: 1.7 $
 *  Version control file: $RCSfile: BrowserRel_io.h,v $
 *  Date last modified: $Date: 2003/08/23 18:43:05 $
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

#ifndef ASCTK_BROWSERREL_IO_HPP
#define ASCTK_BROWSERREL_IO_HPP

int Asc_BrowWriteRelListCmd(Asc_DString *hptr, int argc, CONST84 char *argv[]);
/**<
 *  Write the relation in infix to the hptrreter. This gracefully
 *  handles all the types of relations now supported in ASCEND.
 *  If conditional relations exist,It separates the relation list
 *  in two lists. The list of  normal
 *  relations and the list of conditional relations.
 *  Conditional relations do not have to be solved, they
 *  represent boundaries in conditional modeling.<br><br>
 *
 *  Registered as: bgetrels
 */

int Asc_BrowWriteCondRelListCmd(Asc_DString *hptr, int argc, CONST84 char *argv[]);
/**<
 *  Write the list of conditional relations in infix to the hptrreter.
 *  This gracefully handles all the types of relations now supported in
 *  ASCEND.<br><br>
 *
 *  Registered as: bgetcondrels
 */

int Asc_BrowWriteRelListPostfixCmd(Asc_DString *hptr, int argc, CONST84 char *argv[]);
/**<
 *  Writes to the hptrreter rather than to a file.
 *  As I can see Kirk did not finish this implementation for all the types
 *  of relations.<br><br>
 *
 *  Registered as: bgetrelspf.
 */

int Asc_BrowWriteRelsForAtomCmd(Asc_DString *hptr, int argc, CONST84 char *argv[]);
/**<
 *  Will return a proper Tcl list of all relations associated with
 *  the given atom. Works on the current or the search instance.
 *  Will return HELP_ERROR if the instance is NULL, or the atom is not
 *  a REAL_ATOM_INST. This will be later extended to deal with boolean
 *  atoms. If some of the relations are conditionals, this information
 *  is also provided.<br><br>
 *
 *  Registered as: __brow_relsforatom ?cur?search.
 */

#endif  /* ASCTK_BROWSERREL_IO_HPP */
