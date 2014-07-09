/************************************************************************
 **
 **  @file   vabstractmeasurements.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   18 6, 2014
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentine project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2014 Valentina project
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

#ifndef VABSTRACTMEASUREMENTS_H
#define VABSTRACTMEASUREMENTS_H

#include "vdomdocument.h"

class VAbstractMeasurements : public VDomDocument
{
public:
    VAbstractMeasurements(VContainer *data);
    virtual ~VAbstractMeasurements();
    virtual void Measurements();
    Unit         MUnit() const;
    static const QString TagUnit;
    static const QString AttrValue;
    static qreal UnitConvertor(qreal value, const Unit &from, const Unit &to);
protected:
    void         Measurement(const QString &tag);
    virtual void ReadMeasurement(const QDomElement &domElement, const QString &tag) = 0;
private:
    Q_DISABLE_COPY(VAbstractMeasurements)
};

#endif // VABSTRACTMEASUREMENTS_H
