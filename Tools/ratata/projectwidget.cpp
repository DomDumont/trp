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
#include <QMessageBox>

#include "projectwidget.h"

  void MyTreeWidgetItem::setData ( int column, int role, const QVariant & value )
  {
      if (role == Qt::EditRole)
        {
            qDebug()<<"EDIT ROLE !!! OLD =  "<< this->text(column) << " NEW = "<< value.toString();
            ProjectWidget * tempParent = (ProjectWidget *) this->treeWidget();
        QString oldPath = tempParent->rootDir + this->text(column);
        QString newPath = tempParent->rootDir + value.toString();
        if (QFile::rename(oldPath,newPath) == false)
            {
                QString errMsg = "Invalid Rename";
                tempParent->ShowErrorMsg(errMsg);
                return;
            }

        }

      QTreeWidgetItem::setData(column,role,value);
  }

void ProjectWidget::ShowErrorMsg(QString &msg)
{
             QMessageBox::warning(this, tr("Application"),msg);

}
ProjectWidget::ProjectWidget(QWidget *parent): QTreeWidget(parent)
 {
     InitWidget();

     curFile = "";
     this->scriptsNode = NULL;
     m_projectIsModified = false;

     this->setEditTriggers(QAbstractItemView::NoEditTriggers);
     this->setContextMenuPolicy(Qt::CustomContextMenu);
     connect( this, SIGNAL( customContextMenuRequested( const QPoint& ) ),
         this, SLOT( treeContextMenu( const QPoint& ) ) );

     connect(this,SIGNAL( itemChanged ( QTreeWidgetItem *, int) ), this,SLOT(onItemChanged(QTreeWidgetItem *,int)));
 }


void ProjectWidget::treeContextMenu(const QPoint& aPosition)
{
    QTreeWidgetItem * tempItem = this->selectedItems().at(0);


    QMenu menu(tr("Context Menu"), this);
    if (tempItem == scriptsNode)
        {
        //add actions to menu and create connections
        menu.addAction("Create New Script",this,SLOT(createNewScript()));
        menu.addAction("Import Existing Script",this,SLOT(importExistingScript()));
        }
    else
    if (tempItem == soundsNode)
        {
        //add actions to menu and create connections
        menu.addAction("Import Existing Sound",this,SLOT(importExistingSound()));
        }
    else
  if (tempItem == imagesNode)
        {
        //add actions to menu and create connections
        menu.addAction("Import Existing Image",this,SLOT(importExistingImage()));
        }
    else
    if (tempItem->parent() == scriptsNode)
        {
        //add actions to menu and create connections
        menu.addAction("Open in Explorer",this,SLOT(openInExplorer()));
        menu.addAction("Remove",this,SLOT(deleteFileFromProject()));
        menu.addAction("Rename",this,SLOT(renameFile()));
        }
    else
    if (tempItem->parent() == soundsNode)
        {
        //add actions to menu and create connections
        menu.addAction("Open in Explorer",this,SLOT(openInExplorer()));
        menu.addAction("Remove",this,SLOT(deleteFileFromProject()));
        menu.addAction("Rename",this,SLOT(renameFile()));
        }
    else
    if (tempItem->parent() == imagesNode)
        {
        //add actions to menu and create connections
        menu.addAction("Open in Explorer",this,SLOT(openInExplorer()));
        menu.addAction("Remove",this,SLOT(deleteFileFromProject()));
        menu.addAction("Rename",this,SLOT(renameFile()));
        }

    QAction * resultAction = menu.exec(this->mapToGlobal(aPosition));
    if (resultAction)
        qDebug()<<resultAction->text();

}
 ProjectWidget::~ProjectWidget()
 {
     closeProject();
 }

