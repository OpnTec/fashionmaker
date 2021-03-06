/************************************************************************
 **
 **  @file   vvitconverter.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   15 7, 2015
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentina project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2015 Valentina project
 **  <https://gitlab.com/smart-pattern/valentina> All Rights Reserved.
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

#ifndef VVITCONVERTER_H
#define VVITCONVERTER_H

#include <qcompilerdetection.h>
#include <QCoreApplication>
#include <QString>
#include <QtGlobal>

#include "vabstractmconverter.h"
#include "vabstractconverter.h"

class QDomElement;

class VVITConverter : public VAbstractMConverter
{
    Q_DECLARE_TR_FUNCTIONS(VVITConverter)
public:
    explicit VVITConverter(const QString &fileName);
    virtual ~VVITConverter() Q_DECL_EQ_DEFAULT;

    static const QString MeasurementMaxVerStr;
    static const QString CurrentSchema;
    static Q_DECL_CONSTEXPR const int MeasurementMinVer = FORMAT_VERSION(0, 2, 0);
    static Q_DECL_CONSTEXPR const int MeasurementMaxVer = FORMAT_VERSION(0, 5, 0);

protected:
    virtual int     MinVer() const override;
    virtual int     MaxVer() const override;

    virtual QString MinVerStr() const override;
    virtual QString MaxVerStr() const override;

    virtual QString XSDSchema(int ver) const override;
    virtual void    ApplyPatches() override;
    virtual void    DowngradeToCurrentMaxVersion() override;
    virtual bool    IsReadOnly() const override;

private:
    Q_DISABLE_COPY(VVITConverter)
    static const QString MeasurementMinVerStr;

    void AddNewTagsForV0_3_0();
    QString MUnitV0_2_0();
    void ConvertMeasurementsToV0_3_0();
    QDomElement AddMV0_3_0(const QString &name, qreal value);
    void GenderV0_3_1();
    void PM_SystemV0_3_2();
    void ConvertMeasurementsToV0_3_3();
    void ConverCustomerNameToV0_4_0();

    void ToV0_3_0();
    void ToV0_3_1();
    void ToV0_3_2();
    void ToV0_3_3();
    void ToV0_4_0();
    void ToV0_5_0();
};

//---------------------------------------------------------------------------------------------------------------------
inline int VVITConverter::MinVer() const
{
    return MeasurementMinVer;
}

//---------------------------------------------------------------------------------------------------------------------
inline int VVITConverter::MaxVer() const
{
    return MeasurementMaxVer;
}

//---------------------------------------------------------------------------------------------------------------------
inline QString VVITConverter::MinVerStr() const
{
    return MeasurementMinVerStr;
}

//---------------------------------------------------------------------------------------------------------------------
inline QString VVITConverter::MaxVerStr() const
{
    return MeasurementMaxVerStr;
}

#endif // VVITCONVERTER_H
