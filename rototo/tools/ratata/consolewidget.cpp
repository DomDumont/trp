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

#include "consolewidget.h"

#include <QApplication>
#include <QKeyEvent>


/**
 * returns a common word of the given list
 *
 * @param list String list
 *
 * @return common word in the given string.
 */
static
QString getCommonWord(QStringList& list)
{
        QChar ch;
        QVector<QString> strarray = list.toVector();
        QString common;
        int col = 0,  min_len;
        bool cont = true;

        // get minimum length
        min_len = strarray.at(0).size();
        for (int i=1; i<strarray.size(); ++i) {
                const int len = strarray.at(i).size();
                if (len < min_len)
                        min_len = len;
        }

        while(col < min_len) {
                ch = strarray.at(0)[col];
                for (int i=1; i<strarray.size(); ++i) {
                        const QString& current_string = strarray.at(i);
                        if (ch != current_string[col])
                        {
                                cont = false;
                                break;
                        }
                }
                if (!cont)
                        break;

                common.push_back(ch);
                ++col;
        }
        return common;
}

ConsoleWidget::ConsoleWidget(QWidget *parent, const QString &welcomeText,AppDelegate * _pApp)
        : QTextEdit(parent), errColor_(Qt::red),
        outColor_(Qt::blue), completionColor(Qt::darkGreen),
        promptLength(0), promptParagraph(0)
{
    m_pApp = _pApp;
    QPalette palette = QApplication::palette();
    setCmdColor(palette.text().color());

    //Disable undo/redo
    setUndoRedoEnabled(false);


    //Disable context menu
    //This menu is useful for undo/redo, cut/copy/paste, del, select all,
    // see function ConsoleWidget::contextMenuEvent(...)
    //setContextMenuPolicy(Qt::NoContextMenu);


    //DOMDOM Test
    /*
    prompt = ">";
    promptLength = 1;
    */



    //resets the console
    reset(welcomeText);
        const int tabwidth = QFontMetrics(currentFont()).width('a') * 4;
        setTabStopWidth(tabwidth);
}

void ConsoleWidget::reset(const QString &welcomeText)
{
        clear();
        //set the style of the QTextEdit
#ifdef __APPLE__
        setCurrentFont(QFont("Monaco"));
#else
        QFont f;
        f.setFamily("Courier");
        setCurrentFont(f);
#endif

        append(welcomeText);
        append("");
        //displayPrompt(); //Domdom

        //init attributes
        historyIndex = 0;
        history.clear();
        recordedScript.clear();
}

void ConsoleWidget::displayPrompt()
{
        //Prevent previous text displayed to be undone
        setUndoRedoEnabled(false);
        //displays the prompt
        setTextColor(cmdColor_);
        QTextCursor cur = textCursor();
        cur.insertText(prompt);
        cur.movePosition(QTextCursor::EndOfLine);
        setTextCursor(cur);
        //Saves the paragraph number of the prompt
        promptParagraph = cur.blockNumber();

        //Enable undo/redo for the actual command
        setUndoRedoEnabled(true);
}

QStringList ConsoleWidget::suggestCommand(const QString&, QString& prefix)
{
        prefix = "";
        return QStringList();
}

void ConsoleWidget::mousePressEvent(QMouseEvent* event)
{
    oldPosition = textCursor().position();
        if (event->button() == Qt::MidButton)
        {
                copy();
                QTextCursor cursor = cursorForPosition(event->pos());
                setTextCursor(cursor);
                paste();
                return;
        }

        QTextEdit::mousePressEvent(event);
}


