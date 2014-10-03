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

#ifndef MDICHILD_H
#define MDICHILD_H

#include <QPlainTextEdit>
#include <QObject>

class QPaintEvent;
class QResizeEvent;
class QSize;
class QWidget;

class LineNumberAreaMDI;
class QCompleter;

class Highlighter;
class MDIMainWindow;
 class MdiChild : public QPlainTextEdit
 {
     Q_OBJECT

 public:
     MdiChild(QWidget *parent = 0);


     QString strippedName(const QString &fullFileName);
     void newFile();
     bool loadFile(const QString &fileName);
     bool save();
     bool saveAs();
     bool saveFile(const QString &fileName);
     QString userFriendlyCurrentFile();
     QString currentFile() { return curFile; }
     void SetTabWidth();
     void IncreaseSelectionIndent();
     void DecreaseSelectionIndent();

 protected:
     void closeEvent(QCloseEvent *event);

 private slots:
     void documentWasModified();
     void findForwardCurrentWord();
     void findBackwardCurrentWord();

 private:
     bool maybeSave();
     void setCurrentFile(const QString &fileName);


     QString curFile;
     bool isUntitled;
      QString latestTextToFind;



public:

    Highlighter *highlighter;
    MDIMainWindow *m_mainWindow;

    void lineNumberAreaPaintEvent(QPaintEvent *event);
    int lineNumberAreaWidth();
    int breakpointAreaWidth();
    void highlightLine(int _lineNumber);
    int getTopLine();


protected:
     void resizeEvent(QResizeEvent *event);

 private slots:
     void updateLineNumberAreaWidth(int newBlockCount);
     void highlightCurrentLine();

     void updateLineNumberArea(const QRect &, int);

 private:
     QWidget *lineNumberArea;

//Completion
 public:
     void setCompleter(QCompleter *c);
     QCompleter *completer() const;

 protected:
     void keyPressEvent(QKeyEvent *e);
     void focusInEvent(QFocusEvent *e);

 private slots:
     void insertCompletion(const QString &completion);

 private:
     QString textUnderCursor() const;
 private:
     QCompleter *c;

 };


 class LineNumberAreaMDI : public QWidget
 {
 public:
     LineNumberAreaMDI(MdiChild *editor) : QWidget(editor) {
         codeEditor = editor;
     }

     QSize sizeHint() const {
         return QSize(codeEditor->lineNumberAreaWidth(), 0);
     }

 protected:
     void paintEvent(QPaintEvent *event) {
         codeEditor->lineNumberAreaPaintEvent(event);
     }

     void mousePressEvent ( QMouseEvent * event ) ;
 private:
     MdiChild *codeEditor;
 };
 #endif
