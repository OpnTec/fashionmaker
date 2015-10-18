/************************************************************************
 **
 **  @file   vabstractsimple.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   20 6, 2015
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentine project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2015 Valentina project
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

#ifndef VABSTRACTSIMPLE_H
#define VABSTRACTSIMPLE_H

#include <QObject>
#include <QPen>
#include <QColor>
#include "../vmisc/def.h"

class VAbstractSimple : public QObject
{
    Q_OBJECT
public:
    VAbstractSimple(quint32 id, const QColor &currentColor, Unit patternUnit, qreal *factor = nullptr,
                    QObject *parent = 0);
    virtual ~VAbstractSimple() Q_DECL_OVERRIDE;

    virtual void ChangedActivDraw(const bool &flag)=0;

protected:
    /** @brief id spline id. */
    quint32 id;

    /** @brief factor scale factor. */
    qreal  *factor;

    /** @brief currentColor current color. */
    QColor  currentColor;

    bool    enabled;

    Unit    patternUnit;

    QColor CorrectColor(const QColor &color) const;

    template <class T>
    void   SetPen(T *item, const QColor &color, qreal width);

private:
    Q_DISABLE_COPY(VAbstractSimple)
};

//---------------------------------------------------------------------------------------------------------------------
template <class T>
void VAbstractSimple::SetPen(T *item, const QColor &color, qreal width)
{
    if (factor == nullptr)
    {
        item->setPen(QPen(CorrectColor(color), ToPixel(width, patternUnit)));
    }
    else
    {
        item->setPen(QPen(CorrectColor(color), ToPixel(width, patternUnit)/ *factor));
    }
}

#endif // VABSTRACTSIMPLE_H
