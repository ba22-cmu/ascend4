#ifndef typelex_json_h
#define typelex_json_h


#define Asc_ExtractTypeHN "libr_extract_type"
/** Usage */
#define Asc_ExtractTypeHU \
    Asc_ExtractTypeHN " [-c] <type> [source_file] [-s,destination_file]"

#define Asc_ExtractTypeHS \
    "Returns the ASCEND code that defines an ATOM or MODEL"

#define Asc_ExtractTypeHL1 "\
 *  Extracts the definition of the ATOM or MODEL <type> from the specified\n\
 *  source file (or stdin if not specified) and writes it to the specified\n\
 *  destination file (or stdout if not specified).  Comments are stripped\n\
"

#define Asc_ExtractTypeHL2 "\
 *  from the code unless the -c flag is given.  Returns TCL_OK if successful\n\
 *  If unsuccessful, returns TCL_ERROR and puts the error message into\n\
 *  interp->result. If -s is given instead of destination, output to interp.\n\
 *  source_file may be an interactive string module when in ASCEND.\n\
"

#endif // typelex_json_h
