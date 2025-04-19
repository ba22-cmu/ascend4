/*
 *  BrowserQuery.h
 *  by Kirk Abbott and Ben Allan
 *  Created: 1/94
 *  Version: $Revision: 1.17 $
 *  Version control file: $RCSfile: BrowserQuery.h,v $
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

/** @file
 *  Browser Query Routines
 *  <pre>
 *  To include this header, you must include the following:
 *      #include "tcl.h"
 *      #include "utilities/ascConfig.h"
 *      #include "interface/BrowserQuery.h"
 *  </pre>
 */

#ifndef ASCTK_BROWSERQUERY_HPP
#define ASCTK_BROWSERQUERY_HPP
#if 0
#include <ascend/compiler/dimen.h>
#include <ascend/compiler/instance_enum.h>
#endif

int Asc_BrowIsRelationCmd(Asc_DString *hptr, int argc, CONST84 char *argv[]);
/**<
 *  Will return 1, if the the instance in question is of relation type
 *  of is an array, of array ... of relation. The normal InstanceType
 *  routines return the empty string for array of relations, and it is
 *  sometimes handy to know apriori what the type of the array is.<br><br>
 *
 *  Registered as:   __brow_isrelation  ?current?search?.
 */

int Asc_BrowIsLogRelCmd(Asc_DString *hptr, int argc, CONST84 char *argv[]);
/**<
 *  Will return 1, if the the instance in question is of logical relation
 *  type of is an array, of array ... of logical relation. The normal
 *  InstanceType routines return the empty string for array of logical
 *  relations, and it is sometimes handy to know apriori what the type of
 *  the array is.<br><br>
 *
 *  Registered as:   __brow_islogrel  ?current?search?.
 */

int Asc_BrowIsWhenCmd(Asc_DString *hptr,
                             int argc, CONST84 char *argv[]);
/**<
 *  Will return 1, if the the instance in question is of when
 *  type of is an array, of array ... of when. The normal
 *  InstanceType routines return the empty string for array of whens,
 *  and it is sometimes handy to know apriori what the type of
 *  the array is.<br><br>
 *
 *  Registered as:   __brow_iswhen  ?current?search?.
 */

int Asc_BrowIsInstanceInWhenCmd(Asc_DString *hptr, int argc, CONST84 char *argv[]);
/**<
 *  Will return 1, if the the instance in question may be in the list
 *  of variables or in some CASE of a WHEN Statement: boolean, integer,
 *  symbol, relations.<br><br>
 *
 *  Registered as:   __brow_isinstanceinwhen  ?current?search?.
 */

int Asc_BrowIsModelCmd(Asc_DString *hptr, int argc, CONST84 char *argv[]);
/**<
 *  Will return 1, if the the instance in question is of model type
 *  of is an array, of array ... of model. The normal InstanceType
 *  routines return the empty string for array of models, and it is
 *  sometimes handy to know apriori what the type of the array is.<br><br>
 *
 *  Registered as:   __brow_ismodel  ?current?search?.
 */

struct gl_list_t *Asc_BrowShortestPath(CONST struct Instance *i,
                                              CONST struct Instance *ref,
                                              unsigned int, unsigned int);
/**<
 *  Returns the shortest path (instance pointers) from i back to ref.
 *  If ref NULL, goes to top of instance tree.
 *  <!--  Usage: path = Asc_BrowShortestPath(i,ref,0,UINT_MAX);        -->
 *  The search is recursive on the unsigned arguments.
 */

int Asc_BrowWriteNameRec(char *fname, CONST struct InstanceName *rec);
/**<
 *  Write the string in the given rec into the fname buffer in the
 *  ascend interface format. fname is assumed big enough.
 *  This call being exported as very handy elsewhere, e.g. solver. baa
 */

int Asc_BrowWriteAtomValue(char *ftorv, CONST struct Instance *i);
/**<
 *  Assumes ftorv is big enough and writes some value appropriate for
 *  the instance i into the string ftorv.
 *  This should be done with a DString in the compiler.
 */

int Asc_BrowWriteInstanceNameCmd(Asc_DString *hptr, int argc, CONST84 char *argv[]);
/**<
 *  Print the instance's name to the specified file. The shortest path to
 *  root is always printed. For the purposes of interface1, root is always
 *  the 'first' instance of a simulation (aka as the simulation base ptr)
 *  or NULL. Will accept the symbolic name of a pointer in the form of
 *  current of search as the basis of its queries.
 *
 *  Registered as \"iname inst\" - no args"
 */

