/************************************************************************
 **
 **  @file   vtoolsinglepoint.h
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

#ifndef VTOOLSINGLEPOINT_H
#define VTOOLSINGLEPOINT_H

#include "vtoolpoint.h"

/**
 * @brief The VToolSinglePoint class tool for creation pattern base point. Obly base point can move. All object
 * pattern peace depend on base point.
 */
class VToolSinglePoint : public VToolPoint
{
    Q_OBJECT
public:
    VToolSinglePoint (VPattern *doc, VContainer *data, quint32 id, const Source &typeCreation,
                      const QString &namePP, const QString &mPath, QGraphicsItem * parent = nullptr );
    virtual void setDialog();
    static const QString ToolType;
    QString      name() const;
    void         setName(const QString &name);
    virtual int  type()const;
    enum { Type = UserType + static_cast<int>(Tool::SinglePoint)};
public slots:
    virtual void FullUpdateFromFile();
    virtual void ChangedActivDraw(const QString &newName);
    virtual void SetFactor(qreal factor);
    virtual void ShowContextMenu(QGraphicsSceneContextMenuEvent *event);
signals:
    /**
     * @brief FullUpdateTree handle if need update pattern file.
     */
    void         LiteUpdateTree();
protected:
    virtual void contextMenuEvent ( QGraphicsSceneContextMenuEvent * event );
    virtual void AddToFile();
    virtual void RefreshDataInFile();
    QVariant     itemChange ( GraphicsItemChange change, const QVariant &value );
    virtual void decrementReferens();
    virtual void DeleteTool(bool ask = true);
    virtual void SaveDialog(QDomElement &domElement);
    virtual void hoverEnterEvent ( QGraphicsSceneHoverEvent * event );
    virtual void hoverLeaveEvent ( QGraphicsSceneHoverEvent * event );
    virtual void mousePressEvent( QGraphicsSceneMouseEvent * event );
    virtual void mouseReleaseEvent ( QGraphicsSceneMouseEvent * event );
private:
    QString namePP;
    QString mPath;
    void         setColorLabel(const Qt::GlobalColor & color);
    void         SaveOptions(QDomElement &tag, const VPointF &point);
};

#endif // VTOOLSINGLEPOINT_H
