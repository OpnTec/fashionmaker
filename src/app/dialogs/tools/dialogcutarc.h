/************************************************************************
 **
 **  @file   dialogcutarc.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   7 1, 2014
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

#ifndef DIALOGCUTARC_H
#define DIALOGCUTARC_H

#include "dialogtool.h"

namespace Ui
{
    class DialogCutArc;
}

/**
 * @brief The DialogCutArc class dialog for ToolCutArc.
 */
class DialogCutArc : public DialogTool
{
    Q_OBJECT
public:
    /**
     * @brief DialogCutArc create dialog.
     * @param data container with data
     * @param parent parent widget
     */
    DialogCutArc(const VContainer *data, QWidget *parent = nullptr);
    ~DialogCutArc();
    /**
     * @brief getPointName return name point on arc
     * @return name
     */
    QString           getPointName() const {return pointName;}
    /**
     * @brief setPointName set name point on arc
     * @param value name
     */
    void              setPointName(const QString &value);
    /**
     * @brief getFormula return string with formula length
     * @return formula
     */
    QString           getFormula() const {return qApp->FormulaFromUser(formula);}
    /**
     * @brief setFormula set string with formula length
     * @param value string with formula
     */
    void              setFormula(const QString &value);
    /**
     * @brief getArcId return id of arc
     * @return id
     */
    quint32            getArcId() const {return arcId;}
    /**
     * @brief setArcId set id of arc
     * @param value id
     * @param id don't show this id in list
     */
    void              setArcId(const quint32 &value, const quint32 &id);
public slots:
    /**
     * @brief ChoosedObject gets id and type of selected object. Save right data and ignore wrong.
     * @param id id of point or detail
     * @param type type of object
     */
    virtual void      ChoosedObject(quint32 id, const Valentina::Scenes &type);
    /**
     * @brief DialogAccepted save data and emit signal about closed dialog.
     */
    virtual void      DialogAccepted();
    /** TODO ISSUE 79 : create real function
     * @brief DialogApply apply data and emit signal about applied dialog.
     */
    virtual void      DialogApply(){}
private:
    Q_DISABLE_COPY(DialogCutArc)
    /**
     * @brief ui keeps information about user interface
     */
    Ui::DialogCutArc  *ui;
    /**
     * @brief pointName name of created point
     */
    QString           pointName;
    /**
     * @brief formula string with formula
     */
    QString           formula;
    /**
     * @brief arcId keep id of arc
     */
    quint32            arcId;
};

#endif // DIALOGCUTARC_H
