/************************************************************************
 **
 **  @file   dialogellipticalarc.h
 **  @author Valentina Zhuravska <zhuravska19(at)gmail.com>
 **  @date   15 9, 2016
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

#ifndef DIALOGELLIPTICALARC_H
#define DIALOGELLIPTICALARC_H

#include <qcompilerdetection.h>
#include <QMetaObject>
#include <QObject>
#include <QString>
#include <QtGlobal>

#include "../vmisc/def.h"
#include "dialogtool.h"

namespace Ui
{
    class DialogEllipticalArc;
}

class DialogEllipticalArc : public DialogTool
{
    Q_OBJECT
public:
    DialogEllipticalArc(const VContainer *data, const quint32 &toolId, QWidget *parent = nullptr);
    virtual ~DialogEllipticalArc() Q_DECL_OVERRIDE;

    quint32       GetCenter() const;
    void          SetCenter(const quint32 &value);

    QString       GetRadius1() const;
    void          SetRadius1(const QString &value);

    QString       GetRadius2() const;
    void          SetRadius2(const QString &value);

    QString       GetF1() const;
    void          SetF1(const QString &value);

    QString       GetF2() const;
    void          SetF2(const QString &value);

    QString       GetRotationAngle() const;
    void          SetRotationAngle(const QString &value);

    QString       GetColor() const;
    void          SetColor(const QString &value);

public slots:
    virtual void  ChosenObject(quint32 id, const SceneObject &type) Q_DECL_OVERRIDE;
    /**
     * @brief DeployFormulaTextEdit grow or shrink formula input
     */
    void DeployRadius1TextEdit();
    void DeployRadius2TextEdit();
    void DeployF1TextEdit();
    void DeployF2TextEdit();
    void DeployRotationAngleTextEdit();

    void          Radius1Changed();
    void          Radius2Changed();
    void          F1Changed();
    void          F2Changed();
    void          RotationAngleChanged();

    void          FXRadius1();
    void          FXRadius2();
    void          FXF1();
    void          FXF2();
    void          FXRotationAngle();

protected:
    virtual void  CheckState() Q_DECL_FINAL;
    virtual void  ShowVisualization() Q_DECL_OVERRIDE;
    /**
     * @brief SaveData Put dialog data in local variables
     */
    virtual void  SaveData() Q_DECL_OVERRIDE;
    virtual void  closeEvent(QCloseEvent *event) Q_DECL_OVERRIDE;

private:
    Q_DISABLE_COPY(DialogEllipticalArc)

    /** @brief ui keeps information about user interface */
    Ui::DialogEllipticalArc *ui;

    /** @brief flagRadius1 true if value of radius1 is correct */
    bool          flagRadius1;

    /** @brief flagRadius2 true if value of radius2 is correct */
    bool          flagRadius2;

    /** @brief flagF1 true if value of first angle is correct */
    bool          flagF1;

    /** @brief flagF2 true if value of second angle is correct */
    bool          flagF2;

    /** @brief flagRotationAngle true if value of rotation angle is correct */
    bool          flagRotationAngle;

    /** @brief timerRadius1 timer of check formula of radius1 */
    QTimer        *timerRadius1;

    /** @brief timerRadius2 timer of check formula of radius2 */
    QTimer        *timerRadius2;

    /** @brief timerF1 timer of check formula of first angle */
    QTimer        *timerF1;

    /** @brief timerF2 timer of check formula of second angle */
    QTimer        *timerF2;

    /** @brief timerRotationAngle timer of check formula of rotation angle */
    QTimer        *timerRotationAngle;

    /** @brief radius1 formula of radius1 */
    QString       radius1;

    /** @brief radius2 formula of radius2 */
    QString       radius2;

    /** @brief f1 formula of first angle */
    QString       f1;

    /** @brief f2 formula of second angle */
    QString       f2;

    /** @brief rotationAngle formula of rotation angle */
    QString       rotationAngle;

    /** @brief formulaBaseHeight base height defined by dialogui */
    int           formulaBaseHeightRadius1;
    int           formulaBaseHeightRadius2;
    int           formulaBaseHeightF1;
    int           formulaBaseHeightF2;
    int           formulaBaseHeightRotationAngle;

    qreal         angleF1;
    qreal         angleF2;
    qreal         angleRotation;

    void          EvalRadiuses();
    void          EvalAngles();
    void          CheckAngles();
};

#endif // DIALOGELLIPTICALARC_H
