/************************************************************************
 **
 **  @file   dialogbisector.h
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

#ifndef DIALOGBISECTOR_H
#define DIALOGBISECTOR_H

#include "dialogtool.h"

namespace Ui
{
    class DialogBisector;
}

/**
 * @brief The DialogBisector class dialog for ToolBisector. Help create point and edit option.
 */
class DialogBisector : public DialogTool
{
    Q_OBJECT
public:
    /**
     * @brief DialogBisector create dialog
     * @param data container with data
     * @param parent parent widget
     */
    DialogBisector(const VContainer *data, QWidget *parent = nullptr);
    ~DialogBisector();
    /**
     * @brief getPointName return name of point
     * @return name
     */
    QString            getPointName() const {return pointName;}
    /**
     * @brief setPointName set name of point
     * @param value name
     */
    void               setPointName(const QString &value);
    /**
     * @brief getTypeLine return type of line
     * @return type
     */
    QString            getTypeLine() const;
    /**
     * @brief setTypeLine set type of line
     * @param value type
     */
    void               setTypeLine(const QString &value);
    /**
     * @brief getFormula return string of formula
     * @return formula
     */
    QString            getFormula() const;
    /**
     * @brief setFormula set string of formula
     * @param value formula
     */
    void               setFormula(const QString &value);
    /**
     * @brief getFirstPointId return id of first point
     * @return id
     */
    quint32            getFirstPointId() const;
    /**
     * @brief setFirstPointId set id of first point
     * @param value id
     * @param id don't show this id in list
     */
    void               setFirstPointId(const quint32 &value, const quint32 &id);
    /**
     * @brief getSecondPointId return id of second point
     * @return id
     */
    quint32            getSecondPointId() const;
    /**
     * @brief setSecondPointId set id of second point
     * @param value id
     * @param id don't show this id in list
     */
    void               setSecondPointId(const quint32 &value, const quint32 &id);
    /**
     * @brief getThirdPointId return id of third point
     * @return id
     */
    quint32            getThirdPointId() const;
    /**
     * @brief setThirdPointId set id of third point
     * @param value id
     * @param id don't show this id in list
     */
    void               setThirdPointId(const quint32 &value, const quint32 &id);
public slots:
    /**
     * @brief ChoosedObject gets id and type of selected object. Save right data and ignore wrong.
     * @param id id of point or detail
     * @param type type of object
     */
    virtual void       ChoosedObject(quint32 id, const Valentina::Scenes &type);
    /**
     * @brief DialogAccepted save data and emit signal about closed dialog.
     */
    virtual void       DialogAccepted();
    /** TODO ISSUE 79 : create real function
     * @brief DialogApply apply data and emit signal about applied dialog.
     */
    virtual void      DialogApply(){}
private:
    Q_DISABLE_COPY(DialogBisector)
    /**
     * @brief ui keeps information about user interface
     */
    Ui::DialogBisector *ui;
    /**
     * @brief number number of handled objects
     */
    qint32             number;
    /**
     * @brief pointName name of point
     */
    QString            pointName;
    /**
     * @brief typeLine type of line
     */
    QString            typeLine;
    /**
     * @brief formula formula
     */
    QString            formula;
    /**
     * @brief firstPointId id of first point
     */
    quint32            firstPointId;
    /**
     * @brief secondPointId id of second point
     */
    quint32            secondPointId;
    /**
     * @brief thirdPointId id of third point
     */
    quint32            thirdPointId;
};

inline QString DialogBisector::getTypeLine() const
{
    return typeLine;
}

inline QString DialogBisector::getFormula() const
{
    return qApp->FormulaFromUser(formula);
}

inline quint32 DialogBisector::getFirstPointId() const
{
    return firstPointId;
}

inline quint32 DialogBisector::getSecondPointId() const
{
    return secondPointId;
}

inline quint32 DialogBisector::getThirdPointId() const
{
    return thirdPointId;
}

#endif // DIALOGBISECTOR_H
