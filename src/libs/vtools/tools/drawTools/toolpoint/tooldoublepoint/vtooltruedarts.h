/************************************************************************
 **
 **  @file   vtooltruedarts.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   23 6, 2015
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentine project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2015 Valentina project
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

#ifndef VTOOLTRUEDARTS_H
#define VTOOLTRUEDARTS_H

#include <qcompilerdetection.h>
#include <QDomElement>
#include <QGraphicsItem>
#include <QMetaObject>
#include <QObject>
#include <QPointF>
#include <QString>
#include <QtGlobal>

#include "../ifc/xml/vabstractpattern.h"
#include "../vmisc/def.h"
#include "vtooldoublepoint.h"

template <class T> class QSharedPointer;

class VToolTrueDarts : public VToolDoublePoint
{
    Q_OBJECT
public:
    static void    FindPoint(const QPointF &baseLineP1, const QPointF &baseLineP2, const QPointF &dartP1,
                             const QPointF &dartP2, const QPointF &dartP3, QPointF &p1, QPointF &p2);
    virtual void   setDialog() Q_DECL_OVERRIDE;
    static VToolTrueDarts* Create(DialogTool *dialog, VMainGraphicsScene *scene, VAbstractPattern *doc,
                                  VContainer *data);
    static VToolTrueDarts* Create(quint32 _id,
                                  const quint32 &_p1id, const quint32 &_p2id,
                                  const quint32 &baseLineP1Id,
                                  const quint32 &baseLineP2Id,
                                  const quint32 &dartP1Id,
                                  const quint32 &dartP2Id,
                                  const quint32 &dartP3Id,
                                  const QString &point1Name, const qreal &mx1, const qreal &my1,
                                  const QString &point2Name, const qreal &mx2, const qreal &my2,
                                  VMainGraphicsScene  *scene, VAbstractPattern *doc, VContainer *data,
                                  const Document &parse, const Source &typeCreation);
    static const QString ToolType;
    virtual int    type() const  Q_DECL_OVERRIDE {return Type;}
    enum { Type = UserType + static_cast<int>(Tool::TrueDarts)};

    virtual void   ShowVisualization(bool show) Q_DECL_OVERRIDE;

    quint32 GetBaseLineP1Id() const;
    void    SetBaseLineP1Id(const quint32 &value);

    quint32 GetBaseLineP2Id() const;
    void    SetBaseLineP2Id(const quint32 &value);

    quint32 GetDartP1Id() const;
    void    SetDartP1Id(const quint32 &value);

    quint32 GetDartP2Id() const;
    void    SetDartP2Id(const quint32 &value);

    quint32 GetDartP3Id() const;
    void    SetDartP3Id(const quint32 &value);

protected:
    virtual void contextMenuEvent ( QGraphicsSceneContextMenuEvent * event ) Q_DECL_OVERRIDE;
    virtual void RemoveReferens() Q_DECL_OVERRIDE;
    virtual void SaveDialog(QDomElement &domElement) Q_DECL_OVERRIDE;
    virtual void SaveOptions(QDomElement &tag, QSharedPointer<VGObject> &obj) Q_DECL_OVERRIDE;
    virtual void ReadToolAttributes(const QDomElement &domElement) Q_DECL_OVERRIDE;
    virtual void SetVisualization() Q_DECL_OVERRIDE;

private:
    Q_DISABLE_COPY(VToolTrueDarts)
    quint32 baseLineP1Id;
    quint32 baseLineP2Id;
    quint32 dartP1Id;
    quint32 dartP2Id;
    quint32 dartP3Id;

    VToolTrueDarts(VAbstractPattern *doc,
                   VContainer *data,
                   const quint32 &id,
                   const quint32 &p1id,
                   const quint32 &p2id,
                   const quint32 &baseLineP1Id,
                   const quint32 &baseLineP2Id,
                   const quint32 &dartP1Id,
                   const quint32 &dartP2Id,
                   const quint32 &dartP3Id,
                   const Source &typeCreation,
                   QGraphicsItem * parent = nullptr);
};

#endif // VTOOLTRUEDARTS_H
