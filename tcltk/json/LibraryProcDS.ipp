/*
 * fixme
 */

#define ASC_BUILDING_INTERFACE

#include <time.h>
#include "config.h"

#include <ascend/general/ascMalloc.h>
#include <ascend/general/dstring.h>
#include <ascend/general/list.h>

#include <ascend/compiler/symtab.h>
#include <ascend/compiler/braced.h>
#include <ascend/compiler/notate.h>
#include <ascend/compiler/expr_types.h>
#include <ascend/compiler/syntax.h>
#include <ascend/compiler/module.h>
#include <ascend/compiler/instance_enum.h>
#include <ascend/compiler/dump.h>
#include <ascend/compiler/stattypes.h>
#include <ascend/compiler/slist.h>
#include <ascend/compiler/child.h>
#include <ascend/compiler/childio.h>
#include <ascend/compiler/type_desc.h>
#include <ascend/compiler/typedef.h>
#include <ascend/compiler/extfunc.h>
#include <ascend/compiler/library.h>
#include <ascend/compiler/prototype.h>
#include <ascend/compiler/proc.h>
#include <ascend/compiler/nameio.h>
#include <ascend/compiler/parser.h>
#include <ascend/system/slv_types.h>


int ascjson::Asc_FileIDCopy(FILE *filein, FILE *fileout)
{
  int c;
  while ((c = fgetc(filein)) != EOF) {
    FPUTC(c,fileout);
  }
  return 0;
}


struct int_option {
  int *option_ptr;
  const char *option_name;
  int low;
  int high;
};

STDHLF(Asc_LibrOptionsCmdHC,(Asc_LibrOptionsCmdHL,HLFSTOP));
int ascjson::Asc_LibrOptionsCmdDS( Asc_DString *hptr,
		int argc, CONST84 char **argv
){

/* keep the names here < 60 chars. Data for Options command */
#define OPTIONCOUNT 4
  struct int_option option_list[OPTIONCOUNT] = {
    {&g_compiler_warnings,"-compilerWarnings",0,INT_MAX},
    {&g_parser_warnings,"-parserWarnings",0,5},
    {&g_simplify_relations,"-simplifyRelations",0,1},
    {&g_use_copyanon,"-useCopyAnon",0,1}
  };
#define GOL option_list



  int i, opt, status;
  char buf[80];
  OLD_ASCUSE;  /* see if first arg is -help */
  if (argc == 1) {
    for (i = 0; i < OPTIONCOUNT; i++) {
      sprintf(buf,"%s %d",GOL[i].option_name, *(GOL[i].option_ptr));
      VTcl_AppendElement(hptr,buf);
    }
    return HELP_OK;
  }
  if (argc == 2) {
    for (i = 0; i < OPTIONCOUNT; i++) {
      if (strcmp(argv[1],GOL[i].option_name)==0) {
        sprintf(buf,"%d", *(GOL[i].option_ptr));
        Asc_DStringAppend(hptr,buf,HALL);
        return HELP_OK;
      }
    }
    sprintf(buf, "Unknown option '%s' to %s" ,argv[1], Asc_LibrOptionsCmdHN);
    Asc_DStringAppend(hptr, buf, HALL);
    return HELP_ERROR;
  }
  if (argc == 3) {
    for (i = 0; i < OPTIONCOUNT; i++) {
      if (strcmp(argv[1],GOL[i].option_name)==0) {
        status = JTcl_GetInt(hptr, argv[2],&opt);
        if (status != HELP_OK) {
	  sprintf(buf, "Non-integer value (%s) given for %s %s",
		 argv[2], argv[0], argv[1]); // fixme sizing
          Asc_DStringAppend(hptr, buf, HALL);
          return HELP_ERROR;
        }
	//sprintf(buf
        if (opt < GOL[i].low || opt > GOL[i].high) {
          sprintf(buf, "%s: Value %d out of range [%d - %d] in %s",
		  argv[0], opt, GOL[i].low,
                  GOL[i].high,  argv[1]);
          Asc_DStringAppend(hptr, buf, HALL);
          return HELP_ERROR;
        }
        *(GOL[i].option_ptr) = opt;
        return HELP_OK;
      }
    }

    sprintf(buf," Unknown option '%s' to %s argv[0]", argv[0], argv[0]);
    Asc_DStringAppend(hptr, buf,HALL);
    return HELP_ERROR;
  }
  sprintf(buf,"%d Too many arguments (%s) to  %s",argc, argv[1], argv[0] );
  Asc_DStringAppend(hptr,buf, HALL);
  return HELP_ERROR;
}

STDHLF(Asc_LibrReadCmdHC,(Asc_LibrReadCmdHL,HLFSTOP));
int ascjson::Asc_LibrReadCmdDS( Asc_DString *hptr,
                           int argc, CONST84 char **argv)
{
  struct module_t *mod;
  int relns_flag = 1;
  int result;

  OLD_ASCUSE; /* see if first arg is -help */

  if ( argc < 2 || argc > 3 ) {
    Asc_DStringSet(hptr,
                  "wrong # args: Usage: " Asc_LibrReadCmdHU);
    return HELP_ERROR;
  }

  /* set up the parse relns flag */
  if ( argc == 3 ) {
    relns_flag = atoi(argv[2]);
  }

  SetParseRelnsFlag(relns_flag);
  if((mod = Asc_OpenModule(argv[1],NULL)) == NULL) {
    Asc_DStringAppend(hptr, Asc_LibrReadCmdHN ": Error in opening file ", HALL);
    Asc_DStringAppend(hptr, argv[1], HALL);
    result = HELP_ERROR;
  } else {
    /*
     * the open was successful.  parse the file.
     */
    Asc_DStringSet(hptr, (char*)SCP(Asc_ModuleName(mod)));
    zz_parse(); /*FIXME*/ /* globals under this need fixing; limit to 1 instance for now. */
    result = HELP_OK;
  }
  SetParseRelnsFlag(1);	  /* always reset */
  return result;
}

