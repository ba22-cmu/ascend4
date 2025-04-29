/*
 *  SolverProc.c
 *  by Kirk Abbott and Ben Allan
 *  Created: 1/94
 *  Version: $Revision: 1.65 $
 *  Version control file: $RCSfile: SolverProc.c,v $
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
 */

#define ASC_BUILDING_INTERFACE

#define QLFDID_LENGTH 1023
#define YORN(b) ((b) ? "YES" : "NO")
#define ONEORZERO(b) ((b) ? "1" : "0")
#define SP_DEBUG FALSE
/* if true, prints out extra error messages */

/* global variables: */

int ascjson::g_solvinst_ndx, ascjson::g_solvinst_limit;
extern unsigned long g_unresolved_count;

struct Instance *ascjson::g_solvinst_root=NULL, /* root instan (child of simulation) */
                *ascjson::g_solvinst_cur=NULL;  /* top model instance to be solved */

slv_system_t ascjson::g_solvsys_cur=NULL;        /* a pointer to slv_system_structure */
slv_system_t ascjson::g_browsys_cur=NULL;        /* a pointer to slv_system_structure */

void ascjson::Asc_SolvMemoryCleanup()
{
  system_free_reused_mem();
}

#ifdef ASC_SIGNAL_TRAPS
static
void slv_trap_int(int sigval)
{

  (void)sigval;   /* stop gcc whine about unused parameter */

  FPRINTF(stdout,"\nascend4: SIGINT caught.\n");
  Solv_C_CheckHalt_Flag = 1; /* need to set the tcl var */
#if 0 // fixme: this needs to be replaced with JS var setting
  Asc_DString *hptr = g_hptr;  /*  a local ptr to the global hptr ptr */
  Tcl_SetVar2(hptr,"ascSolvStatVect","menubreak","1",TCL_GLOBAL_ONLY);
#endif
  ascjson::Asc_ScriptInterrupt = 1;
  Asc_SetMethodUserInterrupt(1);
  FPRINTF(stdout,"Type 'exit', or click Toolbox/exit/Confirm to quit.\n");
  Asc_SignalRecover(0);
  // Asc_Prompt(hptr,0); js equivalent?
}

int ascjson::Asc_SolvTrapFP(Asc_DString *hptr, int argc, CONST84 char *argv[])
{
  Asc_SignalHandlerPush(SIGFPE,Asc_SignalTrap);
  Asc_SignalHandlerPush(SIGINT,slv_trap_int);
  return HELP_OK;
}

int ascjson::Asc_SolvUnTrapFP(Asc_DString *hptr, int argc, CONST84 char *argv[])
{
  Asc_SignalHandlerPop(SIGFPE,Asc_SignalTrap);
  return HELP_OK;
}

int ascjson::Asc_SolvTrapINT(Asc_DString *hptr, int argc, CONST84 char *argv[])
{
  Asc_SignalHandlerPush(SIGINT,slv_trap_int);
  return HELP_OK;
}

int ascjson::Asc_SolvUnTrapINT(Asc_DString *hptr, int argc, CONST84 char *argv[])
{
  Asc_SignalHandlerPop(SIGINT,slv_trap_int);
  return HELP_OK;
}
#endif /* ASC_SIGNAL_TRAPS */

int ascjson::Asc_SolvGetModKids(Asc_DString *hptr, int argc, CONST84 char *argv[])
{
  char tmps[QLFDID_LENGTH+1];
  struct Instance *modinst_root=NULL;  /* model instance */
  struct Instance *aryinst_root=NULL;  /* possible model instance */
  struct Instance *aryinst=NULL;       /* possible model instance kid*/
  struct InstanceName rec;
  enum inst_t ikind,aikind;
  unsigned long len,c,aryc,arylen;
  int status;

  if ( argc != 2 ) {
    Asc_DStringSet(hptr, "expected get_model_children <qlfdid>");
    return HELP_ERROR;
  }

  status = Asc_QlfdidSearch3(argv[1],0);
  if (status==0) {
    modinst_root = g_search_inst;	/* catch inst ptr */
  } else {
    Asc_DStringAppend3(hptr,"get_model_children: QlfdidSearch error: ",
                     argv[1], " not found",HALL);
    return HELP_ERROR;
  }

  /* check that instance is model */
  ikind=InstanceKind(modinst_root);
  if (ikind!=MODEL_INST && ikind!=ARRAY_INT_INST && ikind!= ARRAY_ENUM_INST) {
    FPRINTF(ASCERR,  "Instance specified is not a model or array.\n");
    Asc_DStringSet(hptr,
	  "Only MODEL and ARRAY instances may have model children.");
    return HELP_ERROR;
  }

  len=NumberChildren(modinst_root);
  for (c=1;c<=len;c++) {
    ikind=InstanceKind(InstanceChild(modinst_root,c));
    switch (ikind) {
      case MODEL_INST:
        VTcl_AppendElement(hptr,
         SCP(InstanceNameStr(ChildName(modinst_root,c))));
        break;
      case ARRAY_INT_INST:
      case ARRAY_ENUM_INST: /*dumpary names*/
        aryinst_root=InstanceChild(modinst_root,c);
        arylen=NumberChildren(aryinst_root);
        for (aryc=1;aryc<=arylen;aryc++) {
          aryinst=InstanceChild(aryinst_root,aryc);
          aikind=InstanceKind(aryinst);
          switch (aikind) {
            case MODEL_INST:
            case ARRAY_INT_INST: /* write array names in case any children */
            case ARRAY_ENUM_INST: /* are models */
              rec=ChildName(aryinst_root,aryc);
              Asc_BrowWriteNameRec(&tmps[0],&rec);
              Asc_DStringAppend4(hptr," {",
                SCP(InstanceNameStr(ChildName(modinst_root,c))),&tmps[0],"}",HALL);
            default: /*write nothing */
              break;
          }
        }
        break;
      default: /* write nothing if its not a model or ary child */
        break;
    }
  }
  VAEstrip(hptr);
  return HELP_OK;
}

int ascjson::Asc_SolvIncompleteSim(Asc_DString *hptr, int argc, CONST84 char *argv[])
{
  unsigned long pendings;

  if ( argc != 2 ) {
    FPRINTF(ASCERR,  "call is: slv_checksim <simname>\n");
    Asc_DStringSet(hptr, "error in call to slv_checksim");
    return HELP_ERROR;
  }


  g_solvinst_root = Asc_FindSimulationRoot(AddSymbol(argv[1]));


  if (!g_solvinst_root) {
    FPRINTF(ASCERR, "Solve called with NULL root instance.\n");
    Asc_DStringSet(hptr, "Simulation specified not found.");
    return HELP_ERROR;
  }
  pendings = NumberPendingInstances(g_solvinst_root);
  if (pendings>0) {
    FPRINTF(ASCERR,"Found %lu pendings.",pendings);
    Asc_DStringSet(hptr, "1");
  } else {
    Asc_DStringSet(hptr, "0");
  }
  return HELP_OK;
}

int ascjson::Asc_SolvCheckSys(Asc_DString *hptr, int argc, CONST84 char *argv[])
{
  if (g_solvsys_cur != NULL) {
    Asc_DStringSet(hptr, "1");
  } else {
    Asc_DStringSet(hptr, "0");
  }
    return HELP_OK;
}

int ascjson::Asc_SolvGetObjList(Asc_DString *hptr, int argc, CONST84 char *argv[])
{
  int32 *rip=NULL;
  char tmps[MAXIMUM_NUMERIC_LENGTH];
  int i,dev,status;
  FILE *fp;

  if ( argc != 2 ) {
    FPRINTF(ASCERR,  "call is: slv_get_obj_list <out>\n");
    Asc_DStringSet(hptr, "slv_get_obj_list wants output device.");
    return HELP_ERROR;
  }
  if (g_solvsys_cur==NULL) {
    FPRINTF(ASCERR,  "slv_get_obj_list called with NULL pointer\n");
    Asc_DStringSet(hptr, "slv_get_obj_list called without slv_system");
    return HELP_ERROR;
  }
  /* get io option */
  i=3;
  status=JTcl_GetInt(hptr, argv[1],&i);
  if (i<0 || i >2) {
    status=HELP_ERROR;
  }
  if (status!=HELP_OK) {
    FPRINTF(ASCERR,"slv_get_obj_list: first arg is 0,1, or 2\n");
    Asc_DStringFree(hptr);
    Asc_DStringSet(hptr, "slv_get_obj_list: invalid output dev #");
    return status;
  } else {
    dev=i;
  }
  switch (dev) {
  case 0: fp=stdout;
    break;
  case 1: fp=ASCERR;
    break;
  case 2: fp=NULL;
    break;
  default : /* should never be here */
    FPRINTF(ASCERR,"slv_get_obj_list called with strange i/o option\n");
    return HELP_ERROR;
  }
  if (slv_obj_select_list(g_solvsys_cur,&rip)) {
    switch (dev) {
    case 0:
    case 1:
      FPRINTF(fp,"Objective indices:\n");
      for (i=0;rip[i]>-1;i++) {
        FPRINTF(fp,"%d\n",rip[i]);
      }
      break;
    case 2:
      Asc_DStringAppend(hptr,"{",1);
      for (i=0;rip[i]>-1;i++) {
        sprintf(tmps,"%d ",rip[i]);
        Asc_DStringAppend(hptr,tmps,HALL);
      }
      Asc_DStringAppend(hptr,"}",1);
      break;
    default:
      FPRINTF(ASCERR,"wierdness in i/o!");
      break;
    }
    ascfree(rip);
  } else {
    Asc_DStringSet(hptr, "{}");
  }
  return HELP_OK;
}

int ascjson::Asc_SolvSetObjByNum(Asc_DString *hptr, int argc, CONST84 char *argv[])
{
  int32 i,status,len;
  struct rel_relation **rlist=NULL;

  if ( argc != 2 ) {
    FPRINTF(ASCERR,  "call is: slv_set_obj_by_num <num>\n");
    Asc_DStringSet(hptr, "slv_set_obj_by_num wants objective number.");
    return HELP_ERROR;
  }
  if (g_solvsys_cur==NULL) {
    FPRINTF(ASCERR,  "slv_set_obj_by_num called with NULL pointer\n");
    Asc_DStringSet(hptr, "slv_set_obj_by_num called without slv_system");
    return HELP_ERROR;
  }
  /* get io option */
  i=0;
  status=JTcl_GetInt(hptr, argv[1],&i);
  len = slv_get_num_solvers_objs(g_solvsys_cur);

  if (i == -1) { /* remove objective and return */
    slv_set_obj_relation(g_solvsys_cur,NULL);
    return HELP_OK;
  }
  if (i<0 || i >= len) {
    status=HELP_ERROR;
  }
  if (status!=HELP_OK) {
    FPRINTF(ASCERR,"slv_set_obj_by_num: invalid objective number\n");
    Asc_DStringFree(hptr);
    Asc_DStringSet(hptr, "slv_set_obj_by_num: invalid objective number");
    return status;
  } else {
    rlist = slv_get_solvers_obj_list(g_solvsys_cur);
    slv_set_obj_relation(g_solvsys_cur,rlist[i]);
  }
  return HELP_OK;
}

STDHLF(Asc_SolvGetObjNumCmd,(Asc_SolvGetObjNumCmdHL,HLFSTOP));
int ascjson::Asc_SolvGetObjNumCmd(Asc_DString *hptr, int argc, CONST84 char *argv[])
{
  char tmps[MAXIMUM_NUMERIC_LENGTH];
  int num,i,dev,status;
  FILE *fp;

  if ( argc != 2 ) {
    FPRINTF(ASCERR,  "call is: slv_get_obj_num <out>\n");
    Asc_DStringSet(hptr, "slv_get_obj_num wants output device.");
    return HELP_ERROR;
  }
  if (g_solvsys_cur==NULL) {
    FPRINTF(ASCERR,  "slv_get_obj_num called with NULL pointer\n");
    Asc_DStringSet(hptr, "slv_get_obj_num called without slv_system");
    return HELP_ERROR;
  }
  /* get io option */
  i=3;
  status=JTcl_GetInt(hptr, argv[1],&i);
  if (i<0 || i >2) {
    status=HELP_ERROR;
  }
  if (status!=HELP_OK) {
    FPRINTF(ASCERR,"slv_get_obj_num: first arg is 0,1, or 2\n");
    Asc_DStringFree(hptr);
    Asc_DStringSet(hptr, "slv_get_obj_num: invalid output dev #");
    return status;
  } else {
    dev=i;
  }
  switch (dev) {
  case 0: fp=stdout;
    break;
  case 1: fp=ASCERR;
    break;
  case 2: fp=NULL;
    break;
  default : /* should never be here */
    FPRINTF(ASCERR,"slv_get_obj_num called with strange i/o option\n");
    return HELP_ERROR;
  }
  num = ::slv_get_obj_num(g_solvsys_cur);
  switch (dev) {
  case 0:
  case 1:
    FPRINTF(fp,"Objective index: ");
    FPRINTF(fp,"%d\n",num);
    break;
  case 2:
    sprintf(tmps,"%d ",num);
    Asc_DStringAppend(hptr,tmps,HALL);
    break;
  default:
    FPRINTF(ASCERR,"weirdness in i/o!");
    break;
  }
  return HELP_OK;
}

