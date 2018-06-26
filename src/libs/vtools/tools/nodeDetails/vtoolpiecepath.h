/************************************************************************
 **
 **  @file
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   24 11, 2016
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

#ifndef VTOOLPIECEPATH_H
#define VTOOLPIECEPATH_H

#include <QtGlobal>

#include "vabstractnode.h"

class DialogTool;

struct VToolPiecePathInitData : VAbstractNodeInitData
{
    VToolPiecePathInitData()
        : VAbstractNodeInitData(),
          path()
    {}

    VPiecePath path;
};

class VToolPiecePath : public VAbstractNode, public QGraphicsPathItem
{
    Q_OBJECT
public:
    static VToolPiecePath* Create(QSharedPointer<DialogTool> dialog, VMainGraphicsScene *scene, VAbstractPattern *doc,
                                  VContainer *data);
    static VToolPiecePath *Create(VToolPiecePathInitData initData);

    virtual int  type() const override {return Type;}
    enum { Type = UserType + static_cast<int>(Tool::PiecePath)};
    virtual QString getTagName() const override;

    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                       QWidget *widget = nullptr) override;

    virtual void incrementReferens() override;
    virtual void decrementReferens() override;

    void RefreshGeometry();

    static void AddAttributes(VAbstractPattern *doc, QDomElement &domElement, quint32 id, const VPiecePath &path);
public slots:
    virtual void FullUpdateFromFile () override;
    virtual void AllowHover(bool enabled) override;
    virtual void AllowSelecting(bool enabled) override;
protected:
    virtual void AddToFile() override;
    virtual void ShowNode() override;
    virtual void HideNode() override;
    virtual void ToolCreation(const Source &typeCreation) override;
private:
    Q_DISABLE_COPY(VToolPiecePath)

    quint32 m_pieceId;

    VToolPiecePath(const VToolPiecePathInitData &initData, QObject *qoParent = nullptr,
                   QGraphicsItem *parent = nullptr);

    void IncrementNodes(const VPiecePath &path) const;
    void DecrementNodes(const VPiecePath &path) const;
};

#endif // VTOOLPIECEPATH_H
