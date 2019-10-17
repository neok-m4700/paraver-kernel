/*****************************************************************************\
 *                        ANALYSIS PERFORMANCE TOOLS                         *
 *                               libparaver-api                              *
 *                       Paraver Main Computing Library                      *
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


#ifndef PROCESSMODELTASK_H_INCLUDED
#define PROCESSMODELTASK_H_INCLUDED

#include <vector>
#include "processmodelthread.h"

class ProcessModelTask
{

  public:
    ProcessModelTask( TTaskOrder order = 0 ): traceGlobalOrder( order )
    {}

    ~ProcessModelTask()
    {}

    bool operator==( const ProcessModelTask& other ) const
    {
      return traceGlobalOrder == other.traceGlobalOrder &&
             threads          == other.threads;
    }

  protected:
    TTaskOrder traceGlobalOrder;
    std::vector<ProcessModelThread> threads;

  private:
    friend class ProcessModel;

};

#endif // PROCESSMODELTASK_H_INCLUDED
