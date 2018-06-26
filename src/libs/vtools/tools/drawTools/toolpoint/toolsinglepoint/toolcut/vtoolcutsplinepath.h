/************************************************************************
 **
 **  @file   vtoolcutsplinepath.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   15 12, 2013
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

#ifndef VTOOLCUTSPLINEPATH_H
#define VTOOLCUTSPLINEPATH_H

#include <qcompilerdetection.h>
#include <QDomElement>
#include <QGraphicsItem>
#include <QMetaObject>
#include <QObject>
#include <QString>
#include <QtGlobal>

#include "../ifc/xml/vabstractpattern.h"
#include "../vmisc/def.h"
#include "vtoolcut.h"

class VSplinePath;
template <class T> class QSharedPointer;

struct VToolCutSplinePathInitData : VToolSinglePointInitData
{
    VToolCutSplinePathInitData()
        : VToolSinglePointInitData(),
          formula(),
          splinePathId(NULL_ID)
    {}

    QString formula;
    quint32 splinePathId;
};

/**
 * @brief The VToolCutSplinePath class for tool CutSplinePath. This tool find point on splinePath and cut splinePath on
 * two.
 */
class VToolCutSplinePath : public VToolCut
{
    Q_OBJECT
public:
    virtual void setDialog() override;
    static VToolCutSplinePath *Create(QSharedPointer<DialogTool> dialog, VMainGraphicsScene  *scene,
                                      VAbstractPattern *doc, VContainer *data);
    static VToolCutSplinePath *Create(VToolCutSplinePathInitData &initData);
    static const QString ToolType;
    static const QString AttrSplinePath;
    virtual int  type() const override {return Type;}
    enum { Type = UserType + static_cast<int>(Tool::CutSplinePath)};
    virtual void  ShowVisualization(bool show) override;

    Q_REQUIRED_RESULT static VPointF *CutSplinePath(qreal length,
                                                    const QSharedPointer<VAbstractCubicBezierPath> &splPath,
                                                    const QString &pName, VSplinePath **splPath1,
                                                    VSplinePath **splPath2);
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
    Q_DISABLE_COPY(VToolCutSplinePath)

    VToolCutSplinePath(const VToolCutSplinePathInitData &initData, QGraphicsItem *parent = nullptr);
};

#endif // VTOOLCUTSPLINEPATH_H
