/************************************************************************
 **
 **  @file   vtoollineintersectaxis.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   19 10, 2014
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

#ifndef VTOOLLINEINTERSECTAXIS_H
#define VTOOLLINEINTERSECTAXIS_H

#include <qcompilerdetection.h>
#include <QDomElement>
#include <QGraphicsItem>
#include <QLineF>
#include <QMetaObject>
#include <QObject>
#include <QPointF>
#include <QString>
#include <QtGlobal>

#include "../ifc/xml/vabstractpattern.h"
#include "../vpatterndb/vformula.h"
#include "../vmisc/def.h"
#include "vtoollinepoint.h"

template <class T> class QSharedPointer;

class VToolLineIntersectAxis : public VToolLinePoint
{
    Q_OBJECT
public:
    virtual ~VToolLineIntersectAxis() Q_DECL_EQ_DEFAULT;
    virtual void setDialog() Q_DECL_OVERRIDE;

    static VToolLineIntersectAxis *Create(QSharedPointer<DialogTool> dialog, VMainGraphicsScene *scene,
                                          VAbstractPattern *doc, VContainer *data);
    static VToolLineIntersectAxis *Create(const quint32 _id, const QString &pointName, const QString &typeLine,
                                          const QString &lineColor, QString &formulaAngle, const quint32 &basePointId,
                                          const quint32 &firstPointId, const quint32 &secondPointId,
                                          const qreal &mx, const qreal &my, VMainGraphicsScene  *scene,
                                          VAbstractPattern *doc,
                                          VContainer *data, const Document &parse, const Source &typeCreation);

    static QPointF FindPoint(const QLineF &axis, const QLineF &line);

    static const QString ToolType;
    virtual int       type() const Q_DECL_OVERRIDE {return Type;}
    enum { Type = UserType + static_cast<int>(Tool::LineIntersectAxis)};

    QString FirstLinePoint() const;
    QString SecondLinePoint() const;

    VFormula     GetFormulaAngle() const;
    void         SetFormulaAngle(const VFormula &value);

    quint32      GetFirstPointId() const;
    void         SetFirstPointId(const quint32 &value);

    quint32      GetSecondPointId() const;
    void         SetSecondPointId(const quint32 &value);

    virtual void ShowVisualization(bool show) Q_DECL_OVERRIDE;
protected:
    virtual void    contextMenuEvent ( QGraphicsSceneContextMenuEvent * event ) Q_DECL_OVERRIDE;
    virtual void    SaveDialog(QDomElement &domElement) Q_DECL_OVERRIDE;
    virtual void    SaveOptions(QDomElement &tag, QSharedPointer<VGObject> &obj) Q_DECL_OVERRIDE;
    virtual void    ReadToolAttributes(const QDomElement &domElement) Q_DECL_OVERRIDE;
    virtual void    SetVisualization() Q_DECL_OVERRIDE;
    virtual QString MakeToolTip() const Q_DECL_OVERRIDE;
private:
    Q_DISABLE_COPY(VToolLineIntersectAxis)

    QString formulaAngle;
    quint32 firstPointId;
    quint32 secondPointId;

    VToolLineIntersectAxis(VAbstractPattern *doc, VContainer *data, const quint32 &id, const QString &typeLine,
                           const QString &lineColor, const QString &formulaAngle, const quint32 &basePointId,
                           const quint32 &firstPointId, const quint32 &secondPointId, const Source &typeCreation,
                           QGraphicsItem * parent = nullptr);
};

#endif // VTOOLLINEINTERSECTAXIS_H
