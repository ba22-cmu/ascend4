/*	
	fixme
*//*
 *  SimsProc.ipp
*/

#define ASC_BUILDING_INTERFACE

#include "config.h"

#include <ascend/utilities/bit.h>
#include <ascend/compiler/pending.h>
#include <ascend/compiler/simlist.h>
#include <ascend/compiler/library.h>
#include <ascend/compiler/instquery.h>
#include <ascend/compiler/symtab.h>
#include <ascend/compiler/createinst.h>
#include <ascend/compiler/instantiate.h>
#include <ascend/compiler/check.h>
#include <ascend/compiler/copyinst.h>
#include <ascend/compiler/destroyinst.h>
#include <ascend/compiler/prototype.h>
#include <ascend/compiler/instance_io.h>
#include <ascend/compiler/statio.h>
#include <ascend/compiler/statement.h>
#include <ascend/compiler/bintoken.h>

#include <ascend/general/ascMalloc.h>
#include <ascend/general/tm_time.h>
#include <ascend/general/list.h>
#include <ascend/general/dstring.h>
#include <ascend/general/tm_time.h>

#include <ascend/compiler/qlfdid.h>
#include <ascend/system/slv_types.h>

#include <ctype.h>

int ascjson::Asc_SimsQueryCmdDS(  Asc_DString *hptr, int argc, CONST84 char *argv[])
{
  struct Instance *sim;

  if (( argc == 2 ) && ( (strncmp(argv[1],"getcurrent",4)) == 0 )) {
    sim = Asc_GetCurrentSim();
    if (sim) {
      Asc_DStringAppend(hptr,(char *)SCP(GetSimulationName(sim)), HALL);
      return HELP_OK;
    } else {
      Asc_DStringSet(hptr, "1");
      return HELP_OK;
    }
  }

  if (( argc == 3 ) && ( (strncmp(argv[1],"setcurrent",4)) == 0 )) {
    sim = Asc_FindSimulationTop(AddSymbol(argv[2]));
    if (sim) {
      Asc_SetCurrentSim(sim);
      Asc_DStringSet(hptr, "0");
      return HELP_OK;
    } else {
      Asc_DStringSet(hptr, "1");
      return HELP_OK;
    }
  }

  Asc_DStringSet(hptr,"Invalid args: Usage ");
  Asc_DStringAppend(hptr, argv[0], HALL);
  Asc_DStringAppend(hptr, " <setcurrent,getcurrent>", HALL);
  return HELP_ERROR;
}

int ascjson::Asc_SimsUniqueNameCmdDS(Asc_DString *hptr, int argc, CONST84 char *argv[])
{
  if ( argc != 2 ) {
     Asc_DStringSet(hptr, "call is: unique \"name\"");
     return HELP_ERROR;
  }
  if (Asc_SimsUniqueName(AddSymbol(argv[1]))) {
     Asc_DStringSet(hptr, "1");
  } else {
     Asc_DStringSet(hptr, "0");
  }
  return HELP_OK;
}

