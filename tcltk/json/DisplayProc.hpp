/*
 *  DisplayProc.h
 *  by Kirk Abbott and Ben Allan
 *  Created: 1/94
 *  Version: $Revision: 1.7 $
 *  Version control file: $RCSfile: DisplayProc.h,v $
 *  Date last modified: $Date: 2003/08/23 18:43:06 $
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

#ifndef ASCTK_DISPLAYPROC_HPP
#define ASCTK_DISPLAYPROC_HPP

int Asc_DispDefineCmd(Asc_DString *hptr, int argc, CONST84 char *argv[]);
/**<
 *  This function is used mainly by the Library and Display tools for
 *  displaying the types. It will take 0, 1 or 2 arguments. For no
 *  arguements (Unix style argc = 1) it will print the entire list of
 *  currently define types. For the second case it will print the code
 *  associated with the type. if three arguments, 3rd should be a full
 *  name of a file to write the code of type specified to.<br><br>
 *
 *  Registered as: ddefine [type] [file]
 */

int Asc_DispDiffDefineCmd(Asc_DString *hptr,
                                 int argc, CONST84 char *argv[]);
/**<
 *  This function is used mainly by the Library and Display tools for
 *  printing the incremental code associated with the type to the file.
 *  File is a full name of a file to write the code of type specified to.
 *  If no refinement, the code is written. Declarative code only is
 *  considered since the procedures aren't incrementally behaved.<br><br>
 *
 *  Registered as:  ddispdefine <type> [file]
 */

int Asc_DispTypePartsCmd(Asc_DString *hptr,
                                int argc, CONST84 char *argv[]);
/**<
 *  This function returns a list of typenames referenced IN the definition
 *  for the type given. List will be unique and the filter given
 *  (ATOM/MODEL/BOTH) determines the kind of types returned. The
 *  filter may be abbreviated A, M, or B. nonexistent types will
 *  be reported to stderr, but not returned.<br><br>
 *
 *  Registered as:  dgetparts <ATOM,MODEL,BOTH> <type>
 */

int Asc_DispQueryCmd(Asc_DString *hptr,
                            int argc, CONST84 char *argv[]);
/**<
 *  Does all the general query routines for types.
 *  Will always return the empty string in the event of an error.
 *  -- disp define -- returns the list of all defined types.<br><br>
 *
 *  Registered as:  \"disp\"
 */

int Asc_DispHierarchyCmd(Asc_DString *hptr,
                                int argc, CONST84 char *argv[]);
/**<
 *  Returns the hierarchy list of the given type as a valid Tcl list.
 *  Otherwise returns the empty string.<br><br>
 *
 *  Registered as:  hierarchy \"type\"
 */

int Asc_DispFileByTypeCmd(Asc_DString *hptr,
                                 int argc, CONST84 char *argv[]);
/**<
 *  Returns the filename that the given type was defined in. Sufficiently
 *  general enought to be used as is; Will definitely be used with
 *  Asc_DispHierarchyCmd to implement the Library Window ancestry command.<br><br>
 *
 *  Registered as:  file_by_type \"type\"
 */

int Asc_DispChildOneCmd(Asc_DString *hptr,
                               int argc, CONST84 char *argv[]);
/**<
 *  If arg is non-zero returns the specified child. If the specified
 *  child is greater than the number of existing children will return
 *  the last child.
 *  If arg is zero will return the number of children.<br><br>
 *
 *  Registered as:  \"dchild\" arg
 */

int Asc_DispIsRootTypeCmd(Asc_DString *hptr,
                                 int argc, CONST84 char *argv[]);
/**<
 *  Accepts the name of a type and returns 1 if the type is a base type
 *  as defined by the function CheckFundamental. Will also return 1
 *  for models that have no do not refine another.<br><br>
 *
 *  Registered as:  \"disroot_type\"
 */

int Asc_DispRefinesMeCmd(Asc_DString *hptr,
                                int argc, CONST84 char *argv[]);
/**<
 *  Will return ALL the types that refine the given type. I guess this
 *  could be called parents, but I get confused. Will return the empty
 *  string if not refined by any types. This will not be a fast routine.
 *  It could be made faster by setting it up to looke for certain atomic
 *  types.<br><br>
 *
 *  Registered as:  \"drefines_me type\"
 */

int Asc_DispRefinesMeTreeCmd(Asc_DString *hptr,
                                    int argc, CONST84 char *argv[]);
/**<
 *  Return ALL the types that refine the given type in a list-tree format.
 *  Will return the given if not refined by any types. This will not always
 *  be a fast routine (something like o(n^2)).<br><br>
 *
 *  Registered as:  drefinement_tree <type>
 */

#define Asc_DispWriteIpCmd(a,b) (0)
/**<
 *  <!--  extern int Asc_DispWriteIpCmd(Tcl_Interp *, struct Instance *-->);
 *  Out of action at the moment - there is no ip stuff now.
 *  <!--  int Asc_DispWriteIpCmd(hptr,i)                             -->
 *  Returns 1 if something amiss, 0 otherwise.
 *  Appends the contents of the interface pointer of instance to the
 *  hptr as list elements appropriately formatted. Number and format
 *  of list elements depends on the definition of ip in ip.h. If i or
 *  i's ip is NULL, returns 1 and appends nothing to hptr->result.
 *
 *  @todo If ip.h changes, this command should change appropriately.
 */

#endif  /* ASCTK_DISPLAYPROC_HPP */

