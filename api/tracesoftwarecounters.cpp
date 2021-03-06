/*****************************************************************************\
 *                        ANALYSIS PERFORMANCE TOOLS                         *
 *                               libparaver-api                              *
 *                      API Library for libparaver-kernel                    *
 *****************************************************************************
 *     ___     This library is free software; you can redistribute it and/or *
 *    /  __         modify it under the terms of the GNU LGPL as published   *
 *   /  /  _____    by the Free Software Foundation; either version 2.1      *
 *  /  /  /     \   of the License, or (at your option) any later version.   *
 * (  (  ( B S C )                                                           *
 *  \  \  \_____/   This library is distributed in hope that it will be      *
 *   \  \__         useful but WITHOUT ANY WARRANTY; without even the        *
 *    \___          implied warranty of MERCHANTABILITY or FITNESS FOR A     *
 *                  PARTICULAR PURPOSE. See the GNU LGPL for more details.   *
 *                                                                           *
 * You should have received a copy of the GNU Lesser General Public License  *
 * along with this library; if not, write to the Free Software Foundation,   *
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA          *
 * The GNU LEsser General Public License is contained in the file COPYING.   *
 *                                 ---------                                 *
 *   Barcelona Supercomputing Center - Centro Nacional de Supercomputacion   *
\*****************************************************************************/

/* -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- *\
 | @file: $HeadURL$
 | @last_commit: $Date$
 | @version:     $Revision$
\* -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */

#include "tracesoftwarecounters.h"
#include "kernelconnection.h"

using namespace std;

string TraceSoftwareCounters::traceToolID = "software_counters";
string TraceSoftwareCounters::traceToolName = "Software Counters";
string TraceSoftwareCounters::traceToolExtension = "sc";

TraceSoftwareCounters *TraceSoftwareCounters::create( KernelConnection *whichKernel,
                                  char *traceIn,
                                  char *traceOut,
                                  TraceOptions *options,
                                  ProgressController *progress )
{
  return new TraceSoftwareCountersProxy( whichKernel, traceIn, traceOut, options, progress );
}


std::string TraceSoftwareCounters::getID()
{
  return TraceSoftwareCounters::traceToolID;
}


std::string TraceSoftwareCounters::getName()
{
  return TraceSoftwareCounters::traceToolName;
}


std::string TraceSoftwareCounters::getExtension()
{
  return TraceSoftwareCounters::traceToolExtension;
}


TraceSoftwareCountersProxy::TraceSoftwareCountersProxy( KernelConnection *whichKernel,
                                  char *traceIn,
                                  char *traceOut,
                                  TraceOptions *options,
                                  ProgressController *progress )
{
  myTraceSoftwareCounters = whichKernel->newTraceSoftwareCounters( traceIn, traceOut, options, progress );
}

TraceSoftwareCountersProxy::~TraceSoftwareCountersProxy()
{
  delete myTraceSoftwareCounters;
}
