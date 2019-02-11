/************************************************************************
 **
 **  @file   dialogpointofintersectioncurves.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   23 1, 2016
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
    explicit DialogPointOfIntersectionCurves(const VContainer *data, quint32 toolId, QWidget *parent = nullptr);
    virtual ~DialogPointOfIntersectionCurves() override;

    QString GetPointName() const;
    void    SetPointName(const QString &value);

    quint32 GetFirstCurveId() const;
    void    SetFirstCurveId(quint32 value);

    quint32 GetSecondCurveId() const;
    void    SetSecondCurveId(quint32 value);

    VCrossCurvesPoint GetVCrossPoint() const;
    void              SetVCrossPoint(VCrossCurvesPoint vP);

    HCrossCurvesPoint GetHCrossPoint() const;
    void              SetHCrossPoint(HCrossCurvesPoint hP);

public slots:
    virtual void ChosenObject(quint32 id, const SceneObject &type) override;

protected:
    virtual void ShowVisualization() override;

    /**
     * @brief SaveData Put dialog data in local variables
     */
    virtual void SaveData() override;
    virtual bool IsValid() const final;

private slots:
    void CurveChanged();

private:
    Q_DISABLE_COPY(DialogPointOfIntersectionCurves)

    Ui::DialogPointOfIntersectionCurves *ui;

    QString pointName;

    bool flagName;
    bool flagError;
};

//---------------------------------------------------------------------------------------------------------------------
inline bool DialogPointOfIntersectionCurves::IsValid() const
{
    return flagName && flagError;
}

#endif // DIALOGPOINTOFINTERSECTIONCURVES_H
