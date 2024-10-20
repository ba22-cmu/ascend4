// This file will be processed by SCons and output as config.h.
// Don't edit this file unless it's named 'config.h.in' unless
// it doesn't bother you that your changes will be IGNORED!

#ifndef ASCXX_CONFIG_H
#define ASCXX_CONFIG_H

#define DEFAULT_ASCENDLIBRARY "/usr/local/lib/ascend/models"

#define ASC_DLLDIRS "@DLLDIRS@"

#if 1
# define ASCXX_USE_PYTHON
#endif

#ifndef ASC_WITH_GRAPHVIZ
/* #define ASC_WITH_GRAPHVIZ @ASC_WITH_GRAPHVIZ@ */
#endif

#define ASC_BUILDING_INTERFACE

#endif