int ascjson::Asc_SolvGetSlvParmsNew(Asc_DString *hptr, int argc, CONST84 char *argv[])
{
  slv_parameters_t p;
  char *tmps = NULL;
  int solver;
  int status=HELP_OK;
  int i,j;
  p.num_parms = 0;
  p.parms = NULL;

  if ( argc != 2 ) {
    FPRINTF(ASCERR,  "call is: slv_get_parmsnew <solver number>\n");
    Asc_DStringSet(hptr, "error in call to slv_get_parmsnew");
    return HELP_ERROR;
  }
  status=JTcl_GetInt(hptr, argv[1], &solver);
  if(!solver_engine(solver) || (status==HELP_ERROR)) {
    FPRINTF(ASCERR,  "slv_get_parmsnew: solver unknown!\n");
    Asc_DStringFree(hptr);
    Asc_DStringSet(hptr, "slv_get_parmsnew: solver number unknown");
    return HELP_ERROR;
  }

  slv_get_default_parameters(solver,&p);
  tmps=ASC_NEW_ARRAY(char,MAXIMUM_NUMERIC_LENGTH+1);

  Asc_DStringAppend(hptr,"[", 1);
  for (i = 0; i < p.num_parms; i++) {
    Asc_DStringAppend(hptr,"{\"type\":\"",HALL);
    switch (p.parms[i].type) {
    case int_parm:
      Asc_DStringAppend(hptr,"int_parm\",", HALL);
      break;
    case bool_parm:
      Asc_DStringAppend(hptr,"bool_parm\",", HALL);
      break;
    case real_parm:
      Asc_DStringAppend(hptr,"real_parm\",", HALL);
      break;
    case char_parm:
      Asc_DStringAppend(hptr,"char_parm\",", HALL);
      break;
    default:
      Asc_DStringAppend(hptr,"error\",", HALL);
      continue;
    }

    Asc_DStringAppend5(hptr,"\"name\":\"", p.parms[i].name, "\","
	    "\"interface_label\":\"", p.parms[i].interface_label, "\",", HALL);

    switch (p.parms[i].type) {
    case int_parm:
      sprintf(tmps,"\"value\":%d,",p.parms[i].info.i.value);
      Asc_DStringAppend(hptr,tmps,HALL);
      sprintf(tmps,"\"high\":%d,",p.parms[i].info.i.high);
      Asc_DStringAppend(hptr,tmps, HALL);
      sprintf(tmps,"\"low\":%d,",p.parms[i].info.i.low);
      Asc_DStringAppend(hptr,tmps, HALL);
      break;
    case bool_parm:
      sprintf(tmps,"\"value\":%d,",p.parms[i].info.b.value);
      Asc_DStringAppend(hptr,tmps, HALL);
      sprintf(tmps,"\"high\":%d,",p.parms[i].info.b.high);
      Asc_DStringAppend(hptr,tmps, HALL);
      sprintf(tmps,"\"low\":%d,",p.parms[i].info.b.low);
      Asc_DStringAppend(hptr,tmps, HALL);
      break;
    case real_parm:
      sprintf(tmps,"\"value\":%.6e,",p.parms[i].info.r.value);
      Asc_DStringAppend(hptr,tmps, HALL);
      sprintf(tmps,"\"high\":%.6e,",p.parms[i].info.r.high);
      Asc_DStringAppend(hptr,tmps, HALL);
      sprintf(tmps,"\"low\":%.6e,",p.parms[i].info.r.low);
      Asc_DStringAppend(hptr,tmps, HALL);
      break;
    case char_parm:
      Asc_DStringAppend3(hptr, "\"value\":\"", p.parms[i].info.c.value, "\",", HALL);
      Asc_DStringAppend(hptr, "\"option_values\":[", HALL);
      for (j = 0; j < p.parms[i].info.c.high; j++) {
        Asc_DStringAppend3(hptr,"\"", p.parms[i].info.c.argv[j], "\"", HALL);
	if (j < p.parms[i].info.c.high - 1) {
          Asc_DStringAppend(hptr, ",", 1);
	}
      }
      Asc_DStringAppend(hptr, "],", HALL);
      break;
    default:
      FPRINTF(ASCERR,  "slv_get_parmsnew found unrecognized");
      FPRINTF(ASCERR,  " parameter type\n");
      break;
    }
    sprintf(tmps,"\"display\":%d,",p.parms[i].display);
    Asc_DStringAppend(hptr,tmps,HALL);
    Asc_DStringAppend3(hptr,"\"description\":\"",p.parms[i].description, "\"}",HALL);
    if (i < p.num_parms-1) {
      Asc_DStringAppend(hptr,",", 1);
    }
  }
  Asc_DStringAppend(hptr,"]", 1);
  slv_destroy_parms(&p);
  ascfree(tmps);
  return HELP_OK;
}


int ascjson::Asc_SolvSetSlvParmsNew(Asc_DString *hptr, int argc, CONST84 char *argv[])
{
  slv_parameters_t p;
  int tmp_int =0, solver,i,j;
  double tmp_double = 0.1;

  if (g_solvsys_cur==NULL) {
    FPRINTF(ASCERR,  "set_slv_parms called with NULL pointer\n");
    Asc_DStringSet(hptr,"set_slv_parms called without slv_system");
    return HELP_ERROR;
  }

  solver=0;
  if (JTcl_GetInt(hptr, argv[1],&solver)==HELP_ERROR) {
    Asc_DStringFree(hptr);
    Asc_DStringSet(hptr, "set_slv_parms: arg 1 invalid type");
    return HELP_ERROR;
  }
  Asc_DStringFree(hptr);
  i = slv_get_selected_solver(g_solvsys_cur);

  if ( solver != i ) {
    /* THIS WHOLE CONTROL STRUCTURE IS SCREWED UP AT BOTH THE
     * C AND THE TCL LEVEL!!!
     */
	CONSOLE_DEBUG("...");
    ::slv_select_solver(g_solvsys_cur,solver);
/*    FPRINTF(ASCERR,"Warning: Solv_Set_Slv_Parms called ");
 *  FPRINTF(ASCERR,"with solver other than current solver\n");
 *  return HELP_OK;
 */
  }
  ::slv_get_parameters(g_solvsys_cur,&p);

  if ((argc - 2) != (p.num_parms)) {
    /* calling function in slot 0 and solver number in slot 1 */
    Asc_DStringSet(hptr, "set_slv_parms called with wrong number of args.");
    FPRINTF(ASCERR,
      "set_slv_parms expected %d args for %s\n",(p.num_parms + 1),
       ::slv_solver_name(p.whose));
    FPRINTF(ASCERR, "actual argument count: %d\n", (argc - 1));
    FPRINTF(ASCERR, "expected argument count: %d\n", (p.num_parms + 1));
    return HELP_ERROR;
  }

  for (j = 2,i = 0; i < p.num_parms; j++,i++) {
    switch (p.parms[i].type) {
    case int_parm:
      if (JTcl_GetInt(hptr, argv[j],&tmp_int)==HELP_ERROR) {
        Asc_DStringFree(hptr);
        FPRINTF(ASCERR,"set_slv_parms: arg %d of invalid type",j);
        Asc_DStringSet(hptr, "set_slv_parms called with invalid type");
        return HELP_ERROR;
      }
      p.parms[i].info.i.value = tmp_int;
      break;

    case bool_parm:
      if (JTcl_GetInt(hptr, argv[j],&tmp_int)==HELP_ERROR) {
        Asc_DStringFree(hptr);
        FPRINTF(ASCERR,"set_slv_parms: arg %d of invalid type",j);
        Asc_DStringSet(hptr, "set_slv_parms called with invalid type");
        return HELP_ERROR;
      }
      p.parms[i].info.b.value = tmp_int;
      break;

    case real_parm:
      if (Tcl_GetDouble(hptr, argv[j],&tmp_double)==HELP_ERROR) {
        Asc_DStringFree(hptr);
        FPRINTF(ASCERR,"set_slv_parms: arg %d of invalid type",j);
        Asc_DStringSet(hptr, "set_slv_parms called with invalid type");
        return HELP_ERROR;
      }
      p.parms[i].info.r.value = tmp_double;
      break;

    case char_parm:
      ::slv_set_char_parameter(&(p.parms[i].info.c.value),(CONST char *)argv[j]);
      break;
    default:
      FPRINTF(ASCERR,  "slv_get_parmsnew found unrecognized");
      FPRINTF(ASCERR,  " parameter type\n");
    }
  }
  ::slv_set_parameters(g_solvsys_cur,&p);
  return HELP_OK;
}


/* NBP is the number of basic parameters in the slv_parameters_t plus 1
   that we mess with in Asc_SolvGetSlvParms, Asc_SolvSetSlvParms.
   If you add a parameter to this that is handled here, up NBP */
#undef NBP
#define NBP 15
int ascjson::Asc_SolvGetSlvParms(Asc_DString *hptr, int argc, CONST84 char *argv[])
{
  slv_parameters_t p;
  char *tmps = NULL;
  int cursolver;
  int solver;
  int status=HELP_OK;
  int i,n;

  if ( argc != 2 ) {
    ERROR_REPORTER_HERE(ASC_PROG_ERR,"call is: slv_get_parms <solver number>\n");
    Asc_DStringSet(hptr, "error in call to slv_get_parms");
    return HELP_ERROR;
  }
  if (g_solvsys_cur==NULL) {
    ERROR_REPORTER_HERE(ASC_PROG_ERR,"called with NULL pointer\n");
    Asc_DStringSet(hptr,"slv_get_parms called without slv_system");
    return HELP_ERROR;
  }
  status=JTcl_GetInt(hptr, argv[1], &solver);

  if(!solver_engine(solver) || status==HELP_ERROR){
    ERROR_REPORTER_HERE(ASC_PROG_ERR,"solver '%d' unknown!\n",solver);
    Asc_DStringFree(hptr);
    Asc_DStringSet(hptr, "slv_get_parms: solver number unknown");
    return HELP_ERROR;
  }

  /* get parameters for solver*/
  cursolver=slv_get_selected_solver(g_solvsys_cur);
  CONSOLE_DEBUG("...");
  ::slv_select_solver(g_solvsys_cur,solver);
  ::slv_get_parameters(g_solvsys_cur,&p);
  tmps= (char *)ascmalloc((MAXIMUM_NUMERIC_LENGTH+1)*sizeof(char));

  sprintf(tmps,"%d", p.whose);
  VTcl_AppendElement(hptr,tmps);
  sprintf(tmps,"%.16g", p.time_limit);
  VTcl_AppendElement(hptr,tmps);
  sprintf(tmps,"%d", p.iteration_limit);
  VTcl_AppendElement(hptr,tmps);
  sprintf(tmps,"%.16g", p.tolerance.termination);
  VTcl_AppendElement(hptr,tmps);
  sprintf(tmps,"%.16g", p.tolerance.feasible);
  VTcl_AppendElement(hptr,tmps);
  sprintf(tmps,"%.16g", p.tolerance.pivot);
  VTcl_AppendElement(hptr,tmps);
  sprintf(tmps,"%.16g", p.tolerance.singular);
  VTcl_AppendElement(hptr,tmps);
  sprintf(tmps,"%.16g", p.tolerance.stationary);
  VTcl_AppendElement(hptr,tmps);
  sprintf(tmps,"%.16g", p.rho);
  VTcl_AppendElement(hptr,tmps);
  sprintf(tmps,"%s", ONEORZERO(p.partition));
  VTcl_AppendElement(hptr,tmps);
  sprintf(tmps,"%s", ONEORZERO(p.ignore_bounds));
  VTcl_AppendElement(hptr,tmps);
  sprintf(tmps,"%s", ONEORZERO(p.output.more_important!= NULL));
  VTcl_AppendElement(hptr,tmps);
  sprintf(tmps,"%s", ONEORZERO(p.output.less_important!= NULL));
  VTcl_AppendElement(hptr,tmps);
  sprintf(tmps,"%d", p.factor_option);
  VTcl_AppendElement(hptr,tmps);

  if (p.sp.iap) {
    n = p.sp.ilen;
  } else {
    n = 0;
  }
  for (i=0;i<n;i++) {
      sprintf(tmps,"%d",p.sp.iap[i]);
      VTcl_AppendElement(hptr,tmps);
  }
  if (p.sp.rap) {
    n = p.sp.rlen;
  } else {
    n = 0;
  }
  for (i=0;i<n;i++) {
      sprintf(tmps,"%.16g",p.sp.rap[i]);
      VTcl_AppendElement(hptr,tmps);
  }
  if (p.sp.cap) {
    n = p.sp.clen;
  } else {
    n = 0;
  }
  for (i=0;i<n;i++) {
      VTcl_AppendElement(hptr,p.sp.cap[i]);
  }
  ascfree(tmps);
  CONSOLE_DEBUG("...");
  ::slv_select_solver(g_solvsys_cur,cursolver);
  VAEstrip(hptr);
  return HELP_OK;
}

