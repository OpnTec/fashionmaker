/************************************************************************
 **
 **  @file   dialogpointfromcircleandtangent.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   3 6, 2015
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

#ifndef DIALOGPOINTFROMCIRCLEANDTANGENT_H
#define DIALOGPOINTFROMCIRCLEANDTANGENT_H

#include "dialogtool.h"

namespace Ui
{
    class DialogPointFromCircleAndTangent;
}

class DialogPointFromCircleAndTangent : public DialogTool
{
    Q_OBJECT

public:
    DialogPointFromCircleAndTangent(const VContainer *data, const quint32 &toolId, QWidget *parent = 0);
    ~DialogPointFromCircleAndTangent();

    void           SetPointName(const QString &value);

    quint32        GetCircleCenterId() const;
    void           SetCircleCenterId(const quint32 &value);

    QString        GetCircleRadius() const;
    void           SetCircleRadius(const QString &value);

    quint32        GetTangentPointId() const;
    void           SetTangentPointId(const quint32 &value);

    CrossCirclesPoint GetCrossCirclesPoint() const;
    void              SetCrossCirclesPoint(CrossCirclesPoint &p);

public slots:
    virtual void   ChosenObject(quint32 id, const SceneObject &type);
    void           PointChanged();

    void           DeployCircleRadiusTextEdit();
    void           CircleRadiusChanged();
    void           FXCircleRadius();
    void           EvalCircleRadius();

protected:
    virtual void   ShowVisualization();
    /**
     * @brief SaveData Put dialog data in local variables
     */
    virtual void   SaveData();
    virtual void   closeEvent(QCloseEvent *event);
    virtual void   CheckState();

private:
    Q_DISABLE_COPY(DialogPointFromCircleAndTangent)

    Ui::DialogPointFromCircleAndTangent *ui;

    bool          flagCircleRadius;
    QTimer        *timerCircleRadius;
    QString       circleRadius;
    int           formulaBaseHeightCircleRadius;
    qreal         angleCircleRadius;
};

#endif // DIALOGPOINTFROMCIRCLEANDTANGENT_H
