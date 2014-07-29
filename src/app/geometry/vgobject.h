/************************************************************************
 **
 **  @file   vgobject.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   27 12, 2013
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

#ifndef VGOBJECT_H
#define VGOBJECT_H

#include "../options.h"
#include <QString>
#include <QtGlobal>

enum class GOType : char { Point, Arc, Spline, SplinePath, Unknown };

/**
 * @brief The VGObject class keep information graphical objects.
 */
class VGObject
{
public:
    VGObject();
    VGObject(const GOType &type, const quint32 &idObject = 0, const Draw &mode = Draw::Calculation);
    VGObject(const VGObject &obj);
    VGObject& operator= (const VGObject &obj);
    virtual ~VGObject(){}
    quint32         getIdObject() const;
    void            setIdObject(const quint32 &value);
    virtual QString name() const;
    void            setName(const QString &name);
    Draw            getMode() const;
    void            setMode(const Draw &value);
    GOType          getType() const;
    quint32         id() const;
    virtual void    setId(const quint32 &id);
protected:
    /** @brief _id id in container. Ned for arcs, spline and spline paths. */
    quint32 _id;

    /** @brief type type of graphical object */
    GOType  type;

    /** @brief idObject id of parent object. Only for modeling. All another return 0. */
    quint32 idObject;

    /** @brief _name object name */
    QString _name;

    /** @brief mode object created in calculation or drawing mode */
    Draw    mode;
};

#endif // VGOBJECT_H