int ascjson::Asc_SimsCreateInstanceCmdDS( Asc_DString *hptr, int argc, CONST84 char *argv[])
{
  struct TypeDescription *type;
  struct Instance *inew;
  symchar *name, *defmethod;
  int len, k;
  enum CreateInst_format format;

  if (( argc < 3 ) || ( argc > 6 )) {
    Asc_DStringAppend(hptr,"wrong # args:"
                     "Usage sim_create <instance_name> <instance_type>"
                     "[-m defaultMethod] [-p,-r]", HALL);
    return HELP_ERROR;
  }

  name = AddSymbol(argv[1]);
  len = SCLEN(name);
  if ((len<1) || (len > 70)) {
    Asc_DStringSet(hptr, "Simulation name too long (> 70)");
    return HELP_ERROR;
  }
  if (isdigit(SCP(name)[0])) {
    Asc_DStringSet(hptr, "Instance names cannot begin with a digit");
    return HELP_ERROR;
  }

  type = FindType(AddSymbol(argv[2]));
  if (type==NULL) {
    Asc_DStringAppend(hptr,"CreateInstance called with unknown type", HALL);
    return HELP_ERROR;
  }

  /*
   * Check for additional compilation flags.
   */
  format = e_normal;			/* do standard compilation */
  k = 3;
  defmethod = AddSymbol("default_self"); /* when no input with -m */
  while (k < argc) {
    if (strcmp(argv[k],"-r")==0) {
      format = e_no_relations;			/* do no relations */
      k++;
      continue;
    }
    if (strcmp(argv[k],"-p")==0) {
#if 0 // not currently supported
      format = e_patch;			/* do patch compilation */
#endif
      k++;
      continue;
    }
    if (strcmp(argv[k],"-m")==0) {
      if ( (k+1)== argc) {
        k++;
        defmethod = NULL; /* -m <noargs> --> no defaults */
      } else {
        defmethod = AddSymbol(argv[k+1]);
        k += 2;
      }
      continue;
    }
    Asc_DStringAppend(hptr,"Unknown compilation flag",HALL);
    return HELP_ERROR;
  }

  if (Asc_SimsUniqueName(name)) {
#if 0 // bintoken not available in a wasm environment.
    Tcl_GlobalEval(hptr,"Sims_SetupBinTokenCC"); /// FIXME
#endif
    inew = SimsCreateInstance(GetName(type), name, format, defmethod);
    if (inew != NULL) {
      gl_insert_sorted(g_simulation_list,inew,(CmpFunc)Asc_SimsCmpSim);
      Asc_DStringSet(hptr, "0");
      return HELP_OK;
    } else { /* Instantiate returned null */
      Asc_DStringAppend(hptr,"Simulation of type ", HALL);
      Asc_DStringAppend(hptr, argv[2], HALL);
      Asc_DStringAppend(hptr, " with name ", HALL);
      Asc_DStringAppend(hptr, SCP(name), HALL);
      Asc_DStringAppend(hptr, " cannot be instantiated.", HALL);
      return HELP_ERROR;
    }
  } else {				/* sim already exists */
    Asc_DStringAppend(hptr,"A simulation with this name already exists:",HALL);
    Asc_DStringAppend(hptr, SCP(name), HALL);
    return HELP_ERROR;
  }
#ifndef NDEBUG
  return HELP_OK; /* NOT reached */
#endif
}

/*
 *********************************************************************
 * Resume Instantiation functions
 *
 * An experimental UpdateInstanceCmd is included here.
 *
 * Query ? Is it possible for Reinstantiate to move the root instance ?
 * Not sure. At the moment it does not return an instance pointer.
 *********************************************************************
 */

int ascjson::Asc_SimsResumeInstantiateCmdDS(Asc_DString *hptr, int argc, CONST84 char *argv[])
{
  double start_time =0;
  struct Instance *xisting;

  if ( argc != 2 ) {
    Asc_DStringSet(hptr, "call is: sresume <instancename> ");
    return HELP_ERROR;
  }
  xisting = Asc_FindSimulationRoot(AddSymbol(argv[1]));
  if (xisting) {
    if (g_compiler_timing) {
      start_time = tm_cpu_time();
    }
    ReInstantiate(xisting);
    if (g_compiler_timing) {
      FPRINTF(stdout,"Reinstantiation CPU time = %g seconds\n",
              tm_cpu_time() - start_time);
    }
    Asc_DStringSet(hptr, "0");
    return HELP_OK;
  } else {
    Asc_DStringSet(hptr,"Instance not found: ");
    Asc_DStringAppend(hptr,argv[1], HALL);
    return HELP_ERROR;
  }
}

