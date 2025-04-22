/*	ASCEND modelling environment
	Copyright 1997, Carnegie Mellon University
	Copyright (C) 2006 Carnegie Mellon University

	This program is free software; you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation; either version 2, or (at your option)
	any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program.  If not, see <http://www.gnu.org/licenses/>.
*//**
	@file
	Tcl/Tk interface functions for the Integration feature
*//*
	by Kirk Abbott, Ben Allan, John Pye
	Created: 1/94
	Last in CVS: $Revision: 1.32 $ $Date: 2003/08/23 18:43:06 $ $Author: ballan $
*/

#define ASC_BUILDING_INTERFACE
#if 0
#include <tcl.h>
#include <time.h>

#include "config.h"

#include <ascend/compiler/instance_io.h>
#include <ascend/compiler/units.h>
#include <ascend/integrator/integrator.h>
#include <ascend/integrator/samplelist.h>

#include "HelpProc.h"
#include "Integrators.h"
#include "BrowserQuery.h"
#include "Qlfdid.h"
#include "UnitsProc.h"
#include "BrowserProc.h"
#include "HelpProc.h"
#include "SolverGlobals.h"

#define SNULL (char *)NULL
#endif

static SampleList l_samplelist;

/*-------------------------------------------------------
  HANDLING OF OUTPUT FILES
*/

/* vars relating to output files */
static FILE *l_obs_file = NULL;
static char *l_obs_filename = NULL;

static FILE *l_y_file = NULL;
static char *l_y_filename = NULL;

static int l_print_option = 1; /* default si */
static int l_print_fmt = 0; /* default variable */

FILE *ascjson::Asc_IntegOpenYFile(void)
{
  if (l_y_filename==NULL) {
    return NULL;
  }
  l_y_file = fopen(l_y_filename,"a+");

  if (l_y_file==NULL) {
    FPRINTF(ASCERR,
      "WARNING: (integrate) Unable to open\n\t%s\nfor state output log.\n",
      l_y_filename);
  } else {
    time_t t;
    t = time((time_t *)NULL);
    FPRINTF(l_y_file,"DATASET %s", asctime(localtime(&t)));
    FFLUSH(l_y_file);
  }
  return l_y_file;
}

FILE *ascjson::Asc_IntegOpenObsFile(void)
{
  if (l_obs_filename==NULL) {
    return NULL;
  }
  l_obs_file = fopen(l_obs_filename,"a+");
  if (l_obs_file==NULL) {
    FPRINTF(ASCERR,
      "WARNING: (integrate) Unable to open\n\t%s\nfor observation log.\n",
      l_obs_filename);
  } else {
    time_t t;
    t = time((time_t *)NULL);
    FPRINTF(l_obs_file,"DATASET %s", asctime(localtime(&t)));
    FFLUSH(l_obs_file);
  }
  return l_obs_file;
}

FILE *ascjson::Asc_IntegGetYFile(void)
{
  return l_y_file;
}

FILE *ascjson::Asc_IntegGetObsFile(void)
{
  return l_obs_file;
}

void ascjson::Asc_IntegReleaseYFile(void)
{
  l_y_file = NULL;
}
void ascjson::Asc_IntegReleaseObsFile(void)
{
  l_obs_file = NULL;
}

/********************************************************************/
/* string column layout: 26 char columns  if l_print_fmt else variable */
/* numeric format: leading space plus characters from Unit*Value */
/* index format: left padded long int */
/* headline format space plus - s */
#define BCOLSFMT (l_print_fmt ? "%-26s" : "\t%s")
#define BCOLNFMT (l_print_fmt ? " %-25s" : "\t%s")
#define BCOLIFMT (l_print_fmt ? " %25ld" : "\t%ld")
#define BCOLHFMT (l_print_fmt ? " -------------------------" : "\t---")