int ascjson::Asc_SolvSetSlvParms(Asc_DString *hptr, int argc, CONST84 char *argv[])
{
  slv_parameters_t p;
  int tmpbool =0, solver,i,nia,nra;

  int nca = 0;   /*  modified by CWS 5/95 -
                     have one character subparameter too */

  int32 tmplong =100;
  double tmpdouble = 0.1;
  char *tmpchar;

  if (g_solvsys_cur==NULL) {
    FPRINTF(ASCERR,  "set_slv_parms called with NULL pointer\n");
    Asc_DStringSet(hptr,"set_slv_parms called without slv_system");
    return HELP_ERROR;
  }

  if (argc < NBP) {
    FPRINTF(ASCERR,  "call is: set_slv_parms <%d args>\n",NBP-1);
    FPRINTF(ASCERR,  "args are:\n");
    FPRINTF(ASCERR,  "solver number\n");
    FPRINTF(ASCERR,  "time_limit(sec)\n");
    FPRINTF(ASCERR,  "iteration_limit\n");

    FPRINTF(ASCERR,  "termination tolerance\n");
    FPRINTF(ASCERR,  "feasible tolerance\n");
    FPRINTF(ASCERR,  "pivot tolerance\n");
    FPRINTF(ASCERR,  "singular tolerance\n");
    FPRINTF(ASCERR,  "stationary tolerance\n");
    FPRINTF(ASCERR,  "rho\n");

    FPRINTF(ASCERR,  "partitioning enabled\n");
    FPRINTF(ASCERR,  "ignore bounds\n");
    FPRINTF(ASCERR,  "display more important messages\n");
    FPRINTF(ASCERR,  "display less important messages\n");
    FPRINTF(ASCERR,  "factor_option number\n");
    FPRINTF(ASCERR,  "plus engine specific int and real parms\n");

    FFLUSH(ASCERR);
    Asc_DStringSet(hptr, "in set_slv_parms call");
    return HELP_ERROR;
  }
  solver=0;
  if (JTcl_GetInt(hptr, argv[1],&solver)==HELP_ERROR) {
    Asc_DStringFree(hptr);
    Asc_DStringSet(hptr, "set_slv_parms: arg 1 invalid type");
    return HELP_ERROR;
  }
  Asc_DStringFree(hptr);
  i=slv_get_selected_solver(g_solvsys_cur);

  if ( solver != i ) {
    /* THIS WHOLE CONTROL STRUCTURE IS SCREWED UP AT BOTH THE
       C AND THE TCL LEVEL!!! */
	CONSOLE_DEBUG("...");
    ::slv_select_solver(g_solvsys_cur,solver);
/*    FPRINTF(ASCERR,"Warning: Solv_Set_Slv_Parms called ");
    FPRINTF(ASCERR,"with solver other than current solver\n");
    return HELP_OK;*/
  }
  ::slv_get_parameters(g_solvsys_cur,&p);

/*  if (p.whose!=solver) return HELP_OK; *//* fail quietly, user is an idiot */

  /* determine number of total parameters we need from user */
  if (p.sp.iap) {
    nia = p.sp.ilen;
  } else {
    nia = 0;
  }
  if (p.sp.rap) {
    nra = p.sp.rlen;
  } else {
    nra = 0;
  }
  if (p.sp.cap) {
    nca = p.sp.clen;
  } else {
    nca = 0;
  }
  if (argc != (NBP+nia+nra+nca)) { /*args 0 to NBP-1 are the slv0 standard */
    Asc_DStringSet(hptr, "set_slv_parms called with wrong number of args." );
    FPRINTF(ASCERR,
      "set_slv_parms expected %d args for %s\n",(NBP -1+nia+nra+nca),
       ::slv_solver_name(p.whose));
    FPRINTF(ASCERR, "actual argument count: %d\n", argc);
    FPRINTF(ASCERR, "expected argument count: %d\n", NBP+nia+nra+nca);
    FPRINTF(ASCERR, "basic: %d\n", NBP-1);
    FPRINTF(ASCERR, "integer: %d\n", nia);
    FPRINTF(ASCERR, "double: %d\n", nra);
    FPRINTF(ASCERR, "string: %d\n", nca);
    return HELP_ERROR;
  }

  tmpdouble=p.time_limit;
  if( Tcl_GetDouble(hptr, argv[2],&tmpdouble)==HELP_ERROR) {
    Asc_DStringFree(hptr);
    Asc_DStringSet(hptr, "set_slv_parms: arg 2 invalid type");
    return HELP_ERROR;
  }
  p.time_limit=fabs(tmpdouble);

  tmplong=p.iteration_limit;
  if (JTcl_GetInt(hptr, argv[3],&tmplong)==HELP_ERROR) {
    Asc_DStringFree(hptr);
    Asc_DStringSet(hptr, "set_slv_parms: arg 3 invalid type");
    return HELP_ERROR;
  }
  p.iteration_limit = abs(tmplong);

  tmpdouble=p.tolerance.termination;
  if(Tcl_GetDouble(hptr, argv[4],&tmpdouble)==HELP_ERROR) {
    Asc_DStringFree(hptr);
    Asc_DStringSet(hptr, "set_slv_parms: arg 4 invalid type");
    return HELP_ERROR;
  }
  p.tolerance.termination =fabs(tmpdouble);

  tmpdouble=p.tolerance.feasible;
  if (Tcl_GetDouble(hptr, argv[5],&tmpdouble)==HELP_ERROR) {
    Asc_DStringFree(hptr);
    Asc_DStringSet(hptr, "set_slv_parms: arg 5 invalid type");
    return HELP_ERROR;
  }
  p.tolerance.feasible =fabs(tmpdouble);

  tmpdouble=p.tolerance.pivot;
  if (Tcl_GetDouble(hptr, argv[6],&tmpdouble)==HELP_ERROR) {
    Asc_DStringFree(hptr);
    Asc_DStringSet(hptr, "set_slv_parms: arg 6 invalid type");
    return HELP_ERROR;
  }
  p.tolerance.pivot =fabs(tmpdouble);

  tmpdouble=p.tolerance.singular;
  if (Tcl_GetDouble(hptr, argv[7],&tmpdouble)==HELP_ERROR) {
    Asc_DStringFree(hptr);
    Asc_DStringSet(hptr, "set_slv_parms: arg 7 invalid type");
    return HELP_ERROR;
  }
  p.tolerance.singular =fabs(tmpdouble);

  tmpdouble=p.tolerance.stationary;
  if (Tcl_GetDouble(hptr, argv[8],&tmpdouble)==HELP_ERROR) {
    Asc_DStringFree(hptr);
    Asc_DStringSet(hptr, "set_slv_parms: arg 8 invalid type");
    return HELP_ERROR;
  }
  p.tolerance.stationary =fabs(tmpdouble);

  tmpdouble=p.rho;
  if (Tcl_GetDouble(hptr, argv[9],&tmpdouble)==HELP_ERROR) {
    Asc_DStringFree(hptr);
    Asc_DStringSet(hptr, "set_slv_parms: arg 9 invalid type");
    return HELP_ERROR;
  }
  p.rho =fabs(tmpdouble);

  tmpbool=p.partition;
  if(Tcl_ExprBoolean(hptr, argv[10],&tmpbool)==HELP_ERROR) {
    Asc_DStringFree(hptr);
    Asc_DStringSet(hptr, "set_slv_parms: arg 10 invalid type");
    return HELP_ERROR;
  }
  p.partition=tmpbool;

  tmpbool=p.ignore_bounds;
  if ( Tcl_ExprBoolean(hptr, argv[11],&tmpbool)==HELP_ERROR) {
    Asc_DStringFree(hptr);
    Asc_DStringSet(hptr, "set_slv_parms: arg 11 invalid type");
    return HELP_ERROR;
  }
  p.ignore_bounds=tmpbool;

  if (Tcl_ExprBoolean(hptr, argv[12],&tmpbool)==HELP_ERROR) {
    Asc_DStringFree(hptr);
    Asc_DStringSet(hptr, "set_slv_parms: arg 12 invalid type");
    return HELP_ERROR;
  }
  if (tmpbool) {
    p.output.more_important=ASCERR;
  } else {
    p.output.more_important=NULL;
  }

  if (Tcl_ExprBoolean(hptr, argv[13],&tmpbool)==HELP_ERROR) {
    Asc_DStringFree(hptr);
    Asc_DStringSet(hptr, "set_slv_parms: arg 13 invalid type");
    return HELP_ERROR;
  }
  if (tmpbool) {
    p.output.less_important=ASCERR;
  } else {
    p.output.less_important=NULL;
  }

  tmplong=p.factor_option;
  if (JTcl_GetInt(hptr, argv[14],&tmplong)==HELP_ERROR) {
    Asc_DStringFree(hptr);
    Asc_DStringSet(hptr, "set_slv_parms: arg 14 invalid type");
    return HELP_ERROR;
  }
  p.factor_option = abs(tmplong);

  for (i=0;i<nia;i++) {
    tmpbool=p.sp.iap[i];
    if (JTcl_GetInt(hptr, argv[i+NBP],&tmpbool)==HELP_ERROR) {
      Asc_DStringFree(hptr);
      Asc_DStringSet(hptr, "set_slv_parms: integer array arg of invalid type");
      FPRINTF(ASCERR,"int sub-parameter %d (%s) invalid\n",i,argv[i+NBP]);
      return HELP_ERROR;
    }
    p.sp.iap[i]=tmpbool;
  }

  for (i=0;i<nra;i++) {
    tmpdouble=p.sp.rap[i];
    if (Tcl_GetDouble(hptr, argv[i+NBP+nia],&tmpdouble)==HELP_ERROR) {
      Asc_DStringFree(hptr);
      Asc_DStringSet(hptr, "set_slv_parms: real array arg of invalid type" );
        FPRINTF(ASCERR,"real sub-parameter %d (%s) invalid\n",
          i,argv[i+nia+NBP]);
      return HELP_ERROR;
    }
    p.sp.rap[i]=tmpdouble;
  }

   /*  modified by CWS 5/95
       Loop through and copy the strings from TCL land
       to the C side of things.  The strings are deallocated
       in slvI_destroy (slv6_destroy in this case).
    */

    for (i=0;i<nca;i++) {
        tmpchar =
          Asc_MakeInitString(strlen(argv[i+NBP+nia+nra])); /* allocate mem */
        strcpy(tmpchar, argv[i+NBP+nia+nra]);  /* make a copy of string */
        if (p.sp.cap[i] != NULL) {
          ascfree(p.sp.cap[i]);
        }
        /* deallocate old, if any */
        p.sp.cap[i] = tmpchar;  /* save pointer */
     }


  ::slv_set_parameters(g_solvsys_cur,&p);
  return HELP_OK;
}
#undef NBP

