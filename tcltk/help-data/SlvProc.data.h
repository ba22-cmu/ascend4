#ifndef SlvProc_json_h
#define SlvProc_json_h


#define SLVMONITORPREFIX "slvmon"

#define Asc_SolveMonitorCmdHN "slv_monitor"
#define Asc_SolveMonitorCmdHC  slv_monitor 
/* which should eventually be __something, when we clean up the slv_ mess */

#define Asc_SolveMonitorCmdHU \
  "slv_monitor takes no arguments yet"

#define Asc_SolveMonitorCmdHS \
 "Returns the name of a monitor command to watch var/rel changes"

#define Asc_SolveMonitorCmdHL1 "Eventually this should take an argument "
/**  Long help text part 2 */
#define Asc_SolveMonitorCmdHL2 "that is the id of a tcl solver token.\n"
/**  Long help text part 3 */
#define Asc_SolveMonitorCmdHL3 "\
 * This creates a monitor and returns its symbolic handle.\n\
 * Multiple monitors can exist and are manipulated by their\n\
 * symbolic handles.\n\
 * A monitor may be used on a series of unrelated slv_system_t.\n\
 * Currently, this function gets its slv_system_t from g_solvsys_cur,\n\
 * but it should be changed to take a slvsys interface id when\n\
 * the solver interface is changed to work by name.\n\
"

#define SolveMonitorHS \
 "Returns the list of changes in residuals, variables, or steps in variables"

#define SolveMonitorHU \
 "slvmonN destroy OR slvmonN slvsysname change <var,rel> OR\n\
slvmonN slvsysname geometry w h x y rmin rmax vmax OR \n\
slvmonN slvsysname plotdata <value,speed,residual>\n\
"

#define SolveMonitorHL1 \
"\
 * s change var         return scaled values that changed\n\
 * s change rel         return scaled residuals that changed\n\
 * Returns a tcl list whose elements are the changed values\n\
 * with master list indices {index value}\n\
 * values are scaled by nominals, or 1 if nominal is 0.\n\
 * Scaled values > vmax (or rmax) will be returned as having the\n\
 * value of the max, rather than their TRUE value.\n\
"

#define SolveMonitorHL2 \
"\n\
 * s geometry w h x y rmin rmax vmax\n\
 * sets conversion parameters for plotdata (w h x y)\n\
 * and limits for data values. This is called with a window\n\
 * size or range change that will require new drawing.\n\
 * x,y is the upper left corner of a coordinate space like a tk canvas.\n\
 * w,h is the width and height of the space in distinctly plottable points\n\
 * The 0 line of the area is y + h/2 for values. The values will be\n\
 * spread evenly over the width in order of increasing solver master index.\n\
"

#define SolveMonitorHL3 \
"\n\
 * s plotdata value     return plot info for scaled values that changed\n\
 * s plotdata speed     return plot info for scaled rates of value change\n\
 * s plotdata residual  return plot info for scaled residuals that changed\n\
 *\n\
"

#define SolveMonitorHL4 \
"\
 * Each option returns a list of {x y index} for changed values of the\n\
 * variables or relations. The x,y are coordinates at which a point\n\
 * should be plotted based on a transformation derived from whxy info\n\
 * last obtained by the geometry command of the monitor.\n\
"

#define SolveMonitorHL5 \
"\
 * The transformation may specify the same coordinate for more than\n\
 * one relation or variable.\n\
 * If this function raises a floating point exception, it will\n\
 * not return data, but an error.\n\
"

#define SolveMonitorHL6 \
"\
*	value:	The variable values v are scaled by nominal.\n\
*		They are then reduced to vmax if abs(v) > vmax.\n\
*		They are then scaled to the plotting region. The\n\
*		returned value is {plotx ploty mastervarindex} for\n\
*		all variables that changed since the last call or geometry.\n\
"

#define SolveMonitorHL7 \
"\
*	speed:	The variable values v are scaled by nominal.\n\
*		They are then reduced to vmax if abs(v) > vmax.\n\
* 		The delta in the scaled value since the last call\n\
*		is calculated. If the delta has changed, it is scaled\n\
*		to the plotting region and returned as\n\
*		{plotx ploty mastervarindex} for\n\
*		all variables that had a step size change.\n\
"

#define SolveMonitorHL8 \
"\
* residual:	The relation residuals, as scaled by nominals and reduced\n\
*		to rmax if necessary, which have changed since the last call\n\
*		are transformed to a dual log scale in the plot region.\n\
*		Residuals < rmin considered as rmin. Residuals < 0 plotted\n\
*		in the lower half of the plot region. Distance from the\n\
*		vertical center of the canvas indicates the residual magnitude"

#endif // SlvProc_json_h
