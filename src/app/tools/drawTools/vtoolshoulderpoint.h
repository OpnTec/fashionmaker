/************************************************************************
 **
 **  @file   vtoolshoulderpoint.h
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

#ifndef VTOOLSHOULDERPOINT_H
#define VTOOLSHOULDERPOINT_H

#include "vtoollinepoint.h"

/**
 * @brief The VToolShoulderPoint class tool for creation point on shoulder. This tool for special situation, when you
 * want find point along line, but have only length from another point (shoulder).
 */
class VToolShoulderPoint : public VToolLinePoint
{
    Q_OBJECT
public:
    VToolShoulderPoint(VPattern *doc, VContainer *data, const quint32 &id, const QString &typeLine,
                       const QString &formula, const quint32 &p1Line, const quint32 &p2Line, const quint32 &pShoulder,
                       const Source &typeCreation, QGraphicsItem * parent = nullptr);
    virtual void   setDialog();
    static QPointF FindPoint(const QPointF &p1Line, const QPointF &p2Line, const QPointF &pShoulder,
                             const qreal &length);
    static VToolShoulderPoint* Create(DialogTool *dialog, VMainGraphicsScene  *scene, VPattern *doc, VContainer *data);
    static VToolShoulderPoint* Create(const quint32 _id, QString &formula, const quint32 &p1Line, const quint32 &p2Line,
                                      const quint32 &pShoulder, const QString &typeLine, const QString &pointName,
                                      const qreal &mx, const qreal &my, VMainGraphicsScene *scene, VPattern *doc,
                                      VContainer *data, const Document &parse, const Source &typeCreation);
    static const QString ToolType;
    virtual int    type() const {return Type;}
    enum { Type = UserType + static_cast<int>(Tool::ShoulderPoint) };

    quint32 GetP2Line() const;
    void    SetP2Line(const quint32 &value);

    quint32 getPShoulder() const;
    void    setPShoulder(const quint32 &value);

    virtual void   ShowVisualization(bool show);
public slots:
    virtual void   FullUpdateFromFile();
    virtual void   SetFactor(qreal factor);
    virtual void   ShowContextMenu(QGraphicsSceneContextMenuEvent *event);
protected:
    virtual void   contextMenuEvent ( QGraphicsSceneContextMenuEvent * event );
    virtual void   RemoveReferens();
    virtual void   SaveDialog(QDomElement &domElement);
    virtual void   SaveOptions(QDomElement &tag, QSharedPointer<VGObject> &obj);
private:
    /** @brief p2Line id second line point. */
    quint32         p2Line;

    /** @brief pShoulder id shoulder line point. */
    quint32         pShoulder;
};

#endif // VTOOLSHOULDERPOINT_H
