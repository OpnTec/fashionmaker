/************************************************************************
 **
 **  @file   vfilepropertyeditor.cpp
 **  @author hedgeware <internal(at)hedgeware.net>
 **  @date
 **
 **  @brief
 **  @copyright
 **  All rights reserved. This program and the accompanying materials
 **  are made available under the terms of the GNU Lesser General Public License
 **  (LGPL) version 2.1 which accompanies this distribution, and is available at
 **  http://www.gnu.org/licenses/lgpl-2.1.html
 **
 **  This library is distributed in the hope that it will be useful,
 **  but WITHOUT ANY WARRANTY; without even the implied warranty of
 **  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 **  Lesser General Public License for more details.
 **
 *************************************************************************/

#include "vfilepropertyeditor.h"

#include "vfileproperty.h"

#include <QHBoxLayout>
#include <QFileDialog>
#include <QKeyEvent>
#include <QUrl>

using namespace VPE;

VFileEditWidget::VFileEditWidget(QWidget *parent, bool is_directory)
    : QWidget(parent), CurrentFilePath(), ToolButton(nullptr), FileLineEdit(nullptr), FileDialogFilter(), FilterList(),
      Directory(is_directory)
{
    // Create the tool button,ToolButton = new QToolButton(this);
    ToolButton = new QToolButton(this);
    ToolButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Ignored);
    ToolButton->setText("...");
    ToolButton->setFixedWidth(20);
    ToolButton->installEventFilter(this);
    setFocusProxy(ToolButton);  // Make the ToolButton the focus proxy
    setFocusPolicy(ToolButton->focusPolicy());
    connect(ToolButton, SIGNAL(clicked()), this, SLOT(onToolButtonClicked()));

    // Create the line edit widget
    FileLineEdit = new QLineEdit(this);
    FileLineEdit->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    FileLineEdit->setText(CurrentFilePath);
    FileLineEdit->installEventFilter(this);

    // The layout (a horizontal layout)
    QHBoxLayout* layout = new QHBoxLayout(this);
    layout->setSpacing(0);
    layout->setMargin(0);
    layout->addWidget(FileLineEdit);
    layout->addWidget(ToolButton);

    // Accept drops
    setAcceptDrops(true);
}


VFileEditWidget::~VFileEditWidget()
{
    // nothing needs to be done here
}


void VFileEditWidget::setFile(const QString &value, bool emit_signal)
{
    if (CurrentFilePath != value)
    {
        CurrentFilePath = value;
        FileLineEdit->setText(CurrentFilePath);

        if (emit_signal)
        {
            emit dataChangedByUser(CurrentFilePath, this);
            emit commitData(this);
        }
    }
}


void VFileEditWidget::setFilter(const QString &dialog_filter, const QStringList& filter_list)
{
    FileDialogFilter = dialog_filter;
    FilterList = filter_list;
}

void VFileEditWidget::setDirectory(bool dir)
{
    Directory = dir;
}

QString VFileEditWidget::getFile() const
{
    return CurrentFilePath;
}


void VFileEditWidget::onToolButtonClicked()
{
    QString filepath = (Directory ? QFileDialog::getExistingDirectory(0, tr("Directory"), CurrentFilePath)
                                  : QFileDialog::getOpenFileName(0, tr("Open File"), CurrentFilePath,
                                                                 FileDialogFilter));
    if (filepath.isNull() == false)
    {
        setFile(filepath, true);
    }
}


bool VFileEditWidget::eventFilter(QObject *obj, QEvent *ev)
{
    if (ev->type() == QEvent::DragEnter || ev->type() == QEvent::Drop)
    {
        ev->ignore();
        if (ev->type() == QEvent::DragEnter)
        {
            dragEnterEvent(static_cast<QDragEnterEvent*>(ev));
        }
        else if (ev->type() == QEvent::Drop)
        {
            dropEvent(static_cast<QDropEvent*>(ev));
        }

        if (ev->isAccepted())
        {
            return true;
        }
        else
            return QWidget::eventFilter(obj, ev);
    }
    else if (obj == ToolButton && ev->type() == QEvent::KeyPress)
    {
        // Ignore the event, so that eventually the delegate gets the event.
        ev->ignore();
        return true;
    }
    else if (obj == FileLineEdit)
    {
        if (ev->type() == QEvent::FocusOut)
        {
            setFile(FileLineEdit->text(), true);
            // We don't return true here because we still want the line edit to catch the event as well
        }
    }

    // forward the signal to the parent class
    return QWidget::eventFilter(obj, ev);
}

bool VFileEditWidget::isDirectory()
{
    return Directory;
}


void VFileEditWidget::dragEnterEvent(QDragEnterEvent* event)
{
    QString tmpFileName;
    if (checkMimeData(event->mimeData(), tmpFileName))
    {
        event->accept();
        event->acceptProposedAction();
    }
}

// cppcheck-suppress unusedFunction
void VFileEditWidget::dragMoveEvent(QDragMoveEvent* event)
{
    event->acceptProposedAction();
}

// cppcheck-suppress unusedFunction
void VFileEditWidget::dragLeaveEvent(QDragLeaveEvent* event)
{
    event->accept();
}

void VFileEditWidget::dropEvent(QDropEvent* event)
{
    QString tmpFileName;
    if (checkMimeData(event->mimeData(), tmpFileName))
    {
        setFile(tmpFileName);
        emit dataChangedByUser(getFile(), this);
        emit commitData(this);
        event->accept();
        event->acceptProposedAction();
    }
}


bool VFileEditWidget::checkMimeData(const QMimeData* data, QString& file) const
{
    if (data->hasUrls())
    {
        QList<QUrl> tmpUrlList = data->urls();
        QFileInfo tmpFileInfo;

        foreach(QUrl tmpUrl, tmpUrlList)
            if (QFile::exists(tmpUrl.toLocalFile()))
            {
                tmpFileInfo = QFileInfo(tmpUrl.toLocalFile()); break;
            }

        if (checkFileFilter(tmpFileInfo.fileName()))
        {
            file = tmpFileInfo.absoluteFilePath();
            return true;
        }
    }

    return false;
}

bool VFileEditWidget::checkFileFilter(const QString& file) const
{
    if (FilterList.isEmpty())
    {
        return true;
    }

    QFileInfo tmpFileInfo(file);

    if ((Directory && !tmpFileInfo.isDir()) || (!Directory && !tmpFileInfo.isFile()))
    {
        return false;
    }

    foreach(QString tmpFilter, FilterList)
    {
        QRegExp tmpRegExpFilter(tmpFilter, Qt::CaseInsensitive, QRegExp::Wildcard);
        if (tmpRegExpFilter.exactMatch(file))
        {
            return true;
        }
    }

    return false;
}
