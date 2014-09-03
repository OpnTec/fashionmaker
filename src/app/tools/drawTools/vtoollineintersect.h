/************************************************************************
 **
 **  @file   vtoollineintersect.h
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

#ifndef VTOOLLINEINTERSECT_H
#define VTOOLLINEINTERSECT_H

#include "vtoolpoint.h"

/**
 * @brief The VToolLineIntersect class help find point intersection lines.
 */
class VToolLineIntersect:public VToolPoint
{
    Q_OBJECT
public:
    VToolLineIntersect(VPattern *doc, VContainer *data, const quint32 &id, const quint32 &p1Line1,
                       const quint32 &p2Line1, const quint32 &p1Line2, const quint32 &p2Line2,
                       const Source &typeCreation, QGraphicsItem * parent = nullptr);
    virtual void setDialog();
    static VToolLineIntersect *Create(DialogTool *dialog, VMainGraphicsScene *scene, VPattern *doc, VContainer *data);
    static VToolLineIntersect *Create(const quint32 _id, const quint32 &p1Line1Id, const quint32 &p2Line1Id,
                                      const quint32 &p1Line2Id, const quint32 &p2Line2Id, const QString &pointName,
                                      const qreal &mx, const qreal &my, VMainGraphicsScene  *scene, VPattern *doc,
                                      VContainer *data, const Document &parse, const Source &typeCreation);
    static const QString ToolType;
    virtual int  type() const {return Type;}
    enum { Type = UserType + static_cast<int>(Tool::LineIntersect)};

    quint32 getP1Line1() const;
    void    setP1Line1(const quint32 &value);

    quint32 getP2Line1() const;
    void    setP2Line1(const quint32 &value);

    quint32 getP1Line2() const;
    void    setP1Line2(const quint32 &value);

    quint32 getP2Line2() const;
    void    setP2Line2(const quint32 &value);

    virtual void ShowVisualization(bool show);
public slots:
    virtual void FullUpdateFromFile();
    virtual void SetFactor(qreal factor);
    virtual void ShowContextMenu(QGraphicsSceneContextMenuEvent *event);
protected:
    virtual void contextMenuEvent ( QGraphicsSceneContextMenuEvent * event );
    virtual void RemoveReferens();
    virtual void SaveDialog(QDomElement &domElement);
    virtual void SaveOptions(QDomElement &tag, QSharedPointer<VGObject> &obj);
private:
    /** @brief p1Line1 id first point first line. */
    quint32       p1Line1;

    /** @brief p2Line1 id second point first line. */
    quint32       p2Line1;

    /** @brief p1Line2 id first point second line. */
    quint32       p1Line2;

    /** @brief p2Line2 id second point second line.*/
    quint32       p2Line2;
};

#endif // VTOOLLINEINTERSECT_H
