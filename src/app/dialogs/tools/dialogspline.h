/************************************************************************
 **
 **  @file   dialogspline.h
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

#ifndef DIALOGSPLINE_H
#define DIALOGSPLINE_H

#include "dialogtool.h"

namespace Ui
{
    class DialogSpline;
}

/**
 * @brief The DialogSpline class dialog for ToolSpline. Help create spline and edit option.
 */
class DialogSpline : public DialogTool
{
    Q_OBJECT
public:
    DialogSpline(const VContainer *data, QWidget *parent = nullptr);
    ~DialogSpline();

    quint32        getP1() const;
    void          setP1(const quint32 &value);

    quint32        getP4() const;
    void          setP4(const quint32 &value);

    qreal         getAngle1() const;
    void          setAngle1(const qreal &value);

    qreal         getAngle2() const;
    void          setAngle2(const qreal &value);

    qreal         getKAsm1() const;
    void          setKAsm1(const qreal &value);

    qreal         getKAsm2() const;
    void          setKAsm2(const qreal &value);

    qreal         getKCurve() const;
    void          setKCurve(const qreal &value);
public slots:
    virtual void  ChoosedObject(quint32 id, const SceneObject &type);
    virtual void  DialogAccepted();
    /** TODO ISSUE 79 : create real function
     * @brief DialogApply apply data and emit signal about applied dialog.
     */
    virtual void  DialogApply(){}
    virtual void  PointNameChanged();
    virtual void  UpdateList();
private:
    Q_DISABLE_COPY(DialogSpline)

    /** @brief ui keeps information about user interface */
    Ui::DialogSpline *ui;

    /** @brief number number of handled objects */
    qint32        number;

    /** @brief p1 id first point of spline */
    quint32        p1;

    /** @brief p4 id fourth point of spline */
    quint32        p4;

    /** @brief angle1 first angle of spline in degree */
    qreal         angle1;

    /** @brief angle2 second angle of spline in degree */
    qreal         angle2;

    /** @brief kAsm1 first coefficient asymmetry */
    qreal         kAsm1;

    /** @brief kAsm2 second coefficient asymmetry */
    qreal         kAsm2;

    /** @brief kCurve coefficient curve */
    qreal         kCurve;
};

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief getAngle1 return first angle of spline
 * @return angle in degree
 */
inline qreal DialogSpline::getAngle1() const
{
    return angle1;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief getAngle2 return second angle of spline
 * @return angle in degree
 */
inline qreal DialogSpline::getAngle2() const
{
    return angle2;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief getKAsm1 return first coefficient asymmetry
 * @return value. Can be >= 0.
 */
inline qreal DialogSpline::getKAsm1() const
{
    return kAsm1;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief getKAsm2 return second coefficient asymmetry
 * @return value. Can be >= 0.
 */
inline qreal DialogSpline::getKAsm2() const
{
    return kAsm2;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief getKCurve return coefficient curve
 * @return value. Can be >= 0.
 */
inline qreal DialogSpline::getKCurve() const
{
    return kCurve;
}

#endif // DIALOGSPLINE_H
