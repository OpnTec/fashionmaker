/************************************************************************
 **
 **  @file
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   16 9, 2016
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentina project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2016 Valentina project
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

#ifndef VTOOLFLIPPINGBYAXIS_H
#define VTOOLFLIPPINGBYAXIS_H

#include <QtGlobal>

#include "vabstractflipping.h"

struct VToolFlippingByAxisInitData : VAbstractOperationInitData
{
    VToolFlippingByAxisInitData()
        : VAbstractOperationInitData(),
          originPointId(NULL_ID),
          axisType(AxisType::VerticalAxis)
    {}

    quint32 originPointId;
    AxisType axisType;
};

class VToolFlippingByAxis : public VAbstractFlipping
{
    Q_OBJECT
public:
    virtual ~VToolFlippingByAxis() Q_DECL_EQ_DEFAULT;
    virtual void setDialog() Q_DECL_OVERRIDE;
    static VToolFlippingByAxis* Create(QSharedPointer<DialogTool> dialog, VMainGraphicsScene *scene,
                                       VAbstractPattern *doc, VContainer *data);
    static VToolFlippingByAxis* Create(VToolFlippingByAxisInitData initData);

    static const QString ToolType;

    virtual int type() const Q_DECL_OVERRIDE {return Type;}
    enum { Type = UserType + static_cast<int>(Tool::FlippingByAxis)};

    AxisType GetAxisType() const;
    void     SetAxisType(AxisType value);

    QString OriginPointName() const;

    virtual void ShowVisualization(bool show) Q_DECL_OVERRIDE;
protected slots:
    virtual void ShowContextMenu(QGraphicsSceneContextMenuEvent *event, quint32 id=NULL_ID) Q_DECL_OVERRIDE;
protected:
    virtual void    SetVisualization() Q_DECL_OVERRIDE;
    virtual void    SaveDialog(QDomElement &domElement, QList<quint32> &oldDependencies,
                               QList<quint32> &newDependencies) Q_DECL_OVERRIDE;
    virtual void    ReadToolAttributes(const QDomElement &domElement) Q_DECL_OVERRIDE;
    virtual void    SaveOptions(QDomElement &tag, QSharedPointer<VGObject> &obj) Q_DECL_OVERRIDE;
    virtual QString MakeToolTip() const Q_DECL_OVERRIDE;
private:
    Q_DISABLE_COPY(VToolFlippingByAxis)

    quint32  m_originPointId;
    AxisType m_axisType;

    VToolFlippingByAxis(const VToolFlippingByAxisInitData &initData, QGraphicsItem *parent = nullptr);
};

#endif // VTOOLFLIPPINGBYAXIS_H
