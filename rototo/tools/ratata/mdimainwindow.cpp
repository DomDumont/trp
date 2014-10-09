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
#include <QMessageBox>
#include <QProcess>

#include "mdimainwindow.h"
#include "mdichild.h"

#include "breakpoint.h"
#include "projectwidget.h"
#include "watchwidget.h"
#include "callstackwidget.h"
#include "outputwidget.h"
#include "consolewidget.h"
#include "errorwidget.h"
#include "breakpointwidget.h"
#include "grepwidget.h"
#include "prefdialog.h"


#include "dlgnewproject.h"

#include "tcpclient.h"
#include "breakpoint.h"

//USING_NS_CC;




 MDIMainWindow::MDIMainWindow()
 {
     mdiArea = new QMdiArea;
     mdiArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
     mdiArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
     mdiArea->setViewMode(QMdiArea::TabbedView);
     setCentralWidget(mdiArea);
     connect(mdiArea, SIGNAL(subWindowActivated(QMdiSubWindow*)),
             this, SLOT(updateMenus()));
     windowMapper = new QSignalMapper(this);
     connect(windowMapper, SIGNAL(mapped(QWidget*)),
             this, SLOT(setActiveSubWindow(QWidget*)));


    completer = new QCompleter(this);
    completer->setModel(modelFromFile(":/resources/wordlist.txt"));
    completer->setModelSorting(QCompleter::CaseInsensitivelySortedModel);
    completer->setCaseSensitivity(Qt::CaseInsensitive);
    completer->setWrapAround(false);



     createActions();
     createMenus();
     createToolBars();
     createStatusBar();
     createDockWindows();

     prefDlg = new PrefDialog();

     setWindowIcon(QIcon(":/images/time_machine.png"));
     setWindowTitle(tr("Ratata, the rototo IDE"));
     setUnifiedTitleAndToolBarOnMac(true);

     //Variables Init
    breaked = false;
    m_bRunning = false;
    alive = true;

    m_tcpClient = new TcpClient(8080);

    connect(m_tcpClient, SIGNAL(vers_IHM_texte(QString)),m_outputWidget, SLOT(AddText(QString)));
    connect(m_tcpClient, SIGNAL(vers_IHM_texte(QString)),this, SLOT(DebugHook(QString)));
    connect(m_tcpClient, SIGNAL(vers_IHM_connexion_OK()),this, SLOT(OnConnectionChanged()));
    connect(m_tcpClient, SIGNAL(vers_IHM_connexion_Lost()),this, SLOT(OnConnectionChanged()));

    m_breakpointManager = new BreakpointManager();
    connect(m_breakpointManager, SIGNAL(breakpoint_added()),this, SLOT(OnBreakpointChanged()));
    connect(m_breakpointManager, SIGNAL(breakpoint_removed()),this, SLOT(OnBreakpointChanged()));

    updateMenus();
    readSettings();

 }

     MDIMainWindow::~MDIMainWindow()
         {
         alive = false;
         }

void MDIMainWindow::createDockWindows()
{
    QDockWidget *dock;

    // Grep Dock
    m_grepWidget = new GrepWidget(this);
    dock = new QDockWidget(tr("Grep View"), this);
    dock->setObjectName("grepViewDock");
    dock->setAllowedAreas(Qt::AllDockWidgetAreas);
    dock->setWidget(m_grepWidget);
    addDockWidget(Qt::BottomDockWidgetArea, dock);
    viewMenu->addAction(dock->toggleViewAction());

    // Breakpoint Dock
    m_breakpointWidget = new BreakpointWidget();
    dock = new QDockWidget(tr("Breakpoint View"), this);
    dock->setObjectName("breakpointViewDock");
    dock->setAllowedAreas(Qt::AllDockWidgetAreas);
    dock->setWidget(m_breakpointWidget);
    addDockWidget(Qt::BottomDockWidgetArea, dock);
    viewMenu->addAction(dock->toggleViewAction());

    //Error Dock

    m_errorWidget = new ErrorWidget();
    dock = new QDockWidget(tr("Error View"), this);
    dock->setObjectName("errorViewDock");
    dock->setAllowedAreas(Qt::AllDockWidgetAreas);
    dock->setWidget(m_errorWidget);
    addDockWidget(Qt::BottomDockWidgetArea, dock);
    viewMenu->addAction(dock->toggleViewAction());

    connect(m_errorWidget,SIGNAL(itemDoubleClicked(QTreeWidgetItem *, int)), this, SLOT(OnErrorWidgetDoubleClick( QTreeWidgetItem *, int)));
    //Project Dock

    m_projectWidget = new ProjectWidget();


    dock = new QDockWidget(tr("Project View"), this);
    dock->setObjectName("projectViewDock");
    dock->setAllowedAreas(Qt::AllDockWidgetAreas);
    //dock->setWidget(m_projectWidget);
    dock->setWidget(m_projectWidget);
    addDockWidget(Qt::LeftDockWidgetArea, dock);
    viewMenu->addAction(dock->toggleViewAction());
     //Trop tôt ici, openGL n'est pas commencé CCApplication::sharedApplication().run();

    connect(m_projectWidget, SIGNAL(doubleClicked(const QModelIndex &)), this, SLOT(clicSelection(const QModelIndex &)));

    //Watch Dock
    m_watchWidget = new WatchWidget();
    dock = new QDockWidget(tr("Watch View"), this);
    dock->setObjectName("WatchViewDock");
    dock->setAllowedAreas(Qt::AllDockWidgetAreas);
    //dock->setWidget(m_projectWidget);
    dock->setWidget(m_watchWidget);
    addDockWidget(Qt::BottomDockWidgetArea, dock);
    viewMenu->addAction(dock->toggleViewAction());

    //Callstack dock

    m_callstackWidget = new CallstackWidget();
    dock = new QDockWidget(tr("Callstack View"), this);
    dock->setObjectName("callstackViewDock");
    dock->setAllowedAreas(Qt::AllDockWidgetAreas);
    //dock->setWidget(m_projectWidget);
    dock->setWidget(m_callstackWidget);
    addDockWidget(Qt::BottomDockWidgetArea, dock);
    viewMenu->addAction(dock->toggleViewAction());

    //Output dock

    m_outputWidget = new OutputWidget();
    dock = new QDockWidget(tr("Output View"), this);
    dock->setObjectName("outputViewDock");
    dock->setAllowedAreas(Qt::AllDockWidgetAreas);
    dock->setWidget(m_outputWidget);
    addDockWidget(Qt::BottomDockWidgetArea, dock);
    viewMenu->addAction(dock->toggleViewAction());

    //Console dock

    m_consoleWidget = new ConsoleWidget(0,"Welcome to the console !",NULL);
    m_consoleWidget->setPrompt(">");
    dock = new QDockWidget(tr("Console View"), this);
    dock->setObjectName("consoleViewDock");
    dock->setAllowedAreas(Qt::AllDockWidgetAreas);
    dock->setWidget(m_consoleWidget);
    addDockWidget(Qt::BottomDockWidgetArea, dock);
    viewMenu->addAction(dock->toggleViewAction());

}

