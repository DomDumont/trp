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

#include "grepwidget.h"

//#include <QtGui>
#include <QtWidgets>
#include <QDomDocument>
#include <QFile>
#include "mdimainwindow.h"
#include "projectwidget.h"


static void updateComboBox(QComboBox *comboBox)
{
    if (comboBox->findText(comboBox->currentText()) == -1)
        comboBox->addItem(comboBox->currentText());
}


GrepWidget::GrepWidget(QWidget *parent): QWidget(parent)
{
    m_mainWindow = qobject_cast<MDIMainWindow*>(parent);
    Q_ASSERT(m_mainWindow != NULL);

    InitWidget();

}

void GrepWidget::createOutput()
{
    output = new QTreeWidget();
    output->setSelectionBehavior(QAbstractItemView::SelectRows);

    output->setHeaderLabel("Results");

    QStringList headerLabels;
    headerLabels.push_back(tr("File"));
    headerLabels.push_back(tr("Line"));
    headerLabels.push_back(tr("Text"));


    output->setColumnCount(headerLabels.count());
    output->setHeaderLabels(headerLabels);
    connect(output, SIGNAL(itemDoubleClicked(QTreeWidgetItem *,int)),SLOT(openFileOfItem(QTreeWidgetItem *,int)));
    /*
     connect(output, SIGNAL(cellActivated(int,int)),
             this, SLOT(openFileOfItem(int,int)));
             */
}

GrepWidget::~GrepWidget()
{
}

QPushButton *GrepWidget::createButton(const QString &text, const char *member)
{
    QPushButton *button = new QPushButton(text);
    connect(button, SIGNAL(clicked()), this, member);
    return button;
}

QComboBox *GrepWidget::createComboBox(const QString &text)
{
    QComboBox *comboBox = new QComboBox;
    comboBox->setEditable(true);
    comboBox->addItem(text);
    comboBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

    return comboBox;
}
void GrepWidget::SlotTextEdited()
{
find();
}
void GrepWidget::InitWidget()
{
    textLabel = new QLabel(tr("Containing text:"));
    textComboBox = createComboBox();

    QLineEdit *edit = textComboBox->lineEdit();
    Q_ASSERT(connect(edit, SIGNAL(returnPressed ()), this, SLOT(SlotTextEdited())));


    findButton = createButton(tr("&Find"), SLOT(find()));
    createOutput();


    QGridLayout *mainLayout = new QGridLayout;
    mainLayout->setSizeConstraint(QLayout::SetNoConstraint);

    mainLayout->addWidget(textLabel, 0, 0);
    mainLayout->addWidget(textComboBox, 0, 1, 1, 2);
    mainLayout->addWidget(output, 2, 0,1,3);
    mainLayout->addWidget(findButton, 3, 2);
    setLayout(mainLayout);
}


void GrepWidget::find()
{
    output->clear();
    QString text = textComboBox->currentText();
    updateComboBox(textComboBox);
    QStringList files;
    currentDir = m_mainWindow->m_projectWidget->rootDir;
    files = m_mainWindow->m_projectWidget->GetScriptFiles();
    if (!text.isEmpty())
        files = findFiles(files, text);
    showFiles(files);
/*








    currentDir = QDir(path);
    QStringList files;
    if (fileName.isEmpty())
        fileName = "*";
    files = currentDir.entryList(QStringList(fileName),
                                 QDir::Files | QDir::NoSymLinks);

    if (!text.isEmpty())
        files = findFiles(files, text);
    showFiles(files);
*/
}

void GrepWidget::SetText(QString text)
{
textComboBox->setEditText(text);
updateComboBox(textComboBox);
find();
}

void GrepWidget::openFileOfItem(QTreeWidgetItem *w,int col)
{
  if (w->text(1).isEmpty())
    m_mainWindow->openFileFromFilename(currentDir.absoluteFilePath(w->text(0)));
  else
      m_mainWindow->openFileFromFilename(currentDir.absoluteFilePath(w->text(0)),w->text(1).toInt());




    //QTreeWidgetItem *item = output->selectedItems()row, 0);

    //QDesktopServices::openUrl(QUrl::fromLocalFile(currentDir.absoluteFilePath(item->text())));
}

QStringList GrepWidget::findFiles(const QStringList &files, const QString &text)
{
    QProgressDialog progressDialog(this);
    progressDialog.setCancelButtonText(tr("&Cancel"));
    progressDialog.setRange(0, files.size());
    progressDialog.setWindowTitle(tr("Find Files"));

    QStringList foundFiles;

    for (int i = 0; i < files.size(); ++i) {
        progressDialog.setValue(i);
        progressDialog.setLabelText(tr("Searching file number %1 of %2...")
                                    .arg(i).arg(files.size()));
        qApp->processEvents();

        if (progressDialog.wasCanceled())
            break;

        QFile file(currentDir.absoluteFilePath(files[i]));

        if (file.open(QIODevice::ReadOnly)) {
            QString line;
            QTextStream in(&file);
            int lineNumber;
            lineNumber = 1;
            bool atLeastOneFind;
            atLeastOneFind = false;
            QTreeWidgetItem *topLevelItem;
            topLevelItem = NULL;
            while (!in.atEnd())
            {
                if (progressDialog.wasCanceled())
                    break;
                line = in.readLine();
                if (line.contains(text))
                {
                    foundFiles << files[i];

                    if (atLeastOneFind == false)
                    {
                        topLevelItem = new QTreeWidgetItem;
                        topLevelItem->setText(0, files[i]);
                        output->addTopLevelItem(topLevelItem);
                    }

                    // De toutes façons add
                    {
                    QTreeWidgetItem *childItem = new QTreeWidgetItem;
                                    childItem->setText(0, files[i]);
                                    childItem->setText(1, QString::number(lineNumber));
                                    childItem->setText(2, line.trimmed());

                    topLevelItem->addChild(childItem);
                    }
                    atLeastOneFind = true;

                }
                lineNumber++;
            }
        }
    }
    output->expandAll();
    output->resizeColumnToContents(0);
    output->resizeColumnToContents(1);
    return foundFiles;
}

void GrepWidget::showFiles(const QStringList &files)
{
    /*
    for (int i = 0; i < files.size(); ++i)
        {
        QFile file(currentDir.absoluteFilePath(files[i]));
        qint64 size = QFileInfo(file).size();

        QTableWidgetItem *fileNameItem = new QTableWidgetItem(files[i]);
        fileNameItem->setFlags(fileNameItem->flags() ^ Qt::ItemIsEditable);
        QTableWidgetItem *sizeItem = new QTableWidgetItem(tr("%1 KB")
                                             .arg(int((size + 1023) / 1024)));
        sizeItem->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
        sizeItem->setFlags(sizeItem->flags() ^ Qt::ItemIsEditable);

        int row = filesTable->rowCount();
        filesTable->insertRow(row);
        filesTable->setItem(row, 0, fileNameItem);
        filesTable->setItem(row, 1, sizeItem);
        }
    filesFoundLabel->setText(tr("%1 file(s) found").arg(files.size()) +

                             (" (Double click on a file to open it)"));

    filesFoundLabel->setWordWrap(true);
    */
}
