/************************************************************************
 **
 **  @file   dialogseamallowance.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   3 11, 2016
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentine project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2016 Valentina project
 **  <https://bitbucket.org/dismine/valentina> All Rights Reserved.
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

#ifndef DIALOGSEAMALLOWANCE_H
#define DIALOGSEAMALLOWANCE_H

#include "dialogtool.h"
#include "../vpatterndb/vpiece.h"

namespace Ui
{
    class DialogSeamAllowance;
}

class DialogSeamAllowance : public DialogTool
{
    Q_OBJECT

public:
    DialogSeamAllowance(const VContainer *data, const quint32 &toolId, QWidget *parent = nullptr);
    virtual ~DialogSeamAllowance();

    void EnableApply(bool enable);

    VPiece GetPiece() const;
    void   SetPiece(const VPiece &m_piece);

public slots:
    virtual void ChosenObject(quint32 id, const SceneObject &type) Q_DECL_OVERRIDE;
    virtual void ShowDialog(bool click) Q_DECL_OVERRIDE;

protected:
    /** @brief SaveData Put dialog data in local variables */
    virtual void SaveData() Q_DECL_OVERRIDE;
    virtual void CheckState() Q_DECL_OVERRIDE;

private slots:
    void ShowContextMenu(const QPoint &pos);
    void ListChanged();
    void EnableSeamAllowance(bool enable);
    void NodeChanged(int index);
    void ReturnDefBefore();
    void ReturnDefAfter();
    void ChangedSABefore(double d);
    void ChangedSAAfter(double d);

private:
    Q_DISABLE_COPY(DialogSeamAllowance)

    Ui::DialogSeamAllowance *ui;
    VPiece m_piece;
    bool   applyAllowed;

    VPiece CreatePiece() const;

    void    NewItem(const VPieceNode &node);
    quint32 RowId(int i) const;
    bool    MainPathIsValid() const;
    void    ValidObjects(bool value);
    bool    FirstPointEqualLast() const;
    bool    MainPathIsClockwise() const;
    QString GetNodeName(const VPieceNode &node) const;
    void    InitNodesList();

    QListWidgetItem *GetItemById(quint32 id);

    void SetCurrentSABefore(qreal value);
    void SetCurrentSAAfter(qreal value);
};

#endif // DIALOGSEAMALLOWANCE_H