STDHLF(Asc_LibrParseCmdHC,(Asc_LibrParseCmdHL,HLFSTOP));
int ascjson::Asc_LibrParseCmdDS( Asc_DString *hptr,
                     int argc, CONST84 char **argv)
{
  struct module_t *mod;
  int osmerr;
  int result;

  OLD_ASCUSE; /* see if first arg is -help */

  if ( argc != 2) {
    Asc_DStringSet(hptr,
                  "wrong # args: Usage: " Asc_LibrParseCmdHU);
    return HELP_ERROR;
  }

  mod = Asc_OpenStringModule(argv[1],&osmerr,NULL);
  if (mod == NULL) {
    Asc_DStringAppend(hptr, Asc_LibrParseCmdHN ": Insufficient memory to open "
                     "string buffer ", HALL);
    Asc_DStringAppend(hptr, argv[1], HALL);
    result = HELP_ERROR;
  } else {
    /*
     * the open was successful.  parse the string.
     */
    Asc_DStringSet(hptr, (char*)SCP(Asc_ModuleName(mod)));
    zz_parse(); /*FIXME*/ /* singleton in zz */
    Asc_CloseCurrentModule();
    result = HELP_OK;
  }
  return result;
}


static
int LibrModuleList(Asc_DString *hptr, int module_type)
{
  unsigned long c;
  struct gl_list_t *ml;

  if ( module_type < 0 || module_type > 2) {
    Asc_DStringSet(hptr, "module_type given not in [0 .. 2]");
    return HELP_ERROR;
  }
  ml = Asc_ModuleList(module_type);
  if( ml == NULL ) {
    /* module list is empty, return empty string */
    Asc_DStringFree(hptr);
    return HELP_OK;
  }

  for( c = gl_length(ml); c > 0; c-- ) {
    VTcl_AppendElement(hptr, (char *)gl_fetch( ml, c ));
  }
  gl_destroy(ml);
  return HELP_OK;
}


static
int LibrModelDefinitionMethods(Asc_DString *hptr)
{
  struct gl_list_t *pl;
  unsigned long c,len;

  pl = GetUniversalProcedureList();
  if (pl == NULL) {
    return HELP_OK;
  }
  len = gl_length(pl);
  for (c = 1; c <= len; c++) {
    VTcl_AppendElement(hptr,
        (char *)SCP(ProcName((struct InitProcedure *)gl_fetch(pl,c))));
  }
  return HELP_OK;
}

STDHLF(Asc_LibrTypeListCmdHC,(Asc_LibrTypeListCmdHL,HLFSTOP));
int ascjson::Asc_LibrTypeListCmdDS( Asc_DString *hptr,
                        int argc, CONST84 char **argv)
{
  struct gl_list_t *dl;
  unsigned long len;
  unsigned long c;
  CONST struct module_t *module;

  OLD_ASCUSE;  /* see if first arg is -help */

  if ( argc != 2 ) {
    Asc_DStringSet(hptr, "wrong # args: Usage: " Asc_LibrTypeListCmdHU);
    return HELP_ERROR;
  }

  module = Asc_GetModuleByName(argv[1]);
  if( module == NULL ) {
    Asc_DStringAppend(hptr, Asc_LibrTypeListCmdHN
                     ": Cannot find a module having the name ", HALL);
    Asc_DStringAppend(hptr,argv[1], HALL);
    return HELP_ERROR;
  }

  dl = Asc_TypeByModule(module);
  if ( dl == NULL ) {
    Asc_DStringAppend(hptr, Asc_LibrTypeListCmdHN
                     ": The type definition list is NULL for", HALL);
    Asc_DStringAppend(hptr, argv[1], HALL);
    return HELP_ERROR;
  }

  len = gl_length(dl);
  for( c = 1; c <= len; c++ ) {
    VTcl_AppendElement(hptr, (char*)gl_fetch(dl,c));
  }
  gl_destroy(dl);
  return HELP_OK;
}


STDHLF(Asc_LibrDestroyTypesCmdHC, (Asc_LibrDestroyTypesCmdHL,HLFSTOP));
int ascjson::Asc_LibrDestroyTypesCmdDS( Asc_DString *hptr,
                            int argc, CONST84 char **argv)
{
  OLD_ASCUSE;  /* see if first arg is -help */

  FFLUSH(stderr);
  DestroyNotesDatabase(LibraryNote());
  SetUniversalProcedureList(NULL);
  DestroyLibrary();
  DestroyPrototype();
  EmptyTrash();
  Asc_DestroyModules((DestroyFunc)DestroyStatementList);
  WriteChildMissing(NULL,NULL,NULL);
  DefineFundamentalTypes();
  InitNotesDatabase(LibraryNote());
  return HELP_OK;
}



/*
 *  void AddRootName(t);
 *      const struct TypeDescription *t;
 *
 *  AddRootName is called via gl_interate to find all root types, i.e,
 *  types that don't refine other types, and append the names of those
 *  types to the Tcl result.  We need to make a locally global pointer
 *  to the Tcl hptrreter (called lroottypeshptr) so that AddRootName
 *  can access it.
 */
static Asc_DString *lroottypeshptr = NULL;
static void AddRootName(CONST struct TypeDescription *t)
{
  if(( t != NULL ) && ( GetRefinement(t) == NULL )) {
    VTcl_AppendElement(lroottypeshptr, (char *)SCP(GetName(t)));
  }
}

static
int LibrRootTypes(Asc_DString *hptr)
{
  struct gl_list_t *deflist;

  deflist = DefinitionList();
  lroottypeshptr = hptr;
  gl_iterate(deflist,(void (*)(VOIDPTR))AddRootName);
  gl_destroy(deflist);
  return HELP_OK;
}

static
int LibrCatalog(Asc_DString *hptr)
{
  struct gl_list_t *deflist;
  unsigned long len;
  unsigned long c;

  deflist = DefinitionList();
  len = gl_length(deflist);
  for (c = 1; c <= len; c++) {
    VTcl_AppendElement(hptr,
     (char*)SCP(GetName((CONST struct TypeDescription *)gl_fetch(deflist,c))));
  }
  gl_destroy(deflist);
  return HELP_OK;
}



