/*
 *  BrowserMethod.c
 *  by Kirk Abbott and Ben Allan
 *  Created: 1/94
 *  Version: $Revision: 1.24 $
 *  Version control file: $RCSfile: BrowserMethod.c,v $
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

#define ASC_BUILDING_INTERFACE

#define MAXID 256

/*
 */
STDHLF(Asc_BrowInitializeCmd,(Asc_BrowInitializeCmdHL,HLFSTOP));
int ascjson::Asc_BrowInitializeCmd(Asc_DString *hptr,
                         int argc, CONST84 char *argv[])
{
  int status;
  struct Name *name=NULL;
  enum Proc_enum runstat;
  int options=0;
  CONST84 char *qlfdid=NULL;
  struct Instance *i = NULL;
  CONST84 char *stoponerr = NULL;
  CONST84 char *btuifstop = NULL;
  FILE *output=NULL;
  CONST84 char *method=NULL;
  CONST84 char *filename=NULL;
  int k,tmp=0;

  if (argc < 2) {
    /* put help message here */
    Asc_DStringSet(hptr, "wrong # args: Usage: " Asc_BrowInitializeCmdHU);
    return HELP_ERROR;
  }
  output = ASCERR;
  /* this is safe because argv[argc]==NULL by convention */
  for (k=1; k < argc;) {
    if (strcmp(argv[k],"-stopOnErr")==0) {
      stoponerr = argv[k+1];
      k += 2;
      continue;
    }
    if (strcmp(argv[k],"-backtrace")==0) {
      btuifstop = argv[k+1];
      k += 2;
      continue;
    }
    if (strcmp(argv[k],"-method")==0) {
      method = argv[k+1];
      k += 2;
      continue;
    }
    if (strcmp(argv[k],"-qlfdid")==0) {
      qlfdid = argv[k+1];
      k += 2;
      continue;
    }
    if (strcmp(argv[k],"-output")==0) {
      filename = argv[k+1];
      k += 2;
      continue;
    }
    Asc_DStringAppend4(hptr,"Unknown option '",argv[k],"' to ",
                     Asc_BrowInitializeCmdHN, HALL);
    return HELP_ERROR;
  }
  status = Asc_QlfdidSearch3(qlfdid,0); /* does check for NULL, yes? */
  if (status==0) { 		/* catch inst ptr */
    i = g_search_inst;
  } else { 				/* failed. bail out. */
    Asc_DStringAppend3(hptr,Asc_BrowInitializeCmdHN,
                     "Could not find instance ",qlfdid,HALL);
    return HELP_ERROR;
  }
  if (btuifstop != NULL) {
    status = JTcl_GetInt(hptr,btuifstop,&tmp);
    if (status != HELP_OK || tmp < 0 || tmp > 1) {
      Asc_DStringAppend6(hptr,"Non-boolean value (",btuifstop,") given for ",
                       argv[0]," ","-backtrace",HALL);
      return HELP_ERROR;
    }
    if (tmp) {
      options |= WP_BTUIFSTOP;
    }
  }
  if (stoponerr != NULL) {
    status = JTcl_GetInt(hptr,stoponerr,&tmp);
    if (status != HELP_OK || tmp < 0 || tmp > 1) {
      Asc_DStringAppend6(hptr,"Non-boolean value (",stoponerr,") given for ",
                       argv[0]," ","-stopOnErr",HALL);
      return HELP_ERROR;
    }
    if (tmp) {
      options |= WP_STOPONERR;
    }
  }
  if (method != NULL) {
    name = CreateIdName(AddSymbol(method));
  } else {
    Asc_DStringAppend2(hptr,Asc_BrowInitializeCmdHN,
                     "-method <method name> not given",HALL);
    return HELP_ERROR;
  }
  if (filename != NULL) {
    output = fopen(filename,"w+");
    if (output == NULL) {
      Asc_DStringAppend4(hptr,Asc_BrowInitializeCmdHN,
                       "-output ",filename,
                       " cannot open/write",HALL);
      DestroyName(name);
      return HELP_ERROR;
    }
  }
  runstat = Initialize(i,name,QUIET(qlfdid),output,options,NULL,NULL);
  if (filename != NULL && output != ASCERR) {
    fclose(output);
  }
  DestroyName(name);
  if (runstat != Proc_all_ok) {
    Asc_DStringAppend4(hptr, "Error executing method ",method,
                     " in ",qlfdid,HALL);
    return HELP_ERROR;
  }
  return HELP_OK;
}

static void lowerstring(char *str)
{
  while (*str != '\0') {
    if ((*str >= 'A')&&(*str <= 'Z')) {
      *str = *str + ('a' - 'A');
    }
    str++;
  }
}


