/************************************************************************
 **
 **  @file   vplacelabelitem.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   15 10, 2017
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
#ifndef VPLACELABELITEM_H
#define VPLACELABELITEM_H

#include <QSharedDataPointer>
#include <QTypeInfo>
#include <QtGlobal>
#include <QMetaType>

#include "vpointf.h"

class VPlaceLabelItemData;
class VContainer;

class VPlaceLabelItem : public VPointF
{
public:
    VPlaceLabelItem();
    VPlaceLabelItem(const VPlaceLabelItem &item);
    virtual ~VPlaceLabelItem() override;

    VPlaceLabelItem &operator=(const VPlaceLabelItem &item);
#ifdef Q_COMPILER_RVALUE_REFS
    VPlaceLabelItem &operator=(VPlaceLabelItem &&item) Q_DECL_NOTHROW { Swap(item); return *this; }
#endif

    inline void Swap(VPlaceLabelItem &item) Q_DECL_NOTHROW
    { VPointF::Swap(item); std::swap(d, item.d); }

    QString  GetWidthFormula() const;
    QString& GetWidthFormula();
    qreal    GetWidth() const;
    void     SetWidth(qreal value, const QString &formula);

    QString  GetHeightFormula() const;
    QString& GetHeightFormula();
    qreal    GetHeight() const;
    void     SetHeight(qreal value, const QString &formula);

    QString  GetAngleFormula() const;
    QString& GetAngleFormula();
    qreal    GetAngle() const;
    void     SetAngle(qreal value, const QString &formula);

    qreal    GetCorrectionAngle() const;
    void     SetCorrectionAngle(qreal value);

    quint32 GetCenterPoint() const;
    void    SetCenterPoint(quint32 id);

    PlaceLabelType GetLabelType() const;
    void           SetLabelType(PlaceLabelType type);

    PlaceLabelImg LabelShape() const;

private:
    QSharedDataPointer<VPlaceLabelItemData> d;
};

Q_DECLARE_METATYPE(VPlaceLabelItem)
Q_DECLARE_TYPEINFO(VPlaceLabelItem, Q_MOVABLE_TYPE);

#endif // VPLACELABELITEM_H
