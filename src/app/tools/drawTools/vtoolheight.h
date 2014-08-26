/************************************************************************
 **
 **  @file   vtoolheight.h
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

#ifndef VTOOLHEIGHT_H
#define VTOOLHEIGHT_H

#include "vtoollinepoint.h"

/**
 * @brief The VToolHeight class tool for creation point of height. Help find point of projection onto line.
 */
class VToolHeight: public VToolLinePoint
{
    Q_OBJECT
public:

    VToolHeight(VPattern *doc, VContainer *data, const quint32 &id, const QString &typeLine, const quint32 &basePointId,
                const quint32 &p1LineId, const quint32 &p2LineId, const Source &typeCreation,
                QGraphicsItem * parent = nullptr);
    virtual void   setDialog();
    static VToolHeight *Create(DialogTool *dialog, VMainGraphicsScene  *scene, VPattern *doc, VContainer *data);
    static VToolHeight *Create(const quint32 _id, const QString &pointName, const QString &typeLine,
                               const quint32 &basePointId, const quint32 &p1LineId, const quint32 &p2LineId,
                               const qreal &mx, const qreal &my, VMainGraphicsScene  *scene, VPattern *doc,
                               VContainer *data, const Document &parse, const Source &typeCreation);
    static QPointF FindPoint(const QLineF &line, const QPointF &point);
    static const QString ToolType;
    virtual int    type() const {return Type;}
    enum { Type = UserType + static_cast<int>(Tool::Height)};
public slots:
    virtual void   FullUpdateFromFile();
    virtual void   ShowContextMenu(QGraphicsSceneContextMenuEvent *event);
protected:
    virtual void   contextMenuEvent ( QGraphicsSceneContextMenuEvent * event );
    virtual void   AddToFile();
    virtual void   RefreshDataInFile();
    virtual void   SaveDialog(QDomElement &domElement);
private:
    /** @brief p1LineId id first point of line. */
    quint32         p1LineId;

    /** @brief p2LineId id second point of line. */
    quint32         p2LineId;
};

#endif // VTOOLHEIGHT_H