int ascjson::BrowDoAssignment(Asc_DString *hptr,struct Instance *i,
                     char *value_str, char *unit_str)
{
  char buffer[MAXID], *tmps;
  symchar *sym;
  int code = 0;
  switch(InstanceKind(i)) {
  case REAL_ATOM_INST:
  case REAL_INST:
  case REAL_CONSTANT_INST:
    code = Asc_UnitSetRealAtomValueDS(i,value_str,unit_str,0);
    switch (code) {
    case 0:
      break;
    case 1:
      Asc_DStringSet(hptr, "Unparseable units given - Not assigned.");
      return HELP_ERROR;
    case 2:
      Asc_DStringSet(hptr, "Dimensionally incompatible units - Not assigned.");
      return HELP_ERROR;
    case 3:
      Asc_DStringSet(hptr,"Overflow in converting to SI value--Not assigned.");
      return HELP_ERROR;
    case 5:
      Asc_DStringSet(hptr, "Unparseable value given - Not assigned.");
      return HELP_ERROR;
    default:
      return HELP_ERROR;
    }
    break;
  case BOOLEAN_ATOM_INST:
  case BOOLEAN_INST:
  case BOOLEAN_CONSTANT_INST:
    tmps = strcpy(buffer,value_str);
    lowerstring(tmps);
    if(strcmp(tmps,"true")==0 || strcmp(tmps,"1")==0 || strcmp(tmps,"yes")==0){
      SetBooleanAtomValue(i,1,0);
    } else if (strcmp(tmps,"false")==0
               || strcmp(tmps,"0")==0
               || strcmp(tmps,"no")==0) {
      SetBooleanAtomValue(i,0,0);
    } else {
      Asc_DStringSet(hptr, "Incorrect boolean value");
      return HELP_ERROR;
    }
    break;
  case INTEGER_ATOM_INST:
  case INTEGER_INST:
  case INTEGER_CONSTANT_INST:
    if (AtomMutable(i) || !AtomAssigned(i)) {
      SetIntegerAtomValue(i,atol(value_str),0);
    } else {
      Asc_DStringSet(hptr, "Attempting to assign to an immutable integer");
      return HELP_ERROR;
    }
    break;
  case SYMBOL_ATOM_INST:
  case SYMBOL_INST:
    /* the symtab in symtab.c owns the string.
     * an instance only refers to it.
     */
    sym = AddSymbol(value_str); /* this will copy the string */
    SetSymbolAtomValue(i,sym);
    break;
  case SYMBOL_CONSTANT_INST:
    /* the symtab in symtab.c owns the string.
     * an instance only refers to it.
     */
    if (!AtomAssigned(i)) {
      sym = AddSymbol(value_str); /* this will copy the string */
      SetSymbolAtomValue(i,sym);
    }
    break;
  case SET_ATOM_INST:
  case SET_INST:
    /* not yet supported */
    break;
  default:
    Asc_DStringSet(hptr, "The argument to assign is not a atom");
    return HELP_ERROR;
  }
  return HELP_OK;
}

#if 0 // DISUSED
/*
 * This function should probably go away. !!.
 * We will at the next iteration.
 */
int Asc_BrowRunAssignQlfdidCmd2(Asc_DString *hptr,
                            int argc, CONST84 char *argv[])
{
  struct Instance *i;
  CONST84 char *value_str = NULL;
  CONST84 char *unit_str = NULL;
  int nok;

  if (( argc < 3 ) || ( argc > 4 )) {
    Asc_DStringAppend(hptr,"wrong # args: "
                     "Usage: \"qassgn2\" qlfdid value [units]",HALL);
    return HELP_ERROR;
  }
  nok = Asc_QlfdidSearch2(QUIET(argv[1]));
  if (nok) { /* failed. bail out. */
    Asc_DStringAppend(hptr," : Error -- Name not found",(char *)NULL);
    return HELP_ERROR;
  }
  i = g_search_inst;	/* catch inst ptr found in QlfdidSearch */
  value_str = argv[2];
  if ( argc == 4 ) {
    unit_str = argv[3];
  }
  if (strcmp("UNDEFINED",value_str)==0) {
    return HELP_OK;
  }
  nok = BrowDoAssignment(hptr,i,QUIET(value_str),QUIET(unit_str));
  return nok;	/* whatever code returned by BrowDoAssignment */
}
#endif

int ascjson::Asc_BrowRunAssignQlfdidCmd3(Asc_DString *hptr,
                            int argc, CONST84 char *argv[])
{
  struct Instance *i;
  char *value_str = NULL;
  char *unit_str = NULL;
  int nok;
  int relative = 0;

  if (( argc < 3 ) || ( argc > 5 )) {
    Asc_DStringAppend(hptr,"wrong # args: "
		     "Usage: \"qassgn3\" qlfdid value [units] [-relative]",
		     HALL);
    return HELP_ERROR;
  }
/* reading args out of order to get relative flag sorted out */
  if ( argc == 4 ) {
    if (strcmp("-relative",argv[3])==0) {
      relative = 1;
    } else {
      unit_str = QUIET(argv[3]);
    }
  }
  if ( argc == 5 ) {
    relative = 1;
  }

  nok = Asc_QlfdidSearch3(QUIET(argv[1]),relative);
  if (nok) { /* failed. bail out. */
    Asc_DStringAppend(hptr," : Error -- Name not found",HALL);
    return HELP_ERROR;
  }
  i = g_search_inst;	/* catch inst ptr found in QlfdidSearch */
  value_str = QUIET(argv[2]);

  if (strcmp("UNDEFINED",value_str)==0) {
    return HELP_OK;
  }
  nok = BrowDoAssignment(hptr,i,value_str,unit_str);
  return nok;	/* whatever code returned by BrowDoAssignment */
}


