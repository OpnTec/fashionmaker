/************************************************************************
 **
 **  @file   vtooldoublepoint.h
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

#ifndef VTOOLDOUBLEPOINT_H
#define VTOOLDOUBLEPOINT_H

#include <qcompilerdetection.h>
#include <QGraphicsItem>
#include <QGraphicsPathItem>
#include <QMetaObject>
#include <QObject>
#include <QPointF>
#include <QString>
#include <QVariant>
#include <QtGlobal>

#include "../vabstractpoint.h"
#include "../vmisc/def.h"

class VSimplePoint;
template <class T> class QSharedPointer;

class VToolDoublePoint: public VAbstractPoint, public QGraphicsPathItem
{
    Q_OBJECT
public:
    VToolDoublePoint(VAbstractPattern *doc, VContainer *data, quint32 id, quint32 p1id, quint32 p2id,
                     QGraphicsItem * parent = nullptr);
    virtual ~VToolDoublePoint() Q_DECL_EQ_DEFAULT;

    virtual int   type() const Q_DECL_OVERRIDE {return Type;}
    enum { Type = UserType + static_cast<int>(Tool::DoublePoint)};

    QString nameP1() const;
    void    setNameP1(const QString &name);

    QString nameP2() const;
    void    setNameP2(const QString &name);

    virtual void GroupVisibility(quint32 object, bool visible) Q_DECL_OVERRIDE;
public slots:
    void         Label1ChangePosition(const QPointF &pos);
    void         Label2ChangePosition(const QPointF &pos);
    virtual void Disable(bool disable, const QString &namePP) Q_DECL_OVERRIDE;
    virtual void EnableToolMove(bool move) Q_DECL_OVERRIDE;
    void         Point1Choosed();
    void         Point2Choosed();
    void         Point1Selected(bool selected);
    void         Point2Selected(bool selected);
    virtual void FullUpdateFromFile() Q_DECL_OVERRIDE;
    virtual void DoChangePosition(quint32 id, qreal mx, qreal my) Q_DECL_OVERRIDE;
    virtual void AllowHover(bool enabled) Q_DECL_OVERRIDE;
    virtual void AllowSelecting(bool enabled) Q_DECL_OVERRIDE;
    void         AllowLabelHover(bool enabled);
    void         AllowLabelSelecting(bool enabled);
    virtual void ToolSelectionType(const SelectionType &type) Q_DECL_OVERRIDE;

protected:
    VSimplePoint *firstPoint;
    VSimplePoint *secondPoint;

    quint32      p1id;
    quint32      p2id;

    virtual void UpdateNamePosition(quint32 id) Q_DECL_OVERRIDE;
    virtual QVariant itemChange ( GraphicsItemChange change, const QVariant &value ) Q_DECL_OVERRIDE;
    virtual void keyReleaseEvent(QKeyEvent * event) Q_DECL_OVERRIDE;
    virtual void contextMenuEvent ( QGraphicsSceneContextMenuEvent * event ) Q_DECL_OVERRIDE;
    virtual void SaveOptions(QDomElement &tag, QSharedPointer<VGObject> &obj) Q_DECL_OVERRIDE;
    virtual void AddToFile() Q_DECL_OVERRIDE;

private:
    Q_DISABLE_COPY(VToolDoublePoint)
};

#endif // VTOOLDOUBLEPOINT_H
