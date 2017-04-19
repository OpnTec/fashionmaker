/************************************************************************
 **
 **  @file
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   23 2, 2017
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentine project, a pattern making
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

#ifndef VABSTRACTFLOATITEMDATA_H
#define VABSTRACTFLOATITEMDATA_H

#include <QPointF>
#include <QSharedDataPointer>

class VAbstractFloatItemDataPrivate;

class VAbstractFloatItemData
{
public:
    VAbstractFloatItemData();
    VAbstractFloatItemData(const VAbstractFloatItemData &data);

    virtual ~VAbstractFloatItemData();

    VAbstractFloatItemData &operator=(const VAbstractFloatItemData &data);
#ifdef Q_COMPILER_RVALUE_REFS
    VAbstractFloatItemData &operator=(VAbstractFloatItemData &&data) Q_DECL_NOTHROW { Swap(data); return *this; }
#endif

    void Swap(VAbstractFloatItemData &data) Q_DECL_NOTHROW
    { std::swap(d, data.d); }

    // methods, which set and return values of different parameters
    QPointF GetPos() const;
    void    SetPos(const QPointF& ptPos);

    bool IsVisible() const;
    void SetVisible(bool bVisible);

private:
    QSharedDataPointer<VAbstractFloatItemDataPrivate> d;
};

#endif // VABSTRACTFLOATITEMDATA_H