void ascjson::Asc_IntegPrintYHeader(FILE *fp, IntegratorSystem *blsys)
{
  long i,len;
  int *yip;
  char *name;
  struct Instance *in;
  int si;
  if (fp==NULL) {
    return;
  }
  if (blsys==NULL) {
    FPRINTF(ASCERR,"WARNING: (Asc_IntegPrintYHeader: called w/o data\n");
    return;
  }
  if (blsys->n_y == 0) {
    return;
  }
  if (blsys->y == NULL) {
    FPRINTF(ASCERR,"ERROR: (Asc_IntegPrintYHeader: called w/NULL data\n");
    return;
  }
  len = blsys->n_y;
  yip = blsys->y_id;
  si = l_print_option;
  /* output indep var name */
  /* output dep var names */
  FPRINTF(fp,"States: (user index) (name) (units)\n");
  in = T2I(var_instance(blsys->x));
  FPRINTF(fp,"{indvar}");  /* indep id name */
  name = WriteInstanceNameString(in,g_solvinst_cur);
  FPRINTF(fp,"\t{%s}\t{%s}\n",name,Asc_UnitStringDS(in,si));
  ascfree(name);
  for (i=0; i< len; i++) {
    in = T2I(var_instance(blsys->y[i]));
    FPRINTF(fp,"{%ld}",yip[i]);  /* user id # */
    name = WriteInstanceNameString(in,g_solvinst_cur);
    FPRINTF(fp,"\t{%s}\t{%s}\n",name,Asc_UnitStringDS(in,si));
    ascfree(name);
  }
  FPRINTF(fp,BCOLSFMT,"indvar");
  for (i=0; i < len; i++) {
    FPRINTF(fp,BCOLIFMT,yip[i]);
  }
  FPRINTF(fp,"\n");
  for (i=0; i <= len; i++) {
    FPRINTF(fp,BCOLHFMT);
  }
  FPRINTF(fp,"\n");
}
/********************************************************************/
void ascjson::Asc_IntegPrintObsHeader(FILE *fp, IntegratorSystem *blsys)
{
  long i,len;
  int *obsip;
  char *name;
  struct Instance *in;
  int si;
  if (fp==NULL) {
    return;
  }
  if (blsys==NULL) {
	ERROR_REPORTER_HERE(ASC_PROG_ERR,"called without data");
    return;
  }
  if (blsys->n_obs == 0) {
    return;
  }
  if (blsys->obs == NULL) {
	ERROR_REPORTER_HERE(ASC_PROG_ERR,"called with NULL data");
    return;
  }
  len = blsys->n_obs;
  obsip = blsys->obs_id;
  si = l_print_option;
  FPRINTF(fp,"Observations: (user index) (name) (units)\n");
  /* output indep var name */
  /* output obs var names */
  in = T2I(var_instance(blsys->x));
  FPRINTF(fp,"{indvar}");  /* indep id name */
  name = WriteInstanceNameString(in,g_solvinst_cur);
  FPRINTF(fp,"\t{%s}\t{%s}\n",name,Asc_UnitStringDS(in,si));
  ascfree(name);
  for (i=0; i< len; i++) {
    in = T2I(var_instance(blsys->obs[i]));
    FPRINTF(fp,"{%ld}",obsip[i]);  /* user id # */
    name = WriteInstanceNameString(in,g_solvinst_cur);
    FPRINTF(fp,"\t{%s}\t{%s}\n",name,Asc_UnitStringDS(in,si));
    ascfree(name);
  }
  FPRINTF(fp,BCOLSFMT,"indvar");
  for (i=0; i < len; i++) {
    FPRINTF(fp,BCOLIFMT,obsip[i]);
  }
  FPRINTF(fp,"\n");
  for (i=0; i <= len; i++) {
    FPRINTF(fp,BCOLHFMT);
  }
  FPRINTF(fp,"\n");
}

/********************************************************************/

/**
	@return 1 on success
*/
int ascjson::Asc_IntegPrintYLine(FILE *fp, IntegratorSystem *blsys)
{
  long i,len;
  struct var_variable **vp;
  int si;
  if (fp==NULL) {
    return 0;
  }
  if (blsys==NULL) {
    FPRINTF(ASCERR,"WARNING: (Asc_IntegPrintYLine: called w/o data\n");
    return 0;
  }
  if (blsys->n_y == 0) {
    return 0;
  }
  if (blsys->y == NULL) {
    FPRINTF(ASCERR,"ERROR: (Asc_IntegPrintYHeader: called w/NULL data\n");
    return 0;
  }
  vp = blsys->y;
  len = blsys->n_y;
  si = l_print_option;
  FPRINTF(fp,BCOLNFMT,Asc_UnitlessValueDS(T2I(var_instance(blsys->x)),si));
  for (i=0; i < len; i++) {
    FPRINTF(fp,BCOLNFMT, Asc_UnitlessValueDS(T2I(var_instance(vp[i])),si));
  }
  FPRINTF(fp,"\n");
  return 1;
}