int ascjson::Asc_SolvGetInstType(Asc_DString *hptr, int argc, CONST84 char *argv[])
{
  char * it;

  if ( argc != 1 ) {
    FPRINTF(ASCERR,  "call is: slv_get_insttype <no args>\n");
    Asc_DStringSet(hptr, "error in call to slv_get_insttype");
    return HELP_ERROR;
  }
  if (g_solvsys_cur==NULL) {
/*    FPRINTF(ASCERR,  "slv_get_insttype called with NULL pointer\n");
*/
    Asc_DStringSet(hptr, "slv_get_insttype called without slv_system");
    return HELP_ERROR;
  }
  if (g_solvinst_cur==NULL) {
/*  FPRINTF(ASCERR,  "slv_get_insttype called with NULL instance\n");
*/
    Asc_DStringSet(hptr, "slv_get_insttype called without instance");
    return HELP_ERROR;
  }
  it=(char *)InstanceType(g_solvinst_cur);
  VTcl_AppendElement(hptr,it);
  VAEstrip(hptr);
  return HELP_OK;
}

int ascjson::Asc_SolvGetSlvStatPage(Asc_DString *hptr, int argc, CONST84 char *argv[])
{
  slv_status_t s;
  char * tmps=NULL;

  if ( argc != 1 ) {
    FPRINTF(ASCERR,  "call is: slv_get_stat_page <no args>\n");
    Asc_DStringSet(hptr, "error in call to slv_get_stat_page");
    return HELP_ERROR;
  }
  if (g_solvsys_cur==NULL) {
    FPRINTF(ASCERR,  "slv_get_stat_page called with NULL pointer\n");
    Asc_DStringSet(hptr, "slv_get_stat_page called without slv_system");
    return HELP_ERROR;
  }

  ::slv_get_status(g_solvsys_cur,&s);

  tmps= (char *)ascmalloc((MAXIMUM_NUMERIC_LENGTH+1)*sizeof(char));
  /*system status */
  sprintf(tmps,"%d",s.ok);
  VTcl_AppendElement(hptr,tmps);
  sprintf(tmps,"%d",s.over_defined);
  VTcl_AppendElement(hptr,tmps);
  sprintf(tmps,"%d",s.under_defined);
  VTcl_AppendElement(hptr,tmps);
  sprintf(tmps,"%d",s.struct_singular);
  VTcl_AppendElement(hptr,tmps);
  sprintf(tmps,"%d",s.ready_to_solve);
  VTcl_AppendElement(hptr,tmps);
  sprintf(tmps,"%d",s.converged);
  VTcl_AppendElement(hptr,tmps);
  sprintf(tmps,"%d",s.diverged);
  VTcl_AppendElement(hptr,tmps);
  sprintf(tmps,"%d",s.inconsistent);
  VTcl_AppendElement(hptr,tmps);
  sprintf(tmps,"%d",s.calc_ok);
  VTcl_AppendElement(hptr,tmps);
  sprintf(tmps,"%d",s.iteration_limit_exceeded);
  VTcl_AppendElement(hptr,tmps);
  sprintf(tmps,"%d",s.time_limit_exceeded);
  VTcl_AppendElement(hptr,tmps);
  sprintf(tmps,"%d",s.iteration);
  VTcl_AppendElement(hptr,tmps);
  sprintf(tmps,"%.16g",s.cpu_elapsed);
  VTcl_AppendElement(hptr,tmps);

  /*block status*/
  sprintf(tmps,"%d",s.block.number_of);
  VTcl_AppendElement(hptr,tmps);
  sprintf(tmps,"%d",s.block.current_block);
  VTcl_AppendElement(hptr,tmps);
  sprintf(tmps,"%d",s.block.current_size);
  VTcl_AppendElement(hptr,tmps);
  sprintf(tmps,"%d",s.block.previous_total_size);
  VTcl_AppendElement(hptr,tmps);
  sprintf(tmps,"%d",s.block.iteration);
  VTcl_AppendElement(hptr,tmps);
  sprintf(tmps,"%.10g",s.block.cpu_elapsed);
  VTcl_AppendElement(hptr,tmps);
  sprintf(tmps,"%.10g",s.block.residual);
  VTcl_AppendElement(hptr,tmps);
  ascfree(tmps);
  VAEstrip(hptr);
  return HELP_OK;
}

int ascjson::Asc_SolvGetSlvCostPage(Asc_DString *hptr, int argc, CONST84 char *argv[])
{
  slv_status_t s;
  int i;

  if ( argc != 1 ) {
    FPRINTF(ASCERR,  "call is: slv_get_cost_page <no args>\n");
    Asc_DStringSet(hptr, "error in call to slv_get_cost_page");
    return HELP_ERROR;
  }
  if (g_solvsys_cur==NULL) {
    FPRINTF(ASCERR,  "slv_get_cost_page called with NULL pointer\n");
    Asc_DStringSet(hptr, "slv_get_cost_page called without slv_system");
    return HELP_ERROR;
  }

  slv_get_status(g_solvsys_cur,&s);

  if (s.cost)  {
    char * tmps=NULL;
    tmps= (char *)ascmalloc((MAXIMUM_NUMERIC_LENGTH+1)*sizeof(char));
    sprintf(tmps,"%s","\0");
    for (i=0;i<s.costsize;i++) {
      if (!i) {
        sprintf(tmps,"{%d ",s.cost[i].size);
      } else {
        sprintf(tmps," {%d ",s.cost[i].size);
      }
      Asc_DStringAppend(hptr,tmps,HALL);
      sprintf(tmps, "%d ",s.cost[i].iterations);
      Asc_DStringAppend(hptr,tmps,HALL);
      sprintf(tmps, "%d ",s.cost[i].funcs);
      Asc_DStringAppend(hptr,tmps,HALL);
      sprintf(tmps, "%d ",s.cost[i].jacs);
      Asc_DStringAppend(hptr,tmps,HALL);
      sprintf(tmps, "%.8g ",s.cost[i].time);
      Asc_DStringAppend(hptr,tmps,HALL);
      sprintf(tmps, "%.16g ",s.cost[i].resid);
      Asc_DStringAppend(hptr,tmps,HALL);
      sprintf(tmps, "%.8g ",s.cost[i].functime);
      Asc_DStringAppend(hptr,tmps,HALL);
      sprintf(tmps, "%.8g}",s.cost[i].jactime);
      Asc_DStringAppend(hptr,tmps,HALL);
    }
    VAEstrip(hptr);
    ascfree(tmps);
  }
  return HELP_OK;
}

int ascjson::Asc_SolvGetObjectiveVal(Asc_DString *hptr, int argc, CONST84 char *argv[])
{
  struct rel_relation *obj;

  if ( argc != 1 ) {
    FPRINTF(ASCERR,  "call is: slv_get_objval <no args>\n");
    Asc_DStringSet(hptr, "error in call to slv_get_objval");
    return HELP_ERROR;
  }
  if (g_solvsys_cur==NULL) {
    FPRINTF(ASCERR,  "slv_get_objval called with NULL pointer\n");
    Asc_DStringSet(hptr, "slv_get_objval called without slv_system");
    return HELP_ERROR;
  }

  obj= ::slv_get_obj_relation(g_solvsys_cur);
  if( obj == NULL ) {
    Asc_DStringSet(hptr, "none");
  } else {
    /* expect the solver to have updated the objects list valeus */
    Asc_DStringAppend(hptr,Asc_UnitValueDS(T2I(rel_instance(obj))),HALL);
  }
  return HELP_OK;
}

int ascjson::Asc_SolvGetInstName(Asc_DString *hptr, int argc, CONST84 char *argv[])
{
  char *name=NULL;

  if ( argc != 1 ) {
    FPRINTF(ASCERR,"call is: slv_get_instname\n");
    Asc_DStringSet(hptr, "slv_get_instname wants 0 args");
    return HELP_ERROR;
  }
  if (g_solvinst_cur==NULL || g_solvinst_root==NULL) {
#if SP_DEBUG
    FPRINTF(ASCERR,  "slv_get_instname called with NULL pointer\n");
#endif
    Asc_DStringSet(hptr, "none");
    return HELP_OK;
  }
  if (g_solvinst_cur==g_solvinst_root) {
    Asc_DStringSet(hptr, "&");
    return HELP_OK;
  }
  name=WriteInstanceNameString(g_solvinst_cur,g_solvinst_root);
  Asc_DStringAppend(hptr,name,HALL);
  if (name) {
    ascfree(name);
  }
  return HELP_OK;
}

int ascjson::Asc_SolvGetPathName(Asc_DString *hptr, int argc, CONST84 char *argv[])
{
  char *name=NULL;

  if ( argc != 1 ) {
    FPRINTF(ASCERR,"call is: slv_get_pathname\n");
    Asc_DStringSet(hptr, "slv_get_pathname wants 0 args");
    return HELP_ERROR;
  }
  if (g_solvinst_cur==NULL || g_solvinst_root==NULL) {
#if SP_DEBUG
    FPRINTF(ASCERR,  "slv_get_pathname called with NULL pointer\n");
#endif
    Asc_DStringSet(hptr, "none");
    return HELP_OK;
  }
  name = (char *)SCP(Asc_SimsFindSimulationName(g_solvinst_root));
  Asc_DStringAppend(hptr,name,HALL);
  name=NULL;
  if (g_solvinst_cur!=g_solvinst_root) {
    name=WriteInstanceNameString(g_solvinst_cur,g_solvinst_root);
    Asc_DStringAppend2(hptr,".",name,HALL);
    if (name) {
      ascfree(name);
    }
  }
  return HELP_OK;
}


#if 0
/*
 *  Solves g_curinst with solver specified.
 *  This is for commandline use only.
 *  Just a wrapper of slv_interface.c Solve() for now.
 *  no proper type checking yet, sincle solve will trap it (usually)
 *  though there should be by 1-14-94
 */
int ascjson::Asc_SolvCurInst(Asc_DString *hptr,
                  int argc, CONST84 char *argv[])
{
  if ( argc != 2 ) {
    FPRINTF(ASCERR,  "call is: solve\n");
    Asc_DStringSet(hptr, "solvers available: 0:SLV, 1:MINOS");
    return HELP_ERROR;
  }
  if (!g_curinst) {
    FPRINTF(ASCERR, "Solve called with NULL current instance.\n");
    Asc_DStringSet(hptr, "NULL pointer received from Browser.");
    return HELP_ERROR;
  }
  g_solvinst_cur=g_curinst;
  FPRINTF(ASCERR,"Windows will not update until you leave Solve>.\n");
  Solve(g_solvinst_cur);
  return HELP_OK;
}
#endif

