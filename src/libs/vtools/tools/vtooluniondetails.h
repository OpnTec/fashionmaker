/************************************************************************
 **
 **  @file   vtooluniondetails.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   26 12, 2013
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

#ifndef VTOOLUNIONDETAILS_H
#define VTOOLUNIONDETAILS_H

#include <qcompilerdetection.h>
#include <QDomElement>
#include <QDomNode>
#include <QMetaObject>
#include <QObject>
#include <QPointF>
#include <QString>
#include <QVector>
#include <QtGlobal>

#include "../ifc/ifcdef.h"
#include "../ifc/xml/vabstractpattern.h"
#include "../vmisc/def.h"
#include "vabstracttool.h"
#include "../vpatterndb/vpiece.h"

class DialogTool;

#define UNION_VERSSION 2

struct VToolUnionDetailsInitData : VAbstractToolInitData
{
    VToolUnionDetailsInitData()
        : VAbstractToolInitData(),
          d1id(NULL_ID),
          d2id(NULL_ID),
          indexD1(NULL_ID),
          indexD2(NULL_ID),
          retainPieces(false),
          version(UNION_VERSSION)
    {}

    quint32 d1id;
    quint32 d2id;
    quint32 indexD1;
    quint32 indexD2;
    bool retainPieces;
    uint version;
};

/**
 * @brief The VToolUnionDetails class tool union details.
 */
class VToolUnionDetails : public VAbstractTool
{
    Q_OBJECT
public:
    static VToolUnionDetails *Create(QSharedPointer<DialogTool> dialog, VMainGraphicsScene *scene,
                                     VAbstractPattern *doc, VContainer *data);
    static VToolUnionDetails *Create(VToolUnionDetailsInitData initData);

    static const QString ToolType;
    static const QString TagDetail;
    static const QString TagNode;
    static const QString TagChildren;
    static const QString TagChild;
    static const QString AttrIndexD1;
    static const QString AttrIndexD2;
    static const QString AttrIdObject;
    static const QString AttrNodeType;
    static const QString NodeTypeContour;
    static const QString NodeTypeModeling;

    static const quint8 unionVersion;

    virtual QString getTagName() const override;
    virtual void ShowVisualization(bool show) override;
    virtual void incrementReferens() override;
    virtual void decrementReferens() override;
    virtual void GroupVisibility(quint32 object, bool visible) override;

    static QVector<QPair<bool, VPieceNode> > CalcUnitedPath(const VPiecePath &d1Path, const VPiecePath &d2Path,
                                                            quint32 indexD2, quint32 pRotate);
public slots:
    /**
     * @brief FullUpdateFromFile update tool data form file.
     */
    virtual void FullUpdateFromFile () override {}
    virtual void AllowHover(bool) override {}
    virtual void AllowSelecting(bool) override {}
protected:
    virtual void AddToFile() override;
    virtual void SetVisualization() override {}
private:
    Q_DISABLE_COPY(VToolUnionDetails)
    /** @brief d1 first detail id. */
    quint32 d1id;

    /** @brief d2 second detail id. */
    quint32 d2id;

    /** @brief indexD1 index edge in first detail. */
    quint32 indexD1;

    /** @brief indexD2 index edge in second detail. */
    quint32 indexD2;

    uint version;

    VToolUnionDetails(const VToolUnionDetailsInitData &initData, QObject *parent = nullptr);

    void             AddDetail(QDomElement &domElement, const VPiece &d) const;
    void             AddToModeling(const QDomElement &domElement);
    QVector<quint32> GetReferenceObjects() const;
    QVector<quint32> ReferenceObjects(const QDomElement &root, const QString &tag, const QString &attribute) const;
};

#endif // VTOOLUNIONDETAILS_H
