/*
 *  UnitsProc.h
 *  by Kirk Abbott and Ben Allan
 *  Created: 1/94
 *  Version: $Revision: 1.11 $
 *  Version control file: $RCSfile: UnitsProc.h,v $
 *  Date last modified: $Date: 2003/08/23 18:43:09 $
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
 *  Units procedures.
 *  <pre>
 *  Display precision is handled here. (see Set/Get_Prec below.)
 *
 *  Base units are the units which correspond to and are displayed
 *  instead of single DIMENSIONs. Numbers which are dimensional but have
 *  no display unit of corresponding dimensionality will be shown in the
 *  appropriate combination of base units, aka fundamental units.
 *  This should keep the number of DIM/units on the internal lists from
 *  getting out of hand when talking about multipliers, derivatives, etc
 *  </pre>
 */

#ifndef ASCTK_UNITSPROC_HPP
#define ASCTK_UNITSPROC_HPP

#if 0
#include <ascend/compiler/instance_enum.h>
#include <ascend/general/list.h>
#include <ascend/compiler/units.h>
#endif


char *Asc_UnitValue( struct Instance *i);
/**<
 *  Return a pointer to a string containing the value of i in display
 *  units/precision provided i is REAL_ATOM_INST, REAL_INST, REL_INST,
 *  INTEGER_INST, INTEGER_ATOM_INST, otherwise returns null.
 *  This string is not yours and the pointer will only be valid until
 *  the next call to Unit_Value.<br><br>
 *
 *  The contents of the string are:  value units<br><br>
 *
 *  where value will be to UPREC places for reals, or all places for int*
 *  and units will be consistent with value. Wild dim'd numbers will
 *  have * for units and dimensionless numbers will have nothing. If
 *  Unit_Value is called on an invalid instance kind, return is NULL.
 *  If instance is !AtomAssigned, return is "UNDEFINED units"
 *  To destroy the current display string, call is Unit_Value(NULL).
 */

char *Asc_UnitlessValue( struct Instance *i, int SI);
/**<
 *  Like Unit_Value, except that contents of the string returned
 *  do not include the units and the printed value in the string
 *  will be in SI instead of display units if SI is TRUE.
 *  If the number cannot be printed in display units, the SI value
 *  is printed instead with a warning on stderr.
 */

char *Asc_UnitString( struct Instance *i, int SI);
/**<
 *  Like Unit_Value, except that contents of the string returned
 *  do not include the value of the instance. If SI is TRUE, units
 *  returned will be SI units rather than units window specified units.
 */

char *Asc_UnitDimString(const dim_type *dimp, int SI);
/**<
 *  Like Unit_String, except that the instance is not needed.
 *  If SI is TRUE, units returned will be SI units rather than units
 *  window specified units.
 *  If dimp is NULL, return is NULL.
 */

int Asc_UnitConvert(struct Units *u, double in, double *out, int direction);
/**<
 *  Attempts to unit convert the value in double to/from the units given.
 *  If args are wrong or conversion fails, return value is 1 and *out
 *  may be undefined.
 *  If successful return is 0 and out has the resulting value.
 *  If direction is 0 convert in{units} to out{si}.
 *  If direction != 0 convert in{si} to out{units}.
 */

int Asc_UnitSetRealAtomValue(CONST struct Instance *i,
                                    char *valuestr,
                                    char *unitstr,
                                    unsigned depth);
/**<
 *  Sets value of a real or real atom instance if units are
 *  dimensionally compatible and value fits within a double in SI units.
 *  If atom is wild, dimensionality will be set by this call unless
 *  the second char argument is null/empty/"*".
 *  Return codes:
 *    - 0 => ok
 *    - 1 => Unparseable units given- Not assigned
 *    - 2 => Dimensionally incompatible units- Not assigned
 *    - 3 => Overflow in converting to SI value- Not assigned
 *    - 4 => Called with non-real instance
 *    - 5 => Unparseable value given- Not assigned
 *  depth is passed on to the SetRealAtomValue call inside this call.
 */

int Asc_UnitGetCPrec(void);
/**<  Return the current display precision for use in C. */

int Asc_UnitDestroyDisplayList(Asc_DString *hptr, int argc, CONST84 char *argv[]);
/**<
 *  Trash the interface units lookup structure. Don't call this if you
 *  aren't about to exit or bad things will happen when next you display.<br><br>
 *
 *  Registered as:  u_destroy_units
 */

int Asc_UnitDefaultBaseUnits(Asc_DString *hptr, int argc, CONST84 char *argv[]);
/**<
 *  Establishes the short form of SI mks as the display
 *  defaults for units. (kg,s,m,K,C,mole,cd,CR)
 *  it needs to be updated if a new dimension is added.
 *  Cares not what args it gets.<br><br>
 *
 *  Registered as:  u_setSIdef
 */