int ascjson::Asc_SolvGetVRCounts(Asc_DString *hptr, int argc, CONST84 char *argv[])
{
  int solver;
  int status=HELP_OK;
  char * tmps=NULL;
  int tmpi;
  var_filter_t vfilter;
  rel_filter_t rfilter;

  tmps= (char *)ascmalloc((MAXIMUM_NUMERIC_LENGTH+1)*sizeof(char));

  if ( argc != 2 ) {
    FPRINTF(ASCERR,  "call is: solve_get_vr <solver number> \n");
    Asc_DStringSet(hptr, "call is: solve_get_vr <solver number>");
    return HELP_ERROR;
  }
  status=JTcl_GetInt(hptr, argv[1], &solver);
  if (status!=HELP_OK) {
    FPRINTF(ASCERR, "solve_get_vr called with bad solver number.\n");
    Asc_DStringFree(hptr);
    Asc_DStringSet(hptr, "solve_get_vr called with bad solver number.");
    return HELP_ERROR;
  }
  if(!solver_engine(solver)){
    FPRINTF(ASCERR, "unknown solver (%d). Not selected!\n",solver);
    Asc_DStringSet(hptr, "Solver not available.");
    return HELP_ERROR;
  }
  if (!g_solvsys_cur) {
    FPRINTF(ASCERR, "solve_get_vr called with NULL system.\n");
    Asc_DStringSet(hptr, "solve_get_vr: called with NULL system.");
    return HELP_ERROR;
  }

  /*get total relation count   totrels */
  tmpi = ::slv_get_num_solvers_rels(g_solvsys_cur);
  sprintf(tmps,"%d",tmpi);
  VTcl_AppendElement(hptr,tmps);

  /*get active relation count   rels */
  rfilter.matchbits = (REL_ACTIVE);
  rfilter.matchvalue = (REL_ACTIVE);
  tmpi=slv_count_solvers_rels(g_solvsys_cur,&rfilter);
  sprintf(tmps,"%d",tmpi);
  VTcl_AppendElement(hptr,tmps);

  /*get included relation count   inc_rels */
  rfilter.matchbits = (REL_INCLUDED);
  rfilter.matchvalue = (REL_INCLUDED);
  tmpi=slv_count_solvers_rels(g_solvsys_cur,&rfilter);
  sprintf(tmps,"%d",tmpi);
  VTcl_AppendElement(hptr,tmps);

  /*get total variable count  totvars */
  tmpi = slv_get_num_solvers_vars(g_solvsys_cur);
  sprintf(tmps,"%d",tmpi);
  VTcl_AppendElement(hptr,tmps);

  /*get active variable count   vars*/
  vfilter.matchbits = (VAR_ACTIVE);
  vfilter.matchvalue = (VAR_ACTIVE);
  tmpi=slv_count_solvers_vars(g_solvsys_cur,&vfilter);
  sprintf(tmps,"%d",tmpi);
  VTcl_AppendElement(hptr,tmps);

  /*get currently used (free & incident & active) variable count free_vars*/
  vfilter.matchbits = (VAR_FIXED | VAR_INCIDENT | VAR_ACTIVE);
  vfilter.matchvalue = (VAR_INCIDENT | VAR_ACTIVE);
  tmpi=slv_count_solvers_vars(g_solvsys_cur,&vfilter);
  sprintf(tmps,"%d",tmpi);
  VTcl_AppendElement(hptr,tmps);

  /*get active equality  count   eqals*/
  rfilter.matchbits = (REL_EQUALITY | REL_ACTIVE);
  rfilter.matchvalue = (REL_EQUALITY | REL_ACTIVE);
  tmpi=slv_count_solvers_rels(g_solvsys_cur,&rfilter);
  sprintf(tmps,"%d",tmpi);
  VTcl_AppendElement(hptr,tmps);

  /*get used (included and active equalities) relation count    inc_eqals*/
  rfilter.matchbits = (REL_INCLUDED | REL_EQUALITY | REL_ACTIVE);
  rfilter.matchvalue = (REL_INCLUDED | REL_EQUALITY | REL_ACTIVE);
  tmpi=slv_count_solvers_rels(g_solvsys_cur,&rfilter);
  sprintf(tmps,"%d",tmpi);
  VTcl_AppendElement(hptr,tmps);

  /*get inequality count   ineqals*/
  rfilter.matchbits = (REL_EQUALITY | REL_ACTIVE);
  rfilter.matchvalue = (REL_ACTIVE);
  tmpi = ::slv_count_solvers_rels(g_solvsys_cur,&rfilter);
  sprintf(tmps,"%d",tmpi);
  VTcl_AppendElement(hptr,tmps);

  /*get included inequality count  inc_ineqals*/
  rfilter.matchbits = (REL_INCLUDED | REL_EQUALITY | REL_ACTIVE);
  rfilter.matchvalue = (REL_INCLUDED | REL_ACTIVE);
  tmpi = ::slv_count_solvers_rels(g_solvsys_cur,&rfilter);
  sprintf(tmps,"%d",tmpi);
  VTcl_AppendElement(hptr,tmps);

  /* get unused (included and inactive equalities) relation count
   * in_inc_eqals
   */
  rfilter.matchbits = (REL_INCLUDED | REL_EQUALITY | REL_ACTIVE);
  rfilter.matchvalue = (REL_INCLUDED | REL_EQUALITY);
  tmpi=slv_count_solvers_rels(g_solvsys_cur,&rfilter);
  sprintf(tmps,"%d",tmpi);
  VTcl_AppendElement(hptr,tmps);

  /*get included inactive inequality count  in_inc_ineqals*/
  rfilter.matchbits = (REL_INCLUDED | REL_EQUALITY | REL_ACTIVE);
  rfilter.matchvalue = (REL_INCLUDED);
  tmpi = ::slv_count_solvers_rels(g_solvsys_cur,&rfilter);
  sprintf(tmps,"%d",tmpi);
  VTcl_AppendElement(hptr,tmps);

  /*get unincluded relation count   uninc_rels */
  rfilter.matchbits = (REL_INCLUDED);
  rfilter.matchvalue = 0;
  tmpi=slv_count_solvers_rels(g_solvsys_cur,&rfilter);
  sprintf(tmps,"%d",tmpi);
  VTcl_AppendElement(hptr,tmps);

  /*get fixed and incident count   fixed_vars*/
  vfilter.matchbits = (VAR_FIXED | VAR_INCIDENT | VAR_ACTIVE);
  vfilter.matchvalue = (VAR_FIXED | VAR_INCIDENT | VAR_ACTIVE);
  tmpi=slv_count_solvers_vars(g_solvsys_cur,&vfilter);
  sprintf(tmps,"%d",tmpi);
  VTcl_AppendElement(hptr,tmps);

  /*get free and inactive incident count   in_free_vars*/
  vfilter.matchbits = (VAR_FIXED | VAR_INCIDENT | VAR_ACTIVE);
  vfilter.matchvalue = (VAR_INCIDENT);
  tmpi=slv_count_solvers_vars(g_solvsys_cur,&vfilter);
  sprintf(tmps,"%d",tmpi);
  VTcl_AppendElement(hptr,tmps);

  /*get fixed and inactive incident count   in_fixed_vars*/
  vfilter.matchbits = (VAR_FIXED | VAR_INCIDENT | VAR_ACTIVE);
  vfilter.matchvalue = (VAR_FIXED | VAR_INCIDENT);
  tmpi=slv_count_solvers_vars(g_solvsys_cur,&vfilter);
  sprintf(tmps,"%d",tmpi);
  VTcl_AppendElement(hptr,tmps);

  /*get active unattached count   un_vars */
  vfilter.matchbits = (VAR_ACTIVE);
  vfilter.matchvalue = (VAR_ACTIVE);
  tmpi = ::slv_count_solvers_unattached(g_solvsys_cur,&vfilter);
  sprintf(tmps,"%d",tmpi);
  VTcl_AppendElement(hptr,tmps);
  VAEstrip(hptr);

  ascfree(tmps);
  return HELP_OK;
}

int ascjson::Asc_SolvSlvDumpInt(Asc_DString *hptr, int argc, CONST84 char *argv[])
{
  int status,level;

  if ( argc != 2 ) {
    FPRINTF(ASCERR, "call is: slvdump <level>\n");
    Asc_DStringSet(hptr, "Specify a level to slvdump.");
    return HELP_ERROR;
  }
  status=JTcl_GetInt(hptr, argv[1],&level);
  if (status!=HELP_OK) {
    FPRINTF(ASCERR, "slvdump called with non-integer level.\n");
    Asc_DStringFree(hptr);
    Asc_DStringSet(hptr, "slvdump called with non-integer level.");
    return HELP_ERROR;
  }
  if (g_solvsys_cur!=NULL) {
    ::slv_dump_internals(g_solvsys_cur,level);
  } else {
    FPRINTF(ASCERR, "slvdump called with NULL system.\n");
    Asc_DStringSet(hptr, "Empty solver context.");
    return HELP_ERROR;
  }
  return HELP_OK;
}


int ascjson::Asc_SolvSlvPresolve(Asc_DString *hptr, int argc, CONST84 char *argv[])
{

  if ( argc != 1 ) {
    FPRINTF(ASCERR, "call is: presolve <no args>\n");
    Asc_DStringSet(hptr, "no arguments allowed for presolve");
    return HELP_ERROR;
  }

#ifdef ASC_SIGNAL_TRAPS
  if (SETJMP(g_fpe_env)==0) {
#endif /* ASC_SIGNAL_TRAPS */
    if (g_solvsys_cur!=NULL) {
      ::slv_presolve(g_solvsys_cur);
      return HELP_OK;
    } else {
      FPRINTF(ASCERR, "Presolve called with NULL system.\n");
      Asc_DStringSet(hptr, "empty solver context.");
      return HELP_ERROR;
    }
#ifdef ASC_SIGNAL_TRAPS
  } else {
      FPRINTF(ASCERR, "Floating point exception in slv_presolve!!\n");
      Asc_DStringSet(hptr, " Floating point exception in slv_presolve. Help!");
      return HELP_ERROR;
  }
#endif /* ASC_SIGNAL_TRAPS */
}

/* After modification of an instance included in a when var list or
 * after running a procedure, the system must be reconfigured to
 * account for structural changes in the configuration.
 * Asc_SolvReanalyze has to be executed after running a procedure.
 */
int ascjson::Asc_SolvReanalyze(Asc_DString *hptr, int argc, CONST84 char *argv[])
{

  if ( argc != 1 ) {
    FPRINTF(ASCERR, "call is: slv_reanalyze <no args>\n");
    Asc_DStringSet(hptr, "wong # arguments for slv_reanalyze");
    return HELP_ERROR;
  }
  if (g_solvsys_cur!=NULL) {
    system_reanalyze(g_solvsys_cur);
    return HELP_OK;
  } else {
    FPRINTF(ASCERR, "Reanalyze called with NULL system.\n");
    Asc_DStringSet(hptr, "empty solver context.");
    return HELP_ERROR;
  }
}

/*
 * This function needs to be fixed. Right now it does the same as
 * Asc_SolvReanalyze. Here, we are supposed to check if the boolean
 * instance modified is part of some whenvarlist, in the current
 * solver system. The instance to be checked is going to be sent
 * as the second argument to system_reanalyze.
 */
int ascjson::Asc_SolvCheckAndReanalyze(Asc_DString *hptr, int argc, CONST84 char *argv[])
{

  if ( argc != 2 ) {
    FPRINTF(ASCERR, "call is: slv_check_and_reanalyze <instance_name>\n");
    Asc_DStringSet(hptr, "wong # arguments for slv_check_and_reanalyze");
    return HELP_ERROR;
  }
  if (g_solvsys_cur!=NULL) {
    system_reanalyze(g_solvsys_cur);
    return HELP_OK;
  } else {
    FPRINTF(ASCERR, "CheckAndReanalyze called with NULL system.\n");
    Asc_DStringSet(hptr, "empty solver context.");
    return HELP_ERROR;
  }
}

int ascjson::Asc_SolvSlvResolve(Asc_DString *hptr, int argc, CONST84 char *argv[])
{

  if ( argc != 1 ) {
    FPRINTF(ASCERR, "call is: resolve <no args>\n");
    Asc_DStringSet(hptr, "no arguments allowed for resolve");
    return HELP_ERROR;
  }

#ifdef ASC_SIGNAL_TRAPS
  if (SETJMP(g_fpe_env)==0) {
#endif /* ASC_SIGNAL_TRAPS */
    if (g_solvsys_cur!=NULL) {
      ::slv_resolve(g_solvsys_cur);
      return HELP_OK;
    } else {
      FPRINTF(ASCERR, "Resolve called with NULL system.\n");
      Asc_DStringSet(hptr, "empty solver context.");
      return HELP_ERROR;
    }
#ifdef ASC_SIGNAL_TRAPS
  } else {
      FPRINTF(ASCERR, "Floating point exception in slv_resolve!!\n");
      Asc_DStringSet(hptr, " Floating point exception in slv_resolve. Help!");
      return HELP_ERROR;
  }
#endif /* ASC_SIGNAL_TRAPS */
}

/* invoking the name of the beast three times makes it come! */
int ascjson::Asc_SolvSlvSolve(Asc_DString *hptr, int argc, CONST84 char *argv[])
{
  if ( argc != 1 ) {
    FPRINTF(ASCERR, "call is: slv_solve <no args>\n");
    Asc_DStringSet(hptr, "no arguments allowed for slv_solve");
    return HELP_ERROR;
  }
#ifdef ASC_SIGNAL_TRAPS
  if (SETJMP(g_fpe_env)==0) {
#endif /* ASC_SIGNAL_TRAPS */
    if (g_solvsys_cur!=NULL) {
      ::slv_solve(g_solvsys_cur);
      return HELP_OK;
    } else {
      FPRINTF(ASCERR, "slv_solve called with NULL system.\n");
      Asc_DStringSet(hptr, " empty solver context.");
      return HELP_ERROR;
    }
#ifdef ASC_SIGNAL_TRAPS
  } else {
      FPRINTF(ASCERR, "Floating point exception in slv_solve!!\n");
      Asc_DStringSet(hptr, " Floating point exception in slv_solve. Help!");
      return HELP_ERROR;
  }
#endif /* ASC_SIGNAL_TRAPS */
}

