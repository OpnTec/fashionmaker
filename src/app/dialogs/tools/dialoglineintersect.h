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
    DialogLineIntersect(const VContainer *data, QWidget *parent = nullptr);
    ~DialogLineIntersect();

    quint32                 getP1Line1() const;
    void                    setP1Line1(const quint32 &value);

    quint32                 getP2Line1() const;
    void                    setP2Line1(const quint32 &value);

    quint32                 getP1Line2() const;
    void                    setP1Line2(const quint32 &value);

    quint32                 getP2Line2() const;
    void                    setP2Line2(const quint32 &value);

    QString                 getPointName() const;
    void                    setPointName(const QString &value);
public slots:
    virtual void            ChoosedObject(quint32 id, const SceneObject &type);
    virtual void            DialogAccepted();
    /** TODO ISSUE 79 : create real function
     * @brief DialogApply apply data and emit signal about applied dialog.
     */
    virtual void            DialogApply(){}
    void                    P1Line1Changed( int index);
    void                    P2Line1Changed( int index);
    void                    P1Line2Changed( int index);
    void                    P2Line2Changed( int index);
    virtual void            PointNameChanged();
    virtual void            UpdateList();
private:
    Q_DISABLE_COPY(DialogLineIntersect)

    /** @brief ui keeps information about user interface */
    Ui::DialogLineIntersect *ui;

    /** @brief number number of handled objects */
    qint32                  number;

    /** @brief pointName name of point */
    QString                 pointName;

    /** @brief p1Line1 id first point of first line */
    quint32                  p1Line1;

    /** @brief p2Line1 id second point of first line */
    quint32                  p2Line1;

    /** @brief p1Line2 id first point of second line */
    quint32                  p1Line2;

    /** @brief p2Line2 id second point of second line */
    quint32                  p2Line2;

    /** @brief flagPoint keep state of point */
    bool                    flagPoint;

    virtual void            CheckState();
    bool                    CheckIntersecion();
};

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief getP1Line1 return id first point of first line
 * @return id
 */
inline quint32 DialogLineIntersect::getP1Line1() const
{
    return p1Line1;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief getP2Line1 return id second point of first line
 * @return id
 */
inline quint32 DialogLineIntersect::getP2Line1() const
{
    return p2Line1;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief getP1Line2 return id first point of second line
 * @return id
 */
inline quint32 DialogLineIntersect::getP1Line2() const
{
    return p1Line2;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief getP2Line2 return id second point of second line
 * @return id
 */
inline quint32 DialogLineIntersect::getP2Line2() const
{
    return p2Line2;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief getPointName return name of point
 * @return name of point
 */
inline QString DialogLineIntersect::getPointName() const
{
    return pointName;
}

#endif // DIALOGLINEINTERSECT_H
