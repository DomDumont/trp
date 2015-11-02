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

//#include <QtGui>
#include <QtWidgets>
#include <QDomDocument>
#include <QFile>

#include "callstackwidget.h"


CallstackWidget::CallstackWidget(QWidget *parent): QTreeWidget(parent)
 {
     InitWidget();
 }

 CallstackWidget::~CallstackWidget()
 {
 }



 void CallstackWidget::InitWidget()
 {
         QStringList labels;
     labels << tr("Src") << tr("Line")<< tr("Function");
     setHeaderLabels(labels);

  //   header()->setResizeMode(0, QHeaderView::Stretch);
  //   header()->setResizeMode(1, QHeaderView::Stretch);
     //header()->setResizeMode(2, QHeaderView::Stretch);

     header()->setSectionResizeMode(0, QHeaderView::Interactive);
     header()->setSectionResizeMode(1, QHeaderView::Interactive);
     header()->setSectionResizeMode(2, QHeaderView::Interactive);

     groupIcon.addPixmap(style()->standardPixmap(QStyle::SP_DirClosedIcon),
                         QIcon::Normal, QIcon::Off);
     groupIcon.addPixmap(style()->standardPixmap(QStyle::SP_DirOpenIcon),
                         QIcon::Normal, QIcon::On);
     keyIcon.addPixmap(style()->standardPixmap(QStyle::SP_FileIcon));


 }

 void CallstackWidget::AddLine(QString & _file, QString & _line,QString & _func)
 {
     QTreeWidgetItem *tempItem = new QTreeWidgetItem(this);

     tempItem->setText(0, _file);
     tempItem->setText(1, _line);
     tempItem->setText(2, _func);

 }

 void CallstackWidget::ClearAll()
 {
     this->clear();
 }

