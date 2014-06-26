/************************************************************************
 **
 **  @file   vtoolcutspline.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   15 12, 2013
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentine project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2013 Valentina project
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

#ifndef VTOOLCUTSPLINE_H
#define VTOOLCUTSPLINE_H

#include "vtoolcut.h"

/**
 * @brief The VToolCutSpline class for tool CutSpline. This tool find point on spline and cut spline on two.
 */
class VToolCutSpline : public VToolCut
{
    Q_OBJECT
public:

    VToolCutSpline(VPattern *doc, VContainer *data, const quint32 &id, const QString &formula,
                   const quint32 &splineId, const quint32 &spl1id, const quint32 &spl2id,
                   const Source &typeCreation, QGraphicsItem * parent = nullptr);
    virtual void setDialog();
    static void  Create(DialogTool *dialog, VMainGraphicsScene  *scene, VPattern *doc, VContainer *data);
    static void  Create(const quint32 _id, const QString &pointName, QString &formula, const quint32 &splineId,
                        const qreal &mx, const qreal &my, VMainGraphicsScene  *scene, VPattern *doc, VContainer *data,
                        const Document &parse, const Source &typeCreation);
    static const QString ToolType;
    static const QString AttrSpline;
public slots:
    virtual void  FullUpdateFromFile();
    virtual void  CurveChoosed(quint32 id);
    virtual void  ShowContextMenu(QGraphicsSceneContextMenuEvent *event);
protected:
    virtual void  contextMenuEvent ( QGraphicsSceneContextMenuEvent * event );
    virtual void  AddToFile();
    virtual void  RefreshDataInFile();
    virtual void  SaveDialog(QDomElement &domElement);
    virtual void  RefreshCurve(VSimpleCurve *curve, quint32 curveId, SimpleCurvePoint tr);
private:
    Q_DISABLE_COPY(VToolCutSpline)
};

#endif // VTOOLCUTSPLINE_H