/**
	@return 1 on success
*/
int ascjson::Asc_IntegPrintObsLine(FILE *fp, IntegratorSystem *blsys){
  long i,len;
  struct var_variable **vp;
  int si;
  if (fp==NULL) {
    return 0;
  }
  if (blsys==NULL) {
    FPRINTF(ASCERR,"WARNING: (Asc_IntegPrintObsLine: called w/o data\n");
    return 0;
  }
  if (blsys->n_obs == 0) {
    return 0;
  }
  if (blsys->obs == NULL) {
    FPRINTF(ASCERR,"ERROR: (Asc_IntegPrintObsHeader: called w/NULL data\n");
    return 0;
  }
  vp = blsys->obs;
  len = blsys->n_obs;
  si = l_print_option;
  FPRINTF(fp,BCOLNFMT,Asc_UnitlessValueDS(T2I(var_instance(blsys->x)),si));
  for (i=0; i < len; i++) {
    FPRINTF(fp,BCOLNFMT, Asc_UnitlessValueDS(T2I(var_instance(vp[i])),si));
  }
  FPRINTF(fp,"\n");
  return 1;
}


/*---------------------------------------------*/

int ascjson::Asc_IntegSetYFileCmd(Asc_DString *hptr, int argc, CONST84 char *argv[])
{
  size_t len;

  if ( argc != 2 ) {
    FPRINTF(ASCERR, "integrate_set_y_file: called without filename.\n");
    Asc_DStringSet(hptr,
                  "integrate_set_y_file <filename,""> called without arg.");
    return HELP_ERROR;
  }
  if (l_y_filename != NULL) {
    ascfree(l_y_filename);
  }
  len = strlen(argv[1]);
  if (len >0 ) {
    l_y_filename = Asc_MakeInitString((int)len);
    sprintf(l_y_filename,"%s",argv[1]);
  } else {
    l_y_filename = NULL;
  }
  return HELP_OK;
}

/*---------------------------------------------*/

int ascjson::Asc_IntegSetObsFileCmd(Asc_DString *hptr, int argc, CONST84 char *argv[])
{
  size_t len;

  if ( argc != 2 ) {
    FPRINTF(ASCERR, "integrate_set_obs_file: called without filename.\n");
    Asc_DStringSet(hptr,
                  "integrate_set_obs_file <filename,""> called without arg.");
    return HELP_ERROR;
  }
  if (l_obs_filename != NULL) {
    ascfree(l_obs_filename);
  }
  len = strlen(argv[1]);
  if (len >0 ) {
    l_obs_filename = Asc_MakeInitString((int)len);
    sprintf(l_obs_filename,"%s",argv[1]);
  } else {
    l_obs_filename = NULL;
  }
  return HELP_OK;
}

/*---------------------------------------------*/

int ascjson::Asc_IntegSetFileUnitsCmd(Asc_DString *hptr, int argc, CONST84 char *argv[])
{
  if ( argc != 2 ) {
    FPRINTF(ASCERR, "integrate_logunits: called without printoption.\n");
    Asc_DStringSet(hptr,"integrate_logunits <display,si> called without arg.");
    return HELP_ERROR;
  }
  switch (argv[1][0]) {
  case 's':
    l_print_option = 1;
    break;
  case 'd':
    l_print_option = 0;
    break;
  default:
    FPRINTF(ASCERR,"integrate_logunits: called with bogus argument.\n");
    FPRINTF(ASCERR,"logunits remain set to %s.\n",
      (l_print_option ? "si":"display"));
    break;
  }
  return HELP_OK;
}

/*---------------------------------------------*/

int ascjson::Asc_IntegSetFileFormatCmd(Asc_DString *hptr, int argc, CONST84 char *argv[])
{
  if ( argc != 2 ) {
    FPRINTF(ASCERR, "integrate_logformat called without printoption.\n");
    Asc_DStringSet(hptr,
                  "integrate_logformat <fixed,variable> called without arg.");
    return HELP_ERROR;
  }
  switch (argv[1][0]) {
  case 'f':
    l_print_fmt = 1;
    break;
  case 'v':
    l_print_fmt = 0;
    break;
  default:
    FPRINTF(ASCERR,"integrate_logformat: called with bogus argument.\n");
    FPRINTF(ASCERR,"logformat remains set to %s.\n",
      (l_print_fmt ? "fixed":"variable"));
    break;
  }
  return HELP_OK;
}


