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

#include "mdichild.h"

#include "breakpoint.h"

//Completion includes
#include <QCompleter>
#include <QKeyEvent>

#include "highlighter.h"
#include "mdimainwindow.h"
#include "tcpclient.h"

MdiChild::MdiChild(QWidget *parent) : QPlainTextEdit(parent), c(0)
    {

    m_mainWindow = qobject_cast<MDIMainWindow*>(this->parentWidget());
    Q_ASSERT(m_mainWindow != NULL);
     setAttribute(Qt::WA_DeleteOnClose);
     isUntitled = true;

     lineNumberArea = new LineNumberAreaMDI(this);

     connect(this, SIGNAL(blockCountChanged(int)), this, SLOT(updateLineNumberAreaWidth(int)));
     connect(this, SIGNAL(updateRequest(QRect,int)), this, SLOT(updateLineNumberArea(QRect,int)));
     connect(this, SIGNAL(cursorPositionChanged()), this, SLOT(highlightCurrentLine()));



     updateLineNumberAreaWidth(0);
     highlightCurrentLine();

     highlighter = new Highlighter(this->document());

     setTabStopWidth( fontMetrics().width( " " ) * 4); //TODODOMDOM should do this also when font changed ?

     latestTextToFind = "";
    }

void MdiChild::findBackwardCurrentWord()
{
latestTextToFind = textUnderCursor();
 qDebug()<<latestTextToFind;
 bool found = this->find(latestTextToFind,QTextDocument::FindBackward);
 if (found == true)
    {
    qDebug()<<"found";
    }
}
 void MdiChild::findForwardCurrentWord()
 {
latestTextToFind = textUnderCursor();
 qDebug()<<latestTextToFind;
 bool found = this->find(latestTextToFind,0);
 if (found == true)
    {
    qDebug()<<"found";
    }
 }
 int MdiChild::breakpointAreaWidth()
 {
     return fontMetrics().height();
 }

 int MdiChild::lineNumberAreaWidth()
 {
     int digits = 1; // it was 1, add 1 for breakpoints
     int max = qMax(1, blockCount());
     while (max >= 10) {
         max /= 10;
         ++digits;
     }

     int space = 3 + fontMetrics().width(QLatin1Char('9')) * digits;

     return space + breakpointAreaWidth();
 }
 void MdiChild::newFile()
 {
     static int sequenceNumber = 1;

     isUntitled = true;
     curFile = tr("document%1.txt").arg(sequenceNumber++);
     setWindowTitle(curFile + "[*]");

     connect(document(), SIGNAL(contentsChanged()),
             this, SLOT(documentWasModified()));
 }

 bool MdiChild::loadFile(const QString &fileName)
 {
     QFile file(fileName);
     if (!file.open(QFile::ReadOnly | QFile::Text)) {
         QMessageBox::warning(this, tr("MDI"),
                              tr("Cannot read file %1:\n%2.")
                              .arg(fileName)
                              .arg(file.errorString()));
         return false;
     }

     QTextStream in(&file);
     QApplication::setOverrideCursor(Qt::WaitCursor);
     setPlainText(in.readAll());
     QApplication::restoreOverrideCursor();

     setCurrentFile(fileName);

     connect(document(), SIGNAL(contentsChanged()),
             this, SLOT(documentWasModified()));

     return true;
 }

 bool MdiChild::save()
 {
     if (isUntitled) {
         return saveAs();
     } else {
         return saveFile(curFile);
     }
 }

 bool MdiChild::saveAs()
 {
     QString fileName = QFileDialog::getSaveFileName(this, tr("Save As"),
                                                     curFile);
     if (fileName.isEmpty())
         return false;

     return saveFile(fileName);
 }

 bool MdiChild::saveFile(const QString &fileName)
 {
     QFile file(fileName);
     if (!file.open(QFile::WriteOnly | QFile::Text)) {
         QMessageBox::warning(this, tr("MDI"),
                              tr("Cannot write file %1:\n%2.")
                              .arg(fileName)
                              .arg(file.errorString()));
         return false;
     }

     QTextStream out(&file);
     QApplication::setOverrideCursor(Qt::WaitCursor);
     out << toPlainText();
     QApplication::restoreOverrideCursor();

     setCurrentFile(fileName);
     return true;
 }

 QString MdiChild::userFriendlyCurrentFile()
 {
     return strippedName(curFile);
 }

 void MdiChild::closeEvent(QCloseEvent *event)
 {
     if (maybeSave()) {
         event->accept();
     } else {
         event->ignore();
     }
 }

 void MdiChild::documentWasModified()
 {
     setWindowModified(document()->isModified());
 }

 bool MdiChild::maybeSave()
 {
     if (document()->isModified()) {
         QMessageBox::StandardButton ret;
         ret = QMessageBox::warning(this, tr("MDI"),
                      tr("'%1' has been modified.\n"
                         "Do you want to save your changes?")
                      .arg(userFriendlyCurrentFile()),
                      QMessageBox::Save | QMessageBox::Discard
                      | QMessageBox::Cancel);
         if (ret == QMessageBox::Save)
             return save();
         else if (ret == QMessageBox::Cancel)
             return false;
     }
     return true;
 }

 void MdiChild::setCurrentFile(const QString &fileName)
 {
     curFile = QFileInfo(fileName).canonicalFilePath();
     isUntitled = false;
     document()->setModified(false);
     setWindowModified(false);
     setWindowTitle(userFriendlyCurrentFile() + "[*]");
 }

 QString MdiChild::strippedName(const QString &fullFileName)
 {
     return QFileInfo(fullFileName).fileName();
 }


 //ADDED From CodeEditor

 void MdiChild::updateLineNumberAreaWidth(int /* newBlockCount */)
 {
     setViewportMargins(lineNumberAreaWidth(), 0, 0, 0);
 }



 void MdiChild::updateLineNumberArea(const QRect &rect, int dy)
 {
     if (dy)
         lineNumberArea->scroll(0, dy);
     else
         lineNumberArea->update(0, rect.y(), lineNumberArea->width(), rect.height());

     if (rect.contains(viewport()->rect()))
         updateLineNumberAreaWidth(0);
 }



 void MdiChild::resizeEvent(QResizeEvent *e)
 {
     QPlainTextEdit::resizeEvent(e);

     QRect cr = contentsRect();
     lineNumberArea->setGeometry(QRect(cr.left(), cr.top(), lineNumberAreaWidth(), cr.height()));
 }



 void MdiChild::highlightCurrentLine()
 {
     QList<QTextEdit::ExtraSelection> extraSelections;

     if (!isReadOnly()) {
         QTextEdit::ExtraSelection selection;

         QColor lineColor = QColor(Qt::yellow).lighter(160);

         selection.format.setBackground(lineColor);
         selection.format.setProperty(QTextFormat::FullWidthSelection, true);
         selection.cursor = textCursor();
         selection.cursor.clearSelection();
         extraSelections.append(selection);
     }

     setExtraSelections(extraSelections);
 }

 void MdiChild::highlightLine(int _lineNumber)
 {
     QList<QTextEdit::ExtraSelection> extraSelections;

     if (!isReadOnly())
        {

        QTextCursor textCur = textCursor();
        textCur.movePosition(QTextCursor::Start, QTextCursor::MoveAnchor);
        for(int i=1;i<_lineNumber;i++)
            {
            textCur.movePosition(QTextCursor::NextBlock);
            }



         QTextEdit::ExtraSelection selection;

         QColor lineColor = QColor(Qt::red).lighter(160);

         selection.format.setBackground(lineColor);
         selection.format.setProperty(QTextFormat::FullWidthSelection, true);
         selection.cursor = textCur;
         selection.cursor.clearSelection();
         extraSelections.append(selection);

        //NEW AND DANGEROUS, TRY TO DO AN ENSUREVISIBLE
        setTextCursor(textCur);
        ensureCursorVisible();

        }

     setExtraSelections(extraSelections);
 }



 void MdiChild::lineNumberAreaPaintEvent(QPaintEvent *event)
 {
     QPainter painter(lineNumberArea);
     painter.fillRect(event->rect(), Qt::lightGray);


     QTextBlock block = firstVisibleBlock();
     int blockNumber = block.blockNumber();
     int top = (int) blockBoundingGeometry(block).translated(contentOffset()).top();
     int bottom = top + (int) blockBoundingRect(block).height();

     while (block.isValid() && top <= event->rect().bottom()) {
         if (block.isVisible() && bottom >= event->rect().top()) {
             QString number = QString::number(blockNumber + 1);
             painter.setPen(Qt::black);
             painter.drawText(0, top, lineNumberArea->width(), fontMetrics().height(),
                              Qt::AlignRight, number);

             if (this->m_mainWindow->m_breakpointManager->IsBreakpointSet(strippedName(currentFile()),blockNumber +1))
                {
                QBrush tempBrush = painter.brush();
                QPen tempPen = painter.pen();



                painter.setBrush( QBrush( Qt::red ) );
                painter.setPen( Qt::NoPen );
                //painter.drawText(0, top, lineNumberArea->width(), fontMetrics().height(),Qt::AlignLeft, "X");
                QRectF rectangle(0, top, fontMetrics().height(), fontMetrics().height());
                painter.drawEllipse(rectangle);
                //Restore old brush and pen
                painter.setBrush( tempBrush );
                painter.setPen( tempPen );
                }
         }

         block = block.next();
         top = bottom;
         bottom = top + (int) blockBoundingRect(block).height();
         ++blockNumber;
     }
 }


 //Completion

  void MdiChild::setCompleter(QCompleter *completer)
 {
     if (c)
         QObject::disconnect(c, 0, this, 0);

     c = completer;

     if (!c)
         return;

     c->setWidget(this);
     c->setCompletionMode(QCompleter::PopupCompletion);
     c->setCaseSensitivity(Qt::CaseInsensitive);
     QObject::connect(c, SIGNAL(activated(QString)),
                      this, SLOT(insertCompletion(QString)));
 }

 QCompleter *MdiChild::completer() const
 {
     return c;
 }

 void MdiChild::insertCompletion(const QString& completion)
 {
     if (c->widget() != this)
         return;
     QTextCursor tc = textCursor();
     /* TEST DOMDOM
     int extra = completion.length() - c->completionPrefix().length();
     tc.movePosition(QTextCursor::Left);
     tc.movePosition(QTextCursor::EndOfWord);
     tc.insertText(completion.right(extra));
     */
     int extra = completion.length();
     //tc.movePosition(QTextCursor::Left);
     tc.movePosition(QTextCursor::StartOfWord,QTextCursor::KeepAnchor);
     tc.insertText(completion.right(extra));
     setTextCursor(tc);
 }

 QString MdiChild::textUnderCursor() const
 {
     QTextCursor tc = textCursor();
     tc.select(QTextCursor::WordUnderCursor);
     return tc.selectedText();
 }

 void MdiChild::focusInEvent(QFocusEvent *e)
 {
     if (c)
         c->setWidget(this);
     QPlainTextEdit::focusInEvent(e);
 }

 void MdiChild::keyPressEvent(QKeyEvent *e)
 {
     if (c && c->popup()->isVisible())
         {
         // The following keys are forwarded by the completer to the widget
        switch (e->key())
            {
            case Qt::Key_Enter:
            case Qt::Key_Return:
            case Qt::Key_Escape:
            case Qt::Key_Tab:
            case Qt::Key_Backtab:
                 e->ignore();
                 return; // let the completer do default behavior
            default:
                break;
            }
     }

     bool ctrlf3Shortcut =  (! (e->modifiers() & Qt::ShiftModifier) &&(e->modifiers() & Qt::ControlModifier) && e->key() == Qt::Key_F3); // CTRL+F3
     if (ctrlf3Shortcut)
         {
         findForwardCurrentWord();
         return;
         }
    bool shiftCtrlf3Shortcut =  ((e->modifiers() & Qt::ShiftModifier) && (e->modifiers() & Qt::ControlModifier) && e->key() == Qt::Key_F3); // CTRL+F3
     if (shiftCtrlf3Shortcut)
         {
         findBackwardCurrentWord();
         return;
         }
     bool f3Only =  (!(e->modifiers() & Qt::ShiftModifier) && !(e->modifiers() & Qt::ControlModifier) && e->key() == Qt::Key_F3); // F3
     if ( f3Only)
         {
         this->find(latestTextToFind,0);
         }
     bool shiftF3Only =  ((e->modifiers() & Qt::ShiftModifier) && !(e->modifiers() & Qt::ControlModifier) && e->key() == Qt::Key_F3); // F3
     if ( shiftF3Only)
         {
         this->find(latestTextToFind,QTextDocument::FindBackward);
         }

     bool isShortcut = ((e->modifiers() & Qt::ControlModifier) && e->key() == Qt::Key_Space); // CTRL+SPACE
     //qDebug()<<"IsShorcut = "<<isShortcut;
     if (!c || !isShortcut) // do not process the shortcut when we have a completer
         QPlainTextEdit::keyPressEvent(e);

     const bool ctrlOrShift = e->modifiers() & (Qt::ControlModifier | Qt::ShiftModifier);
     if (!c || (ctrlOrShift && e->text().isEmpty()))
         return;

     static QString eow("~!@#$%^&*()_+{}|:\"<>?,./;'[]\\-="); // end of word
     bool hasModifier = (e->modifiers() != Qt::NoModifier) && !ctrlOrShift;
     QString completionPrefix = textUnderCursor();

     if (!isShortcut && (hasModifier || e->text().isEmpty()|| completionPrefix.length() < 3
                       || eow.contains(e->text().right(1)))) {
         c->popup()->hide();
         return;
     }

     if (completionPrefix != c->completionPrefix()) {
         c->setCompletionPrefix(completionPrefix);
         c->popup()->setCurrentIndex(c->completionModel()->index(0, 0));
     }
     QRect cr = cursorRect();
     cr.setWidth(c->popup()->sizeHintForColumn(0)
                 + c->popup()->verticalScrollBar()->sizeHint().width());
     c->complete(cr); // popup it up!
 }
    int MdiChild::getTopLine()
    {
         QTextBlock block = firstVisibleBlock();
     return block.blockNumber()+1;
    }

void LineNumberAreaMDI::mousePressEvent ( QMouseEvent * event )
    {
    QTextCursor cursor = codeEditor->cursorForPosition(event->pos());
    QTextBlock block = cursor.block();
    qDebug()<<block.blockNumber();
    //int tempY = codeEditor->getTopLine() + (event->y()/fontMetrics().height());
    int tempY = block.blockNumber()+1;
    if (codeEditor->m_mainWindow->m_breakpointManager->ToggleBreakpoint(codeEditor->strippedName(codeEditor->currentFile()),tempY) == true)
        {
        QString command;
        command = QString("b %1:%2").arg(codeEditor->strippedName(codeEditor->currentFile())).arg(tempY);
        codeEditor->m_mainWindow->m_tcpClient->SendText(command);
        }
    else
        {
        QString command;
        command = QString("r %1:%2").arg(codeEditor->strippedName(codeEditor->currentFile())).arg(tempY);
        codeEditor->m_mainWindow->m_tcpClient->SendText(command);
        }

    codeEditor->repaint();

    }
