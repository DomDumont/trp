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

#include <QtGui>

#include "dlgnewproject.h"


DlgNewProject::DlgNewProject(QWidget *parent): QDialog(parent)
{
    createMyGroupBox();

    buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok|QDialogButtonBox::Cancel);

    connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
    connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));

     QVBoxLayout *mainLayout = new QVBoxLayout;
     mainLayout->addWidget(myGroupBox);
     mainLayout->addWidget(buttonBox);
     setLayout(mainLayout);
}

DlgNewProject::~DlgNewProject()
{
}

void DlgNewProject::browse()
{
    QString directory = QFileDialog::getExistingDirectory(this,
                            tr("Test"), QDir::currentPath());

    if (!directory.isEmpty())
    {
        qDebug() <<"pas empty";
        lineEditLocation->setText(directory);
        //if (directoryComboBox->findText(directory) == -1)
        //    directoryComboBox->addItem(directory);
        //directoryComboBox->setCurrentIndex(directoryComboBox->findText(directory));
    }
    else
        qDebug() <<"Empty";
}
void DlgNewProject::accept()
   {
       if (lineEditName->text().isEmpty())
        {
            QMessageBox tempBox;
            tempBox.setText("Empty Project Name");
            tempBox.exec();
            return;
        }
       if (lineEditLocation->text().isEmpty())
        {
            QMessageBox tempBox;
            tempBox.setText("Empty Location");
            tempBox.exec();
            return;
        }
        QDialog::accept();
   }
void DlgNewProject::createMyGroupBox()
{
    myGroupBox = new QGroupBox(tr("Create New Project"));
    QGridLayout *layout = new QGridLayout;

    labelName = new QLabel(tr("Name"));
    layout->addWidget(labelName, 1, 0);
    labelLocation = new QLabel(tr("Location"));
    layout->addWidget(labelLocation, 2, 0);
    lineEditName = new QLineEdit;
    layout->addWidget(lineEditName, 1, 1);
    lineEditLocation = new QLineEdit;
    layout->addWidget(lineEditLocation, 2, 1);
    buttonBrowse = new QPushButton(tr("Browse"));
    connect(buttonBrowse, SIGNAL(clicked()),this, SLOT(browse()));
    layout->addWidget(buttonBrowse,2,2);
    checkboxCreateDir = new QCheckBox(tr("Create directory"));
    checkboxCreateDir->setChecked(true);
    layout->addWidget(checkboxCreateDir,1,2);

    /*
    for (int i = 0; i < NumGridRows; ++i) {
        labels[i] = new QLabel(tr("Line %1:").arg(i + 1));
        lineEdits[i] = new QLineEdit;
        layout->addWidget(labels[i], i + 1, 0);
        layout->addWidget(lineEdits[i], i + 1, 1);
    }

    smallEditor = new QTextEdit;
    smallEditor->setPlainText(tr("This widget takes up about two thirds of the "
                                "grid layout."));
    layout->addWidget(smallEditor, 0, 2, 4, 1);

    layout->setColumnStretch(1, 10);
    layout->setColumnStretch(2, 20);


    */
    layout->setColumnMinimumWidth (1, 400 );
    myGroupBox->setLayout(layout);
}
