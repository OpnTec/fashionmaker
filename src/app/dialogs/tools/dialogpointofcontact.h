/************************************************************************
 **
 **  @file   dialogpointofcontact.h
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

#ifndef DIALOGPOINTOFCONTACT_H
#define DIALOGPOINTOFCONTACT_H

#include "ui_dialogpointofcontact.h"
#include "dialogtool.h"

#define DIALOGPOINTOFCONTACT_MAX_FORMULA_HEIGHT 64
namespace Ui
{
    class DialogPointOfContact;
}
/**
 * @brief The DialogPointOfContact class dialog for ToolPointOfContact. Help create point and edit option.
 */
class DialogPointOfContact : public DialogTool
{
    Q_OBJECT
public:
    DialogPointOfContact(const VContainer *data, QWidget *parent = nullptr);

    QString        getPointName() const;
    void           setPointName(const QString &value);

    QString        getRadius() const;
    void           setRadius(const QString &value);

    quint32        getCenter() const;
    void           setCenter(const quint32 &value, const quint32 &id);

    quint32        getFirstPoint() const;
    void           setFirstPoint(const quint32 &value, const quint32 &id);

    quint32        getSecondPoint() const;
    void           setSecondPoint(const quint32 &value, const quint32 &id);
public slots:
    virtual void   ChoosedObject(quint32 id, const Valentina::Scenes &type);
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
    Q_DISABLE_COPY(DialogPointOfContact)

    /** @brief ui keeps information about user interface */
    Ui::DialogPointOfContact *ui;

    /** @brief number number of handled objects */
    qint32         number;

    /** @brief pointName name of point */
    QString        pointName;

    /** @brief radius radius of arc */
    QString        radius;

    /** @brief center id center point of arc */
    quint32         center;

    /** @brief firstPoint id first point of line */
    quint32         firstPoint;

    /** @brief secondPoint id second point of line */
    quint32         secondPoint;

    /** @brief formulaBaseHeight base height defined by dialogui */
    int             formulaBaseHeight;

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
inline QString DialogPointOfContact::getPointName() const
{
    return pointName;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief getRadius return formula radius of arc
 * @return formula
 */
inline QString DialogPointOfContact::getRadius() const
{
    return radius;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief GetCenter return id of center point
 * @return id
 */
inline quint32 DialogPointOfContact::getCenter() const
{
    return center;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief getFirstPoint return id first point
 * @return id
 */
inline  quint32 DialogPointOfContact::getFirstPoint() const
{
    return firstPoint;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief getSecondPoint return id second point
 * @return id
 */
inline quint32 DialogPointOfContact::getSecondPoint() const
{
    return secondPoint;
}

#endif // DIALOGPOINTOFCONTACT_H
