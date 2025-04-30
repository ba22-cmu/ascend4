/*
 *  DisplayProc.c
 *  by Kirk Abbott and Ben Allan
 *  Created: 1/94
 *  Version: $Revision: 1.19 $
 *  Version control file: $RCSfile: DisplayProc.c,v $
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
#if 0
#include <tcl.h>
#include "config.h"
#include <ascend/general/ascMalloc.h>
#include <ascend/general/list.h>

#include <ascend/compiler/symtab.h>
#include <ascend/compiler/module.h>
#include <ascend/compiler/library.h>
#include <ascend/compiler/child.h>
#include <ascend/compiler/type_desc.h>
#include <ascend/compiler/type_descio.h>
#include <ascend/compiler/expr_types.h>
#include <ascend/compiler/stattypes.h>
#include <ascend/compiler/statio.h>
#include <ascend/system/slv_types.h>
#include "HelpProc.h"
#include "DisplayProc.h"
#include "Commands.h"
#endif

#define MAXIMUM_ID_LENGTH 80
#define MAXIMUM_STR_LENGTH 256
#define DISPTAB 4

int ascjson::Asc_DispDefineCmd(Asc_DString *hptr, int argc, CONST84 char *argv[])
{
  /* The format of this command is : ddefine ?arg?, where arg may
   * be none or one. We might add a module arg.
   */
  struct TypeDescription *desc;
  unsigned long length,c;
  struct gl_list_t *list;
  FILE *ddef_outfile=NULL;
  int closefile=0;

  if ( argc > 3 ) {
    Asc_DStringSet(hptr, "ddefine [type [bool_getstring]]");
    return HELP_ERROR;
  }
  if ( argc > 1 ) {/* we will print the types code - Toms argc + 1*/
    desc = FindType(AddSymbol(argv[1]));
    if (desc==NULL) {
      FPRINTF(stderr,"Internal Error : the type %s does not exist\n", argv[1]);
      Asc_DStringSet(hptr, "Type doesn't exist");
      return HELP_ERROR;
    } else {
      if ( argc == 3 ) {
        ddef_outfile=tmpfile();
        if (!ddef_outfile) {
          Asc_DStringSet(hptr, "ddefine: unable to open data file.");
          return HELP_ERROR;
        }
        closefile=1;
      } else {
        ddef_outfile=stdout;
      }
      WriteDefinition(ddef_outfile,desc); /* later store this in a list */
      if (closefile) {
        char *r = file_to_string(ddef_outfile);
        fclose(ddef_outfile);
	Asc_DStringSet(hptr, r);
	free(r);
      }
      return HELP_OK;
    }
  } else {
    list = DefinitionList();
    if (list) {
      length = gl_length(list);
      for(c=1;c<=length;c++) {
        desc = (struct TypeDescription *)gl_fetch(list,c);
        VTcl_AppendElement(hptr,SCP(GetName(desc)));
      }
      VAEstrip(hptr);
      return HELP_OK;
    } else {
      Asc_DStringSet(hptr, "Strange Display Error");
      return HELP_ERROR;
    }
  }
  /* not reached */
}

int ascjson::Asc_DispDiffDefineCmd(Asc_DString *hptr,
                      int argc, CONST84 char *argv[])
{
/* The format of this command is : ddiffdefine arg ?file? where arg
   is a type and file is output destination.
*/
  struct TypeDescription *desc;
  FILE *ddef_outfile=NULL;
  int closefile=0;

  if (argc > 3 || argc <2) {
    Asc_DStringSet(hptr, "ddiffdefine type [filename]");
    return HELP_ERROR;
  }
  desc = FindType(AddSymbol(argv[1]));
  if (desc==NULL) {
    FPRINTF(stderr,"ddiffdefine: the type %s does not exist\n", argv[1]);
    Asc_DStringSet(hptr, "Type doesn't exist");
    return HELP_ERROR;
  } else {
    if ( argc == 3 ) {
        ddef_outfile=tmpfile();
        if (!ddef_outfile) {
          Asc_DStringSet(hptr, "ddiffdefine: unable to open data file.");
          return HELP_ERROR;
        }
        closefile=1;

    } else {
      ddef_outfile=stderr;
    }
    WriteDiffDefinition(ddef_outfile,desc);
    if (closefile) {
      char *r = file_to_string(ddef_outfile);
      fclose(ddef_outfile);
      Asc_DStringSet(hptr, r);
      free(r);
    }
    return HELP_OK;
  }
}