void ProjectWidget::createNewScript()
{
    QString proposedName = "Untitled.rsc";
    bool bOk = false;
    while (bOk == false)
        {
        QString tempFileName = rootDir + proposedName ;
        QFile file(tempFileName );
        if (file.exists())
            {
            proposedName = "New - " + proposedName;
            }
        else
            {

            //Create empty file
            if (file.open(QIODevice::ReadWrite))
                {
                 QTextStream stream(&file);
                stream << "// Empty Script generated by Ratata" << endl;
                }
            else
                {
                    return;
                }

            bOk = true;
            //Add tree item
            MyTreeWidgetItem *tempItem = new MyTreeWidgetItem(scriptsNode);
            tempItem->setText(0, proposedName);
            tempItem->setFlags(tempItem->flags() |  Qt::ItemIsEditable);
            tempItem->setIcon(0,QIcon(":/images/script.png"));
            }
        }

}

 void ProjectWidget::renameFile()
{
    QTreeWidgetItem * tempItem = this->selectedItems().at(0);
    this->editItem(tempItem,0);


}


 void ProjectWidget::openInExplorer()
 {
        QTreeWidgetItem * tempItem = this->selectedItems().at(0);
        QString tempPath = rootDir + tempItem->text(0);

//		QString path = QDir::toNativeSeparators(QApplication::applicationDirPath());
        QDesktopServices::openUrl(QUrl("file:///" + tempPath));
 }
 void  ProjectWidget::importExistingImage()
 {
          QString fileName = QFileDialog::getOpenFileName(this, tr("Open Project"),rootDir, tr("Png files (*.png);;Jpeg files (*.jpg);;All files (*.*);;"));

     if (!fileName.isEmpty())
        {
            QFileInfo fi(fileName);
            QString base = fi.fileName();
            if (fi.absoluteDir() != rootDir)
                {
                    qDebug()<<"Not in project dir, must Copy file";
                    QFile::copy (fileName, rootDir+base );
                }

        MyTreeWidgetItem *tempItem = new MyTreeWidgetItem(imagesNode);
        tempItem->setText(0, base);
        tempItem->setFlags(tempItem->flags() |  Qt::ItemIsEditable);
        tempItem->setIcon(0,QIcon(":/images/image.png"));
        }
 }
  void  ProjectWidget::importExistingSound()
 {
          QString fileName = QFileDialog::getOpenFileName(this, tr("Open Project"),rootDir, tr("Wav files (*.wav);;MP3 files (*.mp3);;All files (*.*);;"));

     if (!fileName.isEmpty())
        {
            QFileInfo fi(fileName);
            QString base = fi.fileName();
            if (fi.absoluteDir() != rootDir)
                {
                    qDebug()<<"Not in project dir, must Copy file";
                    QFile::copy (fileName, rootDir+base );
                }

        MyTreeWidgetItem *tempItem = new MyTreeWidgetItem(soundsNode);
        tempItem->setText(0, base);
        tempItem->setFlags(tempItem->flags() |  Qt::ItemIsEditable);
        tempItem->setIcon(0,QIcon(":/images/music.png"));
        }
 }

 void  ProjectWidget::importExistingScript()
 {
          QString fileName = QFileDialog::getOpenFileName(this, tr("Open Project"),rootDir, tr("Script files (*.rsc);;"));

     if (!fileName.isEmpty())
        {
            QFileInfo fi(fileName);
            QString base = fi.fileName();
            if (fi.absoluteDir() != rootDir)
                {
                    qDebug()<<"Not in project dir, must Copy file";
                    QFile::copy (fileName, rootDir+base );
                }

        MyTreeWidgetItem *tempItem = new MyTreeWidgetItem(scriptsNode);
        tempItem->setText(0, base);
        tempItem->setFlags(tempItem->flags() |  Qt::ItemIsEditable);
        tempItem->setIcon(0,QIcon(":/images/script.png"));
        }
 }
