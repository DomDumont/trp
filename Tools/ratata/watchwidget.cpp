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

#include "watchwidget.h"


WatchWidget::WatchWidget(QWidget *parent): QTabWidget(parent)
 {
     InitWidget();
 }

 WatchWidget::~WatchWidget()
 {
 }

 void WatchWidget::InitWidget()
 {
     QStringList labels;

     labels << tr("Name") << tr("Value");
     localVar.setHeaderLabels(labels);
     localVar.header()->setSectionResizeMode(0, QHeaderView::Interactive);
     localVar.header()->setSectionResizeMode(1, QHeaderView::Interactive);

     globalVar.setHeaderLabels(labels);
     globalVar.header()->setSectionResizeMode(0, QHeaderView::Interactive);
     globalVar.header()->setSectionResizeMode(1, QHeaderView::Interactive);

     memberVar.setHeaderLabels(labels);
     memberVar.header()->setSectionResizeMode(0, QHeaderView::Interactive);
     memberVar.header()->setSectionResizeMode(1, QHeaderView::Interactive);


     this->addTab(&localVar,tr("Local"));
     this->addTab(&globalVar,tr("Global"));
     this->addTab(&memberVar,tr("Member"));
 }

 void WatchWidget::AddLocalLine(QString & _name, QString & _value)
 {
     QTreeWidgetItem *tempItem = new QTreeWidgetItem(&localVar);

 /*
     QFont font = tempItem->font(0);
     font.setPixelSize(10);
     tempItem->setFont(0, font );
*/

     tempItem->setText(0, _name);
     tempItem->setText(1, _value);


 }

 void WatchWidget::AddGlobalLine(QString & _name, QString & _value)
 {
     QTreeWidgetItem *tempItem = new QTreeWidgetItem(&globalVar);

 /*
     QFont font = tempItem->font(0);
     font.setPixelSize(10);
     tempItem->setFont(0, font );
*/

     tempItem->setText(0, _name);
     tempItem->setText(1, _value);


 }

 void WatchWidget::AddMemberLine(QString & _name, QString & _value)
 {
     QTreeWidgetItem *tempItem = new QTreeWidgetItem(&memberVar);

 /*
     QFont font = tempItem->font(0);
     font.setPixelSize(10);
     tempItem->setFont(0, font );
*/

     tempItem->setText(0, _name);
     tempItem->setText(1, _value);


 }

 void WatchWidget::ClearAll()
 {
     localVar.clear();
     globalVar.clear();
     memberVar.clear();
 }