int ascjson::Asc_DispTypePartsCmd(Asc_DString *hptr, int argc, CONST84 char *argv[])
{
  struct gl_list_t *names;
  unsigned long len,c;
  int atoms=FALSE,models=FALSE;
  symchar *name,*oldname;
  struct TypeDescription *t;

  if ( argc != 3 ) {
    Asc_DStringSet(hptr, "wrong args: dgetparts <ATOM,MODEL,BOTH> <type>");
    return HELP_ERROR;
  }
  switch (argv[1][0]) {
  case 'A':
    atoms=TRUE;
    break;
  case 'M':
    models=TRUE;
    break;
  case 'B':
    atoms=models=TRUE;
    break;
  default:
    Asc_DStringSet(hptr, "bad filter: dgetparts <ATOM,MODEL,BOTH> <type>");
    return HELP_ERROR;
  }
  t = FindType(AddSymbol(argv[2]));
  if (t==NULL) {
    Asc_DStringSet(hptr, "dgetparts called with nonexistent type");
    return HELP_ERROR;
  }
  names = GetTypeNamesFromStatList(GetStatementList(t));
  len = gl_length(names);
  oldname = NULL;
  for (c=1;c<=len;c++) {
    name=(symchar *)gl_fetch(names,c);
    if (name == NULL) {
      continue; /* ignore null */
    }
    if (name != oldname) { /*do this if not same symbol as last*/
      t = FindType(name);
      if (t) { /* check ATOM/MODEL and append accordingly */
        switch (GetBaseType(t)) {
        case model_type:
          if (models) {
            VTcl_AppendElement(hptr,(char *)SCP(name));
          }
          break;
        case real_type:
        case boolean_type:
        case integer_type:
        case symbol_type:
        case real_constant_type:
        case boolean_constant_type:
        case integer_constant_type:
        case symbol_constant_type:
          if (atoms) {
            VTcl_AppendElement(hptr,(char *)SCP(name));
          }
          break;
        case set_type:
        case relation_type:
        case array_type:
          break;
        default:
          break;
        }
      } else {
        FPRINTF(stderr,"Type %s refers to missing type %s!\n",
            argv[2],SCP(name));
      }
    }
    oldname = name;
  }
  VAEstrip(hptr);
  gl_destroy(names);
  return HELP_OK;
}

int ascjson::Asc_DispQueryCmd(Asc_DString *hptr,
                 int argc, CONST84 char *argv[])
{
/* The format of this command is : disp arg ?arg?.
*/
  struct TypeDescription *desc;
  unsigned long len,c;
  struct gl_list_t *list;

  if ( argc > 3 ) {
    Asc_DStringSet(hptr, "wrong # args to \"disp\" : try define");
    return HELP_ERROR;
  }
  if (( argc == 2 ) && (strncmp(argv[1],"define",3)==0)) {
    list = DefinitionList();
    if(list==NULL) {
      Asc_DStringFree(hptr);
      return HELP_OK;
    }
    len = gl_length(list);
    if (len==0) {
      Asc_DStringFree(hptr);
      return HELP_OK;
    }
    for(c=1;c<=len;c++) {
      desc = (struct TypeDescription *)gl_fetch(list,c);
      if (desc!=NULL) {
        VTcl_AppendElement(hptr,(char *)SCP(GetName(desc)));
      } else {
        Asc_DStringFree(hptr);
        return HELP_OK;
      }
    }
    VAEstrip(hptr);
    return HELP_OK;
  } else {
    FPRINTF(stderr,"Not yet supported\n");
    return HELP_OK;
  }
}


