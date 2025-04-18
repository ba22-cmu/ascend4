STDHLF_H(Asc_ExtractTypeHC);

/*
 *  int Asc_ExtractTypeDS(hds, argc, argv)
 *      Asc_DString *hds,
 *      int argc;            --the number of arguments
 *      char **argv;         --the array of arguments
 *
 *  When running as part of ASCEND, returns HELP_OK or HELP_ERROR.
 */
int Asc_ExtractTypeDS(Asc_DString *hds, int argc, char **argv);
