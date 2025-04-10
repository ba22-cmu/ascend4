#ifndef SimsProc_json_h
#define SimsProc_json_h


#define Asc_SimBinTokenSetOptionsHN "sim_BinTokenSetOptions"
/**  Usage */
#define Asc_SimBinTokenSetOptionsHU \
  Asc_SimBinTokenSetOptionsHN " <src obj lib build delete" \
  " maxrels verbose housekeep>"

#define Asc_SimBinTokenSetOptionsHS \
  "Defines the next set of file names and other misc to build/load with"

#define Asc_SimBinTokenSetOptionsHL "\
 *  Specifies full path names for source, object, and shared object files\n\
 *  to be used in the next compilation of binary token relations.\n\
 *  Names must be consistent and the name of the shared object must\n\
 *  be unique, e.g. where N is a number changing between instantiations:\n\
 *  /tmp/fooN.c /tmp/fooN.o /tmp/fooN.so <build command> /bin/rm 0 1 1000.\n\
 *  verbose is a code generation option 1 makes more readable code which\n\
 *  may be slower for cc to digest. housekeep if 1 causes intermediate\n\
 *  files to be aggressively deleted. maxrels is the most equations to\n\
 *  be allowed in one generated file. If more are needed, binary will not\n\
 *  be generated. If maxrels = 0, we ignore C generation completely.\n"

#define Asc_SimBinTokenSetOptionsHL2 "\
 *  Bugs: needs to have additional arguments for coping with F77, Java, etc.\n\
 *  Note: This function is called indirectly from sim_create, because\n\
 *  we need consistently updated version values and don't trust the tcl\n\
 *  programmer to remember that. Tinkering, if needed, should be done via\n\
 *  Sim_SetupBinTokenCC in LibraryProc.tcl\n\
"

#endif // SimsProc_json_h
