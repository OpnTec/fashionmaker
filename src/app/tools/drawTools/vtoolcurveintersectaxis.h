/************************************************************************
 **
 **  @file   vtoolcurveintersectaxis.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   21 10, 2014
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

#ifndef VTOOLCURVEINTERSECTAXIS_H
#define VTOOLCURVEINTERSECTAXIS_H

#include "vtoollinepoint.h"

class VToolCurveIntersectAxis : public VToolLinePoint
{
    Q_OBJECT
public:
    VToolCurveIntersectAxis(VPattern *doc, VContainer *data, const quint32 &id, const QString &typeLine,
                            const QString &lineColor, const QString &formulaAngle, const quint32 &basePointId,
                            const quint32 &curveId, const Source &typeCreation, QGraphicsItem * parent = nullptr);
    virtual ~VToolCurveIntersectAxis();
    virtual void setDialog();

    static VToolCurveIntersectAxis *Create(DialogTool *dialog, VMainGraphicsScene *scene, VPattern *doc,
                                           VContainer *data);
    static VToolCurveIntersectAxis *Create(const quint32 _id, const QString &pointName, const QString &typeLine,
                                           const QString &lineColor, QString &formulaAngle, const quint32 &basePointId,
                                           const quint32 &curveId, const qreal &mx, const qreal &my,
                                           VMainGraphicsScene  *scene, VPattern *doc, VContainer *data,
                                           const Document &parse, const Source &typeCreation);

    static QPointF FindPoint(const QPointF &point, qreal angle, const QSharedPointer<VAbstractCurve> &curve);

    static const QString ToolType;
    virtual int       type() const {return Type;}
    enum { Type = UserType + static_cast<int>(Tool::CurveIntersectAxis)};

    VFormula     GetFormulaAngle() const;
    void         SetFormulaAngle(const VFormula &value);

    quint32      getCurveId() const;
    void         setCurveId(const quint32 &value);

    virtual void ShowVisualization(bool show);
protected:
    virtual void contextMenuEvent ( QGraphicsSceneContextMenuEvent * event );
    virtual void SaveDialog(QDomElement &domElement);
    virtual void SaveOptions(QDomElement &tag, QSharedPointer<VGObject> &obj);
    virtual void ReadToolAttributes(const QDomElement &domElement);
    virtual void SetVisualization();
private:
    Q_DISABLE_COPY(VToolCurveIntersectAxis)
    QString formulaAngle;
    quint32 curveId;
};

#endif // VTOOLCURVEINTERSECTAXIS_H
