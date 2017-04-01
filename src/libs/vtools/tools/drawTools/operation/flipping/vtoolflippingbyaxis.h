/************************************************************************
 **
 **  @file
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   16 9, 2016
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

#ifndef VTOOLFLIPPINGBYAXIS_H
#define VTOOLFLIPPINGBYAXIS_H

#include <QtGlobal>

#include "vabstractflipping.h"

class VToolFlippingByAxis : public VAbstractFlipping
{
    Q_OBJECT
public:
    virtual ~VToolFlippingByAxis() Q_DECL_EQ_DEFAULT;
    virtual void setDialog() Q_DECL_OVERRIDE;
    static VToolFlippingByAxis* Create(DialogTool *dialog, VMainGraphicsScene *scene, VAbstractPattern *doc,
                                       VContainer *data);
    static VToolFlippingByAxis* Create(const quint32 _id, quint32 originPointId, AxisType axisType,
                                       const QString &suffix, const QVector<quint32> &source,
                                       const QVector<DestinationItem> &destination, VMainGraphicsScene *scene,
                                       VAbstractPattern *doc, VContainer *data, const Document &parse,
                                       const Source &typeCreation);

    static const QString ToolType;

    virtual int type() const Q_DECL_OVERRIDE {return Type;}
    enum { Type = UserType + static_cast<int>(Tool::FlippingByAxis)};

    AxisType GetAxisType() const;
    void     SetAxisType(AxisType value);

    QString OriginPointName() const;

    virtual void ShowVisualization(bool show) Q_DECL_OVERRIDE;
protected:
    virtual void SetVisualization() Q_DECL_OVERRIDE;
    virtual void SaveDialog(QDomElement &domElement) Q_DECL_OVERRIDE;
    virtual void ReadToolAttributes(const QDomElement &domElement) Q_DECL_OVERRIDE;
    virtual void SaveOptions(QDomElement &tag, QSharedPointer<VGObject> &obj) Q_DECL_OVERRIDE;
    virtual void contextMenuEvent ( QGraphicsSceneContextMenuEvent * event ) Q_DECL_OVERRIDE;
private:
    Q_DISABLE_COPY(VToolFlippingByAxis)

    quint32  m_originPointId;
    AxisType m_axisType;

    VToolFlippingByAxis(VAbstractPattern *doc, VContainer *data, quint32 id, quint32 originPointId,
                        AxisType axisType, const QString &suffix, const QVector<quint32> &source,
                        const QVector<DestinationItem> &destination, const Source &typeCreation,
                        QGraphicsItem *parent = nullptr);
};

#endif // VTOOLFLIPPINGBYAXIS_H
