/************************************************************************
 **
 **  @file   dialoglineintersect.h
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

#ifndef DIALOGLINEINTERSECT_H
#define DIALOGLINEINTERSECT_H

#include "dialogtool.h"

namespace Ui
{
    class DialogLineIntersect;
}

/**
 * @brief The DialogLineIntersect class dialog for ToolLineIntersect. Help create point and edit option.
 */
class DialogLineIntersect : public DialogTool
{
    Q_OBJECT
public:
    /**
     * @brief DialogLineIntersect create dialog
     * @param data container with data
     * @param parent parent widget
     */
    DialogLineIntersect(const VContainer *data, QWidget *parent = nullptr);
    ~DialogLineIntersect();
    /**
     * @brief getP1Line1 return id first point of first line
     * @return id
     */
    quint32                 getP1Line1() const;
    /**
     * @brief setP1Line1 set id first point of first line
     * @param value id
     */
    void                    setP1Line1(const quint32 &value);
    /**
     * @brief getP2Line1 return id second point of first line
     * @return id
     */
    quint32                 getP2Line1() const;
    /**
     * @brief setP2Line1 set id second point of first line
     * @param value id
     */
    void                    setP2Line1(const quint32 &value);
    /**
     * @brief getP1Line2 return id first point of second line
     * @return id
     */
    quint32                 getP1Line2() const;
    /**
     * @brief setP1Line2 set id first point of second line
     * @param value id
     */
    void                    setP1Line2(const quint32 &value);
    /**
     * @brief getP2Line2 return id second point of second line
     * @return id
     */
    quint32                 getP2Line2() const;
    /**
     * @brief setP2Line2 set id second point of second line
     * @param value id
     */
    void                    setP2Line2(const quint32 &value);
    /**
     * @brief getPointName return name of point
     * @return
     */
    QString                 getPointName() const;
    /**
     * @brief setPointName set name of point
     * @param value
     */
    void                    setPointName(const QString &value);
public slots:
    /**
     * @brief ChoosedObject gets id and type of selected object. Save right data and ignore wrong.
     * @param id id of point or detail
     * @param type type of object
     */
    virtual void            ChoosedObject(quint32 id, const Valentina::Scenes &type);
    /**
     * @brief DialogAccepted save data and emit signal about closed dialog.
     */
    virtual void            DialogAccepted();
    /** TODO ISSUE 79 : create real function
     * @brief DialogApply apply data and emit signal about applied dialog.
     */
    virtual void      DialogApply(){}
    /**
     * @brief P1Line1Changed changed first point of first line
     * @param index index in list
     */
    void                    P1Line1Changed( int index);
    /**
     * @brief P2Line1Changed changed second point of first line
     * @param index index in list
     */
    void                    P2Line1Changed( int index);
    /**
     * @brief P1Line2Changed changed first point of second line
     * @param index index in list
     */
    void                    P1Line2Changed( int index);
    /**
     * @brief P2Line2Changed changed second point of second line
     * @param index index in list
     */
    void                    P2Line2Changed( int index);
private:
    Q_DISABLE_COPY(DialogLineIntersect)
    /**
     * @brief ui keeps information about user interface
     */
    Ui::DialogLineIntersect *ui;
    /**
     * @brief number number of handled objects
     */
    qint32                  number;
    /**
     * @brief pointName name of point
     */
    QString                 pointName;
    /**
     * @brief p1Line1 id first point of first line
     */
    quint32                  p1Line1;
    /**
     * @brief p2Line1 id second point of first line
     */
    quint32                  p2Line1;
    /**
     * @brief p1Line2 id first point of second line
     */
    quint32                  p1Line2;
    /**
     * @brief p2Line2 id second point of second line
     */
    quint32                  p2Line2;
    /**
     * @brief flagPoint keep state of point
     */
    bool                    flagPoint;
    /**
     * @brief CheckState check state of dialog. Enable or disable button ok.
     */
    virtual void            CheckState();
    /**
     * @brief CheckIntersecion check intersection of points
     * @return true - line have intersection, false = don't have
     */
    bool                    CheckIntersecion();
};

inline quint32 DialogLineIntersect::getP1Line1() const
{
    return p1Line1;
}

inline quint32 DialogLineIntersect::getP2Line1() const
{
    return p2Line1;
}

inline quint32 DialogLineIntersect::getP1Line2() const
{
    return p1Line2;
}

inline quint32 DialogLineIntersect::getP2Line2() const
{
    return p2Line2;
}

inline QString DialogLineIntersect::getPointName() const
{
    return pointName;
}

#endif // DIALOGLINEINTERSECT_H
