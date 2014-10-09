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

#ifndef PROJECTWIDGET_H
#define PROJECTWIDGET_H

#include <QTreeWidget>


class ProjectWidget : public QTreeWidget
 {
     Q_OBJECT

 public:
    ProjectWidget(QWidget *parent = 0);
     ~ProjectWidget();

     QIcon groupIcon;
     QIcon keyIcon;

     QTreeWidgetItem *projectNode;
     QTreeWidgetItem *scriptsNode;
     QTreeWidgetItem *imagesNode;
     QTreeWidgetItem *soundsNode;

     QString rootDir;
     QString scriptDir;
     QString initialFile;

     QString currentFile() { return curFile; }
     QString userFriendlyCurrentFile();
     void setCurrentFile(const QString &fileName);
     bool m_projectIsModified;




     void InitWidget();
     bool loadProject(const QString &fileName);
     bool createEmptyProject(const QString &fileName);
     void closeProject();
     bool saveProject();
     void ShowErrorMsg(QString &msg);
     QStringList GetScriptFiles();

     public slots:
         void treeContextMenu(const QPoint& aPosition);
         void deleteFileFromProject();
         void renameFile();
         void importExistingScript();
         void importExistingSound();
         void importExistingImage();
         void openInExplorer();
         void createNewScript();

         void onItemChanged(QTreeWidgetItem * item, int col);

    private:
          QString curFile;
          QString strippedName(const QString &fullFileName);
 };

class MyTreeWidgetItem : public QTreeWidgetItem
{
public:
    MyTreeWidgetItem(QTreeWidget *tree) : QTreeWidgetItem(tree)  {}
    MyTreeWidgetItem(QTreeWidget * parent, const QStringList & strings)
                   : QTreeWidgetItem (parent,strings)  {}
    MyTreeWidgetItem ( QTreeWidgetItem * parent, int type = Type ):	QTreeWidgetItem(parent,type) {}
     void setData ( int column, int role, const QVariant & value );
    /*
    bool operator< (const QTreeWidgetItem &other) const
    {
        int sortCol = treeWidget()->sortColumn();
        int myNumber = text(sortCol).toInt();
        int otherNumber = other.text(sortCol).toInt();
        return myNumber < otherNumber;
    }
    */
};
#endif