void MDIMainWindow::OnErrorWidgetDoubleClick( QTreeWidgetItem * _item, int)
{
    qDebug()<<_item->text(1);
    this->openWithFileName(m_projectWidget->rootDir + _item->text(1));
    this->activeMdiChild()->highlightLine(_item->text(2).toInt());


}

void MDIMainWindow::clicSelection(const QModelIndex& I)
{
//    QItemSelectionModel *selection = m_projectWidget->selectionModel();
    QAbstractItemModel  *modele = m_projectWidget->model();
    QVariant elementSelectionne = modele->data(I, Qt::DisplayRole);
    //QMessageBox::information(this, "Elément sélectionné", elementSelectionne.toString());
    openWithFileName(m_projectWidget->rootDir + elementSelectionne.toString());
}

 void MDIMainWindow::closeEvent(QCloseEvent *event)
 {

     mdiArea->closeAllSubWindows();
     if (mdiArea->currentSubWindow())
         {
         event->ignore();
        }
     else
         {
         writeSettings();
         event->accept();
     }
 }

void MDIMainWindow::OnBreakpointChanged()
 {
    m_breakpointWidget->ClearAll();

    for (int i = 0; i < m_breakpointManager->list.size(); ++i)
        {
        Breakpoint temp = m_breakpointManager->list.at(i);
        QString lineStr = QString::number(temp.line);
        m_breakpointWidget->AddLine(temp.file,lineStr);
        }
 }

void MDIMainWindow::OnConnectionChanged()
{
updateMenus();

// On vient de se connecter, on rebalance tous les breakpoints au cas où
if (m_tcpClient->IsConnected())
    {
    for (int i = 0; i < m_breakpointManager->list.size(); ++i)
        {
        Breakpoint temp = m_breakpointManager->list.at(i);

        QString command;
        command = QString("b %1:%2").arg(temp.file).arg(temp.line);
        m_tcpClient->SendText(command);

        }
    }
}