int Asc_UnitGetBaseUnits(Asc_DString *hptr, int argc, CONST84 char *argv[]);
/**<
 *  Returns a list of the current default base units.
 *  If none have been set, calls Unit_default_baseunits before going on.<br><br>
 *
 *  Registered as:  u_getbasedef
 */

int Asc_UnitDump(Asc_DString *hptr, int argc, char CONST84 *argv[]);
/**<
 *  Spew the units defined to stdout, stderr, or a list
 *  if pretty is present.  Option 2 list will have nice whitespace
 *  u_dump 2 --> {dimenId unitname convfactor unitsSI dimen} tuples<br><br>
 *
 *  Registered as:  u_dump <0,1,2> [pretty]
 */

int Asc_DimenDump(Asc_DString *hptr, int argc, CONST84 char *argv[]);
/**<
 *  Spew the dimens registered to stdout, stderr, or a list.<br><br>
 *
 *  Registered as:  u_dims <0,1,2>
 */

int Asc_DimenRelCheck(Asc_DString *hptr, int argc, CONST84 char *argv[]);
/**<
 *  Sets relation dim checking to on (1) or off (0).
 *  Applies only to the checking performed in making unit strings.<br><br>
 *
 *  Registered as:  u_dim_setverify $dimconsistency
 */

int Asc_UnitBaseDimToNum(Asc_DString *hptr, int argc, CONST84 char *argv[]);
/**<
 *  Returns the number of a simple dimension.<br><br>
 *
 *  Registered as:  u_dim2num <dimname>
 */

int Asc_UnitNumToBaseDim(Asc_DString *hptr, int argc, CONST84 char *argv[]);
/**<
 *  Returns the Dim of a corresponding number.<br><br>
 *
 *  Registered as:  u_num2dim <dimindex>
 */

int Asc_UnitMatchBaseDim(Asc_DString *hptr, int argc, CONST84 char *argv[]);
/**<
 *  Returns all the unit names which match the dimension number given.
 *  unit names will be in order of decreasing conversion factor
 *  and increasing length and alphabetically
 *  A bit more efficient than using UnitMatch_AtomDim.<br><br>
 *
 *  Registered as:  u_frombasedim <dimindex>
 */

int Asc_UnitMatchAtomDim(Asc_DString *hptr, int argc, CONST84 char *argv[]);
/**<
 *  Returns all the unit names which match the dimension set of
 *  given real atom type, sorted as in Match_BaseDim.<br><br>
 *
 *  Registered as:  u_fromatomdim <atomname>
 */

int Asc_UnitGetAtomList(Asc_DString *hptr, int argc, CONST84 char *argv[]);
/**<
 *  Returns a list of dimensioned atoms and dimensionalities.
 *  Wild and dimensionless are not considered dimensioned.<br><br>
 *
 *  Registered as:  u_getdimatoms
 */

int Asc_UnitChangeBaseUnit(Asc_DString *hptr, int argc, CONST84 char *argv[]);
/**<
 *  Change the default display unit of the dimension implied by unit to
 *  be unit. Will return an error if unit does not have a simple
 *  dimensionality.
 *  Note: This will not catch semantic errors with dimensionless * dimd<br><br>
 *
 *  Registered as:  u_change_baseunit <unit>
 */

int Asc_UnitSetUser(Asc_DString *hptr, int argc, CONST84 char *argv[]);
/**<
 *  Set the user specified units for the dimensionality they imply.
 *  Note: This will not catch semantic errors with dimensionless * dimd.<br><br>
 *
 *  Registered as:  u_set_user <units>
 */

int Asc_UnitGetAtomsForUnit(Asc_DString *hptr, int argc, CONST84 char *argv[]);
/**<
 * Returns the list of atoms and constants matching the units
 * given, if any.<br><br>
 *
 * Registered as:  u_get_atoms <units>
 */

int Asc_UnitGetPrec(Asc_DString *hptr, int argc, CONST84 char *argv[]);
/**<
 *  Gets the display value precision, principally useful for reals.<br><br>
 *
 *  Registered as:  u_getprec
 */

int Asc_UnitSetPrec(Asc_DString *hptr, int argc, CONST84 char *argv[]);
/**<
 *  Sets the display value precision, principally useful for reals.<br><br>
 *
 *  Registered as:  u_setprec <number 4-16>
 *
 *  @todo Asc_UnitSetPrec() should use ansi prec info instead of 16 upper limit.
 */

int Asc_UnitGetUnits(Asc_DString *hptr, int argc, CONST84 char *argv[]);
/**<
 *  Returns user set (or if never set, then default set) units for dims
 *  of atom type given.<br><br>
 *
 *  Registered as:  u_get_units <atomname>
 */

