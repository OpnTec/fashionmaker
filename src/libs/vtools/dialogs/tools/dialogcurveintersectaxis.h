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

#ifndef DIALOGCURVEINTERSECTAXIS_H
#define DIALOGCURVEINTERSECTAXIS_H

#include <qcompilerdetection.h>
#include <QMetaObject>
#include <QObject>
#include <QString>
#include <QtGlobal>

#include "../vmisc/def.h"
#include "dialogs/tools/../support/../tools/dialogtool.h"
#include "dialogtool.h"

class QCloseEvent;
class QWidget;
class VContainer;

namespace Ui
{
    class DialogCurveIntersectAxis;
}

class DialogCurveIntersectAxis : public DialogTool
{
    Q_OBJECT

public:
    DialogCurveIntersectAxis(const VContainer *data, const quint32 &toolId, QWidget *parent = nullptr);
    virtual ~DialogCurveIntersectAxis() Q_DECL_OVERRIDE;

    void         SetPointName(const QString &value);

    QString      GetTypeLine() const;
    void         SetTypeLine(const QString &value);

    QString      GetAngle() const;
    void         SetAngle(const QString &value);

    quint32      GetBasePointId() const;
    void         SetBasePointId(const quint32 &value);

    quint32      getCurveId() const;
    void         setCurveId(const quint32 &value);

    QString      GetLineColor() const;
    void         SetLineColor(const QString &value);

    virtual void ShowDialog(bool click) Q_DECL_OVERRIDE;
public slots:
    virtual void ChosenObject(quint32 id, const SceneObject &type) Q_DECL_OVERRIDE;
    void         EvalAngle();
    void         AngleTextChanged();
    void         DeployAngleTextEdit();
    void         FXAngle();
protected:
    virtual void ShowVisualization() Q_DECL_OVERRIDE;
    /**
     * @brief SaveData Put dialog data in local variables
     */
    virtual void SaveData() Q_DECL_OVERRIDE;
    virtual void closeEvent(QCloseEvent *event) Q_DECL_OVERRIDE;
private:
    Q_DISABLE_COPY(DialogCurveIntersectAxis)
    Ui::DialogCurveIntersectAxis *ui;

    QString formulaAngle;
    int     formulaBaseHeightAngle;
};

#endif // DIALOGCURVEINTERSECTAXIS_H
