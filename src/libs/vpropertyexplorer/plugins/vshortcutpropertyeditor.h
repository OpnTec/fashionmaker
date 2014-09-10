/************************************************************************
 **
 **  @file   vshortcutpropertyeditor.h
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

#ifndef VSHORTCUTPROPERTYEDITOR_H
#define VSHORTCUTPROPERTYEDITOR_H

#include "vpropertyexplorer_global.h"

#include <QWidget>
#include <QToolButton>
#include <QLineEdit>
#include <QMimeData>

namespace VPE{


class VPROPERTYEXPLORERSHARED_EXPORT VShortcutEditWidget : public QWidget
{
    Q_OBJECT

public:
    VShortcutEditWidget(QWidget* parent);
    virtual ~VShortcutEditWidget();

    //! Needed for proper event handling
    bool eventFilter(QObject* obj, QEvent* evenvt);

    //! Returns the currently set shortcut
    QString getShortcutAsString();

    //! Returns the currently set shortcut
    QKeySequence getShortcut();

signals:
    //! This signal is emitted when the user changed the current shortcut
    void dataChangedByUser(const QKeySequence& sequence, VShortcutEditWidget* editor);

    //! This signal is emitted whenever dataChangedByUser() gets emmitted
    //! and is connected to the delegate's commitData() signal
    void commitData(QWidget* editor);

public slots:
    //! Sets the shortcut
    //! \param shortcut The new shortcut
    //! \emit_signal If true, this will emit the dataChangedByUser()-signal
    void setShortcut(const QString &shortcut, bool emit_signal);

    //! Sets the shortcut
    //! \param shortcut The new shortcut
    //! \emit_signal If true, this will emit the dataChangedByUser()-signal
    void setShortcut(const QKeySequence &shortcut, bool emit_signal);

private slots:
    //! This slot is called when the user edits the line edit (e.g. by removing or pasting text using the mouse)
    void onTextEdited(const QString& text);

protected:
    //! The current key sequence
    QKeySequence CurrentKeySequence;

    //! The line to display and edit the key sequence
    QLineEdit* LineEdit;
};

}

#endif // VFILEPROPERTYEDITOR_H
