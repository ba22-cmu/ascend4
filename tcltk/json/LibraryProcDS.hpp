#ifndef ASC_LIBRARYPROCDS_H
#define ASC_LIBRARYPROCDS_H
/*
 *  LibraryProcDS.h
 * fixme
 */

/** @file
 *  Ascend Library Procedures.
 *  <pre>
 *  To include this header, you must include the following:
 *      #include <stdio.h>
 *      #include "tcl.h"
 *      #include "utilities/ascConfig.h"
 *      #include "interface/LibraryProc.h"
 *  </pre>
 */


int Asc_FileIDCopy(FILE *filein, FILE *fileout);
/**<
 *  Copies the input from a file with specified file id to the other.
 *  It is the responsibility of the user to ensure that the files are
 *  open and writable.
 */

// STDHLF_H(Asc_LibrParseCmd);
int Asc_LibrParseCmdDS( Asc_DString *hptr, int argc, char**argv);

// STDHLF_H(Asc_LibrReadCmd);
int Asc_LibrReadCmdDS( Asc_DString*, int, char**);

// STDHLF_H(Asc_LibrOptionsCmd);
int Asc_LibrOptionsCmdDS( Asc_DString*, int, char**);

// STDHLF_H(Asc_LibrTypeListCmd);
int Asc_LibrTypeListCmdDS( Asc_DString*, int, char**);

// STDHLF_H(Asc_LibrModuleInfoCmd);
int Asc_LibrModuleInfoCmdDS( Asc_DString*, int, char**);

// STDHLF_H(Asc_LibrDestroyTypesCmd);
int Asc_LibrDestroyTypesCmdDS( Asc_DString*, int, char**);

int Asc_GNUTextCmdDS( Asc_DString *hptr, int argc, char**argv);

// STDHLF_H(Asc_LibrHideTypeCmd);
int Asc_LibrHideTypeCmdDS( Asc_DString*, int, char**);

// STDHLF_H(Asc_LibrUnHideTypeCmd);
int Asc_LibrUnHideTypeCmdDS( Asc_DString*, int, char**);

// STDHLF_H(Asc_LibrTypeIsShownCmd);
int Asc_LibrTypeIsShownCmdDS( Asc_DString*, int, char**);

// STDHLF_H(Asc_LibrQueryTypeCmd);
int Asc_LibrQueryTypeCmdDS( Asc_DString*, int, char**);

#endif  /* ASC_LIBRARYPROCDS_H */