void ConsoleWidget::keyPressEvent( QKeyEvent *e )
{
    //If the user wants to copy or cut outside
    //the editing area we perform a copy
    if(textCursor().hasSelection())
    {
        if(e->modifiers() == Qt::CTRL)
        {
            if( e->matches(QKeySequence::Cut) )
            {
                e->accept();
                if(!isInEditionZone())
                {
                        copy();
                }
                else
                {
                        cut();
                }
                return;
            }
            else if(e->matches(QKeySequence::Copy) )
            {
                e->accept();
                copy();
            }
            else
            {
                QTextEdit::keyPressEvent( e );
                return;
            }
        }
    }
/*
    // if the cursor out of editing zone put it back first
    if(!isInEditionZone())
    {
         QTextCursor editCursor = textCursor();
         editCursor.setPosition(oldEditPosition);
         setTextCursor(editCursor);
    }
*/
    // control is pressed
    if ( (e->modifiers() & Qt::ControlModifier) && (e->key() == Qt::Key_C) )
    {
        if ( isSelectionInEditionZone())
        {
            //If Ctrl + C pressed, then undo the current commant
            //append("");
            //displayPrompt();

            //(Thierry Belair:)I humbly suggest that ctrl+C copies the text, as is expected,
            //and indicated in the contextual menu
            copy();
            return;
        }

    }
    else {
        switch (e->key()) {
        case Qt::Key_Tab:
            if(isSelectionInEditionZone())
            {
                handleTabKeyPress();
            }
            return;

        case Qt::Key_Enter:
        case Qt::Key_Return:
            if (isSelectionInEditionZone())
            {
                    handleReturnKeyPress();
            }
            // ignore return key
            return;

        case Qt::Key_Backspace:
            if (handleBackspaceKeyPress() || !isSelectionInEditionZone())
                return;
            break;

        case Qt::Key_Home:
            setHome(e->modifiers() & Qt::ShiftModifier);
        case Qt::Key_Down:
            if (isInEditionZone())
            {
                    handleDownKeyPress();
            }
            return;
        case Qt::Key_Up:
            if (isInEditionZone())
            {
                     handleUpKeyPress();
            }
            return;

        //Default behaviour
        case Qt::Key_End:
        case Qt::Key_Left:
        case Qt::Key_Right:
            break;

        default:
            if (textCursor().hasSelection() ) {
                if (!isSelectionInEditionZone())
                {
                    moveCursor(QTextCursor::End, QTextCursor::MoveAnchor);
                }
                break;
            }
            else
            { //no selection
                //when typing normal characters,
                //make sure the cursor is positionned in the
                //edition zone
                if ( !isInEditionZone() )
                {
                    moveCursor(QTextCursor::End, QTextCursor::MoveAnchor);
                }
            }
        } //end of switch
    } //end of else : no control pressed

    QTextEdit::keyPressEvent( e );
}

void ConsoleWidget::contextMenuEvent ( QContextMenuEvent * event)
{
        QMenu *menu = new QMenu(this);

        QAction *undo = new QAction(tr("Undo"), this);
        undo->setShortcut(tr("Ctrl+Z"));
        QAction *redo = new QAction(tr("Redo"), this);
        redo->setShortcut(tr("Ctrl+Y"));
        QAction *cut = new QAction(tr("Cut"), this);
        cut->setShortcut(tr("Ctrl+X"));
        QAction *copy = new QAction(tr("Copy"), this);
        copy->setShortcut(tr("Ctrl+Ins"));
        QAction *paste = new QAction(tr("Paste"), this);
        paste->setShortcut(tr("Ctrl+V"));
        QAction *del = new QAction(tr("Delete"), this);
        del->setShortcut(tr("Del"));
        QAction *selectAll = new QAction(tr("Select All"), this);
        selectAll->setShortcut(tr("Ctrl+A"));

        menu->addAction(undo);
        menu->addAction(redo);
        menu->addSeparator();
        menu->addAction(cut);
        menu->addAction(copy);
        menu->addAction(paste);
        menu->addAction(del);
        menu->addSeparator();
        menu->addAction(selectAll);

        connect(undo, SIGNAL(triggered()), this, SLOT(undo()));
        connect(redo, SIGNAL(triggered()), this, SLOT(redo()));
        connect(cut, SIGNAL(triggered()), this, SLOT(cut()));
        connect(copy, SIGNAL(triggered()), this, SLOT(copy()));
        connect(paste, SIGNAL(triggered()), this, SLOT(paste()));
        connect(del, SIGNAL(triggered()), this, SLOT(del()));
        connect(selectAll, SIGNAL(triggered()), this, SLOT(selectAll()));


        menu->exec(event->globalPos());

        delete menu;
}

void ConsoleWidget::dropEvent ( QDropEvent * event)
{
        if(!isInEditionZone())
        {
                //Execute un drop a drop at the old position
                //if the drag started out of the editable zone
                QTextCursor cur = textCursor();
                cur.setPosition(oldPosition);
                setTextCursor(cur);
        }
        //Execute a normal drop
        QTextEdit::dropEvent(event);
}
void ConsoleWidget::del()
{
        //Delete only in the editing zone
        if(isInEditionZone())
        {
                textCursor().movePosition(QTextCursor::EndOfWord, QTextCursor::KeepAnchor);
                textCursor().deleteChar();
        }
}