/* hide it out here from the exception clobber */
static int safe_status;
int ascjson::Asc_SolvSlvIterate(Asc_DString *hptr, int argc, CONST84 char *argv[])
{
  slv_status_t s;
  int steps=1;
  double comptime=5.0,start,delta=0.0;
  safe_status=HELP_OK;

  if ( argc > 3 ) {
    FPRINTF(ASCERR, "call is: slv_iterate <steps> [timelimit]\n");
    Asc_DStringSet(hptr, "too many arguments to slv_iterate");
    return HELP_ERROR;
  }
  if ( argc < 2 ) {
    FPRINTF(ASCERR, "call is: slv_iterate <steps> [timelimit]\n");
    Asc_DStringSet(hptr, "need an iteration count for slv_iterate");
    return HELP_ERROR;
  }
  safe_status=JTcl_GetInt(hptr, argv[1],&steps);
  if (safe_status!=HELP_OK || steps <1) {
      FPRINTF(ASCERR, "slv_iterate called with bad step count.\n");
      Asc_DStringFree(hptr);
      Asc_DStringSet(hptr, "slv_iterate called with bad step count.");
      return safe_status;
  }
  if ( argc == 3 ) {
    safe_status=Tcl_GetDouble(hptr, argv[2],&comptime);
    if (safe_status!=HELP_OK || comptime <0.1) {
        FPRINTF(ASCERR, "slv_iterate called with bad time limit.\n");
        Asc_DStringFree(hptr);
        Asc_DStringSet(hptr, "slv_iterate called with bad time limit.");
        return safe_status;
    }
  }
  Asc_DStringFree(hptr);
  if (g_solvsys_cur==NULL) {
    FPRINTF(ASCERR, "slv_iterate called with NULL system.\n");
    Asc_DStringSet(hptr, " empty solver context.");
    return HELP_ERROR;
  }

  start=tm_cpu_time();
  for (safe_status=0;safe_status<steps && delta <comptime;safe_status++) {
#ifdef ASC_SIGNAL_TRAPS
    if (SETJMP(g_fpe_env)==0) {
#endif /* ASC_SIGNAL_TRAPS */
      ::slv_get_status(g_solvsys_cur,&s);
      if (s.ready_to_solve && !Solv_C_CheckHalt_Flag) {
        ::slv_iterate(g_solvsys_cur);
      }
#ifdef ASC_SIGNAL_TRAPS
    } else {
      FPRINTF(ASCERR, "Floating point exception in slv_iterate!!\n");
      Asc_DStringSet(hptr, " Floating point exception in slv_iterate. Help!");
      return HELP_ERROR;
    }
#endif /* ASC_SIGNAL_TRAPS */
    delta=tm_cpu_time()-start;
  }
  return HELP_OK;
}

int ascjson::Asc_SolvAvailSolver(Asc_DString *hptr, int argc, CONST84 char *argv[])
{
  size_t i;

  const struct gl_list_t *L = ::solver_get_engines();
  SlvFunctionsT *S;
  for(i = 1; i <= gl_length(L); ++i){
	S = (SlvFunctionsT *)gl_fetch(L,i);
    VTcl_AppendElement(hptr,S->name);
  }
  VAEstrip(hptr);
  return HELP_OK;
}

/* JP... */
int ascjson::Asc_SolvSolverNum(Asc_DString *hptr , int argc, CONST84 char *argv[]
){
  char buf[8];
  const SlvFunctionsT *solver;

  if ( argc != 2 ) {
    FPRINTF(ASCERR, "call is: slv_number <name>\n");
    Asc_DStringSet(hptr, "one argument expected for slv_number");
    return HELP_ERROR;
  }

  solver = ::solver_engine_named(argv[1]);
  if(solver==NULL){
    FPRINTF(ASCERR, "Unknown solver '%s'!\n",argv[1]);
    Asc_DStringFree(hptr);
    Asc_DStringSet(hptr, "Unknown solver.");
    return HELP_ERROR;
  }else{
    sprintf(buf,"%d",solver->number);
    VTcl_AppendElement(hptr,&buf[0]);
    VAEstrip(hptr);
    return HELP_OK;
  }
  /* not reached */
}
/* ... JP */

int ascjson::Asc_SolvSolverName(Asc_DString *hptr , int argc, CONST84 char *argv[]
){
	const SlvFunctionsT *solver;
	if(argc!=2){
		FPRINTF(ASCERR, "Call is: slv_name <number>\n");
		Asc_DStringSet(hptr, "One argument expected for slv_name");
		return HELP_ERROR;
	}

	solver = solver_engine(atoi(argv[1]));
	if(solver==NULL){
		FPRINTF(ASCERR, "Unknown solver '%s' (=%d).\n",argv[1],atoi(argv[1]));
		Asc_DStringFree(hptr);
		Asc_DStringSet(hptr, "Unknown solver.");
		return HELP_ERROR;
	}else{
		VTcl_AppendElement(hptr,solver->name);
                VAEstrip(hptr);
		return HELP_OK;
	}
}

int ascjson::Asc_SolvLinsolNames(Asc_DString *hptr, int argc, CONST84 char *argv[])
{
  Asc_DStringSet(hptr,linsolqr_fmethods());
  return HELP_OK;
}

int ascjson::Asc_SolvEligSolver(Asc_DString *hptr, int argc, CONST84 char *argv[])
{
  /*  KHACK: removed 'n' from call to slv_eligible_solver
   *  may need to remove 'n' from this function totaly
   */
  slv_parameters_t sp;
  int cur;
  int status=0;
  size_t n;
  int tmpi;
  const SlvFunctionsT *S;
  const struct gl_list_t *L = NULL;

  if (( argc < 2 ) || ( argc > 3 )) {
    FPRINTF(ASCERR, "call is: slv_eligible_solver <solver number> [all]\n");
    Asc_DStringSet(hptr, "slv_eligible_solver: solver number expected");
    return HELP_ERROR;
  }
  if (g_solvsys_cur == NULL) {
    FPRINTF(ASCERR,  "slv_eligible_solver called with NULL pointer\n");
    Asc_DStringSet(hptr, "slv_eligible_solver called without slv_system");
    return HELP_ERROR;
  }

  ::slv_get_parameters(g_solvsys_cur,&sp);
  cur = ::slv_get_selected_solver(g_solvsys_cur);
  if (argc==3 && !!sp.output.less_important) {
    L = ::solver_get_engines();
    FPRINTF(ASCERR,"Solver   Name       ?Eligible\n");
    FPRINTF(ASCERR,"-----------------------------\n");
    for(n=1 ; n<=gl_length(L); ++n) {
      S = (SlvFunctionsT *)gl_fetch(L,n);
      FPRINTF(ASCERR, "%c%3zu     %-11s    %s\n",
              ((n==(size_t)cur) ? '*' : ' '), n,
              S->name, YORN((S->celigible)(g_solvsys_cur)));
    }
  }
  status=JTcl_GetInt(hptr, argv[1], &tmpi);
  Asc_DStringFree(hptr);
  if ((status==HELP_ERROR) || !solver_engine(tmpi<0)){
    Asc_DStringSet(hptr, "slv_eligible_solver: called with invalid solver number");
    return HELP_ERROR;
  } else {
    n = tmpi;
    if (::slv_eligible_solver(g_solvsys_cur)) {
      Asc_DStringSet(hptr, "1");
    } else {
      Asc_DStringSet(hptr, "0");
    }
  }
  return HELP_OK;
}

int ascjson::Asc_SolvSelectSolver(Asc_DString *hptr, int argc, CONST84 char *argv[])
{
  int status=HELP_OK;
  int solver;

  if ( argc != 2 ) {
    FPRINTF(ASCERR, "call is: slv_select_solver <N>\n");
    Asc_DStringSet(hptr, "1 argument expected for slv_select_solver");
    return HELP_ERROR;
  }
  if (g_solvsys_cur==NULL) {
    FPRINTF(ASCERR,  "slv_select_solver called with NULL pointer\n");
    Asc_DStringSet(hptr, "slv_select_solver called without slv_system");
    return HELP_ERROR;
  }
  status=JTcl_GetInt(hptr, argv[1], &solver);
  if(!solver_engine(solver) || (status==HELP_ERROR)) {
    FPRINTF(ASCERR, "Unknown solver (%d). Not selected!\n",solver);
    Asc_DStringFree(hptr);
    Asc_DStringSet(hptr, "Solver not available.");
    return HELP_ERROR;
  } else {
    char num[8];
	CONSOLE_DEBUG("...");
    int i = ::slv_get_selected_solver(g_solvsys_cur);
    if ( solver != i ) {
	  CONSOLE_DEBUG("...");
      i = ::slv_select_solver(g_solvsys_cur,solver);
    }
    sprintf(num,"%d",i);
    VTcl_AppendElement(hptr,&num[0]);
    VAEstrip(hptr);
    return HELP_OK;
  }
  /* not reached */
}

int ascjson::Asc_SolvGetSelectedSolver(Asc_DString *hptr, int argc, CONST84 char *argv[])
{
  int solver;
  char * tmps=NULL;


  tmps= (char *)ascmalloc((MAXIMUM_NUMERIC_LENGTH+1)*sizeof(char));
  if ( argc != 1 ) {
    FPRINTF(ASCERR, "call is: slv_get_solver <N>\n");
    Asc_DStringSet(hptr, "No args allowed for slv_get_solver");
    return HELP_ERROR;
  }
  if (g_solvsys_cur==NULL) {
    FPRINTF(ASCERR,  "slv_get_solver called with NULL pointer\n");
    Asc_DStringSet(hptr, "slv_get_solver called without slv_system");
    return HELP_ERROR;
  }
  solver =  ::slv_get_selected_solver(g_solvsys_cur);
  sprintf(tmps,"%d", solver);
  VTcl_AppendElement(hptr,tmps);
  VAEstrip(hptr);
  ascfree(tmps);
  return HELP_OK;
}

int ascjson::Asc_SolvFlushSolver(Asc_DString *hptr, int argc, CONST84 char *argv[])
{
  slv_system_t systmp;


  if (g_solvsys_cur != NULL) {
    systmp=g_solvsys_cur;
    system_destroy(systmp);
    g_solvsys_cur = NULL;
    g_solvinst_cur = NULL;
    g_solvinst_root = NULL;
  }
  return HELP_OK;
}

int ascjson::Asc_SolvMakeIndependent(Asc_DString *hptr, int argc, CONST84 char *argv[])
{
  int j,k,tmpi,status=HELP_OK;
  int32 maxvar,freevar;
  struct var_variable **vp=NULL;
  var_filter_t vfilter;
  slv_system_t sys=NULL;
  int32 *swapvars=NULL;
  int32 *unassvars=NULL;
  mtx_range_t rng;
  mtx_matrix_t mtx=NULL;
  char res[40];

  if ( argc < 2 ) {
    FPRINTF(ASCERR, "call is: slv_set_independent <ndx ...>\n");
    Asc_DStringSet(hptr, "slv_set_independent wants at least 1 var index");
    return HELP_ERROR;
  }
  sys=g_solvsys_cur;
  if (sys==NULL) {
    FPRINTF(ASCERR,  "slv_set_independent called with NULL pointer\n");
    Asc_DStringSet(hptr, "slv_set_independent without slv_system");
    return HELP_ERROR;
  }
  mtx=slv_get_sys_mtx(sys);
  if (mtx==NULL) {
    FPRINTF(ASCERR,"slv_set_independent found no matrix. odd!\n");
    Asc_DStringSet(hptr, "slv_set_independent found no matrix. odd!");
    return HELP_ERROR;
  }
  vp=slv_get_solvers_var_list(sys);
  if (vp==NULL) {
    FPRINTF(ASCERR,  "slv_set_independent called with NULL varlist\n");
    Asc_DStringSet(hptr, "slv_set_independent called without varlist");
    return HELP_ERROR;
  }

  maxvar=slv_get_num_solvers_vars(sys);

  vfilter.matchbits = (VAR_INCIDENT | VAR_ACTIVE);
  vfilter.matchvalue = (VAR_INCIDENT | VAR_ACTIVE);
  freevar=slv_count_solvers_vars(sys,&vfilter);
  rng.high=freevar-1;
  rng.low=mtx_symbolic_rank(mtx);
  if ( (argc-1) > (rng.high-rng.low+1) ) {
    FPRINTF(ASCERR,  "slv_set_independent called with too many vars\n");
    Asc_DStringSet(hptr, "slv_set_independent called with too many vars");
    return HELP_ERROR;
  }

  swapvars=(int32 *)ascmalloc(sizeof(int32)*(argc-1));
  k=rng.high-rng.low+1;
  unassvars=ASC_NEW_ARRAY(int32,k);
  for (j=0;j<k;j++) {
    unassvars[j]=mtx_col_to_org(mtx,j+rng.low); /* current outsiders */
  }
  for (j=1;j<argc;j++) {
    tmpi=maxvar;
    status=JTcl_GetInt(hptr, argv[j],&tmpi);
    if (tmpi<0 || tmpi >= maxvar) {
      status=HELP_ERROR;
    }
    if (status!=HELP_OK) {
      FPRINTF(ASCERR,
        "slv_set_independent: %d is not number in variable list\n",tmpi);
      Asc_DStringFree(hptr);
      Asc_DStringSet(hptr, "slv_set_independent: invalid variable number");
      if (swapvars) {
        ascfree(swapvars);
      }
      if (unassvars) {
        ascfree(unassvars);
      }
      return status;
    } else {
      swapvars[j-1]=tmpi; /*var index numbers*/
    }
  }
  k=argc-1;
  for (j=0;j<k;j++) {
    if (slv_change_basis(sys,swapvars[j],&rng) ) {
      for (tmpi=rng.low;tmpi<=rng.high;tmpi++) {
        if (unassvars[tmpi-rng.low]!=mtx_col_to_org(mtx,tmpi)) {
          int32 tmpd;
          mtx_swap_cols(mtx,tmpi,rng.high);
          tmpd=unassvars[tmpi-rng.low];
          unassvars[tmpi-rng.low]=unassvars[rng.high-rng.low];
          unassvars[rng.high-rng.low]=tmpd;
          break;
        }
      }
      rng.high--;
    } else {
      char *name;
      name=var_make_name(sys,vp[swapvars[j]]);
      FPRINTF(ASCERR,"Unable to remove %s from the basis.\n",name);
      ascfree(name);
      sprintf(res,"%d",swapvars[j]);
      VTcl_AppendElement(hptr,res);
    }
  }
  if (swapvars) {
    ascfree(swapvars);
  }
  if (unassvars) {
    ascfree(unassvars);
  }
  VAEstrip(hptr);
  return HELP_OK;
}


