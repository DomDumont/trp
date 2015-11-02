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

#include "prefdialog.h"
//#include <QtGui>
#include <QtWidgets>

PrefDialog::PrefDialog(QWidget *parent) :
    QDialog(parent)
{
    QGroupBox *pathGroup = new QGroupBox(tr("Set Rototo Path"));
    rototoPathEdit = new QLineEdit();
     rototoPathEdit->setFixedWidth(500);
    m_button = new QPushButton("Browse",this);

    // Connect button signal to appropriate slot
    connect(m_button, SIGNAL(released()), this, SLOT(handleButton()));

    QGridLayout *echoLayout = new QGridLayout;
    echoLayout->addWidget(rototoPathEdit, 0, 0);
    echoLayout->addWidget(m_button, 0, 1);
    pathGroup->setLayout(echoLayout);

    QGroupBox *fontGroup = new QGroupBox(tr("Set Font"));
    m_fontName = new QLabel(m_font.family());
    m_fontName->setFixedWidth(500);
    m_buttonChangeFont = new QPushButton("Browse",this);
    connect(m_buttonChangeFont, SIGNAL(released()), this, SLOT(handleButtonChangeFont()));


    QGridLayout *fontLayout = new QGridLayout;
    fontLayout->addWidget(m_fontName, 0, 0);
    fontLayout->addWidget(m_buttonChangeFont, 0, 1);
    fontGroup->setLayout(fontLayout);


    QGridLayout *layout = new QGridLayout;
    layout->addWidget(pathGroup, 0, 0);
    layout->addWidget(fontGroup, 1, 0);
    setLayout(layout);

}

void PrefDialog::SetRototoPath(QString _path)
{
    m_rototoPath = _path;
    rototoPathEdit->setText(m_rototoPath);
}

void PrefDialog::SetFont(QFont _font)
{
    m_font = _font;
    m_fontName->setText(m_font.family());
}

void PrefDialog::handleButton()
{
    QString fileName;
#ifdef Q_OS_WIN32
    fileName = QFileDialog::getOpenFileName(this,
        tr("Open Image"), "rototo.exe", tr("exe Files (*.exe)"));
#endif
#ifdef Q_OS_MAC
    fileName = QFileDialog::getOpenFileName(this,
        tr("Open Image"), "rototo.app", tr("Application Files (*.app)"));
#endif

    this->SetRototoPath(fileName);
}

void PrefDialog::handleButtonChangeFont()
{
    bool ok;
QFont font = QFontDialog::getFont(&ok, /*QFont("Helvetica [Cronyx]", 10)*/this->m_font, this);
if (ok)
    {
    this->SetFont(font);
    }
}
