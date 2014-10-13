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

#ifndef GREPWIDGET_H
#define GREPWIDGET_H

#include <QWidget>
 #include <QDir>

class QLabel;
class QComboBox;
class QPushButton;
class QTreeWidget;
class QTreeWidgetItem;
class MDIMainWindow;
class GrepWidget : public QWidget
{
    Q_OBJECT
public:

    GrepWidget(QWidget *parent = 0);
    ~GrepWidget();
    void InitWidget();
    MDIMainWindow *m_mainWindow;
    void SetText(QString text);


public slots:
    void SlotTextEdited();

signals:

private slots:
    void find();
    void openFileOfItem(QTreeWidgetItem *w,int col);

private:
    QLabel *textLabel;
    QComboBox *textComboBox;
    QPushButton *findButton;
    QTreeWidget *output;

    QComboBox *createComboBox(const QString &text = QString());
    QPushButton *createButton(const QString &text, const char *member);
    void createOutput();
    QStringList findFiles(const QStringList &files, const QString &text);
    QDir currentDir;
    void showFiles(const QStringList &files);
};
#endif // GREPWIDGET_H
