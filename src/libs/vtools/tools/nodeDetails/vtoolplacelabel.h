/************************************************************************
 **
 **  @file   vtoolplacelabel.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   15 10, 2017
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentina project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2017 Valentina project
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
#ifndef VTOOLPLACELABEL_H
#define VTOOLPLACELABEL_H

#include <QtGlobal>

#include "vabstractnode.h"
#include "../vgeometry/vplacelabelitem.h"

class DialogTool;

struct VToolPlaceLabelInitData : VAbstractNodeInitData
{
    VToolPlaceLabelInitData()
        : VAbstractNodeInitData(),
          width('0'),
          height('0'),
          angle('0'),
          type(PlaceLabelType::Button),
          centerPoint(NULL_ID),
          visibilityTrigger('1')
    {}

    QString width;
    QString height;
    QString angle;
    PlaceLabelType type;
    quint32 centerPoint;
    QString visibilityTrigger;
};

class VToolPlaceLabel : public VAbstractNode
{
    Q_OBJECT
public:
    static VToolPlaceLabel* Create(QSharedPointer<DialogTool> dialog, VAbstractPattern *doc, VContainer *data);
    static VToolPlaceLabel* Create(VToolPlaceLabelInitData &initData);

    static const QString ToolType;
    virtual QString getTagName() const override;

    static void AddAttributes(VAbstractPattern *doc, QDomElement &domElement, quint32 id, const VPlaceLabelItem &label);
public slots:
    virtual void FullUpdateFromFile () override {}
    virtual void AllowHover(bool enabled) override;
    virtual void AllowSelecting(bool enabled) override;
protected:
    virtual void AddToFile() override;
    virtual void ShowNode() override {}
    virtual void HideNode() override {}
private:
    Q_DISABLE_COPY(VToolPlaceLabel)

    quint32 m_pieceId;

    VToolPlaceLabel(const VToolPlaceLabelInitData &initData, QObject *qoParent = nullptr);
};

#endif // VTOOLPLACELABEL_H
