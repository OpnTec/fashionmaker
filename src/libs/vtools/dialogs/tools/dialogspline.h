/************************************************************************
 **
 **  @file   dialogspline.h
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

template <class T> class QSharedPointer;

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
    DialogSpline(const VContainer *data, const quint32 &toolId, QWidget *parent = nullptr);
    virtual ~DialogSpline() Q_DECL_OVERRIDE;

    VSpline GetSpline() const;
    void    SetSpline(const VSpline &spline);

    QString GetColor() const;
    void    SetColor(const QString &value);
public slots:
    virtual void  ChosenObject(quint32 id, const SceneObject &type) Q_DECL_OVERRIDE;
    virtual void  PointNameChanged() Q_DECL_OVERRIDE;
    virtual void  ShowDialog(bool click) Q_DECL_OVERRIDE;
protected:
    virtual void  CheckState() Q_DECL_FINAL;
    virtual void  ShowVisualization() Q_DECL_OVERRIDE;
    /**
     * @brief SaveData Put dialog data in local variables
     */
    virtual void  SaveData() Q_DECL_OVERRIDE;
    virtual void  closeEvent(QCloseEvent *event) Q_DECL_OVERRIDE;
private slots:
    void DeployAngle1TextEdit();
    void DeployAngle2TextEdit();
    void DeployLength1TextEdit();
    void DeployLength2TextEdit();

    void Angle1Changed();
    void Angle2Changed();
    void Length1Changed();
    void Length2Changed();

    void FXAngle1();
    void FXAngle2();
    void FXLength1();
    void FXLength2();
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

    const QSharedPointer<VPointF> GetP1() const;
    const QSharedPointer<VPointF> GetP4() const;

    void EvalAngle1();
    void EvalAngle2();
    void EvalLength1();
    void EvalLength2();

    VSpline CurrentSpline() const;
};

#endif // DIALOGSPLINE_H
