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

#include "errorwidget.h"

ErrorWidget::ErrorWidget(QWidget *parent): QTreeWidget(parent)
{
    InitWidget();

}


ErrorWidget::~ErrorWidget()
{
}

void ErrorWidget::InitWidget()
{
        QStringList labels;
    labels << tr("Type") << tr("File")<< tr("Line")<< tr("Col")<< tr("Msg");
    setHeaderLabels(labels);

    header()->setSectionResizeMode(0, QHeaderView::Interactive);
    header()->setSectionResizeMode(1, QHeaderView::Interactive);
    header()->setSectionResizeMode(2, QHeaderView::Interactive);
    header()->setSectionResizeMode(3, QHeaderView::Interactive);
    header()->setSectionResizeMode(4, QHeaderView::Interactive);


    groupIcon.addPixmap(style()->standardPixmap(QStyle::SP_DirClosedIcon),
                        QIcon::Normal, QIcon::Off);
    groupIcon.addPixmap(style()->standardPixmap(QStyle::SP_DirOpenIcon),
                        QIcon::Normal, QIcon::On);
    keyIcon.addPixmap(style()->standardPixmap(QStyle::SP_FileIcon));

    this->setSortingEnabled(true);

    header()->setSortIndicatorShown(true);
    header()->setSectionsClickable(true);
}


void ErrorWidget::AddErrorLine(QString & _type, QString & _file, QString & _line,QString & _col,QString & _msg)
{
        QTreeWidgetItem *tempItem = new QTreeWidgetItem(this);

        tempItem->setText(0, _type);
        tempItem->setText(1, _file);
        tempItem->setText(2, _line);
        tempItem->setText(3, _col);
        tempItem->setText(4, _msg);

        if (_type == "[ERR]")
        {
                //QFont font("" , 9 , QFont::Bold );
                QFont font;
                font.setBold(true);
                QBrush b (Qt::red);
                for(int i=0;i<5;i++)
                {
                tempItem->setForeground( i , b );
                tempItem->setFont( i,  font );
                }
        }

}

void ErrorWidget::ClearAll()
{
    this->clear();
}
