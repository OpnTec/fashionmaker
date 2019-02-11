/************************************************************************
 **
 **  @file   dialogpointofintersectioncircles.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   29 5, 2015
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentina project, a pattern making
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

#ifndef DIALOGPOINTOFINTERSECTIONCIRCLES_H
#define DIALOGPOINTOFINTERSECTIONCIRCLES_H

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
    class DialogPointOfIntersectionCircles;
}

class DialogPointOfIntersectionCircles : public DialogTool
{
    Q_OBJECT

public:
    DialogPointOfIntersectionCircles(const VContainer *data, quint32 toolId, QWidget *parent = nullptr);
    virtual ~DialogPointOfIntersectionCircles() override;

    QString        GetPointName() const;
    void           SetPointName(const QString &value);

    quint32        GetFirstCircleCenterId() const;
    void           SetFirstCircleCenterId(const quint32 &value);

    quint32        GetSecondCircleCenterId() const;
    void           SetSecondCircleCenterId(const quint32 &value);

    QString        GetFirstCircleRadius() const;
    void           SetFirstCircleRadius(const QString &value);

    QString        GetSecondCircleRadius() const;
    void           SetSecondCircleRadius(const QString &value);

    CrossCirclesPoint GetCrossCirclesPoint() const;
    void              SetCrossCirclesPoint(const CrossCirclesPoint &p);

public slots:
    virtual void   ChosenObject(quint32 id, const SceneObject &type) override;
    void           PointChanged();

    void           DeployCircle1RadiusTextEdit();
    void           DeployCircle2RadiusTextEdit();

    void           FXCircle1Radius();
    void           FXCircle2Radius();

    void           EvalCircle1Radius();
    void           EvalCircle2Radius();

protected:
    virtual void   ShowVisualization() override;
    /**
     * @brief SaveData Put dialog data in local variables
     */
    virtual void   SaveData() override;
    virtual void   closeEvent(QCloseEvent *event) override;
    virtual bool   IsValid() const final;

private:
    Q_DISABLE_COPY(DialogPointOfIntersectionCircles)

    Ui::DialogPointOfIntersectionCircles *ui;

    QTimer *timerCircle1Radius;
    QTimer *timerCircle2Radius;

    QString circle1Radius;
    QString circle2Radius;

    int formulaBaseHeightCircle1Radius;
    int formulaBaseHeightCircle2Radius;

    QString pointName;

    bool flagCircle1Radius;
    bool flagCircle2Radius;
    bool flagName;
    bool flagError;
};

//---------------------------------------------------------------------------------------------------------------------
inline bool DialogPointOfIntersectionCircles::IsValid() const
{
    return flagCircle1Radius && flagCircle2Radius && flagName && flagError;
}

#endif // DIALOGPOINTOFINTERSECTIONCIRCLES_H
