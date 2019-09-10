/************************************************************************
 **
 **  @file   dialogspline.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   November 15, 2013
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentina project, a pattern making
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

#ifndef DIALOGSPLINE_H
#define DIALOGSPLINE_H

#include <qcompilerdetection.h>
#include <QMetaObject>
#include <QObject>
#include <QSharedPointer>
#include <QString>
#include <QtGlobal>

#include "../vgeometry/vspline.h"
#include "../vmisc/def.h"
#include "dialogtool.h"

namespace Ui
{
    class DialogSpline;
}

/**
 * @brief The DialogSpline class dialog for ToolSpline. Help create spline and edit option.
 */
class DialogSpline : public DialogTool
{
    Q_OBJECT
public:
    DialogSpline(const VContainer *data, quint32 toolId, QWidget *parent = nullptr);
    virtual ~DialogSpline() override;

    VSpline GetSpline() const;
    void    SetSpline(const VSpline &spline);

public slots:
    virtual void  ChosenObject(quint32 id, const SceneObject &type) override;
    virtual void  PointNameChanged() override;
    virtual void  ShowDialog(bool click) override;
protected:
    virtual void  ShowVisualization() override;
    /**
     * @brief SaveData Put dialog data in local variables
     */
    virtual void  SaveData() override;
    virtual void  closeEvent(QCloseEvent *event) override;
    virtual bool  IsValid() const final;
private slots:
    void DeployAngle1TextEdit();
    void DeployAngle2TextEdit();
    void DeployLength1TextEdit();
    void DeployLength2TextEdit();

    void FXAngle1();
    void FXAngle2();
    void FXLength1();
    void FXLength2();

    void EvalAngle1();
    void EvalAngle2();
    void EvalLength1();
    void EvalLength2();
private:
    Q_DISABLE_COPY(DialogSpline)

    /** @brief ui keeps information about user interface */
    Ui::DialogSpline *ui;

    /** @brief spl spline */
    VSpline spl;

    qint32 newDuplicate;

    /** @brief formulaBaseHeight base height defined by dialogui */
    int formulaBaseHeightAngle1;
    int formulaBaseHeightAngle2;
    int formulaBaseHeightLength1;
    int formulaBaseHeightLength2;

    /** @brief timerAngle1 timer of check first angle formula */
    QTimer *timerAngle1;
    QTimer *timerAngle2;
    QTimer *timerLength1;
    QTimer *timerLength2;

    /** @brief flagAngle1 true if value of first angle is correct */
    bool flagAngle1;
    bool flagAngle2;
    bool flagLength1;
    bool flagLength2;
    bool flagError;

    const QSharedPointer<VPointF> GetP1() const;
    const QSharedPointer<VPointF> GetP4() const;

    VSpline CurrentSpline() const;
};

//---------------------------------------------------------------------------------------------------------------------
inline bool DialogSpline::IsValid() const
{
    return flagAngle1 && flagAngle2 && flagLength1 && flagLength2 && flagError;
}

#endif // DIALOGSPLINE_H
