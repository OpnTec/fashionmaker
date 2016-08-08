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

#ifndef VTOOLBASEPOINT_H
#define VTOOLBASEPOINT_H

#include <qcompilerdetection.h>
#include <QDomElement>
#include <QGraphicsItem>
#include <QMetaObject>
#include <QObject>
#include <QString>
#include <QVariant>
#include <Qt>
#include <QtGlobal>

#include "../vwidgets/../vmisc/def.h"
#include "tools/drawTools/toolpoint/toolsinglepoint/../../../../undocommands/../../ifc/xml/vabstractpattern.h"
#include "vtoolsinglepoint.h"

class QDomElement;
class QGraphicsSceneContextMenuEvent;
class QGraphicsSceneHoverEvent;
class QGraphicsSceneMouseEvent;
class VContainer;
class VGObject;
class VMainGraphicsScene;
class VPointF;
template <class T> class QSharedPointer;

/**
 * @brief The VToolBasePoint class tool for creation pattern base point. Only base point can move. All object
 * pattern peace depend on base point.
 */
class VToolBasePoint : public VToolSinglePoint
{
    Q_OBJECT
public:
    virtual ~VToolBasePoint() Q_DECL_OVERRIDE;
    virtual void setDialog() Q_DECL_OVERRIDE;
    static VToolBasePoint *Create(quint32 _id, const QString &nameActivPP, VPointF *point,
                                  VMainGraphicsScene *scene, VAbstractPattern *doc, VContainer *data,
                                  const Document &parse, const Source &typeCreation);
    static const QString ToolType;
    virtual int  type() const Q_DECL_OVERRIDE {return Type;}
    enum { Type = UserType + static_cast<int>(Tool::BasePoint)};
    virtual void ShowVisualization(bool show) Q_DECL_OVERRIDE;
    virtual void decrementReferens() Q_DECL_OVERRIDE;
public slots:
    virtual void FullUpdateFromFile() Q_DECL_OVERRIDE;
    virtual void SetFactor(qreal factor) Q_DECL_OVERRIDE;
    virtual void EnableToolMove(bool move) Q_DECL_OVERRIDE;
signals:
    /**
     * @brief FullUpdateTree handle if need update pattern file.
     */
    void         LiteUpdateTree();
protected:
    virtual void contextMenuEvent ( QGraphicsSceneContextMenuEvent * event ) Q_DECL_OVERRIDE;
    virtual void AddToFile() Q_DECL_OVERRIDE;
    virtual QVariant itemChange ( GraphicsItemChange change, const QVariant &value ) Q_DECL_OVERRIDE;
    virtual void DeleteTool(bool ask = true) Q_DECL_OVERRIDE;
    virtual void SaveDialog(QDomElement &domElement) Q_DECL_OVERRIDE;
    virtual void hoverEnterEvent ( QGraphicsSceneHoverEvent * event ) Q_DECL_OVERRIDE;
    virtual void hoverLeaveEvent ( QGraphicsSceneHoverEvent * event ) Q_DECL_OVERRIDE;
    virtual void mousePressEvent( QGraphicsSceneMouseEvent * event ) Q_DECL_OVERRIDE;
    virtual void mouseReleaseEvent ( QGraphicsSceneMouseEvent * event ) Q_DECL_OVERRIDE;
    virtual void SaveOptions(QDomElement &tag, QSharedPointer<VGObject> &obj) Q_DECL_OVERRIDE;
    virtual void ReadToolAttributes(const QDomElement &domElement) Q_DECL_OVERRIDE;
    virtual void SetVisualization() Q_DECL_OVERRIDE {}
private:
    Q_DISABLE_COPY(VToolBasePoint)

    QString namePP;

    VToolBasePoint (VAbstractPattern *doc, VContainer *data, quint32 id, const Source &typeCreation,
                      const QString &namePP, QGraphicsItem * parent = nullptr );

    void         SetColorLabel(const Qt::GlobalColor & color);
};

#endif // VTOOLBASEPOINT_H