int ascjson::Asc_DispHierarchyCmd(Asc_DString *hptr,
                     int argc, CONST84 char *argv[])
{
  struct TypeDescription *desc, *refines=NULL;
  unsigned long c=0;

  if ( argc != 2 ) {
    Asc_DStringSet(hptr, "wrong # args : Usage \"hierarchy type\"");
    return HELP_ERROR;
  }
  desc = FindType(AddSymbol(argv[1]));
  if (desc==NULL) {
    return HELP_OK;
  }
  do {
    refines = GetRefinement(desc);
    if (refines!=NULL) {
      VTcl_AppendElement(hptr,(char *)SCP(GetName(refines)));
      desc = refines;
      c++;
    }
  } while (refines!=NULL);
  VAEstrip(hptr);
  if(c==0) {
    Asc_DStringFree(hptr);
  }
  return HELP_OK;
}


/*  This function accepts the name of a type and returns the filename that
 *  it was found in.
 */
int ascjson::Asc_DispFileByTypeCmd(Asc_DString *hptr,
                      int argc, CONST84 char *argv[])
{
  struct TypeDescription *desc;
  char *filename;
  symchar *tablename;
  struct module_t *mod;

  if ( argc != 2 ) {
    Asc_DStringSet(hptr, "wrong # args : Usage \"file_by_type type\"");
    return HELP_ERROR;
  }

  tablename = AddSymbol(argv[1]);
  /* Fundamental types are not defined externally -- hence no file */
  if (CheckFundamental(tablename)) {
    return HELP_OK;
  }
  desc = FindType(tablename);
  if (desc==NULL) {
    return HELP_OK;
  }
  mod = GetModule(desc);
  filename = (char *)Asc_ModuleFileName(mod);          /* cast for the CONST */
  Asc_DStringAppend(hptr, filename, HALL);
  return HELP_OK;
}

int ascjson::Asc_DispChildOneCmd(Asc_DString *hptr, int argc, CONST84 char *argv[])
{
  CONST struct TypeDescription *desc;
  ChildListPtr children;
  char buf[MAXIMUM_NUMERIC_LENGTH];            /* string to hold long */
  unsigned long nch, c=0;

  if ( argc != 3 ) {
    Asc_DStringSet(hptr, "wrong # args to \"dchild name num\" ");
    return HELP_ERROR;
  }
  desc = FindType(AddSymbol(argv[1]));
  if (desc==NULL) {
    Asc_DStringFree(hptr);
    return HELP_OK;
  }
  children = GetChildList(desc);
  if (!children) {
    Asc_DStringFree(hptr);
    return HELP_OK;
  }
  nch = ChildListLen(children);
  if(!nch) {
    Asc_DStringFree(hptr);
    return HELP_OK;
  }
  c = atol(argv[2]); /* bug. fixme use strtod */
  if((strcmp(argv[2],"0")==0) || !(c)) {
    sprintf(buf,"%lu",nch);
    Asc_DStringSet(hptr, buf);
    return HELP_OK;
  }
  if(c>nch) {
    VTcl_AppendElement(hptr,(char *)SCP(ChildStrPtr(children,nch)));
  } else {
    VTcl_AppendElement(hptr,(char *)SCP(ChildStrPtr(children,c)));
  }
  VAEstrip(hptr);
  return HELP_OK;
}