int ascjson::Asc_GNUTextCmdDS( Asc_DString *hptr,
                   int , CONST84 char **)
{
  Asc_DStringAppend(hptr,
                   "See the \"License\" buffer in the Script\n"
                   "for information on the GNU License and Warranty\n" ,HALL);
  return HELP_OK;
}


static
int LibrFindType(Asc_DString *hptr, struct TypeDescription *desc)
{
  struct module_t *mod;
  assert(desc!=NULL);

  mod = GetModule(desc);
  if( mod == NULL ) {
    Asc_DStringAppend(hptr, Asc_LibrQueryTypeCmdHN
                     ": Type is a fundamental type:", HALL);
    Asc_DStringAppend(hptr,(char *)SCP(GetName(desc)), HALL);
    return HELP_ERROR;
  }

  Asc_DStringSet(hptr, (char *)SCP(Asc_ModuleName(mod)));
  return HELP_OK;
}

static
int LibrAncestorType(Asc_DString *hptr, struct TypeDescription *desc)
{
  struct gl_list_t *names;
  unsigned long c,len;

  assert(desc!=NULL);
  names = GetAncestorNames(desc);
  if( names == NULL ) {
    Asc_DStringSet(hptr, "-ancestors: out of memory");
    return HELP_ERROR;
  }
  for (c = 1, len = gl_length(names);  c <= len; c++) {
	Asc_DStringAppend(hptr, (char *)gl_fetch(names,c), HALL);
	Asc_DStringAppend(hptr, "\v",1);
  }
  gl_destroy(names);
  return HELP_OK;
}


STDHLF(Asc_LibrModuleInfoCmdHC,(Asc_LibrModuleInfoCmdHL,HLFSTOP));
int ascjson::Asc_LibrModuleInfoCmdDS( Asc_DString *hptr, int argc, CONST84 char **argv)
{
  CONST struct module_t *mod;
  CONST char *string;
  char intbuf[36];
  int i;

  if( argc < 2 ) {
    Asc_DStringSet(hptr, "wrong # args: Usage: " Asc_LibrModuleInfoCmdHU);
    return HELP_ERROR;
  }

  for( i = 1; i < argc; i++ ) {
    if((mod = Asc_GetModuleByName(argv[i])) != NULL ) {
      VTcl_AppendElement(hptr, SCP(Asc_ModuleName(mod)));

      VTcl_AppendElement(hptr, SCP(Asc_ModuleBestName(mod)));
      string = Asc_ModuleString(mod);
      if (string == NULL) {
        VTcl_AppendElement(hptr, asctime(Asc_ModuleTimeModified(mod)));

        VTcl_AppendElement(hptr, "");
      } else {
        sprintf(intbuf,"%d",(int)Asc_ModuleStringIndex(mod));
        VTcl_AppendElement(hptr, intbuf);

        VTcl_AppendElement(hptr, string);
      }
    }
  }
  return HELP_OK;
}


static
int LibrExternalFuncs(Asc_DString *hptr)
{
  char *stringresult;
  stringresult = WriteExtFuncLibraryString();
  if (stringresult!=NULL) {
    Asc_DStringAppend(hptr, stringresult, HALL);
    ascfree(stringresult);
  }
  return HELP_OK;
}



STDHLF(Asc_LibrHideTypeCmdHC, (Asc_LibrHideTypeCmdHL,HLFSTOP));
int ascjson::Asc_LibrHideTypeCmdDS( Asc_DString *hptr,
                        int argc, CONST84 char **argv)
{
  struct TypeDescription *type;
  ChildListPtr clist;
  unsigned long c;

  OLD_ASCUSE;  /* see if first arg is -help */

  if( argc < 2 || argc > 3 ) {
    Asc_DStringSet(hptr, "wrong # args: Usage: " Asc_LibrHideTypeCmdHU);
    return HELP_ERROR;
  }

  type = FindType(AddSymbol(argv[1]));
  if (type==NULL) {
    Asc_DStringAppend(hptr, Asc_LibrHideTypeCmdHN " called with unknown type: ",HALL);
    Asc_DStringAppend(hptr, argv[1], HALL);
    return HELP_ERROR;
  }

  if ( argc == 2 ) {
    SetTypeShowBit(type,FALSE);
    return HELP_OK;
  }

  clist = GetChildList(type);
  if (clist==NULL) {
    Asc_DStringAppend(hptr, Asc_LibrHideTypeCmdHN " called with unknown type part", HALL);
    return HELP_ERROR;
  }
  c = ChildPos(clist,AddSymbol(argv[2]));
  if( c == 0UL ) {
    Asc_DStringAppend(hptr, Asc_LibrHideTypeCmdHN " called with unknown type part" , HALL);
    return HELP_ERROR;
  }
  ChildHide(clist,c);
  return HELP_OK;
}


STDHLF(Asc_LibrUnHideTypeCmdHC, (Asc_LibrUnHideTypeCmdHL,HLFSTOP));
int ascjson::Asc_LibrUnHideTypeCmdDS( Asc_DString *hptr,
                          int argc, CONST84 char **argv)
{
  struct TypeDescription *type;
  ChildListPtr clist;
  unsigned long c;

  OLD_ASCUSE;  /* see if first arg is -help */

  if (argc < 2 || argc > 3) {
    Asc_DStringSet(hptr, "wrong # args: Usage: " Asc_LibrUnHideTypeCmdHU );
    return HELP_ERROR;
  }

  type = FindType(AddSymbol(argv[1]));
  if (type==NULL) {
    Asc_DStringAppend(hptr, Asc_LibrUnHideTypeCmdHN " called with unknown type: ", HALL);
    Asc_DStringAppend(hptr, argv[1], HALL);
    return HELP_ERROR;
  }

  if ( argc == 2 ) {
    SetTypeShowBit(type,TRUE);
    return HELP_OK;
  }

  clist = GetChildList(type);
  if (clist==NULL) {
    Asc_DStringAppend(hptr, Asc_LibrUnHideTypeCmdHN " called with unknown type part", HALL);
    return HELP_ERROR;
  }
  c = ChildPos(clist,AddSymbol(argv[2]));
  if (c == 0UL) {
    Asc_DStringAppend(hptr, Asc_LibrUnHideTypeCmdHN " called with unknown type part", HALL);
    return HELP_ERROR;
  }
  ChildShow(clist,c);
  return HELP_OK;
}