int ascjson::Asc_BrowRunAssignmentCmd(Asc_DString *hptr,
                         int argc, CONST84 char *argv[])
{
  struct Instance *i;
  char *unit_str = NULL;
  char *value_str = NULL;
  int argstart=1;
  int nok;

  if (argc<2 || argc>4) {
    Asc_DStringAppend3(hptr, "Usage: \"", argv[0],
                     "\" [-search] value [units]",HALL);
    return HELP_ERROR;
  }
  if (argv[1][0] == '-') {
    if (strncmp("-search",argv[1],3)!=0) {
      Asc_DStringAppend5(hptr,"Error: ",argv[0]," Unknown option ",argv[1],
                       " want \"-search\"", HALL);
      return HELP_ERROR;
    } else {
      argstart++;
      i = g_search_inst;
    }
  } else {
    i = g_curinst;	/* use the current instance as the context */
  }
  if (!i) {
    Asc_DStringSet(hptr, "Given instance is NULL");
    return HELP_ERROR;
  }
  value_str = QUIET(argv[argstart]);
  if ( argc == 3 && argstart == 1) {
    unit_str = QUIET(argv[2]);
  }
  if ( argc == 4) {
    unit_str = QUIET(argv[3]);
  }
  if (strcmp("UNDEFINED",value_str)==0) {
    return HELP_OK;
  }
  nok = BrowDoAssignment(hptr,i,value_str,unit_str);
  return nok;	/* whatever code returned by BrowDoAssignment */
}


int ascjson::Asc_BrowWriteProcedure(Asc_DString *hptr,
                       int argc, CONST84 char *argv[])
{
  struct InitProcedure *proc;
  struct Instance *i;
  FILE *fp=NULL;

  if ( argc < 3 || argc >4) {
    Asc_DStringSet(hptr,"Usage bgetproc <methodname> <filepathname> [search]");
    return HELP_ERROR;
  }
  if (argc==4) {
    i = g_search_inst;
  } else {
    i = g_curinst;
  }
  if (i==NULL) {
    Asc_DStringSet(hptr, "no instance sent to bgetproc");
    return HELP_ERROR;
  }
  proc = FindProcedure(i,AddSymbol(argv[1]));
  if (proc==NULL) {
    Asc_DStringSet(hptr, "method named not found");
    return HELP_ERROR;
  }
  fp=fopen(argv[2],"w");
  if (fp==NULL) {
     Asc_DStringSet(hptr, "unable to open scratch file.");
     return HELP_ERROR;
  }
  WriteProcedure(fp,proc);
  fclose(fp);

  return HELP_OK;
}


int ascjson::Asc_BrowSetAtomAttribute(Asc_DString *hptr, struct Instance *i,
                             symchar *attr, enum inst_t kind, void *value)
{
  struct Instance *ch;
  if (hptr==NULL) {
    return HELP_ERROR;
  }
  if (i==NULL || attr == NULL || value == NULL) {
    Asc_DStringSet(hptr, "Bad input to C Asc_BrowSetAtomAttribute");
    return HELP_ERROR;
  }
  assert(AscFindSymbol(attr) != NULL);
  ch = ChildByChar(i,attr); /* symchar safe. no array child of atoms */
  if (ch == NULL || InstanceKind(ch) != kind) {
    Asc_DStringSet(hptr, "Mismatched input to C Asc_BrowSetAtomAttribute");
    return HELP_ERROR;
  }
  switch (InstanceKind(ch)) {
  case REAL_INST:
    SetRealAtomValue(ch,*(double *)value,0);
    break;
  case INTEGER_INST:
    SetIntegerAtomValue(ch,*(long *)value,0);
    break;
  case BOOLEAN_INST:
    SetBooleanAtomValue(ch,(*(int *)value != 0),0);
    break;
  case SYMBOL_INST:
    SetSymbolAtomValue(ch,AddSymbol(*(char **)value));
    break;
  default:
    Asc_DStringSet(hptr, "Incorrect child type to C Asc_BrowSetAtomAttribute");
    return HELP_ERROR;
  }
  return HELP_OK;
}
/*
 * status = Asc_BrowSetAtomAttribute(hptr, atominstance,
 *                                   childname,childtype,dataptr);
 * Sets the value of an attribute of the ATOM/REL instance given.
 * Childname must be from the compiler symbol table via AddSymbol or
 * AddSymbolL. Childtype determines what dataptr contains.
 * Childtype must be REAL_INST, INTEGER_INST, BOOLEAN_INST, SYMBOL_INST.
 * SET_INST is not supported at this time. dataptr must point to
 * an appropriate value object for each of the INST types above:
 * double, long, int, symchar *, respectively. Note that a symbol
 * value MUST come from the symbol table.
 * Return a value and message other than HELP_OK if these conditions
 * are not met. Except that if the childname or symbol value given
 * are not in the symbol table, then does not return.
*/




