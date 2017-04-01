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

#include <qcompilerdetection.h>
#include <QCoreApplication>
#include <QString>
#include <QtGlobal>

#include "vabstractmconverter.h"
#include "vabstractconverter.h"

class QDomElement;

class VVSTConverter : public VAbstractMConverter
{
    Q_DECLARE_TR_FUNCTIONS(VVSTConverter)
public:
    explicit VVSTConverter(const QString &fileName);
    virtual ~VVSTConverter() Q_DECL_EQ_DEFAULT;

    static const QString MeasurementMaxVerStr;
    static const QString CurrentSchema;
// GCC 4.6 doesn't allow constexpr and const together
#if !defined(__INTEL_COMPILER) && !defined(__clang__) && defined(__GNUC__) && (__GNUC__ * 100 + __GNUC_MINOR__) <= 406
    static Q_DECL_CONSTEXPR int MeasurementMinVer = CONVERTER_VERSION_CHECK(0, 3, 0);
    static Q_DECL_CONSTEXPR int MeasurementMaxVer = CONVERTER_VERSION_CHECK(0, 4, 4);
#else
    static Q_DECL_CONSTEXPR const int MeasurementMinVer = CONVERTER_VERSION_CHECK(0, 3, 0);
    static Q_DECL_CONSTEXPR const int MeasurementMaxVer = CONVERTER_VERSION_CHECK(0, 4, 4);
#endif

protected:
    virtual int     MinVer() const Q_DECL_OVERRIDE;
    virtual int     MaxVer() const Q_DECL_OVERRIDE;

    virtual QString MinVerStr() const Q_DECL_OVERRIDE;
    virtual QString MaxVerStr() const Q_DECL_OVERRIDE;

    virtual QString XSDSchema(int ver) const Q_DECL_OVERRIDE;
    virtual void    ApplyPatches() Q_DECL_OVERRIDE;
    virtual void    DowngradeToCurrentMaxVersion() Q_DECL_OVERRIDE;
    virtual bool    IsReadOnly() const Q_DECL_OVERRIDE;

private:
    Q_DISABLE_COPY(VVSTConverter)
    static const QString MeasurementMinVerStr;

    void AddNewTagsForV0_4_0();
    void RemoveTagsForV0_4_0();
    void ConvertMeasurementsToV0_4_0();
    QDomElement AddMV0_4_0(const QString &name, qreal value, qreal sizeIncrease, qreal heightIncrease);
    void PM_SystemV0_4_1();
    void ConvertMeasurementsToV0_4_2();

    void ToV0_4_0();
    void ToV0_4_1();
    void ToV0_4_2();
    void ToV0_4_3();
    void ToV0_4_4();
};

//---------------------------------------------------------------------------------------------------------------------
inline int VVSTConverter::MinVer() const
{
    return MeasurementMinVer;
}

//---------------------------------------------------------------------------------------------------------------------
inline int VVSTConverter::MaxVer() const
{
    return MeasurementMaxVer;
}

//---------------------------------------------------------------------------------------------------------------------
inline QString VVSTConverter::MinVerStr() const
{
    return MeasurementMinVerStr;
}

//---------------------------------------------------------------------------------------------------------------------
inline QString VVSTConverter::MaxVerStr() const
{
    return MeasurementMaxVerStr;
}

#endif // VMEASUREMENTCONVERTER_H