static
int LibrGetFundamentals(Asc_DString *hptr)
{
  struct gl_list_t *fundies;
  struct TypeDescription *type;
  unsigned long len,c;
  symchar *name;

  fundies = FindFundamentalTypes();
  len = gl_length(fundies);

  for (c=1;c<=len;c++) {
    type = (struct TypeDescription *)gl_fetch(fundies,c);
    name = GetName(type);
    VTcl_AppendElement(hptr,(char *)SCP(name));
  }
  gl_destroy(fundies);
  return HELP_OK;
}


STDHLF(Asc_LibrTypeIsShownCmdHC, (Asc_LibrTypeIsShownCmdHL, HLFSTOP));
int ascjson::Asc_LibrTypeIsShownCmdDS( Asc_DString *hptr,
                           int argc, CONST84 char **argv)
{
  struct TypeDescription *type;
  char buf[MAXIMUM_NUMERIC_LENGTH];   /* string to hold integer */

  OLD_ASCUSE;  /* see if first arg is -help */

  if ( argc != 2 ) {
    Asc_DStringSet(hptr, "wrong # args: Usage: " Asc_LibrTypeIsShownCmdHU);
    return HELP_ERROR;
  }
  type = FindType(AddSymbol(argv[1]) );
  if( type == NULL ) {
    Asc_DStringAppend(hptr, Asc_LibrTypeIsShownCmdHN " called with unknown type", HALL);
    Asc_DStringAppend(hptr, argv[1], HALL);
    return HELP_ERROR;
  }
  sprintf(buf,"%d",TypeShow(type));
  Asc_DStringSet(hptr, buf);
  return HELP_OK;
}

static
int LibrFileExtsCmd(Asc_DString *hptr)
{
  int i;

  for (i = 0; i <  MOD_FILE_EXTS; i++) {
    VTcl_AppendElement(hptr,(char *)g_alt_ending[i]);
  }
  return HELP_OK;
}

static
int LibrTypeChildren(Asc_DString *hptr, struct TypeDescription *desc)
{
  ChildListPtr children;
  unsigned long nch;
  unsigned long c = 0;
  assert(desc!=NULL);

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
  for(c=1;c<=nch;c++) {
    VTcl_AppendElement(hptr,(char *)SCP(ChildStrPtr(children,c)));
  }
  return HELP_OK;
}


static
int LibrChildInfo(Asc_DString *hptr, struct TypeDescription *desc,
                  symchar *child)
{
  char *s;
  ChildListPtr cl;
  unsigned long nch;
  unsigned long c = 0;

  if (desc == NULL) {
    Asc_DStringAppend(hptr,WriteChildMetaDetails(),HALL);
    return HELP_OK;
  }
  cl = GetChildList(desc);
  assert(cl != NULL);
  if (child!=NULL) {
    c = ChildPos(cl,child);
    if (!c) {
      Asc_DStringAppend(hptr,"child not found: ", HALL);
      Asc_DStringAppend(hptr,(char *)SCP(child), HALL);
      return HELP_ERROR;
    }
    s = WriteChildDetails(cl,c);
    Asc_DStringAppend(hptr,s,HALL);
    ascfree(s);
    return HELP_OK;
  } else {
    nch = ChildListLen(cl);
    for (c = 1; c <= nch; c++) {
      s = WriteChildDetails(cl,c);
      Asc_DStringAppend(hptr,s,HALL);
      ascfree(s);
    }
    return HELP_OK;
  }
}

static
int LibrMethods(Asc_DString *hptr, struct TypeDescription *desc)
{
  struct InitProcedure *ip;
  struct gl_list_t *pl;
  unsigned long len,c;

  assert(desc!=NULL);
  pl = GetInitializationList(desc);
  if (pl!=NULL) {
    len = gl_length(pl);
    for(c=1;c<=len;c++) {
      ip = (struct InitProcedure *)gl_fetch(pl,c);
      VTcl_AppendElement(hptr,(char *)SCP(ProcName(ip)));
    }
  }
  return HELP_OK;
}

static
int LibrNoteDBList(Asc_DString *hptr)
{
  struct gl_list_t *dbl;
  unsigned long len;
  dbl = ListNotesDatabases();
  if (dbl != NULL) {
    len = gl_length(dbl);
    while (len>0) {
      VTcl_AppendElement(hptr,(char *)SCP(gl_fetch(dbl,len)));
      len--;
    }
  }
  return HELP_OK;
}

static
int LibrNoteLangs(Asc_DString *hptr, symchar *dbid)
{
  struct gl_list_t *langs;
  unsigned long len;
  langs = GetNotesAllLanguages(dbid);
  if (langs==NULL) {
    Asc_DStringAppend(hptr,"dbid invalid: ", HALL);
    Asc_DStringAppend(hptr, (char *)SCP(dbid), HALL);
    return HELP_ERROR;
  }
  len = gl_length(langs);
  while (len > 0) {
    VTcl_AppendElement(hptr,(char *)SCP(gl_fetch(langs,len)));
    len--;
  }
  gl_destroy(langs);
  return HELP_OK;
}

/* this function does not return the notes on qualified names
 * since we don't have an elegant way of executing the query.
 * We need more switches to the notes query syntax tcl
 * interface to manage those.
 * This returns notes about simple names.
 * This function is conceptually several functions, arg!
 * The empty symchar "" is treated as NULL.
 */
