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
 **  Copyright (C) 2014 Valentina project
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

#include "vtoollinepoint.h"

class VToolLineIntersectAxis : public VToolLinePoint
{
    Q_OBJECT
public:
    VToolLineIntersectAxis(VPattern *doc, VContainer *data, const quint32 &id, const QString &typeLine,
                           const QString &formulaAngle, const quint32 &basePointId, const quint32 &firstPointId,
                           const quint32 &secondPointId, const Source &typeCreation, QGraphicsItem * parent = nullptr);
    virtual ~VToolLineIntersectAxis();
    virtual void setDialog();

    static VToolLineIntersectAxis *Create(DialogTool *dialog, VMainGraphicsScene *scene, VPattern *doc,
                                          VContainer *data);
    static VToolLineIntersectAxis *Create(const quint32 _id, const QString &pointName, const QString &typeLine,
                                          QString &formulaAngle, const quint32 &basePointId,
                                          const quint32 &firstPointId, const quint32 &secondPointId,
                                          const qreal &mx, const qreal &my, VMainGraphicsScene  *scene, VPattern *doc,
                                          VContainer *data, const Document &parse, const Source &typeCreation);

    static QPointF FindPoint(const QLineF &axis, const QLineF &line);

    static const QString ToolType;
    virtual int       type() const {return Type;}
    enum { Type = UserType + static_cast<int>(Tool::LineIntersectAxis)};

    VFormula     GetFormulaAngle() const;
    void         SetFormulaAngle(const VFormula &value);

    quint32      GetFirstPointId() const;
    void         SetFirstPointId(const quint32 &value);

    quint32      GetSecondPointId() const;
    void         SetSecondPointId(const quint32 &value);

    virtual void ShowVisualization(bool show);
public slots:
    virtual void FullUpdateFromFile();
    virtual void ShowContextMenu(QGraphicsSceneContextMenuEvent *event);
protected:
    virtual void contextMenuEvent ( QGraphicsSceneContextMenuEvent * event );
    virtual void SaveDialog(QDomElement &domElement);
    virtual void SaveOptions(QDomElement &tag, QSharedPointer<VGObject> &obj);
private:
    Q_DISABLE_COPY(VToolLineIntersectAxis)
    QString formulaAngle;
    quint32 firstPointId;
    quint32 secondPointId;
};

#endif // VTOOLLINEINTERSECTAXIS_H
