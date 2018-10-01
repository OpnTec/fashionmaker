/************************************************************************
 **
 **  @file   vtoolheight.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   November 15, 2013
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentina project, a pattern making
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

#ifndef VTOOLHEIGHT_H
#define VTOOLHEIGHT_H

#include <qcompilerdetection.h>
#include <QDomElement>
#include <QGraphicsItem>
#include <QLineF>
#include <QMetaObject>
#include <QObject>
#include <QPointF>
#include <QString>
#include <QtGlobal>

#include "../ifc/xml/vabstractpattern.h"
#include "../vmisc/def.h"
#include "vtoollinepoint.h"

template <class T> class QSharedPointer;

struct VToolHeightInitData : VToolLinePointInitData
{
    VToolHeightInitData()
        : VToolLinePointInitData(),
          basePointId(NULL_ID),
          p1LineId(NULL_ID),
          p2LineId(NULL_ID)
    {}

    quint32 basePointId;
    quint32 p1LineId;
    quint32 p2LineId;
};

/**
 * @brief The VToolHeight class tool for creation point of height. Help find point of projection onto line.
 */
class VToolHeight: public VToolLinePoint
{
    Q_OBJECT
public:
    virtual void   setDialog() override;
    static VToolHeight *Create(const QPointer<DialogTool> &dialog, VMainGraphicsScene  *scene, VAbstractPattern *doc,
                               VContainer *data);
    static VToolHeight *Create(VToolHeightInitData initData);
    static QPointF FindPoint(const QLineF &line, const QPointF &point);
    static const QString ToolType;
    virtual int    type() const override {return Type;}
    enum { Type = UserType + static_cast<int>(Tool::Height)};

    QString FirstLinePointName() const;
    QString SecondLinePointName() const;

    virtual void   ShowVisualization(bool show) override;
protected slots:
    virtual void ShowContextMenu(QGraphicsSceneContextMenuEvent *event, quint32 id=NULL_ID) override;
protected:
    virtual void    SaveDialog(QDomElement &domElement, QList<quint32> &oldDependencies,
                               QList<quint32> &newDependencies) override;
    virtual void    SaveOptions(QDomElement &tag, QSharedPointer<VGObject> &obj) override;
    virtual void    ReadToolAttributes(const QDomElement &domElement) override;
    virtual void    SetVisualization() override;
    virtual QString MakeToolTip() const override;
private:
    Q_DISABLE_COPY(VToolHeight)

    /** @brief p1LineId id first point of line. */
    quint32         p1LineId;

    /** @brief p2LineId id second point of line. */
    quint32         p2LineId;

    VToolHeight(const VToolHeightInitData &initData, QGraphicsItem *parent = nullptr);
};

#endif // VTOOLHEIGHT_H
