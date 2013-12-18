/************************************************************************
 **
 **  @file   dialogcutspline.h
 **  @author Roman Telezhinsky <dismine@gmail.com>
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

namespace Ui {
class DialogCutSpline;
}

class DialogCutSpline : public DialogTool
{
    Q_OBJECT
public:
    DialogCutSpline(const VContainer *data, Draw::Draws mode = Draw::Calculation, QWidget *parent = 0);
    ~DialogCutSpline();
    /**
     * @brief getPointName return name of point
     * @return name
     */
    inline QString    getPointName() const {return pointName;}
    /**
     * @brief setPointName set name of point
     * @param value name
     */
    void              setPointName(const QString &value);
    /**
     * @brief getFormula return string of formula
     * @return formula
     */
    inline QString    getFormula() const {return formula;}
    /**
     * @brief setFormula set string of formula
     * @param value formula
     */
    void              setFormula(const QString &value);
    /**
     * @brief getSplineId return id base point of line
     * @return id
     */
    inline qint64     getSplineId() const {return splineId;}
    /**
     * @brief setSplineId set id spline
     * @param value id
     * @param id don't show this id in list
     */
    void              setSplineId(const qint64 &value, const qint64 &id);
public slots:
    /**
     * @brief ChoosedObject gets id and type of selected object. Save right data and ignore wrong.
     * @param id id of point or detail
     * @param type type of object
     */
    virtual void      ChoosedObject(qint64 id, const Scene::Scenes &type);
    /**
     * @brief DialogAccepted save data and emit signal about closed dialog.
     */
    virtual void      DialogAccepted();
private:
    Q_DISABLE_COPY(DialogCutSpline)
    Ui::DialogCutSpline *ui;
    QString           pointName;
    QString           formula;
    qint64            splineId;
};

#endif // DIALOGCUTSPLINE_H
