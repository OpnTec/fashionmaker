/************************************************************************
 **
 **  @file   dialogendline.h
 **  @author Roman Telezhinsky <dismine@gmail.com>
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
    /**
     * @brief DialogEndLine create dialog
     * @param data container with data
     * @param parent parent widget
     */
    DialogEndLine(const VContainer *data, QWidget *parent = nullptr);
    ~DialogEndLine();
    /**
     * @brief getPointName return name of point
     * @return name
     */
    QString           getPointName() const;
    /**
     * @brief setPointName set name of point
     * @param value name
     */
    void              setPointName(const QString &value);
    /**
     * @brief getTypeLine return type of line
     * @return type
     */
    QString           getTypeLine() const;
    /**
     * @brief setTypeLine set type of line
     * @param value type
     */
    void              setTypeLine(const QString &value);
    /**
     * @brief getFormula return string of formula
     * @return formula
     */
    QString           getFormula() const;
    /**
     * @brief setFormula set string of formula
     * @param value formula
     */
    void              setFormula(const QString &value);
    /**
     * @brief getAngle return angle of line
     * @return angle in degree
     */
    qreal             getAngle() const;
    /**
     * @brief setAngle set angle of line
     * @param value angle in degree
     */
    void              setAngle(const qreal &value);
    /**
     * @brief getBasePointId return id base point of line
     * @return id
     */
    quint32           getBasePointId() const;
    /**
     * @brief setBasePointId set id base point of line
     * @param value id
     * @param id don't show this id in list
     */
    void              setBasePointId(const quint32 &value, const quint32 &id);
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
private:
    Q_DISABLE_COPY(DialogEndLine)
    /**
     * @brief ui keeps information about user interface
     */
    Ui::DialogEndLine *ui;
    /**
     * @brief pointName name of point
     */
    QString           pointName;
    /**
     * @brief typeLine type of line
     */
    QString           typeLine;
    /**
     * @brief formula formula
     */
    QString           formula;
    /**
     * @brief angle angle of line
     */
    qreal             angle;
    /**
     * @brief basePointId id base point of line
     */
    quint32            basePointId;
};

inline QString DialogEndLine::getPointName() const
{
    return pointName;
}

inline QString DialogEndLine::getTypeLine() const
{
    return typeLine;
}

inline QString DialogEndLine::getFormula() const
{
    return formula;
}

inline qreal DialogEndLine::getAngle() const
{
    return angle;
}

inline quint32 DialogEndLine::getBasePointId() const
{
    return basePointId;
}

#endif // DIALOGENDLINE_H
