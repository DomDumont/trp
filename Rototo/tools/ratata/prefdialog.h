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

#ifndef PREFDIALOG_H
#define PREFDIALOG_H

#include <QDialog>

class QLineEdit;
class QFont;
class QLabel;

class PrefDialog : public QDialog
{
    Q_OBJECT
public:
    explicit PrefDialog(QWidget *parent = 0);
    QFont m_font;
    QString m_rototoPath;
signals:

public slots:
    void handleButton();
    void handleButtonChangeFont();
    void SetFont(QFont _font);
    void SetRototoPath(QString _path);

private:
    QLineEdit *rototoPathEdit;
    QPushButton *m_button;
    QPushButton *m_buttonChangeFont;
    QLabel      *m_fontName;
};

#endif // PREFDIALOG_H