void MDIMainWindow::DebugHook(QString _line)
 {
qDebug()<<"[HOOK]"<<_line;
if (_line.startsWith("[PC]"))
    {
    QString temp = _line.mid(4);
    QString file =  temp.left(temp.indexOf(':'));
    QString line =  temp.mid(temp.indexOf(':')+1);
    line = line.left(line.indexOf(';'));

    if (file[0] != '~')
        this->openWithFileName(m_projectWidget->rootDir + file);
    else
        {
        this->openWithFileName(file.replace("~",m_projectWidget->scriptDir));
        }
    this->activeMdiChild()->highlightLine(line.toInt());

    // Now ask for the callstack
    m_callstackWidget->ClearAll();

    QString command;
    command = QString("w");
    this->m_tcpClient->SendText(command);

    // Now ask for local variables
    m_watchWidget->ClearAll();

    command = QString("l m");
    this->m_tcpClient->SendText(command);

    command = QString("l v");
    this->m_tcpClient->SendText(command);



    // And global variables

    command = QString("l g");
    this->m_tcpClient->SendText(command);

    }

if (_line.startsWith("[COMPILATION_START]"))
    {
    m_errorWidget->ClearAll();
    }

if (_line.startsWith("[LV]")||_line.startsWith("[GV]")||_line.startsWith("[MV]"))
    {
    QString type = _line.left(_line.indexOf(':'));
    QString reste = _line.mid(_line.indexOf(':')+1);
    QString name = reste.left(reste.indexOf(':'));
    QString value = reste.mid(reste.indexOf(':')+1);
    value = value.replace((QChar)'\n',(QChar)'\0');
    if (type == "[LV]")
        m_watchWidget->AddLocalLine(name,value);
    else
    if (type == "[GV]")
        m_watchWidget->AddGlobalLine(name,value);
    else
    if (type == "[MV]")
        m_watchWidget->AddMemberLine(name,value);

    }

if (_line.startsWith("[CS]"))
    {
    QString reste = _line.mid(_line.indexOf(':')+1);
    QString file = reste.left(reste.indexOf(':'));
    reste = reste.mid(reste.indexOf(':')+1);
    QString line = reste.left(reste.indexOf(':'));
    QString func =  reste.mid(reste.indexOf(':')+1);
    func = func.replace((QChar)'\n',(QChar)'\0');
    m_callstackWidget->AddLine(file,line,func);
    }

if (_line.startsWith("[ERR]")||_line.startsWith("[WARN]")||_line.startsWith("[INFO]"))
    {
    QString type = _line.left(_line.indexOf(':'));
    QString reste = _line.mid(_line.indexOf(':')+1);
    QString file =  reste.left(reste.indexOf(':'));
    reste =  reste.mid(reste.indexOf(':')+1);
    QString line = reste.left(reste.indexOf(':'));
    reste = reste.mid(reste.indexOf(':')+1);
    QString col = reste.left(reste.indexOf(':'));
    QString msg = reste.mid(reste.indexOf(':')+1);
    msg = msg.replace((QChar)'\n',(QChar)'\0');
    m_errorWidget->AddErrorLine(type,file,line,col,msg);
    }

 }

void MDIMainWindow::newProject()
{
DlgNewProject dialog(this);
if (dialog.exec() == QDialog::Accepted)
    {
        QString tempPath = dialog.lineEditLocation->text();
        QString tempName = dialog.lineEditName->text();
        //builder le chemin
        if (dialog.checkboxCreateDir->checkState() == Qt::Checked)
            {
            qDebug()<<" checked !!!";
            //Mkdir
            QDir dir;
            tempPath = tempPath + QDir::separator () + tempName;
             dir.mkpath(tempPath);
            }
        //Create an empty project
        QString tempFileName = tempPath + QDir::separator () + tempName + ".rap";
        m_projectWidget->createEmptyProject(tempFileName);
        m_projectWidget->loadProject(tempFileName);

        QFile::copy(":/resources/fps_images.png",  tempPath+ QDir::separator ()+"fps_images.png");
        QFile::copy(":/resources/fps_images-hd.png",  tempPath+ QDir::separator ()+"fps_images-hd.png");
        QFile::copy(":/resources/fps_images-ipadhd.png",  tempPath+ QDir::separator ()+"fps_images-ipadhd.png");
        //Update recent project here
        recentProjects.removeAll(tempFileName);
        recentProjects.prepend(tempFileName);
        updateRecentProjectActions();
        updateMenus(); //TODODOMDOM Should not be here


    }
else
    {
    QMessageBox tempBox;
    tempBox.setText("Operation canceled");
    tempBox.exec();
    }
}

 void MDIMainWindow::newFile()
 {
     MdiChild *child = createMdiChild();
     child->newFile();
     child->setCompleter(completer);
     child->show();
 }

 void MDIMainWindow::openRecentProject()
{
    QAction *action = qobject_cast<QAction *>(sender());
    if (action)
        {
        QString fileName = action->data().toString();
        if (!fileName.isEmpty())
            {
            m_projectWidget->loadProject(fileName);

            //Update recent project here
            recentProjects.removeAll(fileName);
            recentProjects.prepend(fileName);
            updateRecentProjectActions();
            updateMenus(); //TODODOMDOM Should not be here
            }
    }

}

void MDIMainWindow::openProjectFromFilename(QString fileName)
{
    if (!fileName.isEmpty())
       {
       m_projectWidget->loadProject(fileName);

       //Update recent project here
       recentProjects.removeAll(fileName);
       recentProjects.prepend(fileName);
       updateRecentProjectActions();
       updateMenus(); //TODODOMDOM Should not be here

       }
}
 void MDIMainWindow::openProject()
 {

     QString fileName = QFileDialog::getOpenFileName(this, tr("Open Project"),NULL, tr("Project files (*.rap);;"));

     openProjectFromFilename(fileName);

 }

void MDIMainWindow::updateRecentFileActions()
{
QMutableStringListIterator i(recentFiles);
while (i.hasNext())
    {
    if (!QFile::exists(i.next()))
    i.remove();
    }
for (int j = 0; j < MaxRecentFiles; ++j)
    {
    if (j < recentFiles.count())
        {
        QString text = tr("&%1 %2")
        .arg(j + 1)
        .arg(strippedName(recentFiles[j]));
        recentFileActions[j]->setText(text);
        recentFileActions[j]->setData(recentFiles[j]);
        recentFileActions[j]->setVisible(true);
        }
    else
        {
        recentFileActions[j]->setVisible(false);
        }
    }
separatorAction->setVisible(!recentFiles.isEmpty());
}

