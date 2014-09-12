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

#ifndef __BREAKPOINTS_H__
#define __BREAKPOINTS_H__

#include <QMutex>
#include <QString>
#include <QList>
#include <QObject>

class Breakpoint
{

public :
    QString file;
    int     line;
    bool    enabled;

public :
    Breakpoint(QString _file,int _line)
    {
        file = _file;
        line = _line;
        enabled = true;
    }

    bool operator==(const Breakpoint &other) const
    {
        //Teste si a.m_heure == b.m_heure etc.
        if (this->file.toLower() == other.file.toLower() && this->line == other.line)
            return true;
        else
            return false;
    }



};



class BreakpointManager: public QObject
{

    Q_OBJECT

    signals :

    void breakpoint_added();
    void breakpoint_removed();

    public:

        BreakpointManager() { m_nbBreakpoints = 0;}
        QList<Breakpoint> list;
        int m_nbBreakpoints;
        bool ToggleBreakpoint(QString file,int line);
        bool IsBreakpointSet(Breakpoint p);
        bool IsBreakpointSet(QString _file,int _line);
        void RemoveBreakpoint(Breakpoint p);
        void AddBreakpoint(Breakpoint p);



};

#endif