/*---------------------------------------------------------------
  DEFINING THE TIMESTEPS FOR INTEGRATION
*/

int ascjson::Asc_IntegGetXSamplesCmd(Asc_DString *hptr, int argc, CONST84 char *argv[])
{
  static char sval[40]; /* buffer long enough to hold a double printed */
  struct Units *du = NULL;
  const dim_type *dp;
  long i,len;
  double *uvalues = NULL;
  char *ustring = NULL;
  double *uv;
  int trydu=0, prec, stat=0;

  if (( argc < 1 ) || ( argc > 2 )) {
    Asc_DStringSet(hptr,
                  "integrate_get_samples: expected 0 or 1 args [display]");
    return HELP_ERROR;
  }
  if (argc==2) {
    trydu = 1;
    if( argv[1][0] != 'd') {
      Asc_DStringSet(hptr, "integrate_get_samples: expected display but got ");
      Asc_DStringAppend2(hptr, argv[1],".",HALL);
      return HELP_ERROR;
    }
  }

  len = samplelist_length(&l_samplelist);
  dp = samplelist_dim(&l_samplelist);

  if (len <1) {
    Asc_DStringSet(hptr, "{} {}");
    return HELP_OK;
  }

  if (trydu) {

    /* Allocate the space for the retrieved values... */
    uvalues = ASC_NEW_ARRAY(double,len);
    if (uvalues == NULL) {
      Asc_DStringSet(hptr, "integrate_get_samples: Insufficient memory.");
      return HELP_ERROR;
    }

	/* Get the display units at string... */
    ustring = Asc_UnitDimStringDS(dp,0);

    /* Get the conversion factor... */
    du = (struct Units *)LookupUnits(ustring);
    if (du == NULL) {
      ERROR_REPORTER_HERE(ASC_PROG_ERR,"LookupUnits failed :-/");
      stat = 1;
    }else{
      stat = 0;
      /* fill 'uvalues' with scaled values (in output units) */
      uv = uvalues;
      for (i=0; i < len; i++) {
      	/* convert to output units */
        stat = Asc_UnitConvertDS(du,samplelist_get(&l_samplelist,i),uv,1);
        if (stat) {
          /* any problems, just stop */
          break;
        }
        uv++;
      }
    }
    if (stat) {
      /* there was a problem, so free the allocated space */
      ascfree(uvalues);
    }
  }

  /* give Tcl the units string */
  Asc_DStringAppend3(hptr,"{",ustring,"} {",HALL);

  /* work out what precision we want */
  prec = Asc_UnitGetCPrecDS();

  len--; /* last one is a special case...? */
  if (!trydu || stat){
  	/* no unit conversion, or failed unit conversion: use the raw values */
  	for(i=0;i<len;i++){
		sprintf(sval,"%.*g ",prec,samplelist_get(&l_samplelist,i));
		Asc_DStringAppend(hptr,sval,HALL);
  	}
  }else{
  	for(i=0;i<len;i++){
  		sprintf(sval,"%.*g ",prec,uvalues[i]);
    	Asc_DStringAppend(hptr,sval,HALL);
  	}
  	ascfree(uvalues);
  }
  for (i=0; i<len; i++) {
    /* print number and a blank */
    sprintf(sval,"%.*g ",prec,uvalues[i]);
    Asc_DStringAppend(hptr,sval,HALL);
  }

  sprintf(sval,"%.*g",prec,uvalues[len]);
  Asc_DStringAppend2(hptr,sval,"}",HALL);

  if (trydu && !stat) {
    ascfree(uvalues);
  }
  return HELP_OK;
}