//default implementation: command always complete
bool ConsoleWidget::isCommandComplete(const QString &)
{
        return true;
}

void ConsoleWidget::clear()
{
     QTextEdit::clear();
}
void ConsoleWidget::setHome(bool select)
{
        QTextCursor cursor = textCursor();
        cursor.movePosition(QTextCursor::StartOfBlock, select ? QTextCursor::KeepAnchor :
                                                                                                                        QTextCursor::MoveAnchor);
        if(textCursor().blockNumber() == promptParagraph)
        {
            cursor.movePosition(QTextCursor::Right, select ? QTextCursor::KeepAnchor :
                                                                                                             QTextCursor::MoveAnchor,
                                                    promptLength);
        }
        setTextCursor(cursor);
}


// If return pressed, do the evaluation and append the result
void ConsoleWidget::handleReturnKeyPress()
{
        //Get the command to validate
    QString command = getCurrentCommand();
    //execute the command and get back its text result and its return value
    if (isCommandComplete(command))
        execCommand(command, false);
    else
    {
        append("");
        moveCursor(QTextCursor::EndOfLine);
    }
}

bool ConsoleWidget::handleBackspaceKeyPress()
{
        QTextCursor cur = textCursor();
        const int col = cur.columnNumber();
        const int blk = cur.blockNumber();
        if (blk == promptParagraph && col == promptLength)
            return true;
        return false;
}

//Treat the tab key & autocomplete the current command
void ConsoleWidget::handleTabKeyPress()
{
        QString command = getCurrentCommand();
        QString commandPrefix;
        QStringList sl = suggestCommand(command, commandPrefix);
        if (sl.count() == 0)
            textCursor().insertText("\t");
        else {
            if (sl.count() == 1)
                replaceCurrentCommand(commandPrefix + sl[0]);
            else
            {
                // common word completion
                QString commonWord = getCommonWord(sl);
                command = commonWord;
#ifdef USE_POPUP_COMPLETER
                PopupCompleter *popup = new PopupCompleter(sl);
                if (popup->exec(this) == QDialog::Accepted)
                        replaceCurrentCommand(commandPrefix + popup->selected());
                delete popup;
#else

                setTextColor(completionColor);
                append(sl.join(", ") + "\n");
                setTextColor(cmdColor());
                displayPrompt();
                textCursor().insertText(commandPrefix + command);
#endif
            }
        }
}
void ConsoleWidget::handleDownKeyPress()
{
        if (history.count())
        {
                QString command = getCurrentCommand();
                do
                {
                        if (++historyIndex >= history.size())
                        {
                                historyIndex = history.size() - 1;
                                break;
                        }
                } while(history[historyIndex] == command);
                replaceCurrentCommand(history[historyIndex]);
        }
}
void ConsoleWidget::handleUpKeyPress()
{
        if (history.count())
        {
                QString command = getCurrentCommand();
                do
                {
                        if (historyIndex)
                                historyIndex--;
                        else
                        {
                                break;
                        }
                } while(history[historyIndex] == command);
                replaceCurrentCommand(history[historyIndex]);
        }
}
//Basically, puts the command into the history list
//And emits a signal (should be called by reimplementations)
QString ConsoleWidget::interpretCommand(const QString &command, int *res)
{
//DOMDOM HERE
QString augmentedCommand = "CCLuaLog(\"CON:\"..("+command+"));";

//m_pApp->ExecuteString(augmentedCommand.toStdString().c_str());


        //Add the command to the recordedScript list
    if (!*res)
        recordedScript.append(command);
    //update the history and its index
    QString modifiedCommand = command;
    modifiedCommand.replace("\n", "\\n");
    history.append(modifiedCommand);
    historyIndex = history.size();
    //emit the commandExecuted signal
    Q_EMIT commandExecuted(modifiedCommand);
    return "";
}
void ConsoleWidget::insertFromMimeData(const QMimeData *source)
{
        if (isSelectionInEditionZone())
        {
                QTextEdit::insertFromMimeData(source);
        }
}

void ConsoleWidget::cut()
{
        //Cut only in the editing zone,
        //perfom a copy otherwise
        if(isInEditionZone())
        {
                QTextEdit::cut();
                return;
        }

        QTextEdit::copy();
}

