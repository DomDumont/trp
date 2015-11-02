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

#include "breakpointwidget.h"

//#include <QtGui>
#include <QtWidgets>
#include <QDomDocument>
#include <QFile>



BreakpointWidget::BreakpointWidget(QWidget *parent): QTreeWidget(parent)
{
    InitWidget();

}


BreakpointWidget::~BreakpointWidget()
{
}

void BreakpointWidget::InitWidget()
{
        QStringList labels;
    labels << tr("File")<< tr("Line");
    setHeaderLabels(labels);

    header()->setSectionResizeMode(0, QHeaderView::Interactive);
    header()->setSectionResizeMode(1, QHeaderView::Interactive);


    groupIcon.addPixmap(style()->standardPixmap(QStyle::SP_DirClosedIcon),
                        QIcon::Normal, QIcon::Off);
    groupIcon.addPixmap(style()->standardPixmap(QStyle::SP_DirOpenIcon),
                        QIcon::Normal, QIcon::On);
    keyIcon.addPixmap(style()->standardPixmap(QStyle::SP_FileIcon));

    this->setSortingEnabled(true);

    header()->setSortIndicatorShown(true);
    header()->setSectionsClickable(true);
}


void BreakpointWidget::AddLine(QString & _file, QString & _line)
{
        QTreeWidgetItem *tempItem = new QTreeWidgetItem(this);


        tempItem->setText(0, _file);
        tempItem->setText(1, _line);


}

void BreakpointWidget::ClearAll()
{
    this->clear();
}
