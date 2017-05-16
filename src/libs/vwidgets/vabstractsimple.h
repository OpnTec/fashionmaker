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

#include <QColor>
#include <QMetaObject>
#include <QObject>
#include <QPen>
#include <QString>
#include <Qt>
#include <QtGlobal>

#include "../ifc/ifcdef.h"
#include "../vgeometry/vgeometrydef.h"
#include "../vmisc/def.h"

class QGraphicsSceneContextMenuEvent;

class VAbstractSimple : public QObject
{
    Q_OBJECT
public:
    VAbstractSimple(quint32 id, Unit patternUnit, qreal *factor = nullptr, QObject *parent = nullptr);
    virtual ~VAbstractSimple();

    virtual void ToolSelectionType(const SelectionType &type);
    virtual void SetEnabled(bool enabled);

    GOType GetType() const;
    void   SetType(const GOType &value);

signals:
    void ShowContextMenu(QGraphicsSceneContextMenuEvent * event);
    void Delete();

public slots:
    void ContextMenu(QGraphicsSceneContextMenuEvent * event);

protected:
    /** @brief id spline id. */
    quint32 id;

    /** @brief factor scale factor. */
    qreal  *factor;

    bool    enabled;

    Unit    patternUnit;

    SelectionType selectionType;

    GOType  type;

    QColor CorrectColor(const QColor &color) const;

    template <class T>
    void SetPen(T *item, const QColor &color, qreal width, Qt::PenStyle penStyle = Qt::SolidLine);

private:
    Q_DISABLE_COPY(VAbstractSimple)

    const static qreal m_defFactor;
};

//---------------------------------------------------------------------------------------------------------------------
template <class T>
void VAbstractSimple::SetPen(T *item, const QColor &color, qreal width, Qt::PenStyle penStyle)
{
    SCASSERT(item)
    item->setPen(QPen(CorrectColor(color), ToPixel(width, patternUnit)/ *factor, penStyle, Qt::RoundCap));
}

#endif // VABSTRACTSIMPLE_H
