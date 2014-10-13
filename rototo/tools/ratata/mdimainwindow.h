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

#ifndef MDIMAINWINDOW_H
#define MDIMAINWINDOW_H

#include <QMainWindow>

#define RATATA_VERSION "0.2.3"

class MdiChild;
class QAction;
class QMenu;
class QMdiArea;
class QMdiSubWindow;
class QSignalMapper;

//Completion
class QCompleter;
class QAbstractItemModel;
class QTreeWidgetItem;

class ProjectWidget;
class WatchWidget;
class CallstackWidget;
class OutputWidget;
class ConsoleWidget;
class ErrorWidget;
class BreakpointWidget;
class GrepWidget;

class QModelIndex;

class TcpClient;

class BreakpointManager;

class PrefDialog;

class MDIMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MDIMainWindow();
    ~MDIMainWindow();

public:
       bool alive;
       bool breaked;
       bool m_bRunning;

       TcpClient *m_tcpClient;
       BreakpointManager * m_breakpointManager;

       QFont m_globalFont;
       QString m_rototoPath;

       QCompleter *completer;
       QAbstractItemModel *modelFromFile(const QString& fileName);

       ProjectWidget    * m_projectWidget;
       WatchWidget      * m_watchWidget;
       ErrorWidget      * m_errorWidget;
       CallstackWidget  * m_callstackWidget;
       OutputWidget     * m_outputWidget;
       ConsoleWidget    * m_consoleWidget;
       BreakpointWidget * m_breakpointWidget;
       GrepWidget       * m_grepWidget;
       QDockWidget      * m_grepDock;

       void openWithFileName(const QString& fileName,int line = -1);
       MdiChild *activeMdiChild();
       void openProjectFromFilename(QString fileName);
       void openFileFromFilename(QString fileName,int line = -1);


protected:
    void closeEvent(QCloseEvent *event);

public slots:

    void grep();

private slots:

   void newFile();
   void newProject();
   void open();
   void openProject();
   void save();
   void saveAs();
   void cut();
   void copy();
   void paste();
   void about();
   void updateMenus();
   void updateWindowMenu();
   MdiChild *createMdiChild();
   void switchLayoutDirection();
   void setActiveSubWindow(QWidget *window);
   void openRecentFile();
   void openRecentProject();

   void ShowPrefDialog();
   void run();
   void doStep();
   void doStepOver();
   void doStepOut();
   void restartEngine();
   void connectDisconnect();
   void stopEngine();
   void clicSelection(const QModelIndex& I);
   void closeProject();
   void launchRototo();

   void toggleBreakpoint();

   void DebugHook(QString);
   void OnConnectionChanged();
   void OnBreakpointChanged();

   void OnErrorWidgetDoubleClick( QTreeWidgetItem *, int);
private:

   QStringList recentFiles;
   enum { MaxRecentFiles = 5 };
   QAction *recentFileActions[MaxRecentFiles];

   QStringList recentProjects;
   enum { MaxRecentProjects = 5 };
   QAction *recentProjectActions[MaxRecentProjects];
   void updateRecentFileActions();
   void updateRecentProjectActions();
   QString strippedName(const QString &fullFileName);
   QAction *separatorAction;
   QAction *separatorAction2;

   void createActions();
   void createMenus();
   void createToolBars();
   void createStatusBar();
   void createDockWindows();
   void readSettings();
   void writeSettings();

   QMdiSubWindow *findMdiChild(const QString &fileName);

   QMdiArea *mdiArea;
   QSignalMapper *windowMapper;


   PrefDialog *prefDlg;

   QMenu *fileMenu;
   QMenu *editMenu;
   QMenu *windowMenu;
   QMenu *viewMenu;
   QMenu *helpMenu;
   QMenu *debugMenu;

   QToolBar *fileToolBar;
   QToolBar *editToolBar;
   QToolBar *debugToolBar;
   QAction *newFileAct;
   QAction *newProjectAct;
   QAction *openFileAct;
   QAction *openProjectAct;
   QAction *saveAct;
   QAction *saveAsAct;
   QAction *exitAct;
   QAction *cutAct;
   QAction *copyAct;
   QAction *pasteAct;
   QAction *closeAct;
   QAction *closeProjectAct;
   QAction *closeAllAct;
   QAction *tileAct;
   QAction *cascadeAct;
   QAction *nextAct;
   QAction *previousAct;
   QAction *separatorAct;
   QAction *aboutAct;
   QAction *toggleBreakpointAct;
   QAction *runAct;
   QAction *spawnRototoAct;
   QAction *prefDialogAct;

   QAction *stepAct;
   QAction *stepOverAct;
   QAction *stepOutAct;
   QAction *stopEngineAct;
   QAction *restartEngineAct;

   QAction *connectDisconnectAct;
   QAction *grepAct;


};

#endif
