/************************************************************************
 **
 **  @file   dialogspline.h
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
                  /**
                   * @brief DialogSpline create dialog
                   * @param data container with data
                   * @param parent parent widget
                   */
                  DialogSpline(const VContainer *data, QWidget *parent = 0);
                  ~DialogSpline();
    /**
     * @brief getP1 return id first point of spline
     * @return id
     */
    qint64        getP1() const;
    /**
     * @brief setP1 set id first point of spline
     * @param value id
     */
    void          setP1(const qint64 &value);
    /**
     * @brief getP4 return id fourth point of spline
     * @return id
     */
    qint64        getP4() const;
    /**
     * @brief setP4 set id fourth point of spline
     * @param value id
     */
    void          setP4(const qint64 &value);
    /**
     * @brief getAngle1 return first angle of spline
     * @return angle in degree
     */
    inline qreal  getAngle1() const {return angle1;}
    /**
     * @brief setAngle1 set first angle of spline
     * @param value angle in degree
     */
    void          setAngle1(const qreal &value);
    /**
     * @brief getAngle2 return second angle of spline
     * @return angle in degree
     */
    inline qreal  getAngle2() const {return angle2;}
    /**
     * @brief setAngle2 set second angle of spline
     * @param value angle in degree
     */
    void          setAngle2(const qreal &value);
    /**
     * @brief getKAsm1 return first coefficient asymmetry
     * @return value. Can be >= 0.
     */
    inline qreal  getKAsm1() const {return kAsm1;}
    /**
     * @brief setKAsm1 set first coefficient asymmetry
     * @param value value. Can be >= 0.
     */
    void          setKAsm1(const qreal &value);
    /**
     * @brief getKAsm2 return second coefficient asymmetry
     * @return value. Can be >= 0.
     */
    inline qreal  getKAsm2() const {return kAsm2;}
    /**
     * @brief setKAsm2 set second coefficient asymmetry
     * @param value value. Can be >= 0.
     */
    void          setKAsm2(const qreal &value);
    /**
     * @brief getKCurve return coefficient curve
     * @return value. Can be >= 0.
     */
    inline qreal  getKCurve() const {return kCurve;}
    /**
     * @brief setKCurve set coefficient curve
     * @param value value. Can be >= 0.
     */
    void          setKCurve(const qreal &value);
public slots:
    /**
     * @brief ChoosedObject gets id and type of selected object. Save right data and ignore wrong.
     * @param id id of point or detail
     * @param type type of object
     */
    virtual void  ChoosedObject(qint64 id, const Scene::Scenes &type);
    /**
     * @brief DialogAccepted save data and emit signal about closed dialog.
     */
    virtual void  DialogAccepted();
private:
    Q_DISABLE_COPY(DialogSpline)
    /**
     * @brief ui keeps information about user interface
     */
    Ui::DialogSpline *ui;
    /**
     * @brief number number of handled objects
     */
    qint32        number;
    /**
     * @brief p1 id first point of spline
     */
    qint64        p1;
    /**
     * @brief p4 id fourth point of spline
     */
    qint64        p4;
    /**
     * @brief angle1 first angle of spline in degree
     */
    qreal         angle1;
    /**
     * @brief angle2 second angle of spline in degree
     */
    qreal         angle2;
    /**
     * @brief kAsm1 first coefficient asymmetry
     */
    qreal         kAsm1;
    /**
     * @brief kAsm2 second coefficient asymmetry
     */
    qreal         kAsm2;
    /**
     * @brief kCurve coefficient curve
     */
    qreal         kCurve;
};

#endif // DIALOGSPLINE_H