static
int LibrGetNotes(Asc_DString *hptr,symchar *type, symchar *lang,
                 symchar *child, symchar *method, long noteptr, long tokenptr,
                 symchar *dbid)
{
#if 0 // fixme
  struct gl_list_t *notes;
  struct Note *n;
  struct bracechar *bc;
  char *text;
  char linenum[40];
  unsigned long len;
  symchar *empty;
  struct Name *qlfdid;
  symchar *typenam, *language, *childname, *methodname;
  struct gl_list_t *tl, *ll, *cl, *ml, *ndl;
  void *hold;

  if (tokenptr != (long)NULL) {
    /* release previously held result */
    ReleaseNoteData(dbid,(void *)tokenptr);
    return HELP_OK;
  }
  if (noteptr == (long)NULL) {
    /* return pointer (as text) to held list we find */
    typenam = (   (type==NULL   || SCLEN(type)<1)   ? NOTESWILD : type);
    language = (   (lang==NULL   || SCLEN(lang)<1)   ? NOTESWILD : lang);
    childname = (  (child==NULL  || SCLEN(child)<1)  ? NOTESWILD : child);
    methodname = ( (method==NULL || SCLEN(method)<1) ? NOTESWILD : method);
    tl = gl_create(2);
    gl_append_ptr(tl,(VOIDPTR)typenam);
    ll = gl_create(2);
    gl_append_ptr(ll,(VOIDPTR)language);
    cl = gl_create(2);
    gl_append_ptr(cl,(VOIDPTR)childname);
    ml = gl_create(2);
    gl_append_ptr(ml,(VOIDPTR)methodname);
    ndl = gl_create(2);
    gl_append_ptr(ndl,(VOIDPTR)nd_empty);
    gl_append_ptr(ndl,(VOIDPTR)nd_name);
    notes = GetNotesList(dbid,tl,ll,cl,ml,ndl);
    gl_destroy(tl);
    gl_destroy(ll);
    gl_destroy(cl);
    gl_destroy(ml);
    gl_destroy(ndl);
    hold = HoldNoteData(dbid,notes);
    sprintf(linenum,"%ld",(long)hold);
    Asc_DStringAppend(hptr,linenum,HALL);
    return HELP_OK;
  } else {
    /* return formatted record */
    notes = GetExactNote(dbid,(struct Note *)noteptr);
  }

  /* list notes must not be held before here because we
   * destroy it at the END.
   */
  if (notes==NULL) {
    Asc_DStringAppend(hptr,"note not found in ", HALL);
    Asc_DStringAppend(hptr,(char *)SCP(dbid), HALL);
    return HELP_ERROR;
  }
  len = gl_length(notes);
  empty = AddSymbol("");
  while (len > 0) {
    n = (struct Note *)gl_fetch(notes,len);
    len--;
    if (n==NULL) {
      continue;
    }
    typenam = GetNoteType(n);
    if (typenam == NULL) {
      typenam = empty;
    }
    childname = GetNoteId(n);
    if (childname == NULL) {
      childname = empty;
    }
    language = GetNoteLanguage(n);
    if (language == NULL) {
      language = empty;
    }
    methodname = GetNoteMethod(n);
    if (methodname == NULL) {
      methodname = empty;
    }
    Asc_DStringAppend(hptr,"{{",(char *)SCP(typenam),"} {",
                                (char *)SCP(language),"} {",(char *)NULL);
    qlfdid = (struct Name *)GetNoteData(n,nd_name);
    if (childname==empty && qlfdid != NULL) {
      text = WriteNameString(qlfdid);
      Asc_DStringAppend(hptr, text,"} {",(char *)NULL);
      ascfree(text);
    } else {
      Asc_DStringAppend(hptr, (char *)SCP(childname),"} {",(char *)NULL);
    }
    bc = GetNoteText(n);
    if (bc == NULL) {
      text = (char *)SCP(empty);
    } else {
      text = (char *)BCS(bc);
    }
    Asc_DStringAppend(hptr, (char *)SCP(methodname),"} {",text,"}",
                     (char *)NULL);
    if (noteptr == (long)NULL) {
      /* close element */
      Asc_DStringAppend(hptr, "} ",(char *)NULL);
    } else {
      /* digging up everything on specific note */
      text = (char *)GetNoteFilename(n);
      if (text == NULL) {
        text = (char *)SCP(empty);
      }
      sprintf(linenum,"%d",GetNoteLineNum(n));
      Asc_DStringAppend(hptr, " {",text,"} {",linenum,"}} ", (char *)NULL);
    }
  }
 /* END */
  gl_destroy(notes);
  return HELP_OK;
#else
  Asc_DStringSet(hptr, "LibrGetNotes not implemented. need new format");
  return HELP_ERROR;
#endif
}

/* Function to set up the tcl regexp engine and call it with the
 * notes in heldlist or database for matches against pattern.
 */
static
int LibrMatchNotes(Asc_DString *hptr, char *pattern,
                   long heldlist,symchar *dbid)
{
#if 0
  struct gl_list_t *notes = NULL;
  int status = HELP_OK;
  void *held;
  char idnum[40];
  struct NoteEngine *ne;
  if (pattern==NULL) {
    Asc_DStringAppend(hptr,"NOTES match needs pattern string to match",
                     (char *)NULL);
    return HELP_ERROR;
  }
  if (heldlist != (long)NULL) {
    notes = HeldNotes(dbid,(void *)heldlist);
    if (notes == NULL) {
      Asc_DStringAppend(hptr,"NOTES database token given not valid",
                       (char *)NULL);
      return HELP_ERROR;
    }
  }
  ne = NotesCreateEngine(hptr,
                         (NEInitFunc *)&Tcl_RegExpCompile,
                         (NECompareFunc *)&Tcl_RegExpExec);
  if (ne == NULL) {
    Asc_DStringAppend(hptr,"NOTES match unable to set up regexp engine",
                     (char *)NULL);
    return HELP_ERROR;
  }
  sprintf(idnum,"xxx");
  notes = GetMatchingNotes(dbid,pattern,notes,ne);
  if (notes != NULL) {
    held = HoldNoteData(dbid,notes);
    sprintf(idnum,"%ld",(long)held);
    Asc_DStringFree(hptr);
    Asc_DStringAppend(hptr,idnum,(char *)NULL);
  } else {
    if (strlen(Tcl_GetStringResult(hptr)) == 0) {
      sprintf(idnum,"%ld",(long)0);
      Asc_DStringFree(hptr);
      Asc_DStringAppend(hptr,idnum,(char *)NULL);
    } else {
      status = HELP_ERROR;
      /* else leave possible error in hptr */
    }
  }
  NotesDestroyEngine(ne);
  return status;
#else
  Asc_DStringSet(hptr, "LibrMatchNotes not implemented. need new format");
  return HELP_ERROR;
#endif
}

