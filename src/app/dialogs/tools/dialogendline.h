/************************************************************************
 **
 **  @file   dialogendline.h
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

#ifndef DIALOGENDLINE_H
#define DIALOGENDLINE_H

#include "dialogtool.h"

namespace Ui
{
    class DialogEndLine;
}

/**
 * @brief The DialogEndLine class dialog for ToolEndLine. Help create point and edit option.
 */
class DialogEndLine : public DialogTool
{
    Q_OBJECT
public:
    DialogEndLine(const VContainer *data, QWidget *parent = nullptr);
    ~DialogEndLine();

    QString           getPointName() const;
    void              setPointName(const QString &value);

    QString           getTypeLine() const;
    void              setTypeLine(const QString &value);

    QString           getFormula() const;
    void              setFormula(const QString &value);

    qreal             getAngle() const;
    void              setAngle(const qreal &value);

    quint32           getBasePointId() const;
    void              setBasePointId(const quint32 &value, const quint32 &id);
public slots:
    virtual void      ChoosedObject(quint32 id, const SceneObject &type);
    virtual void      DialogAccepted();
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
    Q_DISABLE_COPY(DialogEndLine)

    /** @brief ui keeps information about user interface */
    Ui::DialogEndLine *ui;

    /** @brief pointName name of point */
    QString           pointName;

    /** @brief typeLine type of line */
    QString           typeLine;

    /** @brief formula formula */
    QString           formula;

    /** @brief angle angle of line */
    qreal             angle;

    /** @brief basePointId id base point of line */
    quint32            basePointId;
    /** @brief formulaBaseHeight base height defined by dialogui */
    int formulaBaseHeight;

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
inline QString DialogEndLine::getPointName() const
{
    return pointName;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief getTypeLine return type of line
 * @return type
 */
inline QString DialogEndLine::getTypeLine() const
{
    return typeLine;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief getFormula return string of formula
 * @return formula
 */
inline QString DialogEndLine::getFormula() const
{
    return qApp->FormulaFromUser(formula);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief getAngle return angle of line
 * @return angle in degree
 */
inline qreal DialogEndLine::getAngle() const
{
    return angle;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief getBasePointId return id base point of line
 * @return id
 */
inline quint32 DialogEndLine::getBasePointId() const
{
    return basePointId;
}

#endif // DIALOGENDLINE_H
