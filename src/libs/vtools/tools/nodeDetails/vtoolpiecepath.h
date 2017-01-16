/************************************************************************
 **
 **  @file
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   24 11, 2016
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentine project, a pattern making
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

class VToolPiecePath : public VAbstractNode, public QGraphicsPathItem
{
    Q_OBJECT
public:
    static VToolPiecePath* Create(DialogTool *dialog, VMainGraphicsScene *scene, VAbstractPattern *doc,
                                  VContainer *data);
    static VToolPiecePath *Create(quint32 _id, const VPiecePath &path, quint32 pieceId, VMainGraphicsScene *scene,
                                  VAbstractPattern *doc, VContainer *data, const Document &parse,
                                  const Source &typeCreation, const QString &drawName = QString(),
                                  const quint32 &idTool = 0);

    virtual int  type() const Q_DECL_OVERRIDE {return Type;}
    enum { Type = UserType + static_cast<int>(Tool::PiecePath)};
    virtual QString getTagName() const Q_DECL_OVERRIDE;

    virtual void incrementReferens() Q_DECL_OVERRIDE;
    virtual void decrementReferens() Q_DECL_OVERRIDE;

    static void AddAttributes(VAbstractPattern *doc, QDomElement &domElement, quint32 id, const VPiecePath &path);
public slots:
    virtual void FullUpdateFromFile () Q_DECL_OVERRIDE;
    virtual void AllowHover(bool enabled) Q_DECL_OVERRIDE;
    virtual void AllowSelecting(bool enabled) Q_DECL_OVERRIDE;
protected:
    virtual void AddToFile() Q_DECL_OVERRIDE;
    virtual void RefreshDataInFile() Q_DECL_OVERRIDE;
    virtual void ShowNode() Q_DECL_OVERRIDE;
    virtual void HideNode() Q_DECL_OVERRIDE;
private:
    Q_DISABLE_COPY(VToolPiecePath)

    quint32 m_pieceId;

    VToolPiecePath(VAbstractPattern *doc, VContainer *data, quint32 id, quint32 pieceId,  const Source &typeCreation,
                   const QString &drawName = QString(), const quint32 &idTool = 0, QObject *qoParent = nullptr,
                   QGraphicsItem * parent = nullptr );

    void RefreshGeometry();

    void IncrementNodes(const VPiecePath &path) const;
    void DecrementNodes(const VPiecePath &path) const;
};

#endif // VTOOLPIECEPATH_H