int ascjson::Asc_IntegSetXSamplesCmd(Asc_DString *hptr, int argc, CONST84 char *argv[])
{
  struct Units *du = NULL;
  const dim_type *dp;
  dim_type *mydp;
  long i,len;
  double *uvalues = NULL;
  double *uv;
  int stat;

  if (argc == 1) {
    samplelist_assign(&l_samplelist,0L,NULL,NULL);
    return HELP_OK;
  }

  if (argc <4) {
    Asc_DStringSet(hptr,
                  "Syntax: integrate_set_samples"
                  " <units> <value [value...] value>");
    FPRINTF(ASCERR,"ERROR: integrate_set_samples needs at least 3 args.");
    return HELP_ERROR;
  }

  du = (struct Units *)LookupUnits(argv[1]);
  if (du == NULL) {
    Asc_DStringSet(hptr, "integrate_set_samples: first arg not valid units.");
    return HELP_ERROR;
  }
  dp = (const dim_type *)UnitsDimensions(du);
#if INTDEBUG
  FPRINTF(ASCERR,"user dimen looks like\n");
  PrintDimen(ASCERR,dp);
#endif
  mydp = ASC_NEW(dim_type); /* we are giving away */
  if (mydp == NULL) {
    Asc_DStringSet(hptr, "integrate_set_samples: Insufficient memory");
    return HELP_ERROR;
  }
  CopyDimensions(dp,mydp);
#if INTDEBUG
  FPRINTF(ASCERR,"copy of user dimen looks like\n");
  PrintDimen(ASCERR,mydp);
#endif

  len = argc -2;
  uvalues = ASC_NEW_ARRAY(double,len); /* we are giving away */
  if (uvalues==NULL) {
    Asc_DStringSet(hptr, "integrate_set_samples: Insufficient memory");
    ascfree(mydp);
    return HELP_ERROR;
  }
  stat = 0;
  uv = uvalues;
  for (i=0; i<len; i++) {
    if(Tcl_GetDouble(hptr, argv[i+2],uv)!=HELP_OK) {
      stat = 1;
      break;
    }
    stat = Asc_UnitConvertDS(du,*uv,uv,0);
    if (stat) {
      break;
    }
    uv++;
  }
  Asc_DStringFree(hptr);
  if (stat) {
    Asc_DStringSet(hptr, "integrate_set_samples: Invalid value given. (");
    Asc_DStringAppend2(hptr, argv[i+2],")",HALL);
    ascfree(uvalues);
    ascfree(mydp);
    return HELP_ERROR;
  }
  if(!samplelist_assign(&l_samplelist,len,uvalues,mydp)){
    Asc_DStringSet(hptr, "integrate_set_samples: Insufficient memory.");
    ascfree(uvalues);
    ascfree(mydp);
    return HELP_ERROR;
  }
  return HELP_OK;
}

/*----------------------------------------------------------------
  FUNCTIONS THAT QUERY THE INTEGRATOR/SOLVER
*/

/**
	Tcl/Tk interface function: is the specified instance integrable.

	There is a problem with this part of the interface. The
	'integrator_isintegrable' function is being called before the
	'integrator_analyse' step, which means that it really doesn't have all the
	information that it needs to be able to say. I've reworked it so that
	you would expect to get errors back from the 'integrator_solve' step if
	there is any problem.

	All this function now does is to check that the instance is OK, and to
	check that a valid integrator engine is specified.
*/
int ascjson::Asc_IntegInstIntegrableCmd(Asc_DString *hptr, int argc, CONST84 char *argv[])
{
  struct Instance *i=NULL;
  IntegratorEngine integrator=INTEG_UNKNOWN;
  int result=0;         /* 0 = FALSE; 1 = TRUE */

  if ( argc != 3 ) {
    Asc_DStringSet(hptr, "integrate_able <solver,current,search> <lsode>");
    return HELP_ERROR;
  }

  if (strncmp(argv[1],"solver",3)==0) {
    i = g_solvinst_cur;
  } else {
    if (strncmp(argv[1],"search",3)==0) {
      i = g_search_inst;
    } else {
      if (strncmp(argv[1],"current",3)==0) {
        i = g_curinst;
      } else {
        Asc_DStringSet(hptr,
                      "integrate_able: arg 1 is current, search, or solver");
        return HELP_ERROR;
      }
    }
  }

  if (!i) {
    Asc_DStringSet(hptr, "0");
    FPRINTF(ASCERR,"NULL instance sent to integrate_able.\n");
    return HELP_OK;
  }

  integrator = INTEG_UNKNOWN;

  if (strncmp(argv[2],"blsode",3)==0) {
    integrator = INTEG_LSODE;
#ifdef ASC_WITH_IDA
  }else if (strncmp(argv[2],"ida",3)==0) {
    integrator = INTEG_IDA;
#endif
  }

  result = (integrator != INTEG_UNKNOWN);
  if (result) {
    Asc_DStringSet(hptr, "1");
  } else {
    Asc_DStringSet(hptr, "0");
  }
  return HELP_OK;
}

/*-----------------------------------*/