int Asc_BrowCountNamesCmd(Asc_DString *hptr, int argc, CONST84 char *argv[]);
/**<
 *  Returns the counts of ALIASES for all instances in the entire subtree
 *  given current or search.
 *  returned is a list (with labels) of:
 *   - total unique instances counted.
 *   - total ATOM-like instances counted (ATOM, constant,set)
 *   - total names of these atoms.
 *   - total creating names of these atoms.
 *   - total relation-like instances (relation, logical, when)
 *   - total names of these relations.
 *   - total creating names of these relations.
 *   - total MODEL instances counted.
 *   - total names of these models.
 *   - total creating names of these models.
 *   - total array instances counted.
 *   - total names of these arrays.
 *   - total creating names of these arrays.
 *
 *  Registered as "count_names"
 */

int Asc_BrowWriteAliasesCmd(Asc_DString *hptr, int argc, CONST84 char *argv[]);
/**<
 *  Print the instance's names to the hptrreter.
 *
 *  Registered as "ALIASES"
 */

int Asc_BrowWriteISAsCmd(Asc_DString *hptr, int argc, CONST84 char *argv[]);
/**<
 *  Print the instance's construction names to the hptrreter.
 *
 *  Registered as \"isas\"
 */

int Asc_BrowWriteCliqueCmd(Asc_DString *hptr, int argc, CONST84 char *argv[]);
/**<
 *  Print all the instance's clique members.
 *
 *  Registered as \"cliques\"
 */

int Asc_BrowWriteInstanceCmd(Asc_DString *hptr, int argc, CONST84 char *argv[]);
/**<
 */

int Asc_BrowWriteAtomChildren(Asc_DString *hptr, CONST struct Instance *i);
/**<
 *  Append the children of an atom (with values in display units) to
 *  the hptr->result as list elements.
 */

int Asc_BrowWriteDimensions(char *fdims, CONST dim_type *dimp);
/**<
 *  Nearly the string equivalent of PrintDimen in dimen.c, except:
 *   -# this one writes in proper dimensions
 *   -# fdims has the dimensionality of dimp _appended_ to it, except as d
 *   -# fdims is assumed long enough.
 *   -# if dimensionality is wild or none, a * overwrites the string.
 */

int Asc_BrowIsPlotAllowedCmd(Asc_DString *hptr, int argc, CONST84 char *argv[]);
/**<
 *  Will accept the current of search instance and determine if the type
 *  of the instance is 'plottable'. The current implementation makes use
 *  of the type `plt_plot' as the basis of this decision. Will return 1
 *  if TRUE, 0 otherwise. This function should not exist.<br><br>
 *
 *  Registered as :  \"b_isplottable ?cur?search?.
 */

int Asc_BrowPreparePlotFileCmd(Asc_DString *hptr, int argc, CONST84 char *argv[]);
/**<
 *  Will operate on the current or the search instance to produce a file
 *  that may be plotted by the existing plotting programs. Requires a
 *  filename arguement and an optional plot_type specifier
 *  PLAIN_PLOT, GNU_PLOT, XGRAPH_PLOT.<br><br>
 *
 *  Registered as : \"b_prepplotfile\" inst filename type.
 */

int Asc_BrowRefinesMeCmd(Asc_DString *hptr, int argc, CONST84 char *argv[]);
/**<
 *  Will return ALL the library types that refine the current instance.
 *  If instance typedesc is out of sync with library, returns none.
 *  Will return the empty string if not refined by any types.<br><br>
 *
 *  Registered as : \"irefines_me\";
 */

int Asc_BrowWriteValues(Asc_DString *hptr, int argc, CONST84 char *argv[]);
/**<
 *  Write out the real and boolean values of an instance tree starting
 *  at the instance indicated to a file filname. current/root refer to
 *  browser. qlfdid is a full id. Only the first letter of the last arg
 *  is significant.<br><br>
 *  acmd is the string that will be prefixed to the qlfdid value pair,
 *  eg. bwritevalues /tmp/val.txt "ASSIGN {sim" current is one way to write
 *  the values, using the script ASSIGN command. acmd could also be
 *  qassign {sim. As neither of these is the obvious choice in all circum-
 *  stances, the command allows choice.
 *  note that the simulation name should be part of acmd and an open
 *  brace should be provided because a close brace after the name will
 *  be printed.
 */

