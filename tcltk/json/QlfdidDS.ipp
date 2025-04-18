/*
 *  Qlfdid.c
 *  by Kirk Abbott and Ben Allan
 *  Created: 1/94
 *  Version: $Revision: 1.22 $
 *  Version control file: $RCSfile: Qlfdid.c,v $
 *  Date last modified: $Date: 2003/08/23 18:43:07 $
 *  Last modified by: $Author: ballan $
 *
 *  This file is part of the ASCEND Tcl/Tk interface
 *
 *  Copyright 1997, Carnegie Mellon University
 *
 *  The ASCEND Tcl/Tk interface is free software; you can redistribute
 *  it and/or modify it under the terms of the GNU General Public License as
 *  published by the Free Software Foundation; either version 2 of the
 *  License, or (at your option) any later version.
 *
 *  The ASCEND Tcl/Tk interface is distributed in hope that it will be
 *  useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#define ASC_BUILDING_INTERFACE

#include "config.h"
#include <ascend/general/list.h>
#include <ascend/compiler/instance_enum.h>
#include <ascend/compiler/qlfdid.h>

STDHLF(Asc_BrowQlfdidSearchCmdHC,(Asc_BrowQlfdidSearchCmdHL,HLFSTOP));
int ascjson::Asc_BrowQlfdidSearchCmdDS(Asc_DString *hptr, int argc, CONST84 char *argv[])
{
  char temp[MAXIMUM_ID_LENGTH];
  struct gl_list_t *search_list;
  struct SearchEntry *se;
  unsigned long len,c;

  if ( argc != 2 ) {
    Asc_DStringSet(hptr,"wrong # args : Usage is qlfdid \"name\"");
    return HELP_ERROR;
  }
  search_list = Asc_BrowQlfdidSearch(QUIET(argv[1]),temp);
  g_relative_inst = g_search_inst;
  if ((g_search_inst==NULL) || (search_list==NULL)) {
    Asc_DStringAppend(hptr,"Orphaned ", HALL);
    Asc_DStringAppend(hptr, temp, HALL);
    return HELP_ERROR;
  } else {
    len = gl_length(search_list);
    for(c=1;c<=len;c++) {
      se = (SearchEntry *)gl_fetch(search_list,c);
      VTcl_AppendElement(hptr, se->name);
    }
    Asc_SearchListDestroy(search_list);
    return HELP_OK;
  }
}