/* this function returns the notes on everything in more or less
 * easily sortable columns form.
 */
static
int LibrDumpNotes(Asc_DString *hptr, int tmax, long heldlist, symchar *dbid)
{
#if 1
  Asc_DStringSet(hptr, "LibrDumpNotes not implemented. need new format");
  return HELP_ERROR;
#else
  struct gl_list_t *notes;
  struct Note *n;
  struct bracechar *bc;
  char *text;
  char *abbr, idnum[40];
  int tlen; /* length of text in a note */
  int row; /* the 'row' in the database, which may change with new reads
            * and should not be shown to the user.
            */
  unsigned long len;
  symchar *empty;
  struct Name *qlfdid;
  symchar *typenam, *language, *childname, *methodname;

  if (tmax < 5) {
    tmax = 5;
  }

  if (heldlist != (long)NULL) {
    notes = HeldNotes(dbid,(void *)heldlist);
    if (notes == NULL) {
      Asc_DStringAppend(hptr,"NOTES database token given not valid",
                       (char *)NULL);
      return HELP_ERROR;
    }
  } else {
    notes = GetNotes(dbid,NOTESWILD,NOTESWILD,NOTESWILD,NOTESWILD,nd_wild);
    if (notes==NULL) {
      return HELP_OK; /* empty database */
    }
  }
  empty = AddSymbol("~");

  /* process type names */
  row = 0;
  len = gl_length(notes);
  Asc_DStringAppend(hptr,"{",(char *)NULL);
  while (len > 0) {
    n = (struct Note *)gl_fetch(notes,len);
    len--;
    if (n==NULL || GetNoteEnum(n) == nd_vlist) {
      continue;
    }
    typenam = GetNoteType(n);
    if (typenam == NULL) {
      typenam = empty;
    }
    sprintf(idnum,"%d",row);
    Asc_DStringAppend(hptr,"{{",
                     (char *)SCP(typenam),"} ",idnum,
                            "} ", (char *)NULL);
    row++;
  }
  Asc_DStringAppend(hptr,"} {",(char *)NULL);
  /* process languages */
  row = 0;
  len = gl_length(notes);
  while (len > 0) {
    n = (struct Note *)gl_fetch(notes,len);
    len--;
    if (n==NULL || GetNoteEnum(n) == nd_vlist) {
      continue;
    }
    language = GetNoteLanguage(n);
    if (language == NULL) {
      language = empty;
    }
    sprintf(idnum,"%d",row);
    Asc_DStringAppend(hptr,"{{",
                     (char *)SCP(language),"} ",idnum,
                            "} ", (char *)NULL);
    row++;
  }
  Asc_DStringAppend(hptr,"} {",(char *)NULL);
  /* process names. use qlfdid iff id == NULL */
  row = 0;
  len = gl_length(notes);
  while (len > 0) {
    n = (struct Note *)gl_fetch(notes,len);
    len--;
    if (n==NULL || GetNoteEnum(n) == nd_vlist) {
      continue;
    }
    childname = GetNoteId(n);
    qlfdid = (struct Name *)GetNoteData(n,nd_name);
    if (childname == NULL) {
      childname = empty;
    }
    sprintf(idnum,"%d",row);
    if (childname == empty && qlfdid != NULL) {
      text = WriteNameString(qlfdid);
      Asc_DStringAppend(hptr,"{{", text, "} ",idnum, "} ", (char *)NULL);
      ascfree(text);
    } else {
      Asc_DStringAppend(hptr,"{{", (char *)SCP(childname),"} ",idnum, "} ",
                       (char *)NULL);
    }
    row++;
  }
  Asc_DStringAppend(hptr,"} {",(char *)NULL);

  /* process method names */
  row = 0;
  len = gl_length(notes);
  while (len > 0) {
    n = (struct Note *)gl_fetch(notes,len);
    len--;
    if (n==NULL || GetNoteEnum(n) == nd_vlist) {
      continue;
    }
    methodname = GetNoteMethod(n);
    if (methodname == NULL) {
      methodname = empty;
    }
    sprintf(idnum,"%d",row);
    Asc_DStringAppend(hptr,"{{",
                     (char *)SCP(methodname),"} ",idnum,
                            "} ", (char *)NULL);
    row++;
  }
  Asc_DStringAppend(hptr,"} {",(char *)NULL);

  /* process text */
  row = 0;
  len = gl_length(notes);
  abbr = ASC_NEW_ARRAY(char,tmax+1);
  if (abbr == NULL) {
    Asc_DStringFree(hptr);
    Asc_DStringAppend(hptr,"NOTES dump: out of memory",(char *)NULL);
    gl_destroy(notes);
    return HELP_ERROR;
  }
  while (len > 0) {
    n = (struct Note *)gl_fetch(notes,len);
    len--;
    if (n==NULL || GetNoteEnum(n) == nd_vlist) {
      continue;
    }
    bc = GetNoteText(n);
    if (bc == NULL) {
      text = (char *)SCP(empty);
      tlen = 0;
    } else {
      text = (char *)BCS(bc);
      tlen = BCL(bc);
    }
    if (tlen < tmax) {
      sprintf(abbr,"%s",text);
    } else {
      sprintf(abbr,"%.*s...",tmax-3,text); /* fixme variable prec %s fmt */
    }
    sprintf(idnum,"%d",row);
    Asc_DStringAppend(hptr,"{{", abbr, "} ",idnum, "} ", (char *)NULL);
    row++;
  }
  Asc_DStringAppend(hptr,"} {",(char *)NULL);
  ascfree(abbr);

  /* process filename,line */
  row = 0;
  len = gl_length(notes);
  while (len > 0) {
    n = (struct Note *)gl_fetch(notes,len);
    len--;
    if (n==NULL || GetNoteEnum(n) == nd_vlist) {
      continue;
    }
    tlen = GetNoteLineNum(n);
    text = (char *)GetNoteFilename(n);
    if (text == NULL) {
      text = (char *)SCP(empty);
    }
    /* fixme. want leaf name only. use file tail in tcl */
    sprintf(idnum,":%d} %d",tlen,row);
    Asc_DStringAppend(hptr,"{{", text,/*d} d*/ idnum, "} ", (char *)NULL);
    row++;
  }
  Asc_DStringAppend(hptr,"} {", (char *)NULL);

  /* process record number, which we are cheating an using the pointer for. */
  row = 0;
  len = gl_length(notes);
  while (len > 0) {
    n = (struct Note *)gl_fetch(notes,len);
    len--;
    if (n==NULL || GetNoteEnum(n) == nd_vlist) {
      continue;
    }
    sprintf(idnum,"%lu",(unsigned long)n);
    Asc_DStringAppend(hptr,"{", idnum,(char *)NULL);
    sprintf(idnum,"%d",row);
    Asc_DStringAppend(hptr," ", idnum,"} ",(char *)NULL);
    row++;
  }
  Asc_DStringAppend(hptr,"}", (char *)NULL);

  gl_destroy(notes);
  return HELP_OK;
#endif
}

