/************************************************************************
 **
 **  @file   dialogduplicatedetail.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   25 10, 2017
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentina project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2017 Valentina project
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
#ifndef DIALOGDUPLICATEDETAIL_H
#define DIALOGDUPLICATEDETAIL_H

#include "../dialogtool.h"

namespace Ui
{
    class DialogDuplicateDetail;
}

class DialogDuplicateDetail : public DialogTool
{
    Q_OBJECT

public:
    explicit DialogDuplicateDetail(const VContainer *data, const quint32 &toolId, QWidget *parent = nullptr);
    virtual ~DialogDuplicateDetail();

    quint32 Duplicate() const;
    qreal   MoveDuplicateX() const;
    qreal   MoveDuplicateY() const;

    virtual void ShowDialog(bool click) Q_DECL_OVERRIDE;
public slots:
    virtual void ChosenObject(quint32 id, const SceneObject &type) Q_DECL_OVERRIDE;
private:
    Q_DISABLE_COPY(DialogDuplicateDetail)
    Ui::DialogDuplicateDetail *ui;
    quint32 m_idDetail;
    qreal m_mx;
    qreal m_my;
    bool m_firstRelease;
};

//---------------------------------------------------------------------------------------------------------------------
inline quint32 DialogDuplicateDetail::Duplicate() const
{
    return m_idDetail;
}

//---------------------------------------------------------------------------------------------------------------------
inline qreal DialogDuplicateDetail::MoveDuplicateX() const
{
    return m_mx;
}

//---------------------------------------------------------------------------------------------------------------------
inline qreal DialogDuplicateDetail::MoveDuplicateY() const
{
    return m_my;
}

#endif // DIALOGDUPLICATEDETAIL_H
