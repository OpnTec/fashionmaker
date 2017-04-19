/************************************************************************
 **
 **  @file   vpatternlabeldata.h
 **  @author Bojan Kverh
 **  @date   June 16, 2016
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentine project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2013-2015 Valentina project
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

#ifndef VPATTERNINFOGEOMETRY_H
#define VPATTERNINFOGEOMETRY_H

#include <QPointF>

#include "vabstractfloatitemdata.h"

class VPatternLabelDataPrivate;

/**
 * @brief The VPatternLabelData class holds the information about pattern info label geometry
 */
class VPatternLabelData : public VAbstractFloatItemData
{
public:
    VPatternLabelData();
    VPatternLabelData(const VPatternLabelData &data);

    virtual ~VPatternLabelData();

    VPatternLabelData &operator=(const VPatternLabelData &data);
#ifdef Q_COMPILER_RVALUE_REFS
    VPatternLabelData &operator=(VPatternLabelData &&data) Q_DECL_NOTHROW { Swap(data); return *this; }
#endif

    void Swap(VPatternLabelData &data) Q_DECL_NOTHROW
    { std::swap(d, data.d); }

    // methods, which set up label parameters
    QString GetLabelWidth() const;
    void    SetLabelWidth(const QString &dLabelW);

    QString GetLabelHeight() const;
    void    SetLabelHeight(const QString &dLabelH);

    int   GetFontSize() const;
    void  SetFontSize(int iSize);

    QString GetRotation() const;
    void    SetRotation(const QString &dRot);

    quint32 CenterPin() const;
    void    SetCenterPin(const quint32 &centerPin);

    quint32 TopLeftPin() const;
    void    SetTopLeftPin(const quint32 &topLeftPin);

    quint32 BottomRightPin() const;
    void    SetBottomRightPin(const quint32 &bottomRightPin);

private:
    QSharedDataPointer<VPatternLabelDataPrivate> d;
};

#endif // VPATTERNINFOGEOMETRY_H