void ProjectWidget::deleteFileFromProject()
{
    QTreeWidgetItem * tempItem = this->selectedItems().at(0);
    QTreeWidgetItem *parent = tempItem->parent();
    parent->removeChild(tempItem);
}
 void ProjectWidget::InitWidget()
 {
         QStringList labels;
     labels << tr("Project Explorer") ;
     setHeaderLabels(labels);
     header()->setSectionResizeMode(0, QHeaderView::Interactive);

 }
 bool ProjectWidget::createEmptyProject(const QString &fileName)
     {
     QFileInfo fi(fileName);
     QString curFile = fi.absoluteFilePath();
     QString curRoot = fi.absolutePath();


        QDomDocument doc;
    QDomElement docElem = doc.documentElement();

    //Project
    QDomElement project_elem = doc.createElement("project");

    this->rootDir = "toto"; //Change this
    //Scripts
    QDomElement script_elem = doc.createElement("scripts");
    project_elem.appendChild(script_elem);
    //Images
    QDomElement images_elem = doc.createElement("images");
    project_elem.appendChild(images_elem);
    //Sounds
    QDomElement sounds_elem = doc.createElement("sounds");
    project_elem.appendChild(sounds_elem);
    doc.appendChild(project_elem);

    QFile f(curFile);
    f.open(QIODevice::WriteOnly);
    QTextStream stream(&f);
    QString toto = doc.toString();
    stream << toto;
    f.close();


    return true;

     }
 bool ProjectWidget::loadProject(const QString &fileName)
{

    closeProject();
    InitWidget();

    QDomDocument doc;

    //Load file
    QFile f(fileName);
    f.open(QIODevice::ReadOnly);
    doc.setContent(&f);
    f.close();
    setCurrentFile(fileName);
    // The root node is supposed to be a "project" tag, we retrieve its name
    QDomElement root=doc.documentElement();

    //Create project node.
    projectNode = new MyTreeWidgetItem(this);

    projectNode->setText(0, userFriendlyCurrentFile().toStdString().c_str());
    projectNode->setIcon(0,QIcon(":/images/home.png"));

    this->rootDir  = QFileInfo(fileName).absoluteDir().absolutePath()/*+"/"*/;
    this->scriptDir  = QFileInfo(this->rootDir).dir().absolutePath()+"/";
    this->rootDir += "/";


    // We traverse its children
    QDomElement child=root.firstChild().toElement();
    while(!child.isNull())
    {


    if (child.tagName() == "scripts")
        {
        //First create scripts node.
         scriptsNode = new MyTreeWidgetItem(projectNode);
         scriptsNode->setText(0, tr("Scripts"));
         scriptsNode->setIcon(0,QIcon(":/images/folder_closed.png"));

        // We traverse children
        QDomElement grandChild=child.firstChild().toElement();
        while(!grandChild.isNull())
            {
            // We know how to treat triangle and quad
            if (grandChild.tagName() == "file")
                {

                MyTreeWidgetItem *tempItem = new MyTreeWidgetItem(scriptsNode);
                tempItem->setText(0, grandChild.text());
                tempItem->setFlags(tempItem->flags() |  Qt::ItemIsEditable);
                //Check file existence
                QString tempFileName = rootDir + grandChild.text();
                QFile file(tempFileName );
                if (!file.exists())
                    {
                    tempItem->setIcon(0,QIcon(":/images/missing.png"));
                    }
                else
                if (tempItem->text(0) == this->initialFile)
                {
                    tempItem->setIcon(0,QIcon(":/images/initial.png"));
                }
                else
                    {
                    tempItem->setIcon(0,QIcon(":/images/script.png"));
                    }
                //++nbTriangles;
                }
            grandChild = grandChild.nextSibling().toElement();
            }
        }
    else if (child.tagName() == "sounds")
        {
            //First create sound node
         soundsNode = new MyTreeWidgetItem(projectNode);
         soundsNode->setText(0, tr("Sounds"));
         soundsNode->setIcon(0,QIcon(":/images/folder_closed.png"));

        // We traverse children
        QDomElement grandChild=child.firstChild().toElement();
        while(!grandChild.isNull())
            {
            // We know how to treat triangle and quad
            if (grandChild.tagName() == "file")
                {
                MyTreeWidgetItem *tempItem = new MyTreeWidgetItem(soundsNode);
                tempItem->setText(0, grandChild.text());
                tempItem->setIcon(0,QIcon(":/images/music.png"));
                tempItem->setFlags(tempItem->flags() |  Qt::ItemIsEditable);
                //++nbTriangles;
                }
            grandChild = grandChild.nextSibling().toElement();
            }
        }
    else if (child.tagName() == "images")
        {
        // First create images node
         imagesNode = new MyTreeWidgetItem(projectNode);
         imagesNode->setText(0, tr("Images"));
         imagesNode->setIcon(0,QIcon(":/images/folder_closed.png"));

        // We traverse children
        QDomElement grandChild=child.firstChild().toElement();
        while(!grandChild.isNull())
            {
            // We know how to treat triangle and quad
            if (grandChild.tagName() == "file")
                {
                MyTreeWidgetItem *tempItem = new MyTreeWidgetItem(imagesNode);
                tempItem->setText(0, grandChild.text());
                tempItem->setIcon(0,QIcon(":/images/image.png"));
                tempItem->setFlags(tempItem->flags() |  Qt::ItemIsEditable);
                //++nbTriangles;
                }
            grandChild = grandChild.nextSibling().toElement();
            }
        }
    child = child.nextSibling().toElement();
  }

m_projectIsModified = false;
this->expandAll();
return true;
}

