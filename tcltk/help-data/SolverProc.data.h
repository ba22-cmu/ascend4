#ifndef SolverProc_json_h
#define SolverProc_json_h


#define DELETEME 0

#define Asc_SolvGetObjNumCmdHN "slv_get_obj_num"
#define Asc_SolvGetObjNumCmdHC  slv_get_obj_num 
/** Usage */
#define Asc_SolvGetObjNumCmdHU \
  Asc_SolvGetObjNumCmdHN " takes no arguments"

#define Asc_SolvGetObjNumCmdHS \
  "gets the solver list index for the current objective"

#define Asc_SolvGetObjNumCmdHL "\
 *  slv_get_obj_num gets the solver list index for the current \n\
 *  objective. If the objective is NULL, -1 is returned.\n\
"

#endif // SolverProc_json_h
