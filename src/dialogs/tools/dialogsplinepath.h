/************************************************************************
 **
 **  @file   dialogsplinepath.h
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

#ifndef DIALOGSPLINEPATH_H
#define DIALOGSPLINEPATH_H

#include "dialogtool.h"
#include "../../geometry/vsplinepath.h"

namespace Ui
{
    class DialogSplinePath;
}

/**
 * @brief The DialogSplinePath class dialog for ToolSplinePath. Help create spline path and edit option.
 */
class DialogSplinePath : public DialogTool
{
    Q_OBJECT
public:
    /**
     * @brief DialogSplinePath create dialog
     * @param data container with data
     * @param parent parent widget
     */
    DialogSplinePath(const VContainer *data, QWidget *parent = nullptr);
    ~DialogSplinePath();
    /**
     * @brief GetPath return spline path
     * @return path
     */
    VSplinePath        GetPath() const;
    /**
     * @brief SetPath set spline path
     * @param value path
     */
    void               SetPath(const VSplinePath &value);
public slots:
    /**
     * @brief ChoosedObject gets id and type of selected object. Save right data and ignore wrong.
     * @param id id of point or detail
     * @param type don't show this id in list
     */
    virtual void       ChoosedObject(quint32 id, const Valentina::Scenes &type);
    /**
     * @brief DialogAccepted save data and emit signal about closed dialog.
     */
    virtual void       DialogAccepted();
    /**
     * @brief PointChanged selected another point in list
     * @param row number of row
     */
    void               PointChanged(int row);
    /**
     * @brief currentPointChanged changed point in combo box
     * @param index index in list
     */
    void               currentPointChanged( int index );
    /**
     * @brief Angle1Changed changed first angle
     * @param index index in list
     */
    void               Angle1Changed(qreal index );
    /**
     * @brief Angle2Changed changed second angle
     * @param index index in list
     */
    void               Angle2Changed( qreal index );
    /**
     * @brief KAsm1Changed changed first coefficient asymmetry
     * @param d value
     */
    void               KAsm1Changed(qreal d);
    /**
     * @brief KAsm2Changed changed second coefficient asymmetry
     * @param d value
     */
    void               KAsm2Changed(qreal d);
private:
    Q_DISABLE_COPY(DialogSplinePath)
    /**
     * @brief ui keeps information about user interface
     */
    Ui::DialogSplinePath *ui;
    /**
     * @brief path spline path
     */
    VSplinePath        path;
    /**
     * @brief NewItem add point to list
     * @param id id
     * @param kAsm1 first coefficient asymmetry
     * @param angle1 first angle in degree
     * @param kAsm2 second coefficient asymmetry
     * @param angle2 second angle in degree
     */
    void               NewItem(quint32 id, qreal kAsm1, qreal angle1, qreal kAsm2, qreal angle2);
    /**
     * @brief dataPoint show data of point in fields
     * @param id id
     * @param kAsm1 first coefficient asymmetry
     * @param angle1 first angle of spline
     * @param kAsm2 second coefficient asymmetry
     * @param angle2 second angle of spline
     */
    void               DataPoint(quint32 id, qreal kAsm1, qreal angle1, qreal kAsm2, qreal angle2);
    /**
     * @brief EnableFields enable or disable fields
     */
    void               EnableFields();
};

inline VSplinePath DialogSplinePath::GetPath() const
{
    return path;
}

#endif // DIALOGSPLINEPATH_H
