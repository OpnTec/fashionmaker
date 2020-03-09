/************************************************************************
 **
 **  @file   dialoghistory.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   November 15, 2013
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentina project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2013-2015 Valentina project
 **  <https://gitlab.com/smart-pattern/valentina> All Rights Reserved.
 **
 **  Valentina is free software: you can redistribute it and/or modify
 **  it under the terms of the GNU General Public License as published by
 **  the Free Software Foundation, either version 3 of the License, or
 **  (at your option) any later version.
 **
 **  Valentina is distributed in the hope that it will be useful,
 **  but WITHOUT ANY WARRANTY; without even the implied warranty of
 **  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 **  GNU General Public License for more details.
 **
 **  You should have received a copy of the GNU General Public License
 **  along with Valentina.  If not, see <http://www.gnu.org/licenses/>.
 **
 *************************************************************************/

#ifndef DIALOGHISTORY_H
#define DIALOGHISTORY_H

#include "../vtools/dialogs/tools/dialogtool.h"

#include <QDomElement>

class VPattern;

struct HistoryRecord
{
    QString description{};
    quint32 id{NULL_ID};
};

namespace Ui
{
    class DialogHistory;
}

/**
 * @brief The DialogHistory class show dialog history.
 */
class DialogHistory : public DialogTool
{
    Q_OBJECT
public:
    DialogHistory(VContainer *data, VPattern *doc, QWidget *parent = nullptr);
    virtual ~DialogHistory() override;
public slots:
    virtual void      DialogAccepted() override;
    /** TODO ISSUE 79 : create real function
    * @brief DialogApply apply data and emit signal about applied dialog.
    */
    virtual void      DialogApply() override {}
    void              cellClicked(int row, int column);
    void              ChangedCursor(quint32 id);
    void              UpdateHistory();
signals:
    /**
     * @brief ShowHistoryTool signal change color of selected in records tool
     * @param id id of tool
     * @param enable true enable selection, false disable selection
     */
    void              ShowHistoryTool(quint32 id, bool enable);
protected:
    virtual void      closeEvent ( QCloseEvent * event ) override;
    virtual void      changeEvent(QEvent* event) override;
    virtual bool      IsValid() const final {return true;}
    virtual void      showEvent( QShowEvent *event ) override;
private:
    Q_DISABLE_COPY(DialogHistory)

    /** @brief ui keeps information about user interface */
    Ui::DialogHistory *ui;

    /** @brief doc dom document container */
    VPattern          *doc;

    /** @brief cursorRow save number of row where is cursor */
    qint32            cursorRow;

    /** @brief cursorToolRecordRow save number of row selected record */
    qint32            cursorToolRecordRow;

    void              FillTable();
    HistoryRecord     Record(const VToolRecord &tool) const;
    void              InitialTable();
    void              ShowPoint();
    QString           PointName(quint32 pointId) const;
    quint32           AttrUInt(const QDomElement &domElement, const QString &name) const;
    void              RetranslateUi();
    int               CursorRow() const;
};

#endif // DIALOGHISTORY_H
