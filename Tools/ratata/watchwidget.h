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

#ifndef WATCHWIDGET_H
#define WATCHWIDGET_H

#include <QTreeWidget>
#include <QTabWidget>

class WatchWidget : public QTabWidget
 {
     Q_OBJECT

 public:
    WatchWidget(QWidget *parent = 0);
     ~WatchWidget();
    void InitWidget();
    void ClearAll();
    void AddLocalLine(QString & _name, QString & _value);
    void AddGlobalLine(QString & _name, QString & _value);
    void AddMemberLine(QString & _name, QString & _value);

     QIcon groupIcon;
     QIcon keyIcon;

    private:
        QTreeWidget localVar;
        QTreeWidget globalVar;
        QTreeWidget memberVar;

 };

#endif