int Asc_UnitGetUser(Asc_DString *hptr, int argc, CONST84 char *argv[]);
/**<
 *  Returns user set units for dims of atom given or "default" if unset.<br><br>
 *
 *  Registered as:  u_get_user <atomname>
 */

int Asc_UnitGetList(Asc_DString *hptr, int argc, CONST84 char *argv[]);
/**<
 *  Returns user set units for all dims DUList.
 *  Units of defaulted dims not returned.<br><br>
 *
 *  Registered as:  u_get_list
 */

int Asc_UnitClearUser(Asc_DString *hptr, int argc, CONST84 char *argv[]);
/**<
 *  Unsets user set units for dims of atom given.<br><br>
 *
 *  Registered as:  u_clear_user <atomname>
 */

int Asc_UnitGetVal(Asc_DString *hptr, int argc, CONST84 char *argv[]);
/**<
 *  Instance indicated by qlfdid. In general, this is expensive.<br><br>
 *
 *  Registered as:  u_getval <qlfdid> returns as Asc_UnitValue if possible from the
 */

int Asc_UnitBrowGetVal(Asc_DString *hptr, int argc, CONST84 char *argv[]);
/**<
 *  Instance indicated by g_curinst (g_searchinst if "search" given).<br><br>
 *
 *  Registered as:  u_browgetval [search] returns {value} {units} if possible from the
 */

int Asc_UnitSlvGetRelVal(Asc_DString *hptr, int argc, CONST84 char *argv[]);
/**<
 *  Returns as Asc_UnitValue if possible from the relation
 *  indicated by index.<br><br>
 *
 *  Registered as:  u_slvgetrelval <rellist index>
 */

int Asc_UnitSlvGetVarVal(Asc_DString *hptr, int argc, CONST84 char *argv[]);
/**<
 *  Returns as Asc_UnitValue if possible from the variable
 *  indicated by index.<br><br>
 *
 *  Registered as:  u_slvgetvarval <varlist index>
 */

int Asc_UnitSlvGetObjVal(Asc_DString *hptr, int argc, CONST84 char *argv[]);
/**<
 *  Returns as Asc_UnitValue if possible from the obj indicated
 *  by index. Until there is an objlist, this will simply look
 *  at obj and ignore index.
 *
 *  Registered as:  u_slvgetobjval <objlist index>
 */

int Asc_UnitHelpList(Asc_DString *hptr, int argc, CONST84 char *argv[]);
/**<
 *  uhelp command for tcl.
 *  no arg -> return tcl list
 *  "s" -> list names only, "l" -> short explanations also, to stderr.<br><br>
 *
 *  Registered as:  uhelp [s,l]
 */

private:
int check_units_set(Asc_DString *hptr, int argc, CONST84 char *argv[]);
static int Unit_CmpDU(CONST struct DisplayUnit *du1, CONST struct DisplayUnit *du2);
static int check_DU_set();
static int destroy_DUList();
static struct DisplayUnit *Unit_FindOrAddDU(const dim_type *dimp);
struct Units *Unit_DisplayUnits(const dim_type *dimp);
static struct Units *Unit_DisplayFund(const dim_type *dimp);
struct Units *Unit_DisplaySI(const dim_type *dimp);
static void Unit_UpdateFundUnits(struct DisplayUnit *du);
int Unit_UnconvertReal(double val, struct Units *u, double *retval);
int Unit_ConvertReal(double val, struct Units *u, double *retval);
int Unit_PrintUndefined(dim_type *dimp);
int Unit_PrintReal(double val, dim_type *dimp);
int Unitless_PrintReal(double val, dim_type *dimp, int si);
int Unit_PrintInteger(long val, dim_type *dimp);
int Unitless_PrintUndefined();
static int Unit_CmpAtomName(CONST struct TypeDescription *d1, CONST struct TypeDescription *d2);
static void Unit_WriteNumer(Asc_DString *str, struct fraction frac, CONST char *baseunit, int *CONST p);
static void Unit_WriteDenom(Asc_DString *str, struct fraction frac, CONST char *baseunit, int *CONST p);
static int Unit_CmpConv(CONST struct Units *u1, CONST struct Units *u2);
static char *Unit_MakeString(const dim_type *dimp, struct Units * units[NUM_DIMENS]);
void Unit_PrintDU(struct DisplayUnit *du);
dim_type *Unit_FindRelDim(struct Instance *i);
static void Unit_GetUserSet(struct DisplayUnit *du);

static int updatefundunitdim;
//static Asc_DString *unitshptr;

static struct Units * g_base_units[NUM_DIMENS];
/**< These are the working base units. */

static struct Units * g_SI_units[NUM_DIMENS];
/**<
 *  These are the backup base units. If a number cannot be displayed
 *  due to floating point error in conversion, it will be displayed in
 *  these units (all of which have conversion factors of 1.)
 */

public:

#endif  /* ASCTK_UNITSPROC_HPP */

