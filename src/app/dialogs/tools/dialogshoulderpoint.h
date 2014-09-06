/************************************************************************
 **
 **  @file   dialogshoulderpoint.h
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

#ifndef DIALOGSHOULDERPOINT_H
#define DIALOGSHOULDERPOINT_H

#include "dialogtool.h"

namespace Ui
{
    class DialogShoulderPoint;
}

class VisToolShoulderPoint;

/**
 * @brief The DialogShoulderPoint class dialog for ToolShoulderPoint. Help create point and edit option.
 */
class DialogShoulderPoint : public DialogTool
{
    Q_OBJECT
public:
    DialogShoulderPoint(const VContainer *data, const quint32 &toolId, QWidget *parent = nullptr);
    ~DialogShoulderPoint();

    QString        getPointName() const;
    void           setPointName(const QString &value);

    QString        getTypeLine() const;
    void           setTypeLine(const QString &value);

    QString        getFormula() const;
    void           setFormula(const QString &value);

    quint32        getP1Line() const;
    void           setP1Line(const quint32 &value);

    quint32        getP2Line() const;
    void           setP2Line(const quint32 &value);

    quint32        getPShoulder() const;
    void           setPShoulder(const quint32 &value);
public slots:
    virtual void   ChosenObject(quint32 id, const SceneObject &type);
    /**
     * @brief DeployFormulaTextEdit grow or shrink formula input
     */
    void           DeployFormulaTextEdit();
    /**
     * @brief FormulaTextChanged when formula text changes for validation and calc
     */
    void           FormulaTextChanged();
    virtual void   PointNameChanged();
protected:
    virtual void   ShowVisualization();
    /**
     * @brief SaveData Put dialog data in local variables
     */
    virtual void   SaveData();
private:
    Q_DISABLE_COPY(DialogShoulderPoint)

    /** @brief ui keeps information about user interface */
    Ui::DialogShoulderPoint *ui;

    /** @brief number number of handled objects */
    qint32         number;

    /** @brief typeLine type of line */
    QString        typeLine;

    /** @brief formula formula */
    QString        formula;

    /** @brief p1Line id first point of line */
    quint32         p1Line;

    /** @brief p2Line id second point of line */
    quint32         p2Line;

    /** @brief pShoulder id shoulder point */
    quint32         pShoulder;

    /** @brief formulaBaseHeight base height defined by dialogui */
    int             formulaBaseHeight;
    VisToolShoulderPoint *line;
};

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief getPointName return name of point
 * @return name
 */
inline QString DialogShoulderPoint::getPointName() const
{
    return pointName;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief getTypeLine return type of line
 * @return type
 */
inline QString DialogShoulderPoint::getTypeLine() const
{
    return typeLine;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief getFormula return string of formula
 * @return formula
 */
inline QString DialogShoulderPoint::getFormula() const
{
    return qApp->FormulaFromUser(formula);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief getP1Line return id first point of line
 * @return id
 */
inline quint32 DialogShoulderPoint::getP1Line() const
{
    return p1Line;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief getP2Line return id second point of line
 * @return id
 */
inline quint32 DialogShoulderPoint::getP2Line() const
{
    return p2Line;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief getPShoulder return id shoulder point
 * @return id
 */
inline quint32 DialogShoulderPoint::getPShoulder() const
{
    return pShoulder;
}

#endif // DIALOGSHOULDERPOINT_H
