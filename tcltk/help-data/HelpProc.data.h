#ifndef HelpProc_data_h
#define HelpProc_data_h

#define STR(x) #x
#define TOSTR(x) STR(x)

#define Asc_HelpCmdHC help 
#define Asc_HelpCmdHN TOSTR(Asc_HelpCmdHC)
/** Usage */
#define Asc_HelpCmdHU \
  "help [commandname] OR help help"

#define Asc_HelpCmdHS \
  "returns information on C functions ASCEND defines as Tcl commands"

#define Asc_HelpCmdHL1 \
"\
 * Returns help on a registered command name, or command group name,\n\
 * or the list of commands alphabetized or the list groups alphabetized,\n\
 * or the list of commands alphabetized by groups.\n\
 * For help on specific commands you can also try: <commandname> -h\n\
 * Most of our callbacks supply their own help.\n\
"

#define Asc_HelpCmdHL2 \
"\
 *\n\
 * help examples:\n\
 * help -h         returns a short definition & UNIX man page style syntax.\n\
 * help -H         returns a long explanation\n\
 * help help       returns this text.\n\
"

#define Asc_HelpCmdHL3 \
"\
 * help system     returns the description of the system group.\n\
 * help groups     returns the alphabetized list of group names.\n\
 * help all        returns the alphabetized list of commands.\n\
 * help commands   returns the alphabetized listing of groups.\n\
 *\n\
"

#define Asc_HelpCmdHL4 \
"\
 * All output is to the tcl interpreter in the form of list elements.\n\
 * For better formatted help output, use Help which has the same\n\
 * syntax as this command but writes the output more neatly to stdout.\n\
"

#endif // HelpProc_data_h