void ConsoleWidget::dragMoveEvent( QDragMoveEvent * event)
{
        //Get a cursor for the actual mouse position
        QTextCursor cur = textCursor();
        cur.setPosition(cursorForPosition(event->pos()).position());

        if(!isInEditionZone(cursorForPosition(event->pos()).position()))
        {
                //Ignore the event if out of the editable zone
                event->ignore(cursorRect(cur));
        }
        else
        {
                //Accept the event if out of the editable zone
                event->accept(cursorRect(cur));
        }
}

//Tests whether the cursor is in th edition zone or not (after the prompt
//or in the next lines (in case of multi-line mode)
bool ConsoleWidget::isInEditionZone()
{
        const int para = textCursor().blockNumber();
        const int index = textCursor().columnNumber();
        return (para > promptParagraph) || ( (para == promptParagraph) && (index >= promptLength) );
}

//Tests whether the current selection is in th edition zone or not
bool ConsoleWidget::isSelectionInEditionZone()
{
        QTextCursor cursor(document());
        int range[2];

        range[0] = textCursor().selectionStart();
        range[1] = textCursor().selectionEnd();
        for (int i = 0; i < 2; i++)
        {
                cursor.setPosition(range[i]);
                int para = cursor.blockNumber();
                int index = cursor.columnNumber();
                if ((para <= promptParagraph) && ( (para != promptParagraph) || (index < promptLength) ))
                {
                        return false;
                }
        }
        return true;
}
//Replace current command with a new one
void ConsoleWidget::replaceCurrentCommand(const QString &newCommand)
{
        QTextCursor cursor = textCursor();
        cursor.movePosition(QTextCursor::StartOfLine);
        cursor.movePosition(QTextCursor::Right, QTextCursor::MoveAnchor, promptLength);
        cursor.movePosition(QTextCursor::EndOfLine, QTextCursor::KeepAnchor);
        cursor.insertText(newCommand);
}
//Tests whether position (in parameter) is in the edition zone or not (after the prompt
//or in the next lines (in case of multi-line mode)
bool ConsoleWidget::isInEditionZone(const int& pos)
{
        QTextCursor cur = textCursor();
        cur.setPosition(pos);
        const int para = cur.blockNumber();
        const int index = cur.columnNumber();
        return (para > promptParagraph) || ( (para == promptParagraph) && (index >= promptLength) );
}
void ConsoleWidget::correctPathName(QString& pathName)
{
        if(pathName.contains(tr(":\\")))
        {
                pathName.replace('\\', tr("/"));
        }
}

//Get the current command
QString ConsoleWidget::getCurrentCommand()
{
        QTextCursor cursor = textCursor();    //Get the current command: we just remove the prompt
        cursor.movePosition(QTextCursor::StartOfLine);
        cursor.movePosition(QTextCursor::Right, QTextCursor::MoveAnchor, promptLength);
        cursor.movePosition(QTextCursor::EndOfLine, QTextCursor::KeepAnchor);
        QString command = cursor.selectedText();
        cursor.clearSelection();
        return command;
}


//execCommand(QString) executes the command and displays back its result
bool ConsoleWidget::execCommand(const QString &command, bool writeCommand,
                                                     bool showPrompt, QString *result)
{
        QString modifiedCommand = command;
        correctPathName(modifiedCommand);
        //Display the prompt with the command first
        if (writeCommand)
        {
                if (getCurrentCommand() != "")
                {
                        append("");
                        displayPrompt();
                }
                textCursor().insertText(modifiedCommand);
        }
        //execute the command and get back its text result and its return value
        int res = 0;
        QString strRes = interpretCommand(modifiedCommand, &res);
        //According to the return value, display the result either in red or in blue
        if (res == 0)
                setTextColor(outColor_);
        else
                setTextColor(errColor_);

        if(result){
                *result = strRes;
        }
        if (!(strRes.isEmpty() || strRes.endsWith("\n")))
            strRes.append("\n");
        append(strRes);
        moveCursor(QTextCursor::End);
        //Display the prompt again
        if (showPrompt)
            displayPrompt();
        return !res;
}

void ConsoleWidget::setPrompt(const QString &newPrompt, bool display)
{
        prompt = newPrompt;
        promptLength = prompt.length();
        //display the new prompt
        if (display)
            displayPrompt();
}
