/*
 *  ScriptProc.c
 *  by Kirk Abbott and Ben Allan
 *  Created: 1/94
 *  Version: $Revision: 1.22 $
 *  Version control file: $RCSfile: ScriptProc.c,v $
 *  Date last modified: $Date: 2003/08/23 18:43:07 $
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

#define SCRBUF_SIZE 1024

/* this variable is linked to a tcl variable ascScripVect(menubreak). */
int ascjson::Asc_ScriptInterrupt = 0;

int ascjson::Asc_ScriptConfigureInterrupt(int start, Asc_DString *hptr)
{
  static char *varName;
  /* assumes ansi NULL initialization of varName */
  if (start) {
    if (varName == NULL) {
      varName = ASC_NEW_ARRAY(char,60);
      if (varName == NULL) {
        return 1;
      }
      sprintf(varName,"%s","set ascScripVect(menubreak) 0");
      Asc_ScriptInterrupt = 0;
#if 0 // fixme js call
      result = Tcl_GlobalEval(hptr,varName);
      if (result != HELP_OK) {
        return 2;
      }
#endif
/*
      Tcl_SetVar(hptr,varName,"0",TCL_GLOBAL_ONLY);
 */
      sprintf(varName,"%s","ascScripVect(menubreak)");
#if 0 // fixme js call
      Tcl_LinkVar(hptr,varName,
                  (char *)&Asc_ScriptInterrupt,TCL_LINK_INT);
#endif
    }  /* else double call, ignore it */
    return 0;
  } else {
    if (varName!=NULL) {
      sprintf(varName,"%s","ascScripVect(menubreak)");
#if 0 // fixme js call
      Tcl_UnlinkVar(hptr, varName);
#endif
      ascfree(varName);
      varName = NULL;
      return 0;
    } else {
      return 1;
    }
  }
}

/*
 * Refine a qlfdid, if found, to the type specified, if possible.
 */
int ascjson::Asc_ScriptRefineCmd(Asc_DString *hptr, int argc, CONST84 char *argv[])
{
  struct Instance *i;
  int status;

  if (argc!=4) {
    Asc_DStringSet(hptr,"wrong # args : Usage srefine <type> search <qlfdid>");
    return HELP_ERROR;
  }
  status = Asc_QlfdidSearch3(argv[3],0);
  if (status==0) {
    i = g_search_inst;
    if (!i) {
      Asc_DStringSet(hptr, "srefine: NULL instance found in qlfdid search.");
      return HELP_ERROR;
    }
    status = Asc_BrowInstanceRefineCmd(hptr,(int)3,argv);
  } else {
    Asc_DStringAppend3(hptr,"srefine: QlfdidSearch error,",
                     argv[3]," not found.",HALL);
  }
  return status;
}


int ascjson::Asc_ScriptMergeCmd(Asc_DString *hptr, int argc, CONST84 char *argv[])
{
  struct Instance *i, *result=NULL;
  int status;

  if (argc!=3) {
    Asc_DStringSet(hptr, "wrong # args : Usage smerge <qlfdid> <qlfdid>");
    return HELP_ERROR;
  }

  /* Process first qlfdid */
  status = Asc_QlfdidSearch3(argv[1],0);
  if (status==0) {
    i = g_search_inst;
    if (!i) {
      Asc_DStringSet(hptr, "smerge: NULL instance found in qlfdid1 search.");
      return HELP_ERROR;
    }
  } else {
    Asc_DStringAppend3(hptr, "smerge: QlfdidSearch: ",argv[1], " not found,",HALL);
    return HELP_ERROR;
  }

  /* Process second qlfdid */
  status = Asc_QlfdidSearch3(argv[2],0);
  if (status!=0) {
    Asc_DStringAppend3(hptr, "smerge: QlfdidSearch: ",argv[2],
                     " not found,",HALL);
    return HELP_ERROR;
  }
  if (!g_search_inst) {
    Asc_DStringSet(hptr, "smerge: NULL instance found in qlfdid2 search.");
    return HELP_ERROR;
  }

  switch(InstanceKind(i)) {
  case REAL_INST: case BOOLEAN_INST:
  case INTEGER_INST: case SYMBOL_INST:
  case SET_INST: case REL_INST:
    Asc_DStringAppend2(hptr,"AscendIV does not allow merging "
                     "of children of Atoms: ",argv[1],HALL);
    return HELP_ERROR;
  default:
    break;
  }
  switch(InstanceKind(g_search_inst)) {
  case REAL_INST: case BOOLEAN_INST:
  case INTEGER_INST: case SYMBOL_INST:
  case SET_INST: case REL_INST:
    Asc_DStringAppend2(hptr,"AscendIV does not allow merging "
                     "of children of Atoms: ", argv[2], HALL);
    return HELP_ERROR;
  default:
    break;
  }

  /* Do the merge */
  result = MergeInstances(i,g_search_inst);
  PostMergeCheck(result);
  if (!result) {
    Asc_DStringSet(hptr, "Error in merging instances.");
    return HELP_ERROR;
  }
  return HELP_OK;
}

#if 0
int Asc_FastRaiseCmd(Asc_DString *hptr,
                 int argc, CONST84 char *argv[])
{
  Tk_Window tkwin,mainwin;
  Window window;
  Display *display;

  if (argc!=2) {
    Asc_DStringSet(hptr, "wrong # args to asc_raise");
    return HELP_ERROR;
  }
  mainwin = Tk_MainWindow(hptr);
  tkwin = Tk_NameToWindow(hptr, argv[1],mainwin);
  if (!tkwin) {
    return HELP_ERROR; /* a message should be in Tcl_GetStringResult(hptr)  */
  }
  display = Tk_Display(tkwin);
  window = Tk_WindowId(tkwin);
  XRaiseWindow(display,window);
  return HELP_OK;
}
#endif

