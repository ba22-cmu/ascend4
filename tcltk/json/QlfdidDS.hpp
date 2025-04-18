#ifndef QLFDID_HPP
#define QLFDID_HPP

int Asc_BrowQlfdidSearchCmdDS(Asc_DString *hptr, int argc, CONST84 char *argv[]);
/**<
 *  <!--  Asc_BrowQlfdidSearchCmd                                      -->
 *  Will take a fully qualified ascend name and will search for the instance
 *  with that name. Will leave g_search_inst looking at the named instance
 *  if successful and will return a parsed string. Otherwise should return
 *  a TCL_ERROR with the message "Orphaned part", where part is the portion
 *  that could not be found.<br><br>
 *
 *  Registered as : \"qlfdid name\"
 */

#endif  /* QLFDID_HPP */

