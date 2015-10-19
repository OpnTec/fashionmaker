/****************************************************************************
** Copyright (C) 2001-2013 RibbonSoft, GmbH. All rights reserved.
**
** This file is part of the dxflib project.
**
** This file is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation; either version 2 of the License, or
** (at your option) any later version.
**
** Licensees holding valid dxflib Professional Edition licenses may use
** this file in accordance with the dxflib Commercial License
** Agreement provided with the Software.
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
** See http://www.ribbonsoft.com for further details.
**
** Contact info@ribbonsoft.com if any conditions of this licensing are
** not clear to you.
**
**********************************************************************/

#ifndef DL_EXTRUSION_H
#define DL_EXTRUSION_H

#include "dl_global.h"

#include <math.h>


/**
 * Storing and passing around attributes. Attributes
 * are the layer name, color, width and line type.
 *
 * @author Andrew Mustun
 */
class DXFLIB_EXPORT DL_Extrusion
{

public:

    /**
     * Default constructor.
     */
    DL_Extrusion() : direction(new double[3]), elevation(0.0)
    {
        setDirection(0.0, 0.0, 1.0);
    }


    /**
     * Destructor.
     */
    ~DL_Extrusion()
    {
        delete[] direction ;
    }

    DL_Extrusion(const DL_Extrusion &L)
        : direction(new double[3]), elevation(L.elevation)
    {
        setDirection(L.direction[0], L.direction[1], L.direction[0]);
    }

    /**
     * @brief DL_Extrusion Constructor for DXF extrusion.
     */
    DL_Extrusion(double dx, double dy, double dz, double elevation)
        : direction(new double[3]), elevation(elevation)
    {
        setDirection(dx, dy, dz);
    }



    /**
     * @brief setDirection Sets the direction vector.
     */
    void setDirection(double dx, double dy, double dz)
    {
        direction[0]=dx;
        direction[1]=dy;
        direction[2]=dz;
    }



    /**
     * @return direction vector.
     */
    double* getDirection() const
    {
        return direction;
    }

    void getDirection(double dir[]) const
    {
        dir[0]=direction[0];
        dir[1]=direction[1];
        dir[2]=direction[2];
    }



    /**
     * Sets the elevation.
     */
    void setElevation(double elevation)
    {
        this->elevation = elevation;
    }



    /**
     * @return Elevation.
     */
    double getElevation() const
    {
        return elevation;
    }



    /**
     * Copies extrusion (deep copies) from another extrusion object.
     */
    DL_Extrusion & operator = (const DL_Extrusion& extru)
    {
        if ( &extru == this )
        {
            return *this;
        }
        setDirection(extru.direction[0], extru.direction[1], extru.direction[2]);
        setElevation(extru.elevation);

        return *this;
    }



private:
    /**
     * @brief direction Vector of axis along which the entity shall be extruded this is also the Z axis of the Entity
     * coordinate system
     */
    double *direction;
    /**
     * @brief elevation Distance of the entities XY plane from the origin of the world coordinate system
     */
    double elevation;
};

#endif