int ascjson::Asc_ScriptEvalCmd(Asc_DString *hptr, int argc, CONST84 char *argv[])
{
  int result;

  if (argc!=2) {
    Asc_DStringSet(hptr, "Error in running ScriptEval");
    return HELP_ERROR;
  }
  if (Asc_ScriptInterrupt==1) {
    Asc_ScriptInterrupt = 0;
    Asc_DStringSet(hptr, "Solver or Script interrupted by user");
    return HELP_ERROR;
  }
#if 0 // fixme: call js
  result = Tcl_GlobalEval(hptr, argv[1]);
#else
  Asc_DStringSet(hptr, "javascript evals not enabled yet");
  result = HELP_ERROR;
#endif
  return result;
}

STDHLF(Asc_TimeCmd,(Asc_TimeCmdHL1,Asc_TimeCmdHL2,HLFSTOP));

int ascjson::Asc_TimeCmd(Asc_DString *hptr, int argc, CONST84 char *argv[])
{
#if 1 // fixme eval js
  Asc_DStringSet(hptr, "javascript evals not enabled yet");
  return HELP_ERROR;
#else

  clock_t max_clocks=0,min_clocks=0,avg_clocks=0,start=0,stop=0,dc=0,all_start=0,all_stop=0;
  double time_avg, real_time_avg;
  time_t time0, time1;
  int i,n=1, status=HELP_OK;
  char tmps[40];

  if (argc<2||argc>3) {
    Asc_DStringSet(hptr, "call is: asc_clock {javascript script} iterations");
    return HELP_ERROR;
  }
  if (argc==3) {
    status=JTcl_GetInt(hptr, argv[2],&n);
    if (n<1 || status != HELP_OK) {
      Asc_DStringSet(hptr, "asc_clock: called with bad number of iterations.");
      return status;
    }
  }
  time(&time0);
  all_start=clock();
  for (i=0; i<n; i++) {
    if (status != HELP_OK) {
      return status;
    }
    start=clock();
#if 0 // fixme: call out to javascript.
    status=Tcl_GlobalEval(hptr, argv[1]);
#endif
    stop=clock();
    dc=stop-start;
    if (i==0) {
      min_clocks=max_clocks=dc;
    }
    if (dc> max_clocks) {
      max_clocks=dc;
    }
    if (dc< min_clocks) {
      min_clocks=dc;
    }
  }
  all_stop=clock();
  time(&time1);

  dc=all_stop-all_start;
  avg_clocks=dc/n;
  time_avg = ((double)dc) / ((double)CLOCKS_PER_SEC) / ((double)n);
#ifdef ASCDIFFTIME
  real_time_avg=(time1-time0)/((double)n);
#else
  real_time_avg=difftime(time1,time0)/n;
#endif

  sprintf(tmps,"%.8g",real_time_avg);
  VTcl_AppendElement(hptr,tmps);

  sprintf(tmps,"%.8g",time_avg);
  VTcl_AppendElement(hptr,tmps);

  sprintf(tmps,"%ld",(long)avg_clocks);
  VTcl_AppendElement(hptr,tmps);

  sprintf(tmps,"%ld",(long)max_clocks);
  VTcl_AppendElement(hptr,tmps);

  sprintf(tmps,"%ld",(long)min_clocks);
  VTcl_AppendElement(hptr,tmps);

  sprintf(tmps,"%ld",(long)CLOCKS_PER_SEC);
  VTcl_AppendElement(hptr,tmps);

  VAEstrip(hptr);
  return HELP_OK;
#endif
}

#if 0
/*
 *  String Compact: eat extra space in a string and detabify
 */
int Asc_StringCompact(Asc_DString *hptr,
                     int argc, CONST84 char *argv[])
{
  size_t len,i ,c;
  int bracenest=0,wcnt;
  char *result=NULL;

  if (argc != 2) {
    Asc_DStringSet(hptr, "wrong args: stringcompact string");
    return HELP_ERROR;
  }
  len = strlen(argv[1]);
  if (!len) {
    Asc_DStringSet(hptr, "");
    return HELP_OK;
  }
  result= (char *)ascmalloc(sizeof(char)*(len+1));
  if (result == NULL) {
    Asc_DStringSet(hptr, "stringcompact: insufficient memory");
    return HELP_ERROR;
  }
  memset(result,'#',len+1); /* fill whole array with #, including last spot */
  wcnt=1; /* trim leading whitespace */
  for (i=c=0; i < len; i++) {
    switch (argv[1][i]) {
    case '\t':
      if(!wcnt) {
        result[c++] = ' ';
        if (!bracenest) {
          wcnt++;
        }
      }
      break;
    case ' ':
     if(!wcnt) {
        result[c++] = argv[1][i];
        if (!bracenest) {
          wcnt++;
        }
      }
      break;
    case '{':
      wcnt = 0;
      bracenest++;
      result[c++] = argv[1][i];
      break;
    case '}':
      wcnt = 0;
      bracenest--;
      result[c++] = argv[1][i];
      if (bracenest < 0) {
        bracenest = 0;
      }
      break;
    default:
      wcnt = 0;
      result[c++] = argv[1][i];
    }
  }
  if (!bracenest && result[c]==' ') {
    result[c-1] = '\0';
  } else {
    result[c] = '\0';
  }
  Asc_DStringAppend(hptr,result,(char *)NULL);
  ascfree(result);
  return HELP_OK;
}
#endif
