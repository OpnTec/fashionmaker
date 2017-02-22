/************************************************************************
 **
 **  @file   vtoolcutsplinepath.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   15 12, 2013
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentine project, a pattern making
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

/**
 * @brief The VToolCutSplinePath class for tool CutSplinePath. This tool find point on splinePath and cut splinePath on
 * two.
 */
class VToolCutSplinePath : public VToolCut
{
    Q_OBJECT
public:
    virtual void setDialog() Q_DECL_OVERRIDE;
    static VToolCutSplinePath *Create(DialogTool *dialog, VMainGraphicsScene  *scene, VAbstractPattern *doc,
                                      VContainer *data);
    static VToolCutSplinePath *Create(const quint32 _id, const QString &pointName, QString &formula,
                                      const quint32 &splinePathId, const qreal &mx, const qreal &my,
                                      VMainGraphicsScene *scene, VAbstractPattern *doc, VContainer *data,
                                      const Document &parse, const Source &typeCreation);
    static const QString ToolType;
    static const QString AttrSplinePath;
    virtual int  type() const Q_DECL_OVERRIDE {return Type;}
    enum { Type = UserType + static_cast<int>(Tool::CutSplinePath)};
    virtual void  ShowVisualization(bool show) Q_DECL_OVERRIDE;

    static VPointF *CutSplinePath(qreal length, const QSharedPointer<VAbstractCubicBezierPath> &splPath,
                                  const QString &pName, VSplinePath **splPath1,
                                  VSplinePath **splPath2) Q_REQUIRED_RESULT;
protected:
    virtual void  contextMenuEvent ( QGraphicsSceneContextMenuEvent * event ) Q_DECL_OVERRIDE;
    virtual void  SaveDialog(QDomElement &domElement) Q_DECL_OVERRIDE;
    virtual void  SaveOptions(QDomElement &tag, QSharedPointer<VGObject> &obj) Q_DECL_OVERRIDE;
    virtual void  ReadToolAttributes(const QDomElement &domElement) Q_DECL_OVERRIDE;
    virtual void  SetVisualization() Q_DECL_OVERRIDE;
private:
    Q_DISABLE_COPY(VToolCutSplinePath)

    VToolCutSplinePath(VAbstractPattern *doc, VContainer *data, const quint32 &id, const QString &formula,
                       const quint32 &splinePathId, const Source &typeCreation, QGraphicsItem * parent = nullptr);
};

#endif // VTOOLCUTSPLINEPATH_H
