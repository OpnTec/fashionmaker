/************************************************************************
 **
 **  @file   dialogpointofintersectioncurves.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   23 1, 2016
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

#ifndef DIALOGPOINTOFINTERSECTIONCURVES_H
#define DIALOGPOINTOFINTERSECTIONCURVES_H

#include <qcompilerdetection.h>
#include <QMetaObject>
#include <QObject>
#include <QString>
#include <QtGlobal>

#include "../ifc/xml/vabstractpattern.h"
#include "../vmisc/def.h"
#include "dialogtool.h"

namespace Ui {
class DialogPointOfIntersectionCurves;
}

class DialogPointOfIntersectionCurves : public DialogTool
{
    Q_OBJECT

public:
    explicit DialogPointOfIntersectionCurves(const VContainer *data, const quint32 &toolId, QWidget *parent = nullptr);
    virtual ~DialogPointOfIntersectionCurves() Q_DECL_OVERRIDE;

    void    SetPointName(const QString &value);

    quint32 GetFirstCurveId() const;
    void    SetFirstCurveId(const quint32 &value);

    quint32 GetSecondCurveId() const;
    void    SetSecondCurveId(const quint32 &value);

    VCrossCurvesPoint GetVCrossPoint() const;
    void              SetVCrossPoint(const VCrossCurvesPoint &vP);

    HCrossCurvesPoint GetHCrossPoint() const;
    void              SetHCrossPoint(const HCrossCurvesPoint &hP);

public slots:
    virtual void ChosenObject(quint32 id, const SceneObject &type) Q_DECL_OVERRIDE;

protected:
    virtual void ShowVisualization() Q_DECL_OVERRIDE;

    /**
     * @brief SaveData Put dialog data in local variables
     */
    virtual void SaveData() Q_DECL_OVERRIDE;
    virtual void CheckState() Q_DECL_FINAL;

private slots:
    void CurveChanged();

private:
    Q_DISABLE_COPY(DialogPointOfIntersectionCurves)

    Ui::DialogPointOfIntersectionCurves *ui;
};

#endif // DIALOGPOINTOFINTERSECTIONCURVES_H
