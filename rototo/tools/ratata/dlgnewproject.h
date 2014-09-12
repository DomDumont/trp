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

#ifndef DLG_NEWPROJECT_H
#define DLG_NEWPROJECT_H

#include <qdialog.h>

class QDialogButtonBox;
class QLineEdit;
class QCheckBox;
class QGroupBox;
class QLabel;

class DlgNewProject : public QDialog
 {
     Q_OBJECT

 public:
    DlgNewProject(QWidget *parent = 0);
     ~DlgNewProject();
     QLineEdit			*lineEditName;
     QLineEdit			*lineEditLocation;
     QCheckBox			*checkboxCreateDir;

 private slots:
    void browse();
    void accept();

 private:
     void createMyGroupBox();

     QGroupBox			*myGroupBox;
     QDialogButtonBox	*buttonBox;
     QLabel				*labelName;
     QLabel				*labelLocation;
     QPushButton		*buttonBrowse;
 };

#endif
