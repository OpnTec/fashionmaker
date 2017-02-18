/************************************************************************
 **
 **  @file   dialogpointfromarcandtangent.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   5 6, 2015
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

#ifndef DIALOGPOINTFROMARCANDTANGENT_H
#define DIALOGPOINTFROMARCANDTANGENT_H

#include <qcompilerdetection.h>
#include <QMetaObject>
#include <QObject>
#include <QString>
#include <QtGlobal>

#include "../ifc/xml/vabstractpattern.h"
#include "../vmisc/def.h"
#include "dialogtool.h"

namespace Ui
{
    class DialogPointFromArcAndTangent;
}

class DialogPointFromArcAndTangent : public DialogTool
{
    Q_OBJECT

public:
    DialogPointFromArcAndTangent(const VContainer *data, const quint32 &toolId, QWidget *parent = 0);
    ~DialogPointFromArcAndTangent();


    void           SetPointName(const QString &value);

    quint32        GetArcId() const;
    void           SetArcId(const quint32 &value);

    quint32        GetTangentPointId() const;
    void           SetTangentPointId(const quint32 &value);

    CrossCirclesPoint GetCrossCirclesPoint() const;
    void              SetCrossCirclesPoint(const CrossCirclesPoint &p);

public slots:
    virtual void   ChosenObject(quint32 id, const SceneObject &type) Q_DECL_OVERRIDE;

protected:
    virtual void   ShowVisualization() Q_DECL_OVERRIDE;
    /**
     * @brief SaveData Put dialog data in local variables
     */
    virtual void   SaveData() Q_DECL_OVERRIDE;

private:
    Q_DISABLE_COPY(DialogPointFromArcAndTangent)

    Ui::DialogPointFromArcAndTangent *ui;
};

#endif // DIALOGPOINTFROMARCANDTANGENT_H