/**
	Set up the Integrator.

	switches (in Tcl/Tk)
		-engine $name
		-i0 $stepindex
		-i1 $stepindex
		-dt0 $initstepsize
		-dtmin $minstep
		-dtmax $maxstep
		-moststeps $moststeps
*/
int ascjson::Asc_IntegSetupCmd(Asc_DString *hptr, int argc, CONST84 char *argv[])
{
  char buf[MAXIMUM_NUMERIC_LENGTH];         /* string to hold integer */
  CONST84 char *engine = NULL;
  int result = 0;         /* 0 = FALSE; 1 = TRUE */
  long i0=(-1), i1=(-1);
  int k;
  int moststeps=0;
  double dt0=0, dtmin=0, dtmax=0;
  CONST84 char *cdt0=NULL, *cdtmin=NULL, *cdtmax=NULL, *cmoststeps=NULL,
       *ci0=NULL, *ci1=NULL;
  IntegratorReporter *reporter;
  IntegratorSystem *blsys;

  k = 1;
  while (k < (argc-1)) { /* arguments come in pairs */
    if (strcmp(argv[k],"-engine")==0) {
      engine = argv[k+1];
      k+=2;
      continue;
    }
    if (strcmp(argv[k],"-i1")==0) {
      ci1 = argv[k+1];
      k+=2;
      continue;
    }
    if (strcmp(argv[k],"-i0")==0) {
      ci0 = argv[k+1];
      k+=2;
      continue;
    }
    if (strcmp(argv[k],"-moststeps")==0) {
      cmoststeps = argv[k+1];
      k+=2;
      continue;
    }
    if (strcmp(argv[k],"-dtmax")==0) {
      cdtmax = argv[k+1];
      k+=2;
      continue;
    }
    if (strcmp(argv[k],"-dtmin")==0) {
      cdtmin = argv[k+1];
      k+=2;
      continue;
    }
    if (strcmp(argv[k],"-dt0")==0) {
      cdt0 = argv[k+1];
      k+=2;
      continue;
    }
    Asc_DStringAppend4(hptr, argv[0],": unrecognized option ",
                     argv[k],".",HALL);
    return HELP_ERROR;
  }

  if (ci0 != NULL && ci1 != NULL) {
    /* get i0, i1 if both supplied. */
    long i;
    if (JTcl_GetLong(hptr,ci0,&i)==HELP_ERROR|| i<0) {
      Asc_DStringFree(hptr);
      Asc_DStringSet(hptr, "integrate_setup: index i0 invalid");
      return HELP_ERROR;
    }
    i0=i;
    if (JTcl_GetLong(hptr,ci1,&i)==HELP_ERROR|| i<i0) {
      Asc_DStringFree(hptr);
      Asc_DStringSet(hptr, "integrate_setup: index i1 invalid");
      return HELP_ERROR;
    }
    i1=i;
  }
  if (cdt0 != NULL) {
    if (Tcl_GetDouble(hptr,cdt0,&dt0) != HELP_OK) {
      Asc_DStringFree(hptr);
      Asc_DStringAppend3(hptr, "integrate_setup: initial step length invalid"
                       " (",cdt0,")", HALL);
      return HELP_ERROR;
    }
  }
  if (cdtmin != NULL) {
    if (Tcl_GetDouble(hptr,cdtmin,&dtmin) != HELP_OK || dtmin < 0) {
      Asc_DStringFree(hptr);
      Asc_DStringAppend3(hptr, "integrate_setup: minimum step length invalid"
                       " (",cdtmin,")", HALL);
      return HELP_ERROR;
    }
  }
  if (cdtmax != NULL) {
    if (Tcl_GetDouble(hptr,cdtmax,&dtmax) != HELP_OK || dtmax < dtmin) {
      Asc_DStringFree(hptr);
      Asc_DStringAppend3(hptr, "integrate_setup: maximum step length invalid"
                       " (",cdtmax,")", HALL);
      return HELP_ERROR;
    }
  }
  if (cmoststeps != NULL) {
    if (JTcl_GetInt(hptr,cmoststeps,&moststeps) != HELP_OK || moststeps < 0) {
      Asc_DStringFree(hptr);
      Asc_DStringAppend3(hptr, "integrate_setup: maximum internal steps bad"
                       " (",cmoststeps,")", HALL);
      return HELP_ERROR;
    }
  }

  reporter = Asc_GetIntegReporter();

  blsys = integrator_new(g_solvsys_cur,g_solvinst_cur);
  result = integrator_set_engine(blsys, engine);

  if(result) {
	integrator_free(blsys);
    Asc_DStringSet(hptr, "Unsupported integrator");
    Asc_DStringAppend2(hptr," ",engine,HALL);
    return HELP_ERROR;
  }

  integrator_set_reporter(blsys, reporter);
  integrator_set_samples(blsys,&l_samplelist);
  integrator_set_stepzero(blsys,dt0);
  integrator_set_minstep(blsys,dtmin);
  integrator_set_maxstep(blsys,dtmax);
  integrator_set_maxsubsteps(blsys,moststeps);

  result = integrator_analyse(blsys);
  if(result){
     integrator_free(blsys);
     Asc_DStringSet(hptr, "integrate_analyse: error returned");
     return HELP_ERROR;
  }

  /* go and solve it */
  integrator_solve(blsys, i0, i1);

  /* once solution is finished, free whatever we allocated */
  integrator_free(blsys);

  sprintf(buf, "%d", result);
  Asc_DStringSet(hptr, buf);
  return HELP_OK;
}

