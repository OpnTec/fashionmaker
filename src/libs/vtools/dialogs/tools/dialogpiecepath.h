/************************************************************************
 **
 **  @file   dialogpiecepath.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   22 11, 2016
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

#ifndef DIALOGPIECEPATH_H
#define DIALOGPIECEPATH_H

#include "dialogtool.h"

namespace Ui
{
    class DialogPiecePath;
}

class DialogPiecePath : public DialogTool
{
    Q_OBJECT
public:
    explicit DialogPiecePath(const VContainer *data, quint32 toolId, QWidget *parent = nullptr);
    virtual ~DialogPiecePath();

    void EnbleShowMode(bool disable);

    VPiecePath GetPiecePath() const;
    void       SetPiecePath(const VPiecePath &path);

    quint32 GetPieceId() const;
    void    SetPieceId(quint32 id);

    void SetSAWidth(qreal width);

    virtual void SetPiecesList(const QVector<quint32> &list) Q_DECL_OVERRIDE;

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
    void NameChanged();
    void NodeChanged(int index);
    void ReturnDefBefore();
    void ReturnDefAfter();
    void ChangedSABefore(double d);
    void ChangedSAAfter(double d);

private:
    Q_DISABLE_COPY(DialogPiecePath)
    Ui::DialogPiecePath *ui;
    bool m_showMode;
    qreal m_saWidth;

    void InitPathTypes();
    void InitListPieces();
    void InitNodesList();
    void NodeAngleChanged(int index);

    VPiecePath CreatePath() const;

    bool PathIsValid() const;
    void ValidObjects(bool value);
    void NewItem(const VPieceNode &node);

    PiecePathType GetType() const;
    void          SetType(PiecePathType type);

    QListWidgetItem *GetItemById(quint32 id);

    void SetCurrentSABefore(qreal value);
    void SetCurrentSAAfter(qreal value);
};

#endif // DIALOGPIECEPATH_H