int ascjson::Asc_DispRefinesMeCmd(Asc_DString *hptr,
                     int argc, CONST84 char *argv[])
{
/* This function will search the entire library hash table for all types
   that refine it. Hence if a refines b, and c refines b, calling this
   function with b should return a and c. This should be expensive.
   registered as \"drefines_me type\".
*/
  struct gl_list_t *refine_me=NULL;
  symchar *refname=NULL;
  unsigned long len,c;
  int all = 0;

  if (argc == 3 && strcmp(argv[2],"-all")) {
    Asc_DStringSet(hptr, "wrong # args to \"drefines_me type -all\"");
    return HELP_ERROR;
  } 
  if (argc == 3) {
	  all = 1;
  } else {
	  if ( argc != 2 ) {
	    Asc_DStringSet(hptr, "wrong # args to \"drefines_me type\"");
	    return HELP_ERROR;
	  }
  }
  if (all) {
    refine_me = AllTypesThatRefineMe_Flat(AddSymbol(argv[1]));
  } else {
    refine_me =TypesThatRefineMe(AddSymbol(argv[1]));
  }
  if (!refine_me) {
    Asc_DStringFree(hptr);
    return HELP_OK;
  }
  len = gl_length(refine_me);
  if (!len) {
    Asc_DStringFree(hptr);
    gl_destroy(refine_me);
    return HELP_OK;
  }
  for (c=1;c<=len;c++) {
    refname = (symchar *)gl_fetch(refine_me,c);
    if (refname) {
      VTcl_AppendElement(hptr,(char *)SCP(refname));
    }
  }
  VAEstrip(hptr);
  gl_destroy(refine_me);
  return HELP_OK;
}

static Asc_DString *writehierhptr;

static void DispWriteHierTreeChildless(struct HierarchyNode *h) {
  if (!h) {
    return;
  }
  if (!(h->descendents)) {
    return;
  }
  if (gl_length(h->descendents)!=0L) {
    return;
  }
  Asc_DStringAppend3(writehierhptr,"{",(char *)SCP(GetName(h->desc)), " {",HALL);
  Asc_DStringAppend(writehierhptr,"}} ",HALL);
}

static void DispWriteHierTreeParents(struct HierarchyNode *h) {
  if (!h) {
    return;
  }
  if (!(h->descendents)) {
    return;
  }
  if (gl_length(h->descendents)==0L) {
    return;
  }
  Asc_DStringAppend3(writehierhptr,"{",(char *)SCP(GetName(h->desc)),
                   " {",HALL);
  gl_iterate(h->descendents,(void (*)(VOIDPTR))DispWriteHierTreeParents);
  gl_iterate(h->descendents,(void (*)(VOIDPTR))DispWriteHierTreeChildless);
  Asc_DStringAppend(writehierhptr,"}} ",HALL);
}

static void DispWriteHierTree(struct HierarchyNode *h) {
  if (!h) {
    return;
  }
  if (!(h->descendents) || gl_length(h->descendents)==0L) {
    DispWriteHierTreeChildless(h);
  } else {
    DispWriteHierTreeParents(h);
  }
}


int ascjson::Asc_DispRefinesMeTreeCmd(Asc_DString *hptr,
                        int argc, CONST84 char *argv[])
{
  struct HierarchyNode *h=NULL;

  if ( argc != 2 ) {
    Asc_DStringSet(hptr, "call is: drefinement_tree <type>");
    return HELP_ERROR;
  }
  h = AllTypesThatRefineMe_Tree(AddSymbol(argv[1]));
  if (!h) {
    Asc_DStringFree(hptr);
    return HELP_OK;
  }
  writehierhptr=hptr;
  DispWriteHierTree(h);
  DestroyHierarchyNode(h);
  return HELP_OK;
}

/* still has a slight bug -- */

int ascjson::Asc_DispIsRootTypeCmd(Asc_DString *hptr,
                     int argc, CONST84 char *argv[])
{
/* Returns true if is type is a base type, or if the type is of type
   model with no refinements.
*/
  unsigned int fundamental;
  CONST struct TypeDescription *desc;

  if ( argc != 2 ) {
    Asc_DStringSet(hptr, "wrong # args to \"disroot_type\"");
    return HELP_ERROR;
  }
  fundamental = CheckFundamental(AddSymbol(argv[1]));
  if (fundamental) {
    Asc_DStringSet(hptr, "1");
    return HELP_OK;
  } else {
    desc = FindType(AddSymbol(argv[1]));
    if (desc) {
      desc=GetRefinement(desc);
      if (!desc) {
        Asc_DStringSet(hptr, "1");
        return HELP_OK;
      }
    } else { /*  cant find it, so it cannot be fundamental */
      Asc_DStringSet(hptr, "0");
      return HELP_ERROR;
    }
  }
  Asc_DStringSet(hptr, "0");
  return HELP_OK;
}

