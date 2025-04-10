#ifndef BrowserMethod_json_h
#define BrowserMethod_json_h


#define Asc_BrowInitializeCmdHN "brow_runmethod"
#define Asc_BrowInitializeCmdHC  brow_runmethod 
/**  Usage */
#define Asc_BrowInitializeCmdHU \
  Asc_BrowInitializeCmdHN "-method name -qlfdid instance_name options"

#define Asc_BrowInitializeCmdHS \
  "Runs the method named in the instance named"

#define Asc_BrowInitializeCmdHL "\
 * Runs a method with control of where output goes, what kind of error \n\
 * handling, and so forth according to the options:\n\
 *   -stopOnErr 1		causes error to be reported immediately.\n\
 *   -stopOnErr 0		ignores errors (message, but continue).\n\
 *   -backtrace 0		on stopping, offer only local message.\n\
 *   -backtrace 1		on stopping, print method stack unwind.\n\
 *   -output file		send output to file. If not set, to ASCERR.\n\
"

#endif // BrowserMethod_json_h
