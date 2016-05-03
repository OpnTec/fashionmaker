/************************************************************************
 **
 **  @file   vfilepropertyeditor.h
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

#ifndef VFILEPROPERTYEDITOR_H
#define VFILEPROPERTYEDITOR_H

#include "../vpropertyexplorer_global.h"

#include <QWidget>
#include <QToolButton>
#include <QLineEdit>
#include <QMimeData>

namespace VPE
{


class VPROPERTYEXPLORERSHARED_EXPORT VFileEditWidget : public QWidget
{
    Q_OBJECT

public:
    explicit VFileEditWidget(QWidget* parent, bool is_directory = false);
    virtual ~VFileEditWidget() Q_DECL_OVERRIDE;


    //! This function returns the file currently set to this editor
    QString getFile() const;

    //! Needed for proper event handling
    virtual bool eventFilter(QObject* obj, QEvent* ev) Q_DECL_OVERRIDE;

    //! Returns the directory/file setting
    //! \return True, if a directory dialog is being shown, false if a file dialog
    bool isDirectory();

signals:
    //! This signal is emitted when the user changed the curret file.
    //! Actions triggering this signal are either using the file dialog
    //! to select a new file or changing the file path in the line edit.
    void dataChangedByUser(const QString &getFile, VFileEditWidget* editor);

    //! This signal is emitted whenever dataChangedByUser() gets emmitted
    //! and is connected to the delegate's commitData() signal
    void commitData(QWidget* editor);


public slots:
    //! Sets the current file, does not check if it is valid
    //! \param getFile The new filepath the widget should show
    //! \param emit_signal If true, this will emit the dataChangedByUser()-signal (if file differs from the current
    //! file)
    void setFile(const QString &getFile, bool emit_signal = false);

    //! Sets a filter for the file field
    //! \param dialog_filter The filter used for the File Dialog
    //! \param filter_list The list of file endings. The filters are being checked using regular expressions
    void setFilter(const QString& dialog_filter = QString(), const QStringList& filter_list = QStringList());

    //! Sets whether the property stores a directory or a file
    void setDirectory(bool dir);

private slots:
    //! This slot gets activated, when the "..." button gets clicked
    void onToolButtonClicked();

protected:
    virtual void dragEnterEvent(QDragEnterEvent* event) Q_DECL_OVERRIDE;
    virtual void dragMoveEvent(QDragMoveEvent* event) Q_DECL_OVERRIDE;
    virtual void dragLeaveEvent(QDragLeaveEvent* event) Q_DECL_OVERRIDE;
    virtual void dropEvent(QDropEvent* event) Q_DECL_OVERRIDE;

    //! This function checks the mime data, if it is compatible with the filters
    virtual bool checkMimeData(const QMimeData* data, QString& getFile) const;

    //! This checks, if a file is compatible with the filters
    virtual bool checkFileFilter(const QString& getFile) const;


    QString CurrentFilePath;
    QToolButton* ToolButton;
    QLineEdit* FileLineEdit;
    QString FileDialogFilter;
    QStringList FilterList;

    //! Specifies whether it is being looked for a directory (true) or a file (false, default)
    bool Directory;

private:
    Q_DISABLE_COPY(VFileEditWidget)
};

}

#endif // VFILEPROPERTYEDITOR_H
