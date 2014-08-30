/************************************************************************
 **
 **  @file   vtoolspline.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   November 15, 2013
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

#ifndef VTOOLSPLINE_H
#define VTOOLSPLINE_H

#include "vabstractspline.h"

/**
 * @brief The VToolSpline class tool for creation spline. I mean bezier curve.
 */
class VToolSpline:public VAbstractSpline
{
    Q_OBJECT
public:

                 VToolSpline (VPattern *doc, VContainer *data, quint32 id, const Source &typeCreation,
                              QGraphicsItem * parent = nullptr );
    virtual void setDialog();
    static VToolSpline *Create(DialogTool *dialog, VMainGraphicsScene  *scene, VPattern *doc, VContainer *data);
    static VToolSpline *Create(const quint32 _id, const quint32 &p1, const quint32 &p4, const qreal &kAsm1,
                               const qreal kAsm2, const qreal &angle1, const qreal &angle2, const qreal &kCurve,
                               VMainGraphicsScene  *scene, VPattern *doc, VContainer *data,
                               const Document &parse, const Source &typeCreation);
    static const QString ToolType;
    virtual int  type() const {return Type;}
    enum { Type = UserType + static_cast<int>(Tool::Spline)};
public slots:
    void         ControlPointChangePosition (const qint32 &indexSpline, const SplinePointPosition &position,
                                             const QPointF &pos);
protected:
    virtual void contextMenuEvent ( QGraphicsSceneContextMenuEvent * event );
    virtual void RemoveReferens();
    virtual void SaveDialog(QDomElement &domElement);
    virtual void SaveOptions(QDomElement &tag, QSharedPointer<VGObject> &obj);
private:
    void         RefreshGeometry ();
};

#endif // VTOOLSPLINE_H
