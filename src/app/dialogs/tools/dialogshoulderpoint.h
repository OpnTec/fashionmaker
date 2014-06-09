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

#define DIALOGSHOULDERPOINT_MAX_FORMULA_HEIGHT 64
namespace Ui
{
    class DialogShoulderPoint;
}

/**
 * @brief The DialogShoulderPoint class dialog for ToolShoulderPoint. Help create point and edit option.
 */
class DialogShoulderPoint : public DialogTool
{
    Q_OBJECT
public:
    /**
     * @brief DialogShoulderPoint create dialog
     * @param data container with data
     * @param parent parent widget
     */
    DialogShoulderPoint(const VContainer *data, QWidget *parent = nullptr);
    ~DialogShoulderPoint();
    /**
     * @brief getPointName return name of point
     * @return name
     */
    QString        getPointName() const;
    /**
     * @brief setPointName set name of point
     * @param value name
     */
    void           setPointName(const QString &value);
    /**
     * @brief getTypeLine return type of line
     * @return type
     */
    QString        getTypeLine() const;
    /**
     * @brief setTypeLine set type of line
     * @param value type
     */
    void           setTypeLine(const QString &value);
    /**
     * @brief getFormula return string of formula
     * @return formula
     */
    QString        getFormula() const;
    /**
     * @brief setFormula set string of formula
     * @param value formula
     */
    void           setFormula(const QString &value);
    /**
     * @brief getP1Line return id first point of line
     * @return id
     */
    quint32        getP1Line() const;
    /**
     * @brief setP1Line set id first point of line
     * @param value id
     * @param id don't show this id in list
     */
    void           setP1Line(const quint32 &value, const quint32 &id);
    /**
     * @brief getP2Line return id second point of line
     * @return id
     */
    quint32        getP2Line() const;
    /**
     * @brief setP2Line set id second point of line
     * @param value id
     * @param id don't show this id in list
     */
    void           setP2Line(const quint32 &value, const quint32 &id);
    /**
     * @brief getPShoulder return id shoulder point
     * @return id
     */
    quint32        getPShoulder() const;
    /**
     * @brief setPShoulder set id shoulder point
     * @param value id
     * @param id don't show this id in list
     */
    void           setPShoulder(const quint32 &value, const quint32 &id);
public slots:
    /**
     * @brief ChoosedObject gets id and type of selected object. Save right data and ignore wrong.
     * @param id id of point or detail
     * @param type type of object
     */
    virtual void   ChoosedObject(quint32 id, const Valentina::Scenes &type);
    /**
     * @brief DialogAccepted save data and emit signal about closed dialog.
     */
    virtual void   DialogAccepted();
    /**
     * @brief DialogApply apply data and emit signal about applied dialog.
     */
    virtual void      DialogApply();
    /**
     * @brief DeployFormulaTextEdit grow or shrink formula input
     */
    void DeployFormulaTextEdit();
    /**
     * @brief FormulaTextChanged when formula text changes for validation and calc
     */
    void FormulaTextChanged();
private:
    Q_DISABLE_COPY(DialogShoulderPoint)
    /**
     * @brief SaveData Put dialog data in local variables
     */
    void SaveData();
    /**
     * @brief ui keeps information about user interface
     */
    Ui::DialogShoulderPoint *ui;
    /**
     * @brief number number of handled objects
     */
    qint32         number;
    /**
     * @brief pointName name of point
     */
    QString        pointName;
    /**
     * @brief typeLine type of line
     */
    QString        typeLine;
    /**
     * @brief formula formula
     */
    QString        formula;
    /**
     * @brief p1Line id first point of line
     */
    quint32         p1Line;
    /**
     * @brief p2Line id second point of line
     */
    quint32         p2Line;
    /**
     * @brief pShoulder id shoulder point
     */
    quint32         pShoulder;
    /**
     * @brief formulaBaseHeight base height defined by dialogui
     */
    int formulaBaseHeight;
};

inline QString DialogShoulderPoint::getPointName() const
{
    return pointName;
}

inline QString DialogShoulderPoint::getTypeLine() const
{
    return typeLine;
}

inline QString DialogShoulderPoint::getFormula() const
{
    return qApp->FormulaFromUser(formula);
}

inline quint32 DialogShoulderPoint::getP1Line() const
{
    return p1Line;
}

inline quint32 DialogShoulderPoint::getP2Line() const
{
    return p2Line;
}

inline quint32 DialogShoulderPoint::getPShoulder() const
{
    return pShoulder;
}

#endif // DIALOGSHOULDERPOINT_H