void MDIMainWindow::updateRecentProjectActions()
{
QMutableStringListIterator i(recentProjects);
while (i.hasNext())
    {
    if (!QFile::exists(i.next()))
    i.remove();
    }
for (int j = 0; j < MaxRecentProjects; ++j)
    {
    if (j < recentProjects.count())
        {
        QString text = tr("&%1 %2")
        .arg(j + 1)
        .arg(strippedName(recentProjects[j]));
        recentProjectActions[j]->setText(text);
        recentProjectActions[j]->setData(recentProjects[j]);
        recentProjectActions[j]->setVisible(true);
        }
    else
        {
        recentProjectActions[j]->setVisible(false);
        }
    }
separatorAction2->setVisible(!recentProjects.isEmpty());
}

QString MDIMainWindow::strippedName(const QString &fullFileName)
 {
     return QFileInfo(fullFileName).fileName();
 }

 void MDIMainWindow::openWithFileName(const QString& fileName,int line)
 {
     if (!fileName.isEmpty())
        {
         MdiChild *existing = (MdiChild *)findMdiChild(fileName);
         if (existing)
            {
            existing->setFont(m_globalFont);
            //existing->SetTabWidth();
            mdiArea->setActiveSubWindow((QMdiSubWindow *)existing);            

            if (line != -1)
                {
                QPlainTextEdit *toto = (QPlainTextEdit *) existing;
                QTextCursor *cursor = new QTextCursor(toto->textCursor());
                int linenumber = cursor->blockNumber();
                int linedif = line - linenumber;
                if (linedif < 0)
                cursor->movePosition(QTextCursor::PreviousBlock, QTextCursor::MoveAnchor, -linedif);
                else
                cursor->movePosition(QTextCursor::NextBlock, QTextCursor::MoveAnchor, linedif);

                }
            return;
            }
        //Update recent file here
        recentFiles.removeAll(fileName);
        recentFiles.prepend(fileName);
        updateRecentFileActions();


         MdiChild *child = createMdiChild();
         if (child->loadFile(fileName))
            {
             statusBar()->showMessage(tr("File loaded"), 2000);
             child->setCompleter(completer);
             child->setFont(m_globalFont);
             child->SetTabWidth();
                child->show();

             if (line != -1)
                 {
                 QPlainTextEdit *toto = (QPlainTextEdit *) child;
                 QTextCursor *cursor = new QTextCursor(toto->textCursor());
                 int linenumber = cursor->blockNumber();
                 int linedif = line - linenumber;
                 if (linedif < 0)
                 cursor->movePosition(QTextCursor::PreviousBlock, QTextCursor::MoveAnchor, -linedif);
                 else
                 cursor->movePosition(QTextCursor::NextBlock, QTextCursor::MoveAnchor, linedif);
                 }


            }
         else
            {
             child->parentWidget()->close();
            }
        }

 }
void MDIMainWindow::openRecentFile()
{
    QAction *action = qobject_cast<QAction *>(sender());
    if (action)
        openWithFileName(action->data().toString());

}

