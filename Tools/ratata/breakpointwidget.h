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

#ifndef BREAKPOINTWIDGET_H
#define BREAKPOINTWIDGET_H

#include <QWidget>
#include <QTreeWidget>

class BreakpointWidget : public QTreeWidget
{
    Q_OBJECT
public:
    BreakpointWidget(QWidget *parent = 0);
    ~BreakpointWidget();
   void InitWidget();
   void AddLine(QString & _file, QString & _line);
   void ClearAll();




    QIcon groupIcon;
    QIcon keyIcon;

signals:

public slots:

};
#endif // BREAKPOINTWIDGET_H