static
int LibrUnimplemented(Asc_DString *hptr, CONST84 char **argv)
{
  Asc_DStringAppend(hptr,"Unimplemented option in : "
                   Asc_LibrQueryTypeCmdHN,HALL);
  Asc_DStringAppend(hptr, argv[1], HALL);
  return HELP_ERROR;
}

STDHLF(Asc_LibrQueryTypeCmdHC,(Asc_LibrQueryTypeCmdHL1,
                             Asc_LibrQueryTypeCmdHL10,
                             Asc_LibrQueryTypeCmdHL20,
                             Asc_LibrQueryTypeCmdHL30,
                             Asc_LibrQueryTypeCmdHL40,
                             Asc_LibrQueryTypeCmdHL50,
                             Asc_LibrQueryTypeCmdHL60,
                             Asc_LibrQueryTypeCmdHL70,
                             Asc_LibrQueryTypeCmdHL80,
                             Asc_LibrQueryTypeCmdHL85,
                             Asc_LibrQueryTypeCmdHL90,
                             Asc_LibrQueryTypeCmdHL100,
                             Asc_LibrQueryTypeCmdHL110,
                             Asc_LibrQueryTypeCmdHL115,
                             Asc_LibrQueryTypeCmdHL120,
                             Asc_LibrQueryTypeCmdHL130,
                             Asc_LibrQueryTypeCmdHL131,
                             Asc_LibrQueryTypeCmdHL132,
                             Asc_LibrQueryTypeCmdHL133,
                             Asc_LibrQueryTypeCmdHL135,
                             Asc_LibrQueryTypeCmdHL136,
                             Asc_LibrQueryTypeCmdHL137,
                             Asc_LibrQueryTypeCmdHL140,
                             HLFSTOP));