void MDIMainWindow::openFileFromFilename(QString fileName,int line)
{
    openWithFileName(fileName,line);
}


 void MDIMainWindow::open()
{
    QString fileName = QFileDialog::getOpenFileName(this);
    openWithFileName(fileName);
}

 void MDIMainWindow::save()
 {
    if (activeMdiChild() && activeMdiChild()->save())
        statusBar()->showMessage(tr("File saved"), 2000);
 }

 void MDIMainWindow::saveAs()
 {
     if (activeMdiChild() && activeMdiChild()->saveAs())
         statusBar()->showMessage(tr("File saved"), 2000);
 }

 void MDIMainWindow::cut()
 {
     if (activeMdiChild())
         activeMdiChild()->cut();
 }

 void MDIMainWindow::copy()
 {
     if (activeMdiChild())
         activeMdiChild()->copy();
 }

 void MDIMainWindow::paste()
 {
     if (activeMdiChild())
         activeMdiChild()->paste();
 }

 void MDIMainWindow::about()
 {
    QString tempMsg = tr("Ratata, the rototo IDE @Dominique Dumont\nVersion : ");
    tempMsg += RATATA_VERSION;

    QMessageBox::about(this, tr("About Ratata"),tempMsg);
 }

 void MDIMainWindow::updateMenus()
 {
     bool hasMdiChild = (activeMdiChild() != 0);
     saveAct->setEnabled(hasMdiChild);
     saveAsAct->setEnabled(hasMdiChild);
     pasteAct->setEnabled(hasMdiChild);
     closeAct->setEnabled(hasMdiChild);
     closeAllAct->setEnabled(hasMdiChild);
     tileAct->setEnabled(hasMdiChild);
     cascadeAct->setEnabled(hasMdiChild);
     nextAct->setEnabled(hasMdiChild);
     previousAct->setEnabled(hasMdiChild);
     separatorAct->setVisible(hasMdiChild);

     bool hasSelection = (activeMdiChild() &&
                          activeMdiChild()->textCursor().hasSelection());
     cutAct->setEnabled(hasSelection);
     copyAct->setEnabled(hasSelection);

     //runAct->setEnabled(!m_projectWidget->currentFile().isEmpty());
     closeProjectAct->setEnabled(!m_projectWidget->currentFile().isEmpty());

     runAct->setEnabled(m_tcpClient->IsConnected());
     stepAct->setEnabled(m_tcpClient->IsConnected());
     stepOutAct->setEnabled(m_tcpClient->IsConnected());
     stepOverAct->setEnabled(m_tcpClient->IsConnected());
     stopEngineAct->setEnabled(m_tcpClient->IsConnected());
     restartEngineAct->setEnabled(m_tcpClient->IsConnected());

     if (m_tcpClient->IsConnected())
         {
         connectDisconnectAct->setIcon(QIcon(":/images/disconnect.png"));
         connectDisconnectAct->setText(tr("&Disconnect"));
         connectDisconnectAct->setStatusTip(tr("DisConnect from a T.R.P. session"));
         }
     else
         {
         connectDisconnectAct->setIcon(QIcon(":/images/connect.png"));
         connectDisconnectAct->setText(tr("&Connect"));
         connectDisconnectAct->setStatusTip(tr("Connect ratata to a T.R.P. session"));
         }
 }

 void MDIMainWindow::updateWindowMenu()
 {
     windowMenu->clear();
     windowMenu->addAction(closeAct);
     windowMenu->addAction(closeAllAct);
     windowMenu->addSeparator();
     windowMenu->addAction(tileAct);
     windowMenu->addAction(cascadeAct);
     windowMenu->addSeparator();
     windowMenu->addAction(nextAct);
     windowMenu->addAction(previousAct);
     windowMenu->addAction(separatorAct);

     QList<QMdiSubWindow *> windows = mdiArea->subWindowList();
     separatorAct->setVisible(!windows.isEmpty());

     for (int i = 0; i < windows.size(); ++i) {
         MdiChild *child = qobject_cast<MdiChild *>(windows.at(i)->widget());

         QString text;
         if (i < 9) {
             text = tr("&%1 %2").arg(i + 1)
                                .arg(child->userFriendlyCurrentFile());
         } else {
             text = tr("%1 %2").arg(i + 1)
                               .arg(child->userFriendlyCurrentFile());
         }
         QAction *action  = windowMenu->addAction(text);
         action->setCheckable(true);
         action ->setChecked(child == activeMdiChild());
         connect(action, SIGNAL(triggered()), windowMapper, SLOT(map()));
         windowMapper->setMapping(action, windows.at(i));
     }
 }

 MdiChild *MDIMainWindow::createMdiChild()
 {
     MdiChild *child = new MdiChild(this);
     mdiArea->addSubWindow(child);

     connect(child, SIGNAL(copyAvailable(bool)),
             cutAct, SLOT(setEnabled(bool)));
     connect(child, SIGNAL(copyAvailable(bool)),
             copyAct, SLOT(setEnabled(bool)));

     return child;
 }

 void MDIMainWindow::createActions()
 {
    newFileAct = new QAction(QIcon(":/images/new.png"), tr("File ..."), this);
    //TODODOMDOM newFileAct->setShortcut(QKeySequence::New);
    newFileAct->setShortcut(tr("Ctrl+N"));
    newFileAct->setStatusTip(tr("Create a new file"));
    connect(newFileAct, SIGNAL(triggered()), this, SLOT(newFile()));

    newProjectAct = new QAction(QIcon(":/images/new_project.png"), tr("Project ..."), this);
    newProjectAct->setShortcut(Qt::CTRL + Qt::SHIFT + Qt::Key_N);
    //TODODOMDOM newProjectAct->setShortcuts(QKeySequence::New);
    newProjectAct->setStatusTip(tr("Create a new project"));
    connect(newProjectAct, SIGNAL(triggered()), this, SLOT(newProject()));


    openFileAct = new QAction(QIcon(":/images/open.png"), tr("File ..."), this);
    //TODODOMDOM openFileAct->setShortcuts(QKeySequence::Open);
    openFileAct->setStatusTip(tr("Open an existing file"));
    connect(openFileAct, SIGNAL(triggered()), this, SLOT(open()));

    openProjectAct = new QAction(QIcon(":/images/open_project.png"), tr("Project ..."), this);
    //TODODOMDOM openProjectAct->setShortcuts(QKeySequence::Open);
    openProjectAct->setStatusTip(tr("Open an existing project"));
    connect(openProjectAct, SIGNAL(triggered()), this, SLOT(openProject()));

     saveAct = new QAction(QIcon(":/images/save.png"), tr("&Save"), this);
     saveAct->setShortcuts(QKeySequence::Save);
     saveAct->setStatusTip(tr("Save the document to disk"));
     connect(saveAct, SIGNAL(triggered()), this, SLOT(save()));

     saveAsAct = new QAction(tr("Save &As..."), this);
     saveAsAct->setShortcuts(QKeySequence::SaveAs);
     saveAsAct->setStatusTip(tr("Save the document under a new name"));
     connect(saveAsAct, SIGNAL(triggered()), this, SLOT(saveAs()));

     exitAct = new QAction(tr("E&xit"), this);
     exitAct->setShortcuts(QKeySequence::Quit);
     exitAct->setStatusTip(tr("Exit the application"));
     connect(exitAct, SIGNAL(triggered()), qApp, SLOT(closeAllWindows()));

     cutAct = new QAction(QIcon(":/images/cut.png"), tr("Cu&t"), this);
     cutAct->setShortcuts(QKeySequence::Cut);
     cutAct->setStatusTip(tr("Cut the current selection's contents to the "
                             "clipboard"));
     connect(cutAct, SIGNAL(triggered()), this, SLOT(cut()));

     copyAct = new QAction(QIcon(":/images/copy.png"), tr("&Copy"), this);
     copyAct->setShortcuts(QKeySequence::Copy);
     copyAct->setStatusTip(tr("Copy the current selection's contents to the "
                              "clipboard"));
     connect(copyAct, SIGNAL(triggered()), this, SLOT(copy()));

     pasteAct = new QAction(QIcon(":/images/paste.png"), tr("&Paste"), this);
     pasteAct->setShortcuts(QKeySequence::Paste);
     pasteAct->setStatusTip(tr("Paste the clipboard's contents into the current "
                               "selection"));
     connect(pasteAct, SIGNAL(triggered()), this, SLOT(paste()));

     closeAct = new QAction(tr("Cl&ose"), this);
     closeAct->setStatusTip(tr("Close the active window"));
     connect(closeAct, SIGNAL(triggered()),
             mdiArea, SLOT(closeActiveSubWindow()));

     closeProjectAct = new QAction(tr("Close Project"), this);
     closeProjectAct->setStatusTip(tr("Close the active project"));
     connect(closeProjectAct, SIGNAL(triggered()),
             this, SLOT(closeProject()));

     closeAllAct = new QAction(tr("Close &All"), this);
     closeAllAct->setStatusTip(tr("Close all the windows"));
     connect(closeAllAct, SIGNAL(triggered()),
             mdiArea, SLOT(closeAllSubWindows()));

     tileAct = new QAction(tr("&Tile"), this);
     tileAct->setStatusTip(tr("Tile the windows"));
     connect(tileAct, SIGNAL(triggered()), mdiArea, SLOT(tileSubWindows()));

     cascadeAct = new QAction(tr("&Cascade"), this);
     cascadeAct->setStatusTip(tr("Cascade the windows"));
     connect(cascadeAct, SIGNAL(triggered()), mdiArea, SLOT(cascadeSubWindows()));

     nextAct = new QAction(tr("Ne&xt"), this);
     nextAct->setShortcuts(QKeySequence::NextChild);
     nextAct->setStatusTip(tr("Move the focus to the next window"));
     connect(nextAct, SIGNAL(triggered()),
             mdiArea, SLOT(activateNextSubWindow()));

     previousAct = new QAction(tr("Pre&vious"), this);
     previousAct->setShortcuts(QKeySequence::PreviousChild);
     previousAct->setStatusTip(tr("Move the focus to the previous "
                                  "window"));
     connect(previousAct, SIGNAL(triggered()),
             mdiArea, SLOT(activatePreviousSubWindow()));

     separatorAct = new QAction(this);
     separatorAct->setSeparator(true);

     aboutAct = new QAction(tr("&About"), this);
     aboutAct->setStatusTip(tr("Show the application's About box"));
     connect(aboutAct, SIGNAL(triggered()), this, SLOT(about()));

    toggleBreakpointAct = new QAction(tr("&Toggle Breakpoint"), this);
    toggleBreakpointAct->setShortcut(tr("F9"));
    toggleBreakpointAct->setStatusTip(tr("Toggle Breakpoint"));
    connect(toggleBreakpointAct, SIGNAL(triggered()),this, SLOT(toggleBreakpoint()));

    runAct = new QAction(QIcon(":/images/debug.png"),tr("&Run"), this);
    runAct->setShortcut(tr("F5"));
    runAct->setStatusTip(tr("Run"));
    connect(runAct, SIGNAL(triggered()),this, SLOT(run()));


    connectDisconnectAct = new QAction(QIcon(":/images/connect.png"),tr("&Connect"), this);
    connectDisconnectAct->setStatusTip(tr("Connect ratata to a T.R.P. session"));
    connect(connectDisconnectAct, SIGNAL(triggered()),this, SLOT(connectDisconnect()));


    stepAct = new QAction(QIcon(":/images/step.png"),tr("&Step In"), this);
    stepAct->setShortcut(tr("F11"));
    stepAct->setStatusTip(tr("Step In"));
    connect(stepAct, SIGNAL(triggered()),this, SLOT(doStep()));

    stepOverAct = new QAction(QIcon(":/images/arrow-step-over.png"),tr("&Step Over"), this);
    stepOverAct->setShortcut(tr("F10"));
    stepOverAct->setStatusTip(tr("Step Over"));
    connect(stepOverAct, SIGNAL(triggered()),this, SLOT(doStepOver()));

    stepOutAct = new QAction(QIcon(":/images/arrow-step-out.png"),tr("&Step Out"), this);
    stepOutAct->setShortcut(tr("F12"));
    stepOutAct->setStatusTip(tr("Step Out"));
    connect(stepOutAct, SIGNAL(triggered()),this, SLOT(doStepOut()));


    restartEngineAct = new QAction(QIcon(":/images/arrow-refresh.png"),tr("&Restart Engine"), this);
    restartEngineAct->setShortcut(tr("F6"));
    restartEngineAct->setStatusTip(tr("Restart Engine"));
    connect(restartEngineAct, SIGNAL(triggered()),this, SLOT(restartEngine()));


    stopEngineAct = new QAction(QIcon(":/images/stop.png"),tr("Stop Engine"), this);
    stopEngineAct->setShortcut(tr("ctrl+F5"));
    stopEngineAct->setStatusTip(tr("Stop Engine"));
    connect(stopEngineAct, SIGNAL(triggered()),this, SLOT(stopEngine()));


    spawnRototoAct = new QAction(QIcon(":/images/television.png"),tr("Launch Rototo"), this);
    spawnRototoAct->setStatusTip(tr("Launch Rototo"));
    connect(spawnRototoAct, SIGNAL(triggered()),this, SLOT(launchRototo()));


    prefDialogAct = new QAction(tr("Preferences"), this);
    prefDialogAct->setStatusTip(tr("Preferences"));
    connect(prefDialogAct, SIGNAL(triggered()),this, SLOT(ShowPrefDialog()));

    for (int i = 0; i < MaxRecentFiles; ++i)
        {
        recentFileActions[i] = new QAction(this);
        recentFileActions[i]->setVisible(false);
        connect(recentFileActions[i], SIGNAL(triggered()),
        this, SLOT(openRecentFile()));
        }

    for (int i = 0; i < MaxRecentProjects; ++i)
        {
        recentProjectActions[i] = new QAction(this);
        recentProjectActions[i]->setVisible(false);
        connect(recentProjectActions[i], SIGNAL(triggered()),
        this, SLOT(openRecentProject()));
        }

 }

 void MDIMainWindow::launchRototo()
 {
     QProcess *process = new QProcess();
     QString workingDir = QFileInfo(this->m_rototoPath).dir().absolutePath()+"/";
     process->setWorkingDirectory(workingDir);
     process->startDetached("open "+ this->m_rototoPath);

 }

 void MDIMainWindow::closeProject()
 {
     m_projectWidget->closeProject();
     updateMenus(); //TODODOMDOM Should not be here
 }
 void MDIMainWindow::createMenus()
 {
    fileMenu = menuBar()->addMenu(tr("&File"));
    QMenu *tempMenu;

    tempMenu = fileMenu->addMenu(tr("&New"));
    tempMenu->addAction(newProjectAct);
    tempMenu->addAction(newFileAct);

    tempMenu = fileMenu->addMenu(tr("&Open"));
    tempMenu->addAction(openProjectAct);
    tempMenu->addAction(openFileAct);


    fileMenu->addAction(saveAct);
    fileMenu->addAction(saveAsAct);
    fileMenu->addSeparator();
    fileMenu->addAction(closeAct);
    fileMenu->addAction(closeProjectAct);

    separatorAction = fileMenu->addSeparator();
    for (int i = 0; i < MaxRecentFiles; ++i)
        fileMenu->addAction(recentFileActions[i]);

    separatorAction2 = fileMenu->addSeparator();
    for (int i = 0; i < MaxRecentProjects; ++i)
        fileMenu->addAction(recentProjectActions[i]);


     fileMenu->addSeparator();
     fileMenu->addAction(exitAct);

    editMenu = menuBar()->addMenu(tr("&Edit"));
    editMenu->addAction(cutAct);
    editMenu->addAction(copyAct);
    editMenu->addAction(pasteAct);
    editMenu->addSeparator();
    editMenu->addAction(prefDialogAct);

    debugMenu = menuBar()->addMenu(tr("&Debug"));
    debugMenu->addAction(spawnRototoAct);
    debugMenu->addAction(connectDisconnectAct);    
    debugMenu->addAction(runAct);
    debugMenu->addAction(stepAct);
    debugMenu->addAction(stepOverAct);
    debugMenu->addAction(stepOutAct);
    debugMenu->addAction(stopEngineAct);
    debugMenu->addAction(restartEngineAct);

    viewMenu = menuBar()->addMenu(tr("&View"));

    windowMenu = menuBar()->addMenu(tr("&Window"));
    updateWindowMenu();
    connect(windowMenu, SIGNAL(aboutToShow()), this, SLOT(updateWindowMenu()));

    menuBar()->addSeparator();



    helpMenu = menuBar()->addMenu(tr("&Help"));
    helpMenu->addAction(aboutAct);


 }

 void MDIMainWindow::createToolBars()
 {
     fileToolBar = addToolBar(tr("File"));
     fileToolBar->setObjectName(tr("FileToolBar"));
     fileToolBar->addAction(newFileAct);
     fileToolBar->addAction(openFileAct);
     fileToolBar->addAction(saveAct);

     editToolBar = addToolBar(tr("Edit"));
     editToolBar->setObjectName(tr("EditToolBar"));
     editToolBar->addAction(cutAct);
     editToolBar->addAction(copyAct);
     editToolBar->addAction(pasteAct);

     debugToolBar = addToolBar(tr("Debug"));
     debugToolBar->setObjectName(tr("DebugToolBar"));
     debugToolBar->addAction(spawnRototoAct);
     debugToolBar->addAction(connectDisconnectAct);
     debugToolBar->addAction(runAct);
     debugToolBar->addAction(stepAct);
     debugToolBar->addAction(stepOverAct);
     debugToolBar->addAction(stepOutAct);
     debugToolBar->addAction(stopEngineAct);
     debugToolBar->addAction(restartEngineAct);

 }

 void MDIMainWindow::createStatusBar()
 {
     statusBar()->showMessage(tr("Ready"));
 }

 void MDIMainWindow::readSettings()
 {
    QSettings settings("Deliriom", "Ratata");
    QPoint pos = settings.value("pos", QPoint(200, 200)).toPoint();
    QSize size = settings.value("size", QSize(400, 400)).toSize();
    move(pos);
    resize(size);
    recentFiles = settings.value("recentFiles").toStringList();
    updateRecentFileActions();
    recentProjects = settings.value("recentProjects").toStringList();
    updateRecentProjectActions();

    this->restoreState(settings.value("state").toByteArray());

    QString tempString =   settings.value("fonte","").toString();
    if (m_globalFont.fromString(tempString) == false)
        {
            qDebug()<<"Cannot retrieve font from settings";
        }

    m_rototoPath =   settings.value("rototo","").toString();
    if (m_rototoPath == "")
        {
            m_rototoPath = "./Rototo.app";
            qDebug()<<"Cannot retrieve rototo from settings";
        }


 }

 void MDIMainWindow::writeSettings()
 {
     QSettings settings("Deliriom", "Ratata");
     settings.setValue("pos", pos());
     settings.setValue("size", size());
     settings.setValue("recentFiles", recentFiles);
     settings.setValue("recentProjects", recentProjects);
     settings.setValue("state", this->saveState());
     settings.setValue("fonte", this->m_globalFont.toString());
     settings.setValue("rototo", this->m_rototoPath);


 }

 MdiChild *MDIMainWindow::activeMdiChild()
 {
     if (QMdiSubWindow *activeSubWindow = mdiArea->activeSubWindow())
         return qobject_cast<MdiChild *>(activeSubWindow->widget());
     return 0;
 }

 QMdiSubWindow *MDIMainWindow::findMdiChild(const QString &fileName)
 {
     QString canonicalFilePath = QFileInfo(fileName).canonicalFilePath();

     foreach (QMdiSubWindow *window, mdiArea->subWindowList()) {
         MdiChild *mdiChild = qobject_cast<MdiChild *>(window->widget());
         if (mdiChild->currentFile().toLower() == canonicalFilePath.toLower())
             return window;
     }
     return 0;
 }

 void MDIMainWindow::switchLayoutDirection()
 {
     if (layoutDirection() == Qt::LeftToRight)
         qApp->setLayoutDirection(Qt::RightToLeft);
     else
         qApp->setLayoutDirection(Qt::LeftToRight);
 }

 void MDIMainWindow::setActiveSubWindow(QWidget *window)
 {
     if (!window)
         return;
     mdiArea->setActiveSubWindow(qobject_cast<QMdiSubWindow *>(window));
 }


 void MDIMainWindow::stopEngine()
{
 QString command;
 command = QString("a");
 this->m_tcpClient->SendText(command);

}

 void MDIMainWindow::restartEngine()
 {
     QString command;
     command = QString("f");
     this->m_tcpClient->SendText(command);
 }

