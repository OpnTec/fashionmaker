/************************************************************************
 **
 **  @file   vtoolline.h
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

#ifndef VTOOLLINE_H
#define VTOOLLINE_H

#include "vdrawtool.h"
#include <QGraphicsLineItem>

/**
 * @brief The VToolLine class tool for creation line.
 */
class VToolLine: public VDrawTool, public QGraphicsLineItem
{
    Q_OBJECT
public:
    VToolLine(VPattern *doc, VContainer *data, quint32 id, quint32 firstPoint, quint32 secondPoint,
              const QString &typeLine, const QString &lineColor, const Source &typeCreation,
              QGraphicsItem * parent = nullptr);
    virtual void     setDialog();
    static VToolLine *Create(DialogTool *dialog, VMainGraphicsScene  *scene, VPattern *doc, VContainer *data);
    static VToolLine *Create(const quint32 &_id, const quint32 &firstPoint, const quint32 &secondPoint,
                             const QString &typeLine, const QString &lineColor, VMainGraphicsScene  *scene,
                             VPattern *doc, VContainer *data, const Document &parse, const Source &typeCreation);
    static const QString TagName;
    virtual void     paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget = 0);
    virtual int      type() const {return Type;}
    enum { Type = UserType + static_cast<int>(Tool::Line)};
    virtual QString  getTagName() const;
    quint32 GetFirstPoint() const;
    void    SetFirstPoint(const quint32 &value);

    quint32 GetSecondPoint() const;
    void    SetSecondPoint(const quint32 &value);

    virtual void     ShowVisualization(bool show);

    virtual void     SetTypeLine(const QString &value);
    virtual void     SetLineColor(const QString &value);
public slots:
    virtual void     FullUpdateFromFile();
    virtual void     ChangedActivDraw(const QString &newName);
    virtual void     ShowTool(quint32 id, bool enable);
    virtual void     SetFactor(qreal factor);
    void             Disable(bool disable);
protected:
    virtual void     contextMenuEvent ( QGraphicsSceneContextMenuEvent * event );
    virtual void     AddToFile();
    virtual void     RefreshDataInFile();
    virtual void     hoverEnterEvent ( QGraphicsSceneHoverEvent * event );
    virtual void     hoverLeaveEvent ( QGraphicsSceneHoverEvent * event );
    virtual void     RemoveReferens();
    virtual QVariant itemChange ( GraphicsItemChange change, const QVariant &value );
    virtual void     keyReleaseEvent(QKeyEvent * event);
    virtual void     SaveDialog(QDomElement &domElement);
    virtual void     SaveOptions(QDomElement &tag, QSharedPointer<VGObject> &obj);
    virtual void     ReadToolAttributes(const QDomElement &domElement);
private:
    /** @brief firstPoint id first line point. */
    quint32           firstPoint;

    /** @brief secondPoint id second line point. */
    quint32           secondPoint;

    void             RefreshGeometry();
};

#endif // VTOOLLINE_H
