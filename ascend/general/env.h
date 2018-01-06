/*	ASCEND modelling environment
	Copyright (C) 2006 Carnegie Mellon University

	This program is free software; you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation; either version 2, or (at your option)
	any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program.  If not, see <http://www.gnu.org/licenses/>.
*//**
	@file
	Environment variable helper routines
*//*
	by John Pye, May 2006.
*/

#ifndef ASC_ENV_H
#define ASC_ENV_H

#include "platform.h"

/**	@addtogroup general_env General Environment Variables
	@{
*/

typedef char *(GetEnvFn)(const char *name);
/**<
	This is the type of env var function that you must send to 'env_subst'.
	It doesn't have to actually consult the environment; it could do all
	sorts of other stuff if you wanted.
*/

typedef int (PutEnvFn)(const char *inputstring);
/**<
	This the type of a putenv function that can be used to set and environment
	variable.
*/

ASC_DLLSPEC int env_import(const char *varname,GetEnvFn *getenvptr,PutEnvFn *putenvptr,int free_after_getenv);
/**<
	Attempts to read from a getenv function, and if the value is found, write it
	using a putenv function. You would use this to copy values from one
	environment to another.

	For the reason why we use this function, see explanations in 
	<ascend/utilities/ascEnvVar.h>.

	@param free_after_getenv should be 1 if values returned from *getenvptr
	require to be freed using ASC_FREE

	@return 0 on success, nonzero on error. -1 means that the value didn't exist
	in getenv, otherwise the errors are those returned by putenv.
*/

ASC_DLLSPEC int env_import_default(const char *varname,GetEnvFn *getenvptr,PutEnvFn *putenvptr,const char *defaultvalue,int free_after_getenv);
/**<
	Attempts to read from an environment variable from a getenv function; if value
	is found, saves it using the putenv function. If no value is found, use the
	defaultvalue with the putenv function.

	For the reason why we use this function, see explanations in 
	<ascend/utilities/ascEnvVar.h>.

	@return 0 on success, nonzero on error. Errors are those returned by putenv.
*/


ASC_DLLSPEC char *env_subst(const char *path,GetEnvFn *getenvptr,int free_after_getenv);
/**<
	Perform variable substitution on a string in shell-like way.
	This should replace any $VARNAME with the result of
	(*getenvptr)(VARNAME).

	At present there will be no allowance for lowercase env var names
	only uppercase and underscores. No escaping of dollar signs is
	allowed for yet, and not ${BRACKETING}TO_STOP adjacent characters
	from being swallowed up is allowed for, either. These can be added
	later, 'as an exercise'.
*/

/* @} */

#endif
