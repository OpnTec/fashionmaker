/************************************************************************
 **
 **  @file   vtoollinepoint.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   November 15, 2013
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

#ifndef VTOOLLINEPOINT_H
#define VTOOLLINEPOINT_H

#include <qcompilerdetection.h>
#include <QGraphicsItem>
#include <QMetaObject>
#include <QObject>
#include <QString>
#include <QtGlobal>

#include "../vpatterndb/vformula.h"
#include "../vtoolsinglepoint.h"
#include "../vmisc/def.h"

class QDomElement;
class VAbstractPattern;
class VContainer;
class VGObject;
template <class T> class QSharedPointer;

/**
 * @brief The VToolLinePoint class parent for all tools what create point with line.
 */
class VToolLinePoint : public VToolSinglePoint
{
    Q_OBJECT
public:
    VToolLinePoint(VAbstractPattern *doc, VContainer *data, const quint32 &id, const QString &typeLine, const
                   QString &lineColor, const QString &formula, const quint32 &basePointId, const qreal &angle,
                   QGraphicsItem * parent = nullptr);
    virtual ~VToolLinePoint() Q_DECL_OVERRIDE;
    virtual int       type() const Q_DECL_OVERRIDE {return Type;}
    enum { Type = UserType + static_cast<int>(Tool::LinePoint)};

    VFormula GetFormulaLength() const;
    void     SetFormulaLength(const VFormula &value);

    quint32 GetBasePointId() const;
    void    SetBasePointId(const quint32 &value);

    qreal   GetAngle() const;
    void    SetAngle(const qreal &value);

    QString GetLineColor() const;
    void    SetLineColor(const QString &value);

public slots:
    virtual void      SetFactor(qreal factor) Q_DECL_OVERRIDE;
    virtual void      Disable(bool disable, const QString &namePP) Q_DECL_OVERRIDE;
    virtual void      FullUpdateFromFile() Q_DECL_OVERRIDE;
protected:
    /** @brief formula string with length formula. */
    QString           formulaLength;

    /** @brief angle line angle. */
    qreal             angle;

    /** @brief basePointId id base line point. */
    quint32           basePointId;

    /** @brief mainLine line item. */
    QGraphicsLineItem *mainLine;

    /** @brief lineColor color of a line. */
    QString           lineColor;

    virtual void      RefreshGeometry();
    virtual void      RemoveReferens() Q_DECL_OVERRIDE;
    virtual void      SaveOptions(QDomElement &tag, QSharedPointer<VGObject> &obj) Q_DECL_OVERRIDE;
private:
    Q_DISABLE_COPY(VToolLinePoint)
};

#endif // VTOOLLINEPOINT_H
