/************************************************************************
 **
 **  @file   vnodesplinepath.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   November 15, 2013
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

#ifndef VNODESPLINEPATH_H
#define VNODESPLINEPATH_H

#include <qcompilerdetection.h>
#include <QGraphicsPathItem>
#include <QMetaObject>
#include <QObject>
#include <QString>
#include <QtGlobal>

#include "../ifc/xml/vabstractpattern.h"
#include "../vwidgets/../vmisc/def.h"
#include "vabstractnode.h"

class VContainer;

/**
 * @brief The VNodeSplinePath class spline path detail node.
 */
class VNodeSplinePath : public VAbstractNode
{
    Q_OBJECT
public:
    static void  Create(VAbstractPattern *doc, VContainer *data, quint32 id,
                        quint32 idSpline, const Document &parse,
                        const Source &typeCreation, const QString &drawName = QString(), const quint32 &idTool = 0);

    static const QString ToolType;
    virtual QString getTagName() const Q_DECL_OVERRIDE;
public slots:
    virtual void FullUpdateFromFile() Q_DECL_OVERRIDE {}
    virtual void AllowHover(bool enabled) Q_DECL_OVERRIDE;
    virtual void AllowSelecting(bool enabled) Q_DECL_OVERRIDE;
protected:
    virtual void AddToFile() Q_DECL_OVERRIDE;
    virtual void RefreshDataInFile() Q_DECL_OVERRIDE;
    virtual void ShowNode() Q_DECL_OVERRIDE {}
    virtual void HideNode() Q_DECL_OVERRIDE {}
private:
    Q_DISABLE_COPY(VNodeSplinePath)

    VNodeSplinePath(VAbstractPattern *doc, VContainer *data, quint32 id, quint32 idSpline, const Source &typeCreation,
                    const QString &drawName = QString(), const quint32 &idTool = 0, QObject *qoParent = nullptr);
};

#endif // VNODESPLINEPATH_H