int Asc_BrowFindTypeCmd(Asc_DString *hptr, int argc, CONST84 char *argv[]);
/**<
 *  Implementation of find by type. More will be said later.
 *  <pre>
 *  Old Usage : __brow_find_type type attribute ?value?
 *  New usage:
 *  __brow_find_type <current,search> <typename> [attr [loval [hival]]]
 *
 *  The Tcl window corresponding to this function has 4 boxes to fill.
 *  Type, Atributtes, Low Value and High Value.
 *
 *  There is nothing about this function from Kirk. After looking trough
 *  the code, I can make the following remarks.
 *
 *  1) The default context for finding by type is the instance currently
 *      highlighted.
 *  2) Obviously, the type one wants to find has to be provided in the first
 *      box of the Tcl window.
 *  3) For each type of instance, there exists a filtering function
 *      which will perform according to the values given in the other
 *      3 boxes. When the type one provides is a Model or an
 *      array, the filtering functions of these types do nothing.
 *  4) The box Attributes correponds to the children of the type
 *      selected. So, for example, "included" is a valid attribute of
 *      a relation.
 *  5) When the Attribute is provided, the value of the
 *      attribute should also be provided in the box "Low Value", if
 *      applicable. For example, for the attribute "included" a valid
 *      value is TRUE (or FALSE). In this way, one can ask for
 *      specific relations among the total list.
 *  6) There are two special attributes, which do not correpond to
 *      children of an instance: "VALUE" and "UNDEFINED".
 *      i)For example, if the type requested is a boolean, we can
 *        use the attribute VALUE for finding booleans with a particular
 *        value. The value selected should be provided in the box "Lower
 *        Value". This capability can be used for boolean, integer,
 *        symbol, relation, and logic_relation.
 *        Similarly, "UNDEFINED" is used when the value of
 *        a symbol, integer or real has not been defined. In that case
 *        the last two boxes are left empty.
 *     ii)Specifically for the type "relation", the Attribute "VALUE" is
 *        associated with the residual of the relation. We can specify
 *        the range of the residuals we are interesting in, by using the
 *        last two boxes.
 *
 *        My modification to this function is an extension to deal with
 *        logical relations, and whens. For conditional relations I did
 *        nothing. They will show up in the list of relations. For whens,
 *        I do the same as the case of models and arrays. For Logrelations,
 *        the filtering can be done in terms of the logical residual.
 *        Note that for this case, Feasibles Lower Value for the attribute
 *        "VALUE" are  "TRUE" and "FALSE".
 *
 *        VRR
 *  </pre>
 */

int Asc_BrowRelationRelopCmd(Asc_DString *hptr, int argc, CONST84 char *argv[]);
/**<
 *  Returns the a string describing the type of the relation. Valid
 *  results are:
 *  equal, notequal, less, greater, maximize, minimize.
 */

int Asc_BrowClearVarsCmd(Asc_DString *hptr, int argc, CONST84 char *argv[]);
/**<
 *  Sets fixed flag of all variables to FALSE in Browser current instance,or if
 *  qlfdid given, in the qlfdid instance.
 */

private:
int BrowWriteInstance(Asc_DString *hptr, struct Instance *i,
                      int show_child_atoms, int show_passed_parts);
int BrowIsRelation(struct Instance *i);
int BrowIsLogRel(struct Instance *i);
int BrowIsWhen(struct Instance *i);
int BrowIsInstanceInWhen(struct Instance *i);
int BrowIsModel(struct Instance *i);
int BrowIsAtomicArray(struct Instance *i);
static void CountNames(struct Instance *i);
int BrowWriteInstSet(char *ftorv, CONST struct set_t *s);
int BrowWriteFrac(char *fdims, struct fraction frac, CONST char *str, int *CONST p);
int BrowWriteTypeOrValue(char *ftorv, CONST struct Instance *parent, CONST struct Instance *child, unsigned long cnum);
int BrowWriteArrayChildren(Asc_DString *hptr, CONST struct Instance *i);
int BrowTypeOfArrayIsShown(struct Instance *child);
void BrowListModelChildren(Asc_DString *hptr, struct Instance *i, int atoms, int show_passed_parts);
static void BrowWriteRBValues(struct Instance *i);
static void BrowWriteRBValues2(struct Instance *i);
struct Instance *Brow_MatchAttr(struct Instance *i, symchar *attr_desc);
int Special_AttrMatch(int argc, char **argv);
struct Instance *FilterModels(struct Instance *i, int argc, char **argv);
struct Instance *FilterReals(struct Instance *i, int argc, char **argv);
struct Instance *FilterBooleans(struct Instance *i, int argc, char **argv);
struct Instance *FilterIntegers(struct Instance *i, int argc, char **argv);
struct Instance *FilterSymbols(struct Instance *i, int argc, char **argv);
struct Instance *FilterLogRelations(struct Instance *i, int argc, char **argv);
struct Instance *FilterWhens(struct Instance *i, int , char **);
struct Instance *FilterRelations(struct Instance *i, int argc, char **argv);
struct Instance *FilterSets(struct Instance *i, int argc, char **argv);
struct gl_list_t *Brow_FilterFindList(struct gl_list_t *list, int argc, char **argv);
static void Brow_MatchType(struct Instance *i);
struct gl_list_t *BrowFindTypeList(struct Instance *i, int argc, char **argv);
int BrowLogRelRelopCmd(Asc_DString *hptr, int argc, CONST84 char *argv[]);
public:

#endif /* ASCTK_BROWSERQUERY_HPP */

