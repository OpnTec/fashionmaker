/************************************************************************
 **
 **  @file   vtoolalongline.h
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

#ifndef VTOOLALONGLINE_H
#define VTOOLALONGLINE_H

#include "vtoollinepoint.h"

/**
 * @brief The VToolAlongLine class tool for creation point along line.
 */
class VToolAlongLine : public VToolLinePoint
{
    Q_OBJECT
public:

    VToolAlongLine(VPattern *doc, VContainer *data, quint32 id, const QString &formula, const quint32 &firstPointId,
                   const quint32 &secondPointId, const QString &typeLine, const Source &typeCreation,
                   QGraphicsItem * parent = nullptr);
    virtual void setDialog();
    static VToolAlongLine* Create(DialogTool *dialog, VMainGraphicsScene  *scene, VPattern *doc, VContainer *data);
    static VToolAlongLine* Create(const quint32 _id, const QString &pointName, const QString &typeLine,
                                  QString &formula, const quint32 &firstPointId, const quint32 &secondPointId,
                                  const qreal &mx, const qreal &my, VMainGraphicsScene  *scene, VPattern *doc,
                                  VContainer *data, const Document &parse, const Source &typeCreation);
    static const QString ToolType;
public slots:
    virtual void FullUpdateFromFile();
    virtual void SetFactor(qreal factor);
    virtual void ShowContextMenu(QGraphicsSceneContextMenuEvent *event);
protected:
    virtual void contextMenuEvent ( QGraphicsSceneContextMenuEvent * event );
    virtual void AddToFile();
    virtual void RefreshDataInFile();
    virtual void RemoveReferens();
    virtual void SaveDialog(QDomElement &domElement);
private:
    /** @brief secondPointId id second point of line. */
    quint32       secondPointId;
};

#endif // VTOOLALONGLINE_H
