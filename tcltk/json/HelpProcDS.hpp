#ifndef ASC_HELPPROCDS_H
#define ASC_HELPPROCDS_H

/*
 *  HelpProcDS.h
 *  by Ben Allan
 *  Created: 4/2025
 *
 *  This file is part of the ASCEND general utilities
 *
 *  Copyright 2025, Benjamin Allan
 *
 *  The ASCEND general utilities are free software; you can redistribute
 *  it and/or modify it under the terms of the GNU General Public License as
 *  published by the Free Software Foundation; either version 2 of the
 *  License, or (at your option) any later version.
 *
 *  The ASCEND general utilities are distributed in hope that it will be
 *  useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/** @file
 *  Functions & macros for the Ascend general help system.
 *  <pre>
 *  To include this header, you must include the following:
 *      #include "utilities/ascConfig.h"
 *  </pre>
 */

#include "config.h"
#include "ascend/general/dstring.h"
#include "tcltk/help-data/HelpProc.data.h"
#include "tcltk/json/ascjson_util.hpp"

#ifdef _TCL
#define HELP_OK TCL_OK
#define HELP_ERROR TCL_ERROR 
#define HELP_BREAK TCL_BREAK
#else
#define CONST84
/* help_ returns should mirror tcl */
#define HELP_OK                  0
#define HELP_ERROR               1
#define HELP_BREAK               3
#endif

/**
 * The largest number of strings that will be used in building a long
 * help string.
 */
#define MAXHELPARGS 100
/** Token for end of help strings. */
#define HLFSTOP (char *)NULL

/**
 *  This macro should be inserted at the beginning of all existing
 *  and future ascend callbacks registered in TCL or JSON.
 *  It checks for the command option -help and returns the helpstring
 *  with the code HELP_OK if -help is found. If -help is not found,
 *  this does nothing.
 *  All ASCEND help DS callbacks should be written using the standard arguments
 *  Asc_DString *hptr, int argc, char *argv[].
 *  This macro insists on it.
 */
#define ASCUSE \
	Asc_DString hds; \
	Asc_DStringInit(&hds); \
	if (Asc_HelpCheckDS(&hds, argc, argv) != HELP_OK) { \
		setc(__func__, SVcstr, Asc_DStringValue(&hds), 0); \
		Asc_DStringFree(&hds); \
		return getc(__func__); \
       	} else \
		Asc_DStringFree(&hds)

#define OLD_ASCUSE \
	Asc_DString hds; \
	Asc_DStringInit(&hds); \
	if (Asc_HelpCheckDS(&hds, argc, argv) != HELP_OK) \
		return HELP_OK;


/**
 * Prototype for a function which returns a char *
 * containing the long form help for a command.
 * provide: char *myfunc(void) to HelpDefine
 * The returned char * should be created with ascmalloc, because we
 * will ascfree it eventually.
 */
typedef char *(*HLFunc)(void);

void Asc_HelpDestroy();

#endif  /* ASC_HELPPROCDS_H */

