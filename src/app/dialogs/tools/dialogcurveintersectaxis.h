/************************************************************************
 **
 **  @file   dialogcurveintersectaxis.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   21 10, 2014
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentine project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2014 Valentina project
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

#ifndef DIALOGCURVEINTERSECTAXIS_H
#define DIALOGCURVEINTERSECTAXIS_H

#include "dialogtool.h"

namespace Ui {
class DialogCurveIntersectAxis;
}

class VisToolCurveIntersectAxis;

class DialogCurveIntersectAxis : public DialogTool
{
    Q_OBJECT

public:
    DialogCurveIntersectAxis(const VContainer *data, const quint32 &toolId, QWidget *parent = nullptr);
    ~DialogCurveIntersectAxis();

    QString      getPointName() const;
    void         setPointName(const QString &value);

    QString      getTypeLine() const;
    void         setTypeLine(const QString &value);

    QString      getAngle() const;
    void         setAngle(const QString &value);

    quint32      getBasePointId() const;
    void         setBasePointId(const quint32 &value);

    quint32      getCurveId() const;
    void         setCurveId(const quint32 &value);

    virtual void ShowDialog(bool click);
public slots:
    virtual void ChosenObject(quint32 id, const SceneObject &type);
    void         PutAngle();
    void         EvalAngle();
    void         AngleTextChanged();
    void         DeployAngleTextEdit();
protected:
    virtual void ShowVisualization();
    /**
     * @brief SaveData Put dialog data in local variables
     */
    virtual void SaveData();
    virtual void closeEvent(QCloseEvent *event);
private:
    Q_DISABLE_COPY(DialogCurveIntersectAxis)
    Ui::DialogCurveIntersectAxis *ui;

    /** @brief number number of handled objects */
    qint32  number;

    /** @brief typeLine type of line */
    QString typeLine;

    QString formulaAngle;
    quint32 basePointId;
    quint32 curveId;
    int     formulaBaseHeightAngle;

    VisToolCurveIntersectAxis *line;
};

#endif // DIALOGCURVEINTERSECTAXIS_H