void ProjectWidget::setCurrentFile(const QString &fileName)
 {
     curFile = QFileInfo(fileName).canonicalFilePath();
     setWindowModified(false); //TODODOMDOM check this
 }

QString ProjectWidget::userFriendlyCurrentFile()
{
    return strippedName(curFile);
}

QString ProjectWidget::strippedName(const QString &fullFileName)
{
    return QFileInfo(fullFileName).fileName();
}

void ProjectWidget::closeProject()
{
    if (curFile != "")
        this->saveProject();
    this->clear();
    curFile = "";
    m_projectIsModified = false;
}

QStringList ProjectWidget::GetScriptFiles()
{
  QStringList foundFiles;
  if (this->scriptsNode == NULL)
      return foundFiles;

  for (int i=0;i<scriptsNode->childCount();i++)
      {
      foundFiles << scriptsNode->child(i)->text(0);
      }
  return foundFiles;
}

bool ProjectWidget::saveProject()
{
    QDomDocument doc;
    QDomElement docElem = doc.documentElement();

    //Project
    QDomElement project_elem = doc.createElement("project");

    //Scripts
    QDomElement script_elem = doc.createElement("scripts");
    for (int i=0;i<scriptsNode->childCount();i++)
        {
        QDomElement file_elem = doc.createElement("file");
        QDomText t = doc.createTextNode(scriptsNode->child(i)->text(0));
        file_elem.appendChild(t);
        script_elem.appendChild(file_elem);
        }
    project_elem.appendChild(script_elem);
    //Images
    QDomElement images_elem = doc.createElement("images");
    for (int i=0;i<imagesNode->childCount();i++)
        {
        QDomElement file_elem = doc.createElement("file");
        QDomText t = doc.createTextNode(imagesNode->child(i)->text(0));
        file_elem.appendChild(t);
        images_elem.appendChild(file_elem);
        }
    project_elem.appendChild(images_elem);
    //Sounds
    QDomElement sounds_elem = doc.createElement("sounds");
    for (int i=0;i<soundsNode->childCount();i++)
        {
        QDomElement file_elem = doc.createElement("file");
        QDomText t = doc.createTextNode(soundsNode->child(i)->text(0));
        file_elem.appendChild(t);
        sounds_elem.appendChild(file_elem);
        }
    project_elem.appendChild(sounds_elem);
    doc.appendChild(project_elem);



    QFile f(curFile);

    f.open(QIODevice::WriteOnly);
    QTextStream stream(&f);
    QString toto = doc.toString();
    stream << toto;
    f.close();


    return true;
}

void ProjectWidget::onItemChanged(QTreeWidgetItem * item, int col)
{
    QString oldName = rootDir + item->text(col);
    qDebug()<<" Old Name = "<<oldName;
    //file.rename(oldName,
}
