/************************************************************************
 **
 **  @file   dialognormal.h
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

#ifndef DIALOGNORMAL_H
#define DIALOGNORMAL_H

#include "dialogtool.h"

namespace Ui
{
    class DialogNormal;
}

/**
 * @brief The DialogNormal class dialog for ToolNormal. Help create point and edit option.
 */
class DialogNormal : public DialogTool
{
    Q_OBJECT
public:
    DialogNormal(const VContainer *data, QWidget *parent = nullptr);
    ~DialogNormal();

    QString          getPointName() const;
    void             setPointName(const QString &value);

    QString          getTypeLine() const;
    void             setTypeLine(const QString &value);

    QString          getFormula() const;
    void             setFormula(const QString &value);

    qreal            getAngle() const;
    void             setAngle(const qreal &value);

    quint32          getFirstPointId() const;
    void             setFirstPointId(const quint32 &value, const quint32 &id);

    quint32          getSecondPointId() const;
    void             setSecondPointId(const quint32 &value, const quint32 &id);
public slots:
    virtual void     ChoosedObject(quint32 id, const SceneObject &type);
    virtual void     DialogAccepted();
    /**
     * @brief DialogApply apply data and emit signal about applied dialog.
     */
    virtual void     DialogApply();
    /**
     * @brief DeployFormulaTextEdit grow or shrink formula input
     */
    void             DeployFormulaTextEdit();
    /**
     * @brief FormulaTextChanged when formula text changes for validation and calc
     */
    void             FormulaTextChanged();
    virtual void     PointNameChanged();
private:
    Q_DISABLE_COPY(DialogNormal)

    /** @brief ui keeps information about user interface */
    Ui::DialogNormal *ui;

    /** @brief number number of handled objects */
    qint32           number;

    /** @brief pointName name of point */
    QString          pointName;

    /** @brief typeLine type of line */
    QString          typeLine;

    /** @brief formula formula */
    QString          formula;

    /** @brief angle aditional angle of normal */
    qreal            angle;

    /** @brief firstPointId id first point of line */
    quint32          firstPointId;

    /** @brief secondPointId id second point of line */
    quint32          secondPointId;

    /** @brief formulaBaseHeight base height defined by dialogui */
    int              formulaBaseHeight;

    /**
     * @brief SaveData Put dialog data in local variables
     */
    void SaveData();
};

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief getPointName return name of point
 * @return name
 */
inline QString DialogNormal::getPointName() const
{
    return pointName;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief getTypeLine return type of line
 * @return type
 */
inline QString DialogNormal::getTypeLine() const
{
    return typeLine;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief getFormula return string of formula
 * @return formula
 */
inline QString DialogNormal::getFormula() const
{
    return qApp->FormulaFromUser(formula);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief getAngle return aditional angle of normal
 * @return angle in degree
 */
inline qreal DialogNormal::getAngle() const
{
    return angle;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief getFirstPointId return id of first point
 * @return id
 */
inline quint32 DialogNormal::getFirstPointId() const
{
    return firstPointId;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief getSecondPointId return id of second point
 * @return id
 */
inline quint32 DialogNormal::getSecondPointId() const
{
    return secondPointId;
}

#endif // DIALOGNORMAL_H