void MDIMainWindow::doStep()
{
    QString command;
    command = QString("s");
    this->m_tcpClient->SendText(command);
}

void MDIMainWindow::doStepOver()
{
    QString command;
    command = QString("n");
    this->m_tcpClient->SendText(command);
}

void MDIMainWindow::doStepOut()
{
    QString command;
    command = QString("o");
    this->m_tcpClient->SendText(command);
}

void MDIMainWindow::toggleBreakpoint()
{

}

void MDIMainWindow::connectDisconnect()
{
if (connectDisconnectAct->text() == "&Connect")
    {
    m_tcpClient->ConnectTo("127.0.0.1");
    }
else
    {
    m_tcpClient->DisConnect();
    }
}

void MDIMainWindow::run()
{
    QString command;
    command = QString("c");
    this->m_tcpClient->SendText(command);
}

void MDIMainWindow::ShowPrefDialog()
{
prefDlg->SetFont(m_globalFont);
prefDlg->SetRototoPath(m_rototoPath);
prefDlg->exec();

m_globalFont = prefDlg->m_font;
m_rototoPath = prefDlg->m_rototoPath;

//Now update all opened child windows.
foreach (QMdiSubWindow *window, mdiArea->subWindowList())
{
    MdiChild *mdiChild = qobject_cast<MdiChild *>(window->widget());
    mdiChild->setFont(m_globalFont);

}
}




QAbstractItemModel *MDIMainWindow::modelFromFile(const QString& fileName)
 {
     QFile file(fileName);
     if (!file.open(QFile::ReadOnly))
         return new QStringListModel(completer);

 #ifndef QT_NO_CURSOR
     QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
 #endif
     QStringList words;

     while (!file.atEnd()) {
         QByteArray line = file.readLine();
         if (!line.isEmpty())
             words << line.trimmed();
     }

 #ifndef QT_NO_CURSOR
     QApplication::restoreOverrideCursor();
 #endif
     return new QStringListModel(words, completer);
 }
