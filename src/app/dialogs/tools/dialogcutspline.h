/************************************************************************
 **
 **  @file   dialogcutspline.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   15 12, 2013
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

#ifndef DIALOGCUTSPLINE_H
#define DIALOGCUTSPLINE_H

#include "dialogtool.h"

namespace Ui
{
    class DialogCutSpline;
}

class VisToolCutSpline;

/**
 * @brief The DialogCutSpline class dialog for ToolCutSpline.
 */
class DialogCutSpline : public DialogTool
{
    Q_OBJECT
public:
    DialogCutSpline(const VContainer *data, const quint32 &toolId, QWidget *parent = nullptr);
    ~DialogCutSpline();

    QString             getPointName() const;
    void                setPointName(const QString &value);

    QString             getFormula() const;
    void                setFormula(const QString &value);

    quint32             getSplineId() const;
    void                setSplineId(const quint32 &value);
public slots:
    virtual void        ChosenObject(quint32 id, const SceneObject &type);
    /**
     * @brief DeployFormulaTextEdit grow or shrink formula input
     */
    void                DeployFormulaTextEdit();
protected:
    virtual void        ShowVisualization();
    /**
     * @brief SaveData Put dialog data in local variables
     */
    virtual void        SaveData();
private:
    Q_DISABLE_COPY(DialogCutSpline)

    /** @brief ui keeps information about user interface */
    Ui::DialogCutSpline *ui;

    /** @brief formula string with formula */
    QString             formula;

    /**  @brief splineId keep id of spline */
    quint32             splineId;

    /** @brief formulaBaseHeight base height defined by dialogui */
    int                 formulaBaseHeight;

    VisToolCutSpline     *path;
};

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief getPointName return name of point
 * @return name
 */
inline QString DialogCutSpline::getPointName() const
{
    return pointName;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief getFormula return string of formula
 * @return formula
 */
inline QString DialogCutSpline::getFormula() const
{
    return qApp->FormulaFromUser(formula);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief getSplineId return id base point of line
 * @return id
 */
inline quint32 DialogCutSpline::getSplineId() const
{
    return splineId;
}

#endif // DIALOGCUTSPLINE_H