#if 0 && defined(DISUSED)
int ascjson::Asc_SimsUpdateInstanceCmdDS(Asc_DString *hptr, int argc, CONST84 char *argv[])
{
  struct Instance *target;
  struct TypeDescription *desc, *patchdef;
  CONST struct StatementList *slist;
  double start_time;


  if ( argc != 3 ) {
    Asc_DStringSet(hptr, "call is: __sims_update qlfdid type");
    return HELP_ERROR;
  }
  /*
   * Find the target instance, and leave g_search_inst
   * looking at it.
   */
  if (Asc_QlfdidSearch3(argv[1],0)) {
    Asc_DStringSet(hptr, "target instance not found");
    return HELP_ERROR;
  }
  target = g_search_inst;

  patchdef = FindType(AddSymbol(argv[2]));
  if (!patchdef) {
    Asc_DStringSet(hptr, "Cannot find patch type");
    return HELP_ERROR;
  }
  desc = InstanceTypeDesc(target);
  if (GetPatchOriginal(patchdef)!=desc) {
    Asc_DStringSet(hptr, "Inconsistent types in patch");
    return HELP_ERROR;
  }

  start_time = tm_cpu_time();		/* do the update */
  slist = GetStatementList(patchdef);
  UpdateInstance(g_root,target,slist);
  if (g_compiler_timing) {
    PRINTF("Reinstantiation CPU time = %g seconds\n",
             tm_cpu_time() - start_time);
  }

  return HELP_OK;
}
#endif


int ascjson::Asc_SimsCopyInstanceCmdDS( Asc_DString *hptr, int argc, CONST84 char *argv[])
{
  struct Instance *target;
  double start_time;

  if ( argc != 2 ) {
    Asc_DStringSet(hptr, "call is: __sims_copy qlfdid");
    return HELP_ERROR;
  }

  /*
   * Find the target instance, and leave g_search_inst
   * looking at it.
   */
  if (Asc_QlfdidSearch3(argv[1],0)) {
    Asc_DStringSet(hptr, "target instance not found");
    return HELP_ERROR;
  }

  start_time = tm_cpu_time();
  target = g_search_inst;
  target = CopyInstance(target);
  start_time = tm_cpu_time() - start_time;
  FPRINTF(stderr,"Time to copy instance = %g\n",start_time);
  DestroyInstance(target,NULL);

  return HELP_OK;
}


int ascjson::Asc_SimsProtoTypeInstanceCmdDS(Asc_DString *hptr, int argc, CONST84 char *argv[])
{
  struct Instance *target, *result;
  CONST struct TypeDescription *desc;
  double start_time;

  if ( argc != 2 ) {
    Asc_DStringSet(hptr, "call is: __sims_proto qlfdid");
    return HELP_ERROR;
  }

  if (Asc_QlfdidSearch3(argv[1],0)) {
    Asc_DStringSet(hptr, "target instance not found");
    return HELP_ERROR;
  }

  start_time = tm_cpu_time();
  target = g_search_inst;
  switch (InstanceKind(target)) {
  case ARRAY_ENUM_INST:
  case ARRAY_INT_INST:
  case SIM_INST:
    Asc_DStringSet(hptr, "Cannot prototype this array or sim instances");
    return HELP_ERROR;
  default:
    break;
  }

  desc = InstanceTypeDesc(target);
  if (LookupPrototype(GetName(desc))) {
    Asc_DStringSet(hptr, "a prototype already exists");
    return HELP_OK;
  }
  result = CopyInstance(target);
  start_time = tm_cpu_time() - start_time;
  if (result) {
    AddPrototype(result);
    FPRINTF(stderr,"Time to prototype instance = %g\n",start_time);
    return HELP_OK;
  } else {
    Asc_DStringSet(hptr, "Error in prototyping instance");
    return HELP_ERROR;
  }
}


int ascjson::Asc_SimsSaveInstanceCmdDS( Asc_DString *hptr, int argc, CONST84 char *argv[])
{
  FILE *fp = NULL;
  struct Instance *target;
  double start_time;

  if ( argc != 3 ) {
    Asc_DStringSet(hptr, "call is: __sims_saveinst qlfdid file");
    return HELP_ERROR;
  }

  /*
   * Find the target instance, and leave g_search_inst
   * looking at it.
   */
  if (Asc_QlfdidSearch3(argv[1],0)) {
    Asc_DStringSet(hptr, "target instance not found");
    return HELP_ERROR;
  }
  /*
   * Set up the file pointer.
   */
  fp = fopen(argv[2],"w");
  if (!fp) {
    Asc_DStringSet(hptr, "error in opening file");
    return HELP_ERROR;
  }

  start_time = tm_cpu_time();
  target = g_search_inst;
  SaveInstance(fp,target,0);
  start_time = tm_cpu_time() - start_time;
  FPRINTF(stderr,"Time to save instance = %g\n",start_time);

  if (fp) {
    fclose(fp);
  }
  return HELP_OK;
}



