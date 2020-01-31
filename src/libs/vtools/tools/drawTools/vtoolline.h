/************************************************************************
 **
 **  @file   vtoolline.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   November 15, 2013
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentina project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2013-2015 Valentina project
 **  <https://gitlab.com/smart-pattern/valentina> All Rights Reserved.
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

#include <qcompilerdetection.h>
#include <QDomElement>
#include <QMetaObject>
#include <QObject>
#include <QString>
#include <QVariant>
#include <QtGlobal>

#include "../ifc/xml/vabstractpattern.h"
#include "../vmisc/def.h"
#include "vdrawtool.h"
#include "../vwidgets/scalesceneitems.h"

template <class T> class QSharedPointer;

struct VToolLineInitData : VAbstractToolInitData
{
    VToolLineInitData()
        : VAbstractToolInitData(),
          firstPoint(NULL_ID),
          secondPoint(NULL_ID),
          typeLine(TypeLineLine),
          lineColor(ColorBlack)
    {}

    quint32 firstPoint;
    quint32 secondPoint;
    QString typeLine;
    QString lineColor;
};

/**
 * @brief The VToolLine class tool for creation line.
 */
class VToolLine: public VDrawTool, public VScaledLine
{
    Q_OBJECT
public:
    virtual void     setDialog() override;
    static VToolLine *Create(const QPointer<DialogTool> &dialog, VMainGraphicsScene  *scene, VAbstractPattern *doc,
                             VContainer *data);
    static VToolLine *Create(VToolLineInitData initData);

    virtual int      type() const override {return Type;}
    enum { Type = UserType + static_cast<int>(Tool::Line)};
    virtual QString  getTagName() const override;

    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                       QWidget *widget = nullptr) override;

    QString FirstPointName() const;
    QString SecondPointName() const;

    QString GetLineColor() const;
    void    SetLineColor(const QString &value);

    virtual void     ShowVisualization(bool show) override;

    virtual void     SetTypeLine(const QString &value) override;
    virtual void     GroupVisibility(quint32 object, bool visible) override;
public slots:
    virtual void     FullUpdateFromFile() override;
    virtual void     ShowTool(quint32 id, bool enable) override;
    virtual void     Disable(bool disable, const QString &namePP) override;
    virtual void     AllowHover(bool enabled) override;
    virtual void     AllowSelecting(bool enabled) override;
protected slots:
    virtual void ShowContextMenu(QGraphicsSceneContextMenuEvent *event, quint32 id=NULL_ID) override;
protected:
    virtual void     contextMenuEvent ( QGraphicsSceneContextMenuEvent * event ) override;
    virtual void     AddToFile() override;
    virtual void     hoverEnterEvent ( QGraphicsSceneHoverEvent * event ) override;
    virtual void     hoverLeaveEvent ( QGraphicsSceneHoverEvent * event ) override;
    virtual void     RemoveReferens() override;
    virtual QVariant itemChange ( GraphicsItemChange change, const QVariant &value ) override;
    virtual void     keyReleaseEvent(QKeyEvent * event) override;
    virtual void     SaveDialog(QDomElement &domElement, QList<quint32> &oldDependencies,
                                QList<quint32> &newDependencies) override;
    virtual void     SaveOptions(QDomElement &tag, QSharedPointer<VGObject> &obj) override;
    virtual void     ReadToolAttributes(const QDomElement &domElement) override;
    virtual void     SetVisualization() override;
    virtual QString  MakeToolTip() const override;
private:
    Q_DISABLE_COPY(VToolLine)

    /** @brief firstPoint id first line point. */
    quint32           firstPoint;

    /** @brief secondPoint id second line point. */
    quint32           secondPoint;

    /** @brief lineColor color of a line. */
    QString           lineColor;

    bool m_acceptHoverEvents;

    VToolLine(const VToolLineInitData &initData, QGraphicsItem *parent = nullptr);

    void RefreshGeometry();
};

#endif // VTOOLLINE_H
