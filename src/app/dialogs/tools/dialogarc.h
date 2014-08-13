/************************************************************************
 **
 **  @file   dialogarc.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   November 15, 2013
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentine project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2013 Valentina project
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

#ifndef DIALOGARC_H
#define DIALOGARC_H

#include "dialogtool.h"

namespace Ui
{
    class DialogArc;
}

/**
 * @brief The DialogArc class dialog for ToolArc. Help create arc and edit option.
 */
class DialogArc : public DialogTool
{
    Q_OBJECT
public:
    DialogArc(const VContainer *data, const quint32 &toolId, QWidget *parent = nullptr);
    ~DialogArc();

    quint32       GetCenter() const;
    void          SetCenter(const quint32 &value);

    QString       GetRadius() const;
    void          SetRadius(const QString &value);

    QString       GetF1() const;
    void          SetF1(const QString &value);

    QString       GetF2() const;
    void          SetF2(const QString &value);
public slots:
    virtual void  ChosenObject(quint32 id, const SceneObject &type);
    /**
     * @brief DeployFormulaTextEdit grow or shrink formula input
     */
    void DeployFormulaTextEdit();
    void DeployF1TextEdit();
    void DeployF2TextEdit();
    virtual void  ValChenged(int row);
    void          PutRadius();
    void          PutF1();
    void          PutF2();
    // cppcheck-suppress unusedFunction
    void          LineAngles();
    void          RadiusChanged();
    void          F1Changed();
    void          F2Changed();
protected:
    virtual void  CheckState();
    /**
     * @brief SaveData Put dialog data in local variables
     */
    virtual void  SaveData();
private:
    Q_DISABLE_COPY(DialogArc)

    /** @brief ui keeps information about user interface */
    Ui::DialogArc *ui;

    /** @brief flagRadius true if value of radius is correct */
    bool          flagRadius;

    /** @brief flagF1 true if value of first angle is correct */
    bool          flagF1;

    /** @brief flagF2 true if value of second angle is correct */
    bool          flagF2;

    /** @brief timerRadius timer of check formula of radius */
    QTimer        *timerRadius;

    /** @brief timerF1 timer of check formula of first angle */
    QTimer        *timerF1;

    /** @brief timerF2 timer of check formula of second angle */
    QTimer        *timerF2;

    /** @brief center id of center point */
    quint32       center;

    /** @brief radius formula of radius */
    QString       radius;

    /** @brief f1 formula of first angle */
    QString       f1;

    /** @brief f2 formula of second angle */
    QString       f2;

    /** @brief formulaBaseHeight base height defined by dialogui */
    int           formulaBaseHeight;
    int           formulaBaseHeightF1;
    int           formulaBaseHeightF2;

    void          EvalRadius();
    void          EvalF1();
    void          EvalF2();
    void          ShowLineAngles();

};

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief GetCenter return id of center point
 * @return id id
 */
inline quint32 DialogArc::GetCenter() const
{
    return center;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief GetRadius return formula of radius
 * @return formula
 */
inline QString DialogArc::GetRadius() const
{
    return radius;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief GetF1 return formula first angle of arc
 * @return formula
 */
inline QString DialogArc::GetF1() const
{
    return f1;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief GetF2 return formula second angle of arc
 * @return formula
 */
inline QString DialogArc::GetF2() const
{
    return f2;
}

#endif // DIALOGARC_H
