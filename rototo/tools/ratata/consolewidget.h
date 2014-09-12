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

#ifndef CONSOLEWIDGET_H
#define CONSOLEWIDGET_H

#include <QStringList>
#include <QTextEdit>
#include <QMenu>

class AppDelegate;

class ConsoleWidget : public QTextEdit
{
        Q_OBJECT
public:
        //constructor
        ConsoleWidget(QWidget *parent = NULL, const QString &welcomeText = "",AppDelegate * _pApp = NULL);
        //set the prompt of the console
        void setPrompt(const QString &prompt, bool display = true);
        //execCommand(QString) executes the command and displays back its result
        bool execCommand(const QString &command, bool writeCommand = true,
                                         bool showPrompt = true, QString *result = NULL);
        //saves a file script
        int saveScript(const QString &fileName);
        //loads a file script
        int loadScript(const QString &fileName);
        //clear & reset the console (useful sometimes)
        void clear();
        void reset(const QString &welcomeText = "Welcome to Ratata Console");

        //cosmetic methods !

        // @{
        /// get/set command color
        QColor cmdColor() const { return cmdColor_; }
        void setCmdColor(QColor c) {cmdColor_ = c;}
        // @}

        // @{
        /// get/set error color
        QColor errColor() const { return errColor_; }
        void setErrColor(QColor c) {errColor_ = c;}
        // @}

        // @{
        /// get/set output color
        QColor outColor() const { return outColor_; }
        void setOutColor(QColor c) {outColor_ = c;}
        // @}
        void setCompletionColor(QColor c) {completionColor = c;}

        // @{
        /// get set font
        void setFont(const QFont& f);
        QFont font() const { return currentFont(); }
        // @}

        void correctPathName(QString& pathName);

private:
        void dropEvent( QDropEvent * event);
        void dragMoveEvent( QDragMoveEvent * event);

        void keyPressEvent(QKeyEvent * e);
        void contextMenuEvent( QContextMenuEvent * event);

        //Return false if the command is incomplete (e.g. unmatched braces)
        virtual bool isCommandComplete(const QString &command);
        //Get the command to validate
        QString getCurrentCommand();

        //Replace current command with a new one
        void replaceCurrentCommand(const QString &newCommand);

        //Test whether the cursor is in the edition zone
        bool isInEditionZone();
        bool isInEditionZone(const int& pos);

        //Test whether the selection is in the edition zone
        bool isSelectionInEditionZone();
        //Change paste behaviour
        void insertFromMimeData(const QMimeData *);


//protected attributes
protected:
        //colors
        QColor cmdColor_, errColor_, outColor_, completionColor;

        int oldPosition;
        // cached prompt length
        int promptLength;
        // The prompt string
        QString prompt;
        // The commands history
        QStringList history;
        //Contains the commands that has succeeded
        QStringList recordedScript;
        // Current history index (needed because afaik QStringList does not have such an index)
        int historyIndex;
        //Holds the paragraph number of the prompt (useful for multi-line command handling)
        int promptParagraph;

protected:
        //Implement paste with middle mouse button
        void mousePressEvent(QMouseEvent*);

        //execute a validated command (should be reimplemented and called at the end)
        //the return value of the function is the string result
        //res must hold back the return value of the command (0: passed; else: error)
        virtual QString interpretCommand(const QString &command, int *res);
        //give suggestions to autocomplete a command (should be reimplemented)
        //the return value of the function is the string list of all suggestions
        //the returned prefix is useful to complete "sub-commands"
        virtual QStringList suggestCommand(const QString &cmd, QString &prefix);


public Q_SLOTS:
        //Contextual menu slots
        void cut();
        //void paste();
        void del();
        //displays the prompt
        void displayPrompt();

Q_SIGNALS:
        //Signal emitted after that a command is executed
        void commandExecuted(const QString &command);

private:
        void handleTabKeyPress();
        void handleReturnKeyPress();
        bool handleBackspaceKeyPress();
        void handleUpKeyPress();
        void handleDownKeyPress();
        void setHome(bool);

        AppDelegate *m_pApp;
};


#endif
