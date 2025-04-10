#ifndef argv_h
#define argv_h

extern "C" {

/// free the result of toArgv correctly.
void freeArgv(char **argv);

/// Split string vs into argc/argv on sep characters.
/// The input string is not overwritten.
/// \return 0 if ok, or errno. if errno, *argv is undefined
/// 
/// Call freeArgv on argv if the return value is 0.
///
/// int err = toArgv(vs, "\v", &argc, &argv);
int toArgv(const char *vs, const char *sep, int *argc, char ***argv) ;

}

#endif
