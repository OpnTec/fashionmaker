/************************************************************************
 **
 **  @file
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   12 9, 2016
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentina project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2016 Valentina project
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

#ifndef VTOOLFLIPPINGBYLINE_H
#define VTOOLFLIPPINGBYLINE_H

#include <QtGlobal>

#include "vabstractflipping.h"

struct VToolFlippingByLineInitData : VAbstractOperationInitData
{
    VToolFlippingByLineInitData()
        : VAbstractOperationInitData(),
          firstLinePointId(NULL_ID),
          secondLinePointId(NULL_ID)
    {}

    quint32 firstLinePointId;
    quint32 secondLinePointId;
};


class VToolFlippingByLine : public VAbstractFlipping
{
    Q_OBJECT
public:
    virtual ~VToolFlippingByLine() Q_DECL_EQ_DEFAULT;
    virtual void setDialog() override;
    static VToolFlippingByLine* Create(const QPointer<DialogTool> &dialog, VMainGraphicsScene *scene,
                                       VAbstractPattern *doc, VContainer *data);
    static VToolFlippingByLine* Create(VToolFlippingByLineInitData initData);

    static const QString ToolType;

    virtual int type() const override {return Type;}
    enum { Type = UserType + static_cast<int>(Tool::FlippingByLine)};

    QString FirstLinePointName() const;
    QString SecondLinePointName() const;

    virtual void ShowVisualization(bool show) override;
protected slots:
    virtual void ShowContextMenu(QGraphicsSceneContextMenuEvent *event, quint32 id=NULL_ID) override;
protected:
    virtual void    SetVisualization() override;
    virtual void    SaveDialog(QDomElement &domElement, QList<quint32> &oldDependencies,
                               QList<quint32> &newDependencies) override;
    virtual void    ReadToolAttributes(const QDomElement &domElement) override;
    virtual void    SaveOptions(QDomElement &tag, QSharedPointer<VGObject> &obj) override;
    virtual QString MakeToolTip() const override;
private:
    Q_DISABLE_COPY(VToolFlippingByLine)

    quint32 m_firstLinePointId;
    quint32 m_secondLinePointId;

    VToolFlippingByLine(const VToolFlippingByLineInitData &initData, QGraphicsItem *parent = nullptr);
};

#endif // VTOOLFLIPPINGBYLINE_H