/*
 *********************************************************************
 * Destroy Simulation functions
 *
 * The DestroyInstance code correctly deals with *all*
 * instances. As such SIM_INST are correctly handled as well
 * DO NOT refer to it after this. Parts of the sim instance that
 * are shared by other instances, (as in the case of universals)
 * are properly handled, and will not be deallocated unless this
 * simulation is the only parent.
 *********************************************************************
 */

int ascjson::Asc_SimsDestroySimulationCmdDS(Asc_DString *hptr, int argc, CONST84 char *argv[])
{
  struct Instance *xisting, *current;
  unsigned long ndx;

  if ( argc != 2 ) {
    Asc_DStringSet(hptr, "wrong # args: Usage: sim_destroy <simname>");
    return HELP_ERROR;
  }
  xisting = Asc_FindSimulationTop(AddSymbol(argv[1]));
  if (xisting) {
    /* the next line grabs the index in the list */
    ndx = gl_search(g_simulation_list,xisting,(CmpFunc)Asc_SimsCmpSim);
    current = Asc_GetCurrentSim();
    if (current) {
      if (Asc_SimsCmpSim(xisting,current)==0) {
        Asc_SetCurrentSim(NULL);
        ::sim_destroy(xisting);
        gl_delete(g_simulation_list,ndx,0);
        Asc_DStringSet(hptr,"Simulation deleted: ");
	Asc_DStringAppend(hptr, argv[1], HALL);
        return HELP_OK;
      }
    }
    ::sim_destroy(xisting);		/* prepares for cleanup */
    gl_delete(g_simulation_list,ndx,0);
    Asc_DStringAppend(hptr,"Simulation deleted: ", HALL);
    Asc_DStringAppend(hptr,argv[1], HALL);
    return HELP_OK;
  } else {
    Asc_DStringAppend(hptr,"Simulation not found: ", HALL);
    Asc_DStringAppend(hptr, argv[1], HALL);
    return HELP_ERROR;
  }
}

int ascjson::Asc_BrowShowPendingsDS(Asc_DString *hptr, int argc, CONST84 char *argv[])
{
  struct Instance *i = NULL;
  char buf[MAXIMUM_NUMERIC_LENGTH];       /* string to hold long */
  unsigned long p=0L;

  if ( argc != 3 ) {
    Asc_DStringAppend(hptr,"wrong # args: Usage: "
                     "\"bnumpendings\" ?sim?inst ?simname?current?search?",HALL);
    return HELP_ERROR;
  }
  if ( (strncmp(argv[1],"simulation",3)) == 0 ) {
    i = Asc_FindSimulationRoot(AddSymbol(argv[2]));
  } else if ( (strncmp(argv[1],"instance",3)) == 0 ) {
    if ( (strncmp(argv[2],"current",3)) == 0 ) {
      i = g_curinst;
    } else {
      i = g_search_inst;
    }
  }
  if (!i) {
    Asc_DStringSet(hptr, "10000"); /* Just some big number */
    return HELP_OK;
  }
  p = NumberPendingInstances(i);
  if (p>0L) {
    sprintf(buf,"%lu",p);
    Asc_DStringSet(hptr, buf);
  }
  return HELP_OK;
}

