/************************************************************************
 **
 **  @file   vgobject.h
 **  @author Roman Telezhinsky <dismine@gmail.com>
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
#include "../exception/vexception.h"
#include <QCoreApplication>
#include <QHash>


namespace GObject
{
    /**
     * @brief The NodeDetail enum type of graphical objects.
     */
    enum Type { Point, Arc, Spline, SplinePath };
    Q_DECLARE_FLAGS(Types, Type)
}
Q_DECLARE_OPERATORS_FOR_FLAGS(GObject::Types)

/**
 * @brief The VGObject class keep information graphical objects.
 */
class VGObject
{
public:
    /**
     * @brief VGObject default constructor.
     */
    VGObject();
    /**
     * @brief VGObject constructor.
     * @param type type graphical object.
     * @param idObject id parent object.
     * @param mode mode creation. Used in modeling mode.
     */
    VGObject(const GObject::Type &type, const quint32 &idObject = 0,
             const Valentina::Draws &mode = Valentina::Calculation);
    /**
     * @brief VGObject copy constructor.
     * @param obj object.
     */
    VGObject(const VGObject &obj);
    /**
     * @brief operator = assignment operator.
     * @param obj object
     * @return object
     */
    VGObject& operator= (const VGObject &obj);
    virtual ~VGObject(){}
    /**
     * @brief getIdObject return parent id.
     * @return parent id or 0 if object don't have parent.
     */
    quint32          getIdObject() const;
    /**
     * @brief setIdObject set parent id.
     * @param value parent id.
     */
    void            setIdObject(const quint32 &value);
    /**
     * @brief name return name graphical object.
     * @return name
     */
    virtual QString name() const;
    /**
     * @brief setName set name graphical object.
     * @param name name graphical object.
     */
    void            setName(const QString &name);
    /**
     * @brief getMode return mode creation.
     * @return mode.
     */
    Valentina::Draws     getMode() const;
    /**
     * @brief setMode set mode creation.
     * @param value mode.
     */
    void            setMode(const Valentina::Draws &value);
    /**
     * @brief getType return object type.
     * @return type.
     */
    GObject::Type   getType() const;
    /**
     * @brief id return id object.
     * @return id
     */
    quint32          id() const;
    /**
     * @brief setId set id object.
     * @param id id.
     */
    virtual void    setId(const quint32 &id);
protected:
    /**
     * @brief _id id in container. Ned for arcs, spline and spline paths.
     */
    quint32          _id;
    /**
     * @brief type type of graphical object
     */
    GObject::Type   type;
    /**
     * @brief idObject id of parent object. Only for modeling. All another return 0.
     */
    quint32          idObject;
    /**
     * @brief _name object name
     */
    QString         _name;
    /**
     * @brief mode object created in calculation or drawing mode
     */
    Valentina::Draws     mode;
};

#endif // VGOBJECT_H
