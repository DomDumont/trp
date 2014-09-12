/*
  ==============================================================================

   This file is part of the T.R.P. Engine
   Copyright (c) 2014 - Dominique Dumont

   Permission is granted to use this software under the terms of either:
   a) the GPL v3 (or any later version)
   b) the Affero GPL v3

   Details of these licenses can be found at: www.gnu.org/licenses

   T.R.P. is distributed in the hope that it will be useful, but WITHOUT ANY
   WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
   A PARTICULAR PURPOSE.  See the GNU General Public License for more details.

   -----------------------------------------------------------------------------

   To release a closed-source product which uses T.R.P., commercial licenses are
   available: visit veed.fr for more information.

  ==============================================================================
*/

#include "breakpoint.h"


bool BreakpointManager::ToggleBreakpoint(QString _file,int _line)
{
    Breakpoint temp(_file,_line);
    if (IsBreakpointSet(temp) == true)
        {
        RemoveBreakpoint(temp);
        return false;
        }
    else
        {
        AddBreakpoint(temp);
        return true;
        }
}

bool  BreakpointManager::IsBreakpointSet(Breakpoint _p)
    {
     for (int i = 0; i < list.size(); ++i)
     {
    Breakpoint temp = list.at(i);
    if (temp == _p)
        {
        return true;
        }
     }
     return false;
    }

bool BreakpointManager::IsBreakpointSet(QString _file,int _line)
{
    Breakpoint temp(_file,_line);
    return IsBreakpointSet(temp);
}

void BreakpointManager::RemoveBreakpoint(Breakpoint _p)
    {
    for (int i = 0; i < list.size(); ++i)
         {
        Breakpoint temp = list.at(i);
        if (temp == _p)
            {
            list.removeAt(i);
            m_nbBreakpoints --;
            emit breakpoint_removed();
            break;
            }
         }
    }

void BreakpointManager::AddBreakpoint(Breakpoint p)
{
    list.append(p);
    m_nbBreakpoints ++;
    emit breakpoint_added();
}