static
void BrowWritePendingStatements(FILE *f, CONST struct Instance *i)
{
  CONST struct BitList *blist;
  CONST struct TypeDescription *desc;
  CONST struct StatementList *slist;
  CONST struct Statement *stat;
  CONST struct gl_list_t *list;
  unsigned long c,len;
  blist = InstanceBitList(i);
  if ((blist!=NULL)&&(!BitListEmpty(blist))) {
    FPRINTF(stderr,"PENDING STATEMENTS\n");
    desc = InstanceTypeDesc(i);
    slist = GetStatementList(desc);
    list = GetList(slist);
    len = gl_length(list);
    for(c=1;c<=len;c++) {
      if (ReadBit(blist,c-1)) {
        stat = (struct Statement *)gl_fetch(list,c);
        WriteStatement(f,stat,4);
        if (StatementType(stat)== SELECT) {
          c = c + SelectStatNumberStats(stat);
        }
      }
    }
  }
}

int ascjson::Asc_BrowWritePendingsSTDOUTDS(Asc_DString *hptr, int argc, CONST84 char *argv[])
{
  struct Instance *i;

  if ( argc != 2 ) {
    Asc_DStringSet(hptr, "wrong # args : Usage \"bwritependings\" simname");
    return HELP_ERROR;
  }
  i = Asc_FindSimulationRoot(AddSymbol(argv[1]));
  if (!i) {
    Asc_DStringSet(hptr, "given simulation nonexistent !");
    return HELP_ERROR;
  }
  BrowWritePendingStatements(stdout,i);
  return HELP_OK;
}

int ascjson::Asc_SimListPendingDS( Asc_DString *hptr, int argc, CONST84 char *argv[])
{
  struct Instance *i;
  FILE *fp = stdout;

  if (( argc != 2 ) && ( argc != 3 )) {
    Asc_DStringSet(hptr,
                  "simlistpending: simlistpending simname [optional filename]");
    return HELP_ERROR;
  }
  i = Asc_FindSimulationRoot(AddSymbol(argv[1]));
  if (!i) {
    Asc_DStringSet(hptr, "simlistpending: given simulation not found!");
    return HELP_ERROR;
  }
  if ( argc == 3 ) {
    fp = fopen(argv[2],"w+");
    if (fp==NULL) {
      Asc_DStringSet(hptr, "simlistpending: given bad filename");
      return HELP_ERROR;
    }
  }
  FPRINTF(fp,"\n-------------------------------------\n");
  FPRINTF(fp," Pendings statements for simulation %s\n\n",argv[1]);
  CheckInstance(fp,i);
  FPRINTF(fp,"\n-------------------------------------\n");
  if ( argc == 3 ) {
    fclose(fp);
  }
  return HELP_OK;
}


STDHLF(Asc_SimBinTokenSetOptions, (Asc_SimBinTokenSetOptionsHL,Asc_SimBinTokenSetOptionsHL2,HLFSTOP));
int ascjson::Asc_SimBinTokenSetOptionsDS( Asc_DString *hptr, int argc, CONST84 char **argv)
{
  const char *srcname, *objname, *libname, *buildcommand, *unlinkcommand;
  long maxrels;
  int verbose, housekeep;
  int s1,s2,s3;

  if (argc != 9 ) {
    Asc_HelpGetUsageDS(hptr,Asc_SimBinTokenSetOptionsHN);
    return HELP_ERROR;
  };

  srcname = argv[1];
  objname = argv[2];
  libname = argv[3];
  buildcommand = argv[4];
  unlinkcommand = argv[5];
  s1 = JTcl_GetLong(hptr, argv[6],&maxrels);
  Asc_DStringFree(hptr);
  s2 = JTcl_GetInt(hptr, argv[7],&verbose);
  Asc_DStringFree(hptr);
  s3 = JTcl_GetInt(hptr, argv[8],&housekeep);
  Asc_DStringFree(hptr);
  if (srcname == NULL || objname == NULL || libname == NULL ||
      buildcommand == NULL || unlinkcommand == NULL ||
      s1 != HELP_OK || s2 != HELP_OK || s3 != HELP_OK) {
    Asc_DStringAppend(hptr, "Asc_SimBinTokenSetOptions input bogosity", HALL);
    return HELP_ERROR;
  }
  BinTokenSetOptions(srcname,objname,libname,buildcommand,unlinkcommand,
                     (unsigned long)maxrels,verbose,housekeep);
  return HELP_OK;
}
