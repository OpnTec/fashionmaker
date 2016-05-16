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
    virtual void     setDialog() Q_DECL_OVERRIDE;
    static VToolLine *Create(DialogTool *dialog, VMainGraphicsScene  *scene, VAbstractPattern *doc, VContainer *data);
    static VToolLine *Create(const quint32 &_id, const quint32 &firstPoint, const quint32 &secondPoint,
                             const QString &typeLine, const QString &lineColor, VMainGraphicsScene  *scene,
                             VAbstractPattern *doc, VContainer *data, const Document &parse,
                             const Source &typeCreation);

    virtual int      type() const Q_DECL_OVERRIDE {return Type;}
    enum { Type = UserType + static_cast<int>(Tool::Line)};
    virtual QString  getTagName() const Q_DECL_OVERRIDE;
    quint32 GetFirstPoint() const;
    void    SetFirstPoint(const quint32 &value);

    quint32 GetSecondPoint() const;
    void    SetSecondPoint(const quint32 &value);

    virtual void     ShowVisualization(bool show) Q_DECL_OVERRIDE;

    virtual void     SetTypeLine(const QString &value) Q_DECL_OVERRIDE;
    virtual void     SetLineColor(const QString &value) Q_DECL_OVERRIDE;
    virtual void     GroupVisibility(quint32 object, bool visible) Q_DECL_OVERRIDE;
public slots:
    virtual void     FullUpdateFromFile() Q_DECL_OVERRIDE;
    virtual void     ShowTool(quint32 id, bool enable) Q_DECL_OVERRIDE;
    virtual void     SetFactor(qreal factor) Q_DECL_OVERRIDE;
    virtual void     Disable(bool disable, const QString &namePP) Q_DECL_OVERRIDE;
    virtual void     AllowHover(bool enabled) Q_DECL_OVERRIDE;
    virtual void     AllowSelecting(bool enabled) Q_DECL_OVERRIDE;
protected:
    virtual void     contextMenuEvent ( QGraphicsSceneContextMenuEvent * event ) Q_DECL_OVERRIDE;
    virtual void     AddToFile() Q_DECL_OVERRIDE;
    virtual void     RefreshDataInFile() Q_DECL_OVERRIDE;
    virtual void     hoverEnterEvent ( QGraphicsSceneHoverEvent * event ) Q_DECL_OVERRIDE;
    virtual void     hoverLeaveEvent ( QGraphicsSceneHoverEvent * event ) Q_DECL_OVERRIDE;
    virtual void     RemoveReferens() Q_DECL_OVERRIDE;
    virtual QVariant itemChange ( GraphicsItemChange change, const QVariant &value ) Q_DECL_OVERRIDE;
    virtual void     keyReleaseEvent(QKeyEvent * event) Q_DECL_OVERRIDE;
    virtual void     SaveDialog(QDomElement &domElement) Q_DECL_OVERRIDE;
    virtual void     SaveOptions(QDomElement &tag, QSharedPointer<VGObject> &obj) Q_DECL_OVERRIDE;
    virtual void     ReadToolAttributes(const QDomElement &domElement) Q_DECL_OVERRIDE;
    virtual void     SetVisualization() Q_DECL_OVERRIDE;
private:
    Q_DISABLE_COPY(VToolLine)

    /** @brief firstPoint id first line point. */
    quint32           firstPoint;

    /** @brief secondPoint id second line point. */
    quint32           secondPoint;

    VToolLine(VAbstractPattern *doc, VContainer *data, quint32 id, quint32 firstPoint, quint32 secondPoint,
              const QString &typeLine, const QString &lineColor, const Source &typeCreation,
              QGraphicsItem * parent = nullptr);

    void             RefreshGeometry();
};

#endif // VTOOLLINE_H