int ascjson::Asc_SolvImportQlfdid(Asc_DString *hptr ,int argc, CONST84 char *argv[]
){
  int status, listc,prevs=0;
  char *temp=NULL;
  CONST84 char **listargv=NULL;
  slv_system_t systmp;
  enum inst_t ikind;
  struct Instance *solvinst_pot=NULL;  /* potential solve instance */
  struct Instance *solvinst_root_pot=NULL;  /* potential solve instance */

  if (argc<2 || argc>3) {
    Asc_DStringSet(hptr, "slv_import_qlfdid <qlfdid> [test]");
    return HELP_ERROR;
  }

  status=Asc_BrowQlfdidSearchCmdDS(hptr, (int)2, argv);
  temp = Asc_DStringResult(hptr);
  fprintf(stderr,"slvimportiname: %s\n",temp);

  if (status==HELP_OK) {
    /* catch inst ptr */
    solvinst_pot = g_search_inst;
    /* catch root name */
    status=VTcl_SplitList(hptr, temp, &listc, &listargv);
    if (status!=HELP_OK) { /* this should never happen */
      freeArgv(listargv);
      Asc_DStringFree(hptr);
      Asc_DStringSet(hptr, "slv_import_qlfdid: error in split list for sim");
      FPRINTF(ASCERR, "wierdness in slv_import_qlfdid splitlist.\n");
      solvinst_pot =NULL;
      if (temp) {
        ascfree(temp);
      }
      temp=NULL;
      return status;
    }
    /* catch root inst ptr */
    solvinst_root_pot = Asc_FindSimulationRoot(AddSymbol(listargv[0]));
    freeArgv(listargv);
    if (!solvinst_root_pot) { /*an error we should never reach, knock wood */
      Asc_DStringFree(hptr);
      FPRINTF(ASCERR, "NULL simulation found by slv_import_qlfdid. %s\n",temp);
      Asc_DStringSet(hptr, "slv_import_qlfdid: Simulation specified not found.");
      if (temp) {
        ascfree(temp);
      }
      temp=NULL;
      return HELP_ERROR;
    }
  } else {
    /* failed. bail out. */
    Asc_DStringSet(hptr, "slv_import_qlfdid: Asc_BrowQlfdidSearchCmd: ");
    Asc_DStringAppend(hptr, temp, HALL);
    FPRINTF(ASCERR, "slv_import_qlfdid: Asc_BrowQlfdidSearchCmd error\n");
    if (temp) {
      ascfree(temp);
    }
    temp=NULL;
    return status;
  }
  /* got something worth having */
  if(temp){
    ascfree(temp);
  }
  temp=NULL;
  Asc_DStringFree(hptr);

  /* check that instance is model */
  ikind=InstanceKind(solvinst_pot);
  if (ikind!=MODEL_INST) {
    switch (argc) {
      case 3: /* just testing */
        Asc_DStringSet(hptr, "1");
        return HELP_OK;
      default: /*report import error */
        FPRINTF(ASCERR,  "Instance imported is not a solvable kind.\n");
        Asc_DStringSet(hptr, "Instance kind not MODEL.");
        return HELP_ERROR;
    }
  }

  /* check instance is complete */
  if (NumberPendingInstances(solvinst_pot)!=0) {
    switch (argc) {
      case 3: /* just testing */
        Asc_DStringSet(hptr, "1");
        CheckInstance(ASCERR,solvinst_pot);
        return HELP_OK;
      default: /*report import error */
        FPRINTF(ASCERR,  "Instance imported is incomplete: %ld pendings.\n",
                NumberPendingInstances(solvinst_pot));
        Asc_DStringSet(hptr, "Instance has pendings: Not imported.");
        return HELP_ERROR;
    }
  }

  if(argc == 2){ /*not just testing */
    /* Here we will check to see if we really need to do
        all of this work by:
        1) Checking if the potential and current instance pointers are equal
        2) Checking a global counter to see if the compiler has been called
    */
    if (g_solvsys_cur == NULL) {
      g_compiler_counter = 1; /* initialize compiler counter */
    }
    if (solver_engine(prevs) == NULL) {
      prevs = slv_lookup_client("QRSlv"); // should take this from envvar
    }
    if (g_solvinst_cur == solvinst_pot && g_compiler_counter == 0
        && g_solvinst_cur != NULL) {
      prevs = ::slv_get_selected_solver(g_solvsys_cur);
      CONSOLE_DEBUG("...");
      ::slv_select_solver(g_solvsys_cur,prevs);
      Asc_DStringSet(hptr, "Solver instance created.");
#if SP_DEBUG
      FPRINTF(ASCERR,"YOU JUST AVOIDED A TOTAL REBUILD\n");
#endif
      return HELP_OK;
    }

    /* flush old system */
    g_solvinst_cur=solvinst_pot;
    g_solvinst_root=solvinst_root_pot;
    if (g_solvsys_cur != NULL) {
      prevs = ::slv_get_selected_solver(g_solvsys_cur);
      if (solver_engine(prevs) == NULL) {
	      prevs = slv_lookup_client("QRSlv"); // should take this string from envvar?
      }
      systmp=g_solvsys_cur;
      system_destroy(systmp);
      g_solvsys_cur = NULL;
    }

    /* create system */
    if( g_solvsys_cur == NULL ) {
      g_solvsys_cur = system_build(g_solvinst_cur);
      if( g_solvsys_cur == NULL ) {
        FPRINTF(ASCERR,"system_build returned NULL.\n");
        Asc_DStringSet(hptr, "Bad relations found: solve system not created.");
        return HELP_ERROR;
      }
    }

    if( g_solvsys_cur == NULL ) {
      FPRINTF(ASCERR,"system_build returned NULL!\n");
      Asc_DStringSet(hptr, "importqlfdid:  solve system not created.");
      return HELP_ERROR;
    }
	CONSOLE_DEBUG("...");
    ::slv_select_solver(g_solvsys_cur,prevs);
    Asc_DStringSet(hptr, "Solver instance created.");
    g_compiler_counter = 0;  /* set counter to 0 after full import */
  } else {
    Asc_DStringSet(hptr, "0");
  }
  return HELP_OK;
}


int ascjson::Asc_SolvGetLnmEpsilon(Asc_DString *hptr ,int argc, CONST84 char *argv[]
){
  char buf[MAXIMUM_NUMERIC_LENGTH];   /* string to hold integer */

  if ( argc > 1 ) {
    Asc_DStringSet(hptr, "slv_lnmget takes no argument.");
    return HELP_ERROR;
  }
  sprintf(buf, "%g",FuncGetLnmEpsilon());
  Asc_DStringSet(hptr, buf);
  return HELP_OK;
}

int ascjson::Asc_SolvSetLnmEpsilon(Asc_DString *hptr,
                       int argc, CONST84 char *argv[])
{
  double eps;

  if ( argc != 2 ) {
    Asc_DStringSet(hptr, "slv_lnmset takes 1 positive # argument.");
    return HELP_ERROR;
  }
  eps=FuncGetLnmEpsilon();
  if( Tcl_GetDouble(hptr, argv[1],&eps)==HELP_ERROR) {
    Asc_DStringFree(hptr);
    Asc_DStringSet(hptr, "slv_lnmset: arg 1 not real number");
    return HELP_ERROR;
  }
  if (eps < 0.5) {
    FuncSetLnmEpsilon(eps);
  } else {
    FPRINTF(ASCERR,"Modified log epsilon > 0.5 not allowed. Eps = %g.\n",eps);
  }
  return HELP_OK;
}

/*
 * Solv_C_CheckHalt_Flag is defined in slv.[ch].
 */
int ascjson::Asc_SolvSetCHaltFlag(Asc_DString *hptr,
                       int argc, CONST84 char *argv[])
{
  int value;

  if ( argc != 2 ) {
    Asc_DStringSet(hptr, "wrong # args : Usage slv_set_haltflag");
    return HELP_ERROR;
  }
  value = atoi(argv[1]);
  if (value) {
    Solv_C_CheckHalt_Flag = 1; /* any nonzero value will set the flag on. */
  } else {
    Solv_C_CheckHalt_Flag = 0; /* otherwise turn it off */
  }
  return HELP_OK;
}

