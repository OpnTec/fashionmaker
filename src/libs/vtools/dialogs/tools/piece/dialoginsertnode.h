/************************************************************************
 **
 **  @file   dialoginsertnode.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   21 3, 2017
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentina project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2017 Valentina project
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

#ifndef DIALOGINSERTNODE_H
#define DIALOGINSERTNODE_H

#include "../dialogtool.h"
#include "../vpatterndb/vpiecenode.h"

namespace Ui
{
    class DialogInsertNode;
}

class DialogInsertNode : public DialogTool
{
    Q_OBJECT

public:
    explicit DialogInsertNode(const VContainer *data, quint32 toolId, QWidget *parent = nullptr);
    virtual ~DialogInsertNode();

    virtual void SetPiecesList(const QVector<quint32> &list) override;

    quint32 GetPieceId() const;
    void    SetPieceId(quint32 id);

    VPieceNode GetNode() const;
    void       SetNode(const VPieceNode &node);

public slots:
    virtual void ChosenObject(quint32 id, const SceneObject &type) override;

protected:
    virtual bool IsValid() const final;

private:
    Q_DISABLE_COPY(DialogInsertNode)
    Ui::DialogInsertNode *ui;

    VPieceNode m_node;
    bool m_flagItem;
    bool m_flagError;

    void CheckPieces();
    void CheckItem();
};

//---------------------------------------------------------------------------------------------------------------------
inline bool DialogInsertNode::IsValid() const
{
    return m_flagItem && m_flagError;
}

#endif // DIALOGINSERTNODE_H
