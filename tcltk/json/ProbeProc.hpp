/*
 *  ProbeProc.h
 *  by Ben Allan
 *  Created: 6/97
 *  Version: $Revision: 1.12 $
 *  Version control file: $RCSfile: ProbeProc.h,v $
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

/** @file
 *  Probe procedures.
 *  This probe implementation constitutes a total rework of the probe.
 *  <pre>
 *  To include this header, you must include the following:
 *      #include "tcl.h"
 *      #include "utilities/ascConfig.h"
 *      #include "interface/ProbeProc.h" (which is sort of obvious)
 *  </pre>
 */

#ifndef ASCTK_PROBEPROC_HPP
#define ASCTK_PROBEPROC_HPP


int Asc_ProbeCmd(Asc_DString *hptr, int argc, CONST84 char *argv[]);
/**<
 * <!--  status = Asc_ProbeCmd(cdata,hptr, argc,argv);                -->
 * This is the tcl callback for our commandline help facility.
 * (I'm tempted to quote Lord of the Rings, but it suffices to
 * say we now have ONE probe command.)
 */
private:

int AppendEntryItem(Asc_DString *hptr,struct ProbeEntry *e);
void ProbeAppendAll(Asc_DString *hptr, struct gl_list_t *p);

public:

#endif /* ASCTK_PROBEPROC_HPP */