#define LONGHELP(b,ms) ((b)?ms:"")
int ascjson::Asc_SolvHelpList(Asc_DString *hptr,
                   int argc, CONST84 char *argv[])
{
  boolean detail=1;

  if ( argc > 2 ) {
    FPRINTF(ASCERR,"call is: slvhelp [s,l] \n");
    Asc_DStringSet(hptr, "Too many args to slvhelp. Want 0 or 1 args");
    return HELP_ERROR;
  }
  if ( argc == 2 ) {
    if (argv[1][0]=='s') {
      detail=0;
    }
    if (argv[1][0]=='l') {
      detail=1;
    }
    PRINTF("%-25s%s\n","slv_trapint",
           LONGHELP(detail,"turn ctrl-c traps on for solver"));
    PRINTF("%-25s%s\n","slv_untrapint",
           LONGHELP(detail,"turn ctrl-c traps off."));
    PRINTF("%-25s%s\n","slv_trapfp",
           LONGHELP(detail,"turn floating point traps on for solver"));
    PRINTF("%-25s%s\n","slv_untrapfp",
           LONGHELP(detail,"turn floating point traps off. take core dump."));
    PRINTF("%-25s%s\n","slv_checksim",
           LONGHELP(detail,"see if simulation has pendings:0ok,1incomplete"));
    PRINTF("%-25s%s\n","slv_checksys",
           LONGHELP(detail,"see if solver is occupied:0free,1busy"));
    PRINTF("%-25s%s\n","slv_get_parms",
           LONGHELP(detail,"get list of solver parameters."));
    PRINTF("%-25s%s\n","set_slv_parms",
           LONGHELP(detail,"set list of solver parameters."));
    PRINTF("%-25s%s\n","slv_get_insttype",
           LONGHELP(detail,"get typename of model instance being solved."));

    PRINTF("%-25s%s\n","slv_get_cost_page",
           LONGHELP(detail,"get list of block costs."));
    PRINTF("%-25s%s\n","slv_get_stat_page",
           LONGHELP(detail,"get list of status values."));
    PRINTF("%-25s%s\n","slv_get_objval",
           LONGHELP(detail,"get value of objective function"));
    PRINTF("%-25s%s\n","slv_get_instname",
           LONGHELP(detail,"get instance path name from instroot to instcur"));
    PRINTF("%-25s%s\n","slv_get_pathname",
           LONGHELP(detail,"get solver inst qlfdid"));
    PRINTF("%-25s%s\n","slvdump",
           LONGHELP(detail,"dump something about the solver insides."));

    PRINTF("%-25s%s\n","slv_reanalyze",
           LONGHELP(detail,"reanalyze the solver lists of g_solvsys_cur ."));
    PRINTF("%-25s%s\n","slv_check_and_reanalyze",
           LONGHELP(detail,"reanalyze g_solvsys_cur if a whenvar changes."));
    PRINTF("%-25s%s\n","slv_get_vr",
           LONGHELP(detail,"return some counts of rels/vars."));
    PRINTF("%-25s%s\n","slv_presolve",
           LONGHELP(detail,"call presolve on the g_solvsys_cur."));
    PRINTF("%-25s%s\n","slv_resolve",
           LONGHELP(detail,"call resolve on g_solvsys_cur."));
    PRINTF("%-25s%s\n","slv_solve",
           LONGHELP(detail,"call solve on g_solvsys_cur."));
    PRINTF("%-25s%s\n","slv_iterate",
           LONGHELP(detail,"call solve_iterate on g_solvsys_cur."));

    PRINTF("%-25s%s\n","slv_available",
           LONGHELP(detail,"list names of all known solvers"));

    PRINTF("%-25s%s\n","slv_number",
           LONGHELP(detail,"lookup the solver number for a named solver"));

    PRINTF("%-25s%s\n","slv_linsol_names",
           LONGHELP(detail,"list names of all linear options for Slv class"));
    PRINTF("%-25s%s\n","slv_eligible_solver",
           LONGHELP(detail,"boolean check of current solver eligibility"));
    PRINTF("%-25s%s\n","slv_select_solver",
           LONGHELP(detail,"set solver to use."));
    PRINTF("%-25s%s\n","slv_get_solver",
           LONGHELP(detail,"return solver number in use."));
    PRINTF("%-25s%s\n","slv_flush_solver",
           LONGHELP(detail,"blow away g_solvsys_cur"));
    PRINTF("%-25s%s\n","slv_set_independent",
           LONGHELP(detail,"select set of independent (superbasic) vars"));

    PRINTF("%-25s%s\n","slv_import_qlfdid",
           LONGHELP(detail,"focus solver on qualified name, or test it."));
    PRINTF("%-25s%s\n","get_model_children",
           LONGHELP(detail,"return the list of MODEL children of a qlfdid"));

    PRINTF("%-25s%s\n","slv_lnmget",
           LONGHELP(detail,"return lnm epsilon value"));
    PRINTF("%-25s%s\n","slv_lnmset",
           LONGHELP(detail,"set lnm epsilon value"));
    PRINTF("%-25s%s\n","integration commands",
           LONGHELP(detail,""));
    PRINTF("%-25s%s\n","integrate_able",
           LONGHELP(detail,"check solver problem for integrability"));
    PRINTF("%-25s%s\n","integrate_setup",
           LONGHELP(detail,"setup and integrate an ivp in solver"));
    PRINTF("%-25s%s\n","integrate_cleanup",
           LONGHELP(detail,"tidy up after an ivp in solver"));
    PRINTF("%-25s%s\n","slvhelp",
           LONGHELP(detail,"slvhelp s(=names only) l(=this list)."));

    PRINTF("\n");
  }
  if ( argc == 1 ) {
    char * tmps=NULL;
    tmps= (char *)ascmalloc((MAXIMUM_NUMERIC_LENGTH+1)*sizeof(char));
    sprintf(tmps,"slv_checksys");
    VTcl_AppendElement(hptr,tmps);
    sprintf(tmps,"slv_trapfp");
    VTcl_AppendElement(hptr,tmps);
    sprintf(tmps,"slv_untrapfp");
    VTcl_AppendElement(hptr,tmps);
    sprintf(tmps,"slv_checksim");
    VTcl_AppendElement(hptr,tmps);
    sprintf(tmps,"slv_get_parm");
    VTcl_AppendElement(hptr,tmps);
    sprintf(tmps,"set_slv_parm");
    VTcl_AppendElement(hptr,tmps);
    sprintf(tmps,"slv_get_insttype");
    VTcl_AppendElement(hptr,tmps);

    sprintf(tmps,"slv_get_cost_page");
    VTcl_AppendElement(hptr,tmps);
    sprintf(tmps,"slv_get_stat_page");
    VTcl_AppendElement(hptr,tmps);
    sprintf(tmps,"slv_get_objval");
    VTcl_AppendElement(hptr,tmps);
    sprintf(tmps,"slv_get_instname");
    VTcl_AppendElement(hptr,tmps);
    sprintf(tmps,"slv_get_pathname");
    VTcl_AppendElement(hptr,tmps);
    sprintf(tmps,"slvdump");
    VTcl_AppendElement(hptr,tmps);

    sprintf(tmps,"slv_reanalyze");
    VTcl_AppendElement(hptr,tmps);
    sprintf(tmps,"slv_check_and_reanalyze");
    VTcl_AppendElement(hptr,tmps);
    sprintf(tmps,"slv_get_vr");
    VTcl_AppendElement(hptr,tmps);
    sprintf(tmps,"slv_presolve");
    VTcl_AppendElement(hptr,tmps);
    sprintf(tmps,"slv_resolve");
    VTcl_AppendElement(hptr,tmps);
    sprintf(tmps,"slv_solve");
    VTcl_AppendElement(hptr,tmps);
    sprintf(tmps,"slv_iterate");
    VTcl_AppendElement(hptr,tmps);

    sprintf(tmps,"slv_available");
    VTcl_AppendElement(hptr,tmps);

    sprintf(tmps,"slv_number");
    VTcl_AppendElement(hptr,tmps);

    sprintf(tmps,"slv_linsol_names");
    VTcl_AppendElement(hptr,tmps);
    sprintf(tmps,"slv_eligible_solver");
    VTcl_AppendElement(hptr,tmps);
    sprintf(tmps,"slv_select_solver");
    VTcl_AppendElement(hptr,tmps);
    sprintf(tmps,"slv_get_solver");
    VTcl_AppendElement(hptr,tmps);
    sprintf(tmps,"slv_flush_solver");
    VTcl_AppendElement(hptr,tmps);
    sprintf(tmps,"slv_set_independent");
    VTcl_AppendElement(hptr,tmps);

    sprintf(tmps,"slv_import_qlfdid");
    VTcl_AppendElement(hptr,tmps);
    sprintf(tmps,"slv_import_sim");
    VTcl_AppendElement(hptr,tmps);
    sprintf(tmps,"slv_lnmget");
    VTcl_AppendElement(hptr,tmps);
    sprintf(tmps,"slv_lnmset");
    VTcl_AppendElement(hptr,tmps);
    sprintf(tmps,"integrate_able");
    VTcl_AppendElement(hptr,tmps);
    sprintf(tmps,"integrate_setup");
    VTcl_AppendElement(hptr,tmps);
    sprintf(tmps,"integrate_cleanup");
    VTcl_AppendElement(hptr,tmps);
    sprintf(tmps,"slvhelp");
    VTcl_AppendElement(hptr,tmps);
    ascfree(tmps);
  }
  VAEstrip(hptr);
  return HELP_OK;
}


/*NOTE: Output is not terribly meaninful when put to stdout or ASCERR */
int ascjson::Asc_SolvNearBounds(Asc_DString *hptr,
                     int argc, CONST84 char *argv[])
{
  int32 *rip=NULL;
  real64 epsilon;
  char tmps[MAXIMUM_NUMERIC_LENGTH];
  int i,dev,status,count;
  FILE *fp;

  if ( argc != 3 ) {
    FPRINTF(ASCERR,  "call is: slv_near_bounds epsilon <out>\n");
    Asc_DStringSet(hptr, "slv_near_bounds wants epsilon and output device.");
    return HELP_ERROR;
  }
  if (g_solvsys_cur==NULL) {
    FPRINTF(ASCERR,  "slv_near_bounds called with NULL pointer\n");
    Asc_DStringSet(hptr, "slv_near_bounds called without slv_system");
    return HELP_ERROR;
  }
  /* get io option */
  i=3;
  status=Tcl_GetDouble(hptr, argv[1],&epsilon);
  status=JTcl_GetInt(hptr, argv[2],&i);
  if (i<0 || i >2) {
    status=HELP_ERROR;
  }
  if (status!=HELP_OK) {
    FPRINTF(ASCERR,"slv_near_bounds: first arg is 0,1, or 2\n");
    Asc_DStringFree(hptr);
    Asc_DStringSet(hptr, "slv_near_bounds: invalid output dev #");
    return status;
  } else {
    dev=i;
  }
  switch (dev) {
  case 0: fp=stdout;
    break;
  case 1: fp=ASCERR;
    break;
  case 2: fp=NULL;
    break;
  default : /* should never be here */
    FPRINTF(ASCERR,"slv_near_bounds called with strange i/o option\n");
    return HELP_ERROR;
  }
  if ((count = ::slv_near_bounds(g_solvsys_cur,epsilon,&rip)) > 0) {
    count += 2;
    switch (dev) {
    case 0:
    case 1:
      FPRINTF(fp,"Objective indices:\n");
      for (i=0; i < count;i++) {
        FPRINTF(fp,"%d\n",rip[i]);
      }
      break;
    case 2:
      Asc_DStringAppend(hptr,"{",HALL);
      for (i=0; i < count;i++) {
        sprintf(tmps,"%d ",rip[i]);
        Asc_DStringAppend(hptr,tmps,HALL);
      }
      Asc_DStringAppend(hptr,"}",HALL);
      break;
    default:
      FPRINTF(ASCERR,"wierdness in i/o!");
      break;
    }
  } else {
    Asc_DStringSet(hptr, "{}");
  }
  if (rip) {
    ascfree(rip);
  }

  return HELP_OK;
}

/*NOTE: Output is not terribly meaninful when put to stdout or ASCERR */
int ascjson::Asc_SolvFarFromNominal(Asc_DString *hptr,
                          int argc, CONST84 char *argv[])
{
  int32 *rip=NULL;
  real64 bignum;
  char tmps[MAXIMUM_NUMERIC_LENGTH];
  int i,dev,status,count;
  FILE *fp;

  if ( argc != 3 ) {
    FPRINTF(ASCERR,  "call is: slv_far_from_nom <bignum> <out>\n");
    Asc_DStringSet(hptr,
                  "slv_far_from_nominals wants bignum and output device.");
    return HELP_ERROR;
  }
  if (g_solvsys_cur==NULL) {
    FPRINTF(ASCERR,  "slv_far_from_nominals called with NULL pointer\n");
    Asc_DStringSet(hptr, "slv_far_from_nominals called without slv_system");
    return HELP_ERROR;
  }
  /* get io option */
  i=3;
  status=Tcl_GetDouble(hptr, argv[1],&bignum);
  status=JTcl_GetInt(hptr, argv[2],&i);
  if (i<0 || i >2) {
    status=HELP_ERROR;
  }
  if (status!=HELP_OK) {
    FPRINTF(ASCERR,"slv_far_from_nominals: first arg is 0,1, or 2\n");
    Asc_DStringFree(hptr);
    Asc_DStringSet(hptr, "slv_far_from_nominals: invalid output dev #");
    return status;
  } else {
    dev=i;
  }
  switch (dev) {
  case 0: fp=stdout;
    break;
  case 1: fp=ASCERR;
    break;
  case 2: fp=NULL;
    break;
  default : /* should never be here */
    FPRINTF(ASCERR,"slv_far_from_nominals called with strange i/o option\n");
    return HELP_ERROR;
  }
  if ((count = ::slv_far_from_nominals(g_solvsys_cur,bignum,&rip)) > 0) {
    switch (dev) {
    case 0:
    case 1:
      FPRINTF(fp,"Objective indices:\n");
      for (i=0; i < count;i++) {
        FPRINTF(fp,"%d\n",rip[i]);
      }
      break;
    case 2:
      Asc_DStringAppend(hptr,"{",HALL);
      for (i=0; i < count;i++) {
        sprintf(tmps,"%d ",rip[i]);
        Asc_DStringAppend(hptr,tmps,HALL);
      }
      Asc_DStringAppend(hptr,"}",HALL);
      break;
    default:
      FPRINTF(ASCERR,"wierdness in i/o!");
      break;
    }
  } else {
    Asc_DStringSet(hptr, "{}");
  }
  if (rip) {
    ascfree(rip);
  }

  return HELP_OK;
}