/********************************************************************/

int ascjson::Asc_IntegCleanupCmd(Asc_DString *hptr, int argc, CONST84 char *argv[])
{

  if (argc!=1) {
    Asc_DStringSet(hptr, "integrate_cleanup takes no arguments");
    return HELP_ERROR;
  }

  /* integrator_cleanup(); */
  return HELP_OK;
}

/*-------------------------------------------------------------------
  REPORTER FUNCTIONS
*/

FILE *integ_y_out;
FILE *integ_obs_out;

static int airInit(IntegratorSystem *blsys)
{
	ascjson *aj = (ascjson *)blsys->clientdata;
	return aj->Asc_IntegReporterInit(blsys);
}
static int airWrite(IntegratorSystem *blsys)
{
	ascjson *aj = (ascjson *)blsys->clientdata;
	return aj->Asc_IntegReporterWrite(blsys);
}
static int airWriteObs(IntegratorSystem *blsys) 
{
	ascjson *aj = (ascjson *)blsys->clientdata;
	return aj->Asc_IntegReporterWriteObs(blsys);
}
static int airClose(IntegratorSystem *blsys)
{
	ascjson *aj = (ascjson *)blsys->clientdata;
	return aj->Asc_IntegReporterClose(blsys);
}

IntegratorReporter *ascjson::Asc_GetIntegReporter(){
	IntegratorReporter *r;
	r = (IntegratorReporter *)ascmalloc(sizeof(IntegratorReporter));
	r->init = airInit;
	r->write = airWrite;
	r->write_obs = airWriteObs;
	r->close = airClose;
	CONSOLE_DEBUG("CREATED INTEGRATORREPORTER FOR ascjson INTERFACE");
	return r;
}

int ascjson::Asc_IntegReporterInit(IntegratorSystem *blsys){
	int status = 1;

	CONSOLE_DEBUG("INITIALISING REPORTER");

	/* set up output files */
	integ_y_out = Asc_IntegOpenYFile();
	integ_obs_out = Asc_IntegOpenObsFile();

	CONSOLE_DEBUG("RELEASING FILES");

	Asc_IntegReleaseYFile();
	Asc_IntegReleaseObsFile();

	CONSOLE_DEBUG("WRITING HEADERS");

	/* write headers to yout, obsout and initial points */

	Asc_IntegPrintYHeader(integ_y_out,blsys);
	status &= Asc_IntegPrintYLine(integ_y_out,blsys);
	Asc_IntegPrintObsHeader(integ_obs_out,blsys);
	status &= Asc_IntegPrintObsLine(integ_obs_out,blsys);

	return status;
	return 0;
}

int ascjson::Asc_IntegReporterWrite(IntegratorSystem *blsys){
	/* write out a line of stuff */
    return Asc_IntegPrintYLine(integ_y_out,blsys);
}

int ascjson::Asc_IntegReporterWriteObs(IntegratorSystem *blsys){
	return Asc_IntegPrintObsLine(integ_obs_out,blsys);
	return 0;
}

int ascjson::Asc_IntegReporterClose(IntegratorSystem *blsys){
	/* close the file streams */
	if (integ_y_out!=NULL) {
		fclose(integ_y_out);
	}

	if (integ_obs_out!=NULL) {
		fclose(integ_obs_out);
	}
	return 1;
}

