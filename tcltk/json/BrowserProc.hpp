/*
 *  BrowserProc.h
 *  by Kirk Abbott and Ben Allan
 *  Created: 1/94
 *  Version: $Revision: 1.9 $
 *  Version control file: $RCSfile: BrowserProc.h,v $
 *  Date last modified: $Date: 2003/08/23 18:43:04 $
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

#ifndef ASCTK_BROWSERPROC_HPP
#define ASCTK_BROWSERPROC_HPP

int Asc_BrowRootInitCmd(Asc_DString *hptr, int argc, CONST84 char *argv[]);
/**<
 *  <!--  Asc_BrowRootInitCmd                                          -->
 *  This should set initialize the root.<br><br>
 *  Registered as:  rootinit
 */

int Asc_BrowRootCmd(Asc_DString *hptr, int argc, CONST84 char *argv[]);
/**<
 *  This should set the root.<br><br>
 *  Registered as:  root $arg$
 */

int Asc_BrowRootBackupCmd(Asc_DString *hptr, int argc, CONST84 char *argv[]);
/**<
 *  This should backup to the old root.<br><br>
 *  Registered as:  oldinst -- will be supseded by the general inst routines.
 */

int Asc_BrowRootNCmd(Asc_DString *hptr, int argc, CONST84 char *argv[]);
/**<
 *  This should set the root. Uses direct addressing. The instance must
 *  already exist in the instance query list. For this to be used.<br><br>
 *  Registered as:  rootn $arg$
 */

int Asc_BrowTransferCmd(Asc_DString *hptr, int argc, CONST84 char *argv[]);
/**<
 *  Used to transfer from a search instance to the maim browser. One of
 *  the main steps in exportin to the browser from any window. The sims
 *  export to browser is a easier case and can be handled through rootinit.<br><br>
 *  Registered as:  btransfer
 */

int Asc_BrowSimListCmd(Asc_DString *hptr, int argc, CONST84 char *argv[]);
/**<
 *  Should list all the active simulations.<br><br>
 *  Registered as:  slist
 */

int Asc_BrowSimTypeCmd(Asc_DString *hptr, int argc, CONST84 char *argv[]);
/**<
 *  Returns the type of a simulation given the name of the simulation.
 *  Will return an error message if the simlist is null or the name not
 *  found.<br><br>
 *  Registered as:  simtype
 */

int Asc_BrowInstStatCmd(Asc_DString *hptr, int argc, CONST84 char *argv[]);
/**<
 *  Print instance tree stats to stdout, a la Tom.<br><br>
 *  Registered as:  bstatistics
 */

int Asc_BrowInstListCmd(Asc_DString *hptr, int argc, CONST84 char *argv[]);
/**<
 *  Should list all the working instances.<br><br>
 *  Registered as:  ilist
 */

int Asc_BrowPrintCmd(Asc_DString *hptr, int argc, CONST84 char *argv[]);
/**<
 *  Should print the current instance or the simulation list if no
 *  arguements are given.<br><br>
 *  Registered as:  bprint
 */

int Asc_BrowInstQueryCmd(Asc_DString *hptr, int argc, CONST84 char *argv[]);
/**<
 *  <!--  Asc_BrowInstQueryCmd                                         -->
 *  Returns the name, type, previous instance, # of children, # of parents,
 *  child list, parents list, and  if parent atomic, respectively.
 *  It also returns the appropriate value (TRUE or FALSE) fo the queries:
 *  is the instance assignable ? fixable ? mutable ? constant ?
 *  is it in the list of variables of a WHEN statement ?
 *  If current instance is null, returns NULL_INSTANCE and HELP_ERROR.
 *  Takes the search instance also. if no third argument given, uses
 *  current.<br><br>
 *
 *  Registered as:  inst querykind [cur,search];
 *  arg : name, type, old, nchild, nparents, child, parent,
 *        isassignable, isfixable, ismutable, isconstant,
 *        iswhenvar, atomchild;
 */

int Asc_BrowRunInitializeCmd(Asc_DString *hptr, int argc, CONST84 char *argv[]);
/**<
 *  <!--  Asc_BrowRunInitializeCmd                                     -->
 *  Will take the current instance and run the named procedure associated
 *  with it. Returns an HELP_OK or HELP_ERROR status flag.<br><br>
 *  Registered as:  \"runproc\" $name$
 */

int Asc_BrowInstanceMergeCmd(Asc_DString *hptr, int argc, CONST84 char *argv[]);
/**<
 *  <!--  Asc_BrowInstanceMergeCmd                                     -->
 *  NOTE WELL -- this function will attempt to merge the current instance
 *  g_curinst and the search instance g_search_inst. Hence to set up
 *  g_search_inst appropriately, call Asc_BrowQlfdidSeachCmd() FIRST.<br><br>
 *  Registered as:  \"bmerge\" <noargs>
 */

int Asc_BrowInstanceRefineCmd(Asc_DString *hptr, int argc, CONST84 char *argv[]);
/**<
 *  <!--  Asc_BrowInstanceRefineCmd                                    -->
 *  Will take the current or search instance as specified and will try
 *  to refine it to the specified type. Will return HELP_OK if all works
 *  and a spew of HELP_ERRORs if things are not perfect. Note this function
 *  makes use of RefineClique in instance.c and hence refines all the
 *  members of the clique.<br><br>
 *  Registered as:  \"brefine\" ?current?search? type;
 */

int Asc_BrowMakeAlikeCmd(Asc_DString *hptr, int argc, CONST84 char *argv[]);
/**<
 *  <!--  int Asc_BrowMakeAlikeCmd;                                    -->
 *  Will make two instances ARE_ALIKE.
 *  Operates on the current and the search instances.<br><br>
 *  Registered as:  \"bmakealike\" current search.
 */

/*
 *  Other useful queries exported for general use.
 */

int Asc_BrowAnonTypesCmd(Asc_DString *hptr, int argc, CONST84 char** argv);

private:

int Asc_BrowInstIsAtomic(struct Instance *i);
/**<
 *  <!--  int Asc_BrowInstIsAtomic(i);                                 -->
 *  Returns true if the instance is one of the atom types. It will fiil
 *  for relations, as at the moment they are not strictly considered
 *  as being atomic.
 */

int Asc_BrowInstIsSubAtomic(struct Instance *i);
/**<
 *  <!--  int Asc_BrowInstIsSubAtomic(i);                              -->
 *  Returns true if the instance is a child of one of the atom types.
 *  i.e, sub-atomic :-)
 */

int Asc_BrowInstIsConstant(struct Instance *i);
/**<
 *  <!--  int Asc_BrowInstIsConstant(i);                               -->
 *  Returns true if the instance is a constant.
 */

int Asc_BrowInstIsMutable(struct Instance *i);
/**<
 *  <!--  int Asc_BrowInstIsMutable(i);                                -->
 *  Returns true if the instance is mutable. The instance must have the
 *  notion of a *value*. Otherwise returns false.
 */

void InitInstList(void);
int BrowRootInit(char*);
int BrowTransfer(struct gl_list_t *search_list);
int Asc_BrowInstAtomValue(Asc_DString *hptr, struct Instance *i);

public:

#endif /* ASCTK_BROWSERPROC_HPP */

