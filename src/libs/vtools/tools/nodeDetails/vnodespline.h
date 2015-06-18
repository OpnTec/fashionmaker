/************************************************************************
 **
 **  @file   vnodespline.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   November 15, 2013
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

#ifndef VNODESPLINE_H
#define VNODESPLINE_H

#include "vabstractnode.h"
#include <QGraphicsPathItem>

/**
 * @brief The VNodeSpline class spline detail node.
 */
class VNodeSpline:public VAbstractNode, public QGraphicsPathItem
{
    Q_OBJECT
public:
    VNodeSpline(VAbstractPattern *doc, VContainer *data, quint32 id, quint32 idSpline, const Source &typeCreation,
                const quint32 &idTool = 0, QObject *qoParent = nullptr, QGraphicsItem * parent = nullptr);

    static VNodeSpline *Create(VAbstractPattern *doc, VContainer *data, quint32 id, quint32 idSpline,
                               const Document &parse,
                               const Source &typeCreation, const quint32 &idTool = 0, QObject *parent = nullptr);
    static const QString TagName;
    static const QString ToolType;
    virtual void DeleteNode();
    virtual void RestoreNode();
    virtual int  type() const {return Type;}
    enum { Type = UserType + static_cast<int>(Tool::NodeSpline)};
    virtual QString getTagName() const;
public slots:
    virtual void FullUpdateFromFile ();
protected:
    virtual void AddToFile ();
    virtual void RefreshDataInFile();
    virtual void mouseReleaseEvent ( QGraphicsSceneMouseEvent * event );
    virtual void hoverMoveEvent ( QGraphicsSceneHoverEvent * event );
    virtual void hoverLeaveEvent ( QGraphicsSceneHoverEvent * event );
private:
    void         RefreshGeometry ();
};

#endif // VNODESPLINE_H
