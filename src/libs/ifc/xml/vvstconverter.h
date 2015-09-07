/************************************************************************
 **
 **  @file   vmeasurementconverter.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   15 7, 2015
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentine project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2015 Valentina project
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

#ifndef VMEASUREMENTCONVERTER_H
#define VMEASUREMENTCONVERTER_H

#include "vabstractmconverter.h"

class VVSTConverter : public VAbstractMConverter
{
    Q_DECLARE_TR_FUNCTIONS(VVSTConverter)
public:
    VVSTConverter(const QString &fileName);
    virtual ~VVSTConverter() Q_DECL_OVERRIDE;

    static const QString    MeasurementMaxVerStr;
    static const QString    CurrentSchema;

protected:
    virtual int     MinVer() const Q_DECL_OVERRIDE;
    virtual int     MaxVer() const Q_DECL_OVERRIDE;

    virtual QString MinVerStr() const Q_DECL_OVERRIDE;
    virtual QString MaxVerStr() const Q_DECL_OVERRIDE;

    QString         XSDSchema(int ver) const;
    virtual void    ApplyPatches() Q_DECL_OVERRIDE;

private:
    Q_DISABLE_COPY(VVSTConverter)
    static const QString    MeasurementMinVerStr;

    void AddNewTagsForV0_4_0();
    void RemoveTagsForV0_4_0();
    void ConvertMeasurementsToV0_4_0();
    QDomElement AddMV0_4_0(const QString &name, qreal value, qreal sizeIncrease, qreal heightIncrease);

    void ToV0_4_0();
};

#endif // VMEASUREMENTCONVERTER_H
