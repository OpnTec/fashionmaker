/************************************************************************
 **
 **  @file   dialogheight.h
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

#ifndef DIALOGHEIGHT_H
#define DIALOGHEIGHT_H

#include "dialogtool.h"

namespace Ui
{
    class DialogHeight;
}

/**
 * @brief The DialogHeight class dialog for ToolHeight. Help create point and edit option.
 */
class DialogHeight : public DialogTool
{
    Q_OBJECT
public:
    /**
     * @brief DialogHeight create dialog
     * @param data container with data
     * @param parent parent widget
     */
    DialogHeight(const VContainer *data, QWidget *parent = nullptr);
    ~DialogHeight();
    /**
     * @brief getPointName return name of point
     * @return name
     */
    QString          getPointName() const;
    /**
     * @brief setPointName set name of point
     * @param value name
     */
    void             setPointName(const QString &value);
    /**
     * @brief getTypeLine return type of line
     * @return type
     */
    QString          getTypeLine() const;
    /**
     * @brief setTypeLine set type of line
     * @param value type
     */
    void             setTypeLine(const QString &value);
    /**
     * @brief getBasePointId return id base point of height
     * @return id
     */
    quint32          getBasePointId() const;
    /**
     * @brief setBasePointId set id base point of height
     * @param value id
     * @param id don't show this id in list
     */
    void             setBasePointId(const quint32 &value, const quint32 &id);
    /**
     * @brief getP1LineId return id first point of line
     * @return id id
     */
    quint32          getP1LineId() const;
    /**
     * @brief setP1LineId set id first point of line
     * @param value id
     * @param id don't show this id in list
     */
    void             setP1LineId(const quint32 &value, const quint32 &id);
    /**
     * @brief getP2LineId return id second point of line
     * @return id
     */
    quint32          getP2LineId() const;
    /**
     * @brief setP2LineId set id second point of line
     * @param value id
     * @param id don't show this id in list
     */
    void             setP2LineId(const quint32 &value, const quint32 &id);
public slots:
    /**
     * @brief ChoosedObject gets id and type of selected object. Save right data and ignore wrong.
     * @param id id of point or detail
     * @param type type of object
     */
    virtual void     ChoosedObject(quint32 id, const Scene::Scenes &type);
    /**
     * @brief DialogAccepted save data and emit signal about closed dialog.
     */
    virtual void     DialogAccepted();
private:
    Q_DISABLE_COPY(DialogHeight)
    /**
     * @brief ui keeps information about user interface
     */
    Ui::DialogHeight *ui;
    /**
     * @brief number number of handled objects
     */
    qint32           number;
    /**
     * @brief pointName name of point
     */
    QString          pointName;
    /**
     * @brief typeLine type of line
     */
    QString          typeLine;
    /**
     * @brief basePointId id base point of height
     */
    quint32           basePointId;
    /**
     * @brief p1LineId id first point of line
     */
    quint32           p1LineId;
    /**
     * @brief p2LineId id second point of line
     */
    quint32           p2LineId;
};

inline QString DialogHeight::getPointName() const
{
    return pointName;
}

inline QString DialogHeight::getTypeLine() const
{
    return typeLine;
}

inline quint32 DialogHeight::getBasePointId() const
{
    return basePointId;
}

inline quint32 DialogHeight::getP1LineId() const
{
    return p1LineId;
}

inline quint32 DialogHeight::getP2LineId() const
{
    return p2LineId;
}

#endif // DIALOGHEIGHT_H
