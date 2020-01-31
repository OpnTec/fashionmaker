/************************************************************************
 **
 **  @file
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   31 1, 2017
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentina project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2017 Valentina project
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

#ifndef VTOOLPIN_H
#define VTOOLPIN_H

#include <QtGlobal>

#include "vabstractnode.h"

class DialogTool;

struct VToolPinInitData : VAbstractNodeInitData
{
    VToolPinInitData()
        : VAbstractNodeInitData(),
          pointId(NULL_ID)
    {}

    quint32 pointId;
};

class VToolPin : public VAbstractNode
{
    Q_OBJECT
public:
    static VToolPin* Create(const QPointer<DialogTool> &dialog, VAbstractPattern *doc, VContainer *data);
    static VToolPin *Create(VToolPinInitData initData);

    static const QString ToolType;
    virtual QString getTagName() const override;
public slots:
    virtual void FullUpdateFromFile () override {}
    virtual void AllowHover(bool enabled) override;
    virtual void AllowSelecting(bool enabled) override;
protected:
    virtual void AddToFile() override;
    virtual void ShowNode() override {}
    virtual void HideNode() override {}
private:
    Q_DISABLE_COPY(VToolPin)

    quint32 m_pieceId;

    VToolPin(const VToolPinInitData &initData, QObject *qoParent = nullptr);
};

#endif // VTOOLPIN_H
