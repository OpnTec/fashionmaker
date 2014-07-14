/************************************************************************
 **
 **  @file   dialogpointofintersection.h
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

#ifndef DIALOGPOINTOFINTERSECTION_H
#define DIALOGPOINTOFINTERSECTION_H

#include "dialogtool.h"

namespace Ui
{
    class DialogPointOfIntersection;
}

/**
 * @brief The DialogPointOfIntersection class dialog for ToolPointOfIntersection. Help create point and edit option.
 */
class DialogPointOfIntersection : public DialogTool
{
    Q_OBJECT
public:
    DialogPointOfIntersection(const VContainer *data, QWidget *parent = nullptr);
    ~DialogPointOfIntersection();

    QString        getPointName() const;
    void           setPointName(const QString &value);

    quint32        getFirstPointId() const;
    void           setFirstPointId(const quint32 &value, const quint32 &id);

    quint32        getSecondPointId() const;
    void           setSecondPointId(const quint32 &value, const quint32 &id);
public slots:
    virtual void   ChoosedObject(quint32 id, const SceneObject &type);
    virtual void   DialogAccepted();
    /** TODO ISSUE 79 : create real function
     * @brief DialogApply apply data and emit signal about applied dialog.
     */
    virtual void   DialogApply(){}
    virtual void   PointNameChanged();
private:
    Q_DISABLE_COPY(DialogPointOfIntersection)

    /** @brief ui keeps information about user interface */
    Ui::DialogPointOfIntersection *ui;

    /** @brief number number of handled objects */
    qint32         number;

    /** @brief pointName name of point */
    QString        pointName;

    /** @brief firstPointId id first point of line */
    quint32        firstPointId;

    /** @brief secondPointId id second point of line */
    quint32        secondPointId;
};

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief getPointName return name of point
 * @return name
 */
inline QString DialogPointOfIntersection::getPointName() const
{
    return pointName;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief getFirstPointId return id of first point
 * @return id
 */
inline quint32 DialogPointOfIntersection::getFirstPointId() const
{
    return firstPointId;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief getSecondPointId return id of second point
 * @return id
 */
inline quint32 DialogPointOfIntersection::getSecondPointId() const
{
    return secondPointId;
}

#endif // DIALOGPOINTOFINTERSECTION_H
