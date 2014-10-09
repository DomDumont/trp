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

#include "grepwidget.h"

#include <QtGui>
#include <QDomDocument>
#include <QFile>


static void updateComboBox(QComboBox *comboBox)
{
    if (comboBox->findText(comboBox->currentText()) == -1)
        comboBox->addItem(comboBox->currentText());
}


GrepWidget::GrepWidget(QWidget *parent): QWidget(parent)
{
    InitWidget();

}

void GrepWidget::createOutput()
{
    output = new QTreeWidget();
    output->setSelectionBehavior(QAbstractItemView::SelectRows);


     connect(output, SIGNAL(cellActivated(int,int)),
             this, SLOT(openFileOfItem(int,int)));
}

GrepWidget::~GrepWidget()
{
}

QPushButton *GrepWidget::createButton(const QString &text, const char *member)
{
    QPushButton *button = new QPushButton(text);
    connect(button, SIGNAL(clicked()), this, member);
    return button;
}

QComboBox *GrepWidget::createComboBox(const QString &text)
{
    QComboBox *comboBox = new QComboBox;
    comboBox->setEditable(true);
    comboBox->addItem(text);
    comboBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

    return comboBox;
}

void GrepWidget::InitWidget()
{
    textLabel = new QLabel(tr("Containing text:"));
    textComboBox = createComboBox();
    findButton = createButton(tr("&Find"), SLOT(find()));
    createOutput();


    QGridLayout *mainLayout = new QGridLayout;
    mainLayout->setSizeConstraint(QLayout::SetNoConstraint);

    mainLayout->addWidget(textLabel, 0, 0);
    mainLayout->addWidget(textComboBox, 0, 1, 1, 2);
    mainLayout->addWidget(output, 2, 0,1,3);
    mainLayout->addWidget(findButton, 3, 2);
    setLayout(mainLayout);
}


void GrepWidget::find()
{
    output->clear();
    QString text = textComboBox->currentText();
    updateComboBox(textComboBox);
/*








    currentDir = QDir(path);
    QStringList files;
    if (fileName.isEmpty())
        fileName = "*";
    files = currentDir.entryList(QStringList(fileName),
                                 QDir::Files | QDir::NoSymLinks);

    if (!text.isEmpty())
        files = findFiles(files, text);
    showFiles(files);
*/
}


void GrepWidget::openFileOfItem(int row, int /* column */)
{
    //QTreeWidgetItem *item = output->selectedItems()row, 0);

    //QDesktopServices::openUrl(QUrl::fromLocalFile(currentDir.absoluteFilePath(item->text())));
}