int ascjson::Asc_LibrQueryTypeCmdDS( Asc_DString *hptr,
                         int argc, CONST84 char **argv)
{
  enum qtype {
    q_error,
    q_ancestors,
    q_basemethods,
    q_catalog,
    q_childnames,
    q_childinfo,
    q_definition,
    q_exists,
    q_externalfunctions,
    q_findtype,
    q_filetypes,
    q_fundamentals,
    q_language,
    q_methods,
    q_modulelist,
    q_notes,
    q_notesdump,
    q_notekinds,
    q_notesmatch,
    q_notesdblist,
    q_roottypes
  } q = q_error;
  symchar *type=NULL;
  symchar *method=NULL;
  symchar *child=NULL;
  symchar *language=NULL;
  symchar *dbid=NULL;
  int mtype=0;
  int i;
  int status;
  std::intptr_t noteptr = (std::intptr_t)NULL; /* parsed as long and cast to ptr. */
  std::intptr_t tokenptr = (std::intptr_t)NULL; /* parsed as long and cast to ptr. */
  char *pattern = NULL;
  struct TypeDescription *desc=NULL;

  OLD_ASCUSE;  /* see if first arg is -help */

  if (argc < 2) {
    Asc_DStringAppend(hptr,Asc_LibrQueryTypeCmdHN " called without arguments",
                     HALL);
    return HELP_ERROR;
  }
  if (strcmp(argv[1],"-ancestors")==0) {
    q = q_ancestors;
  }
  if (strcmp(argv[1],"-basemethods")==0) {
    q = q_basemethods;
  }
  if (strcmp(argv[1],"-catalog")==0) {
    q = q_catalog;
  }
  if (strcmp(argv[1],"-childnames")==0) {
    q = q_childnames;
  }
  if (strcmp(argv[1],"-childinfo")==0) {
    q = q_childinfo;
  }
  if (strcmp(argv[1],"-definition")==0) {
    q = q_definition;
  }
  if (strcmp(argv[1],"-exists")==0) {
    q = q_exists;
  }
  if (strcmp(argv[1],"-externalfunctions")==0) {
    q = q_externalfunctions;
  }
  if (strcmp(argv[1],"-findtype")==0) {
    q = q_findtype;
  }
  if (strcmp(argv[1],"-filetypes")==0) {
    q = q_filetypes;
  }
  if (strcmp(argv[1],"-fundamentals")==0) {
    q = q_fundamentals;
  }
  if (strcmp(argv[1],"-language")==0) {
    q = q_language;
  }
  if (strcmp(argv[1],"-methods")==0) {
    q = q_methods;
  }
  if (strcmp(argv[1],"-modulelist")==0) {
    q = q_modulelist;
  }
  if (strcmp(argv[1],"-notesdblist")==0) {
    q = q_notesdblist;
  }
  if (strcmp(argv[1],"-notes")==0) {
    q = q_notes;
  }
  if (strcmp(argv[1],"-notesdump")==0) {
    q = q_notesdump;
  }
  if (strcmp(argv[1],"-notekinds")==0) {
    q = q_notekinds;
  }
  if (strcmp(argv[1],"-notesmatch")==0) {
    q = q_notesmatch;
  }
  if (strcmp(argv[1],"-roottypes")==0) {
    q = q_roottypes;
  }
  if (q==q_error) {
    Asc_DStringAppend(hptr,"Unknown option to "
                     Asc_LibrQueryTypeCmdHN ": ",HALL);
    Asc_DStringAppend(hptr,argv[1], HALL);
    return HELP_ERROR;
  }
  /* pick off the options */
  for (i=2; i < argc; /* ifs do increment */ ) {
    if (strcmp(argv[i],"-type")==0) {
      if (i < (argc-1)) {
        type = AddSymbol(argv[i+1]);
      }
      i += 2;
      continue;
    }
    if (strcmp(argv[i],"-dbid")==0) {
      if (i < (argc-1)) {
        dbid = AddSymbol(argv[i+1]);
      }
      i += 2;
      continue;
    }
    if (strcmp(argv[i],"-child")==0) {
      if (i < (argc-1)) {
        child = AddSymbol(argv[i+1]);
      }
      i += 2;
      continue;
    }
    if (strcmp(argv[i],"-method")==0) {
      if (i < (argc-1)) {
        method = AddSymbol(argv[i+1]);
      }
      i += 2;
      continue;
    }
    if (strcmp(argv[i],"-pattern")==0) {
      if (i < (argc-1)) {
        pattern = argv[i+1];
      }
      i += 2;
      continue;
    }
    if (strcmp(argv[i],"-language")==0) {
      if (i < (argc-1)) {
        language = AddSymbol(argv[i+1]);
      }
      i += 2;
      continue;
    }
    if (strcmp(argv[i],"-destroytoken")==0 ||
        strcmp(argv[i],"-notestoken")==0) {
      if (i < (argc-1)) {
        status = JTcl_GetLong(hptr, argv[i+1],&tokenptr);
        if (status != HELP_OK) {
          return status;
        }
      }
      i += 2;
      continue;
    }
    if (strcmp(argv[i],"-record")==0) {
      if (i < (argc-1)) {
        status = JTcl_GetLong(hptr, argv[i+1],&noteptr);
        if (status != HELP_OK) {
          return status;
        }
      }
      i += 2;
      continue;
    }
    if (strcmp(argv[i],"-mtype")==0 ||
        strcmp(argv[i],"-textwidth")==0) {
      if (i < (argc-1)) {
        status = JTcl_GetInt(hptr, argv[i+1],&mtype);
        if (status != HELP_OK) {
          return status;
        }
      }
      i += 2;
      continue;
    }
    Asc_DStringAppend(hptr,"Unknown option to " Asc_LibrQueryTypeCmdHN, HALL);
    Asc_DStringAppend(hptr, argv[i], HALL);
    return HELP_ERROR;
  }

  if (type != NULL) {
    desc = FindType(type);
    if (q != q_exists && desc == NULL) {
      Asc_DStringAppend(hptr,"Unknown type to " Asc_LibrQueryTypeCmdHN,HALL);
      Asc_DStringAppend(hptr, SCP(type), HALL);
      return HELP_ERROR;
    }
  }
  switch (q) {
  case q_ancestors:
    return LibrAncestorType(hptr,desc);
  case q_basemethods:
    return LibrModelDefinitionMethods(hptr);
  case q_catalog:
    return LibrCatalog(hptr);
  case q_childnames:
    return LibrTypeChildren(hptr,desc);
  case q_childinfo:
    return LibrChildInfo(hptr,desc,child);
  case q_definition:
    return LibrUnimplemented(hptr, argv);
  case q_exists:
    if (desc==NULL) {
      Asc_DStringSet(hptr,"0");
    } else {
      Asc_DStringSet(hptr,"1");
    }
    return HELP_OK;
  case q_externalfunctions:
    return LibrExternalFuncs(hptr);
  case q_findtype:
    return LibrFindType(hptr,desc);
  case q_filetypes:
    return LibrFileExtsCmd(hptr);
  case q_fundamentals:
    return LibrGetFundamentals(hptr);
  case q_language:
    return LibrUnimplemented(hptr, argv);
  case q_methods:
    return LibrMethods(hptr,desc);
  case q_modulelist:
    return LibrModuleList(hptr,mtype);
  case q_notekinds:
    return LibrNoteLangs(hptr,dbid);
  case q_notesdblist:
    return LibrNoteDBList(hptr);
  case q_notes:
    return LibrGetNotes(hptr,type,language,child,method,
                        noteptr,tokenptr,dbid);
  case q_notesmatch:
    return LibrMatchNotes(hptr,pattern,tokenptr,dbid);
  case q_notesdump:
    if (mtype == 0) {
      mtype = 15;
    }
    return LibrDumpNotes(hptr,mtype,tokenptr,dbid);
  case q_roottypes:
    return LibrRootTypes(hptr);
  default:
    Asc_DStringAppend(hptr,"Unhandled option in " Asc_LibrQueryTypeCmdHN ": ",HALL);
    Asc_DStringAppend(hptr, argv[1], HALL);
    break;
  }
  return HELP_ERROR;
}
