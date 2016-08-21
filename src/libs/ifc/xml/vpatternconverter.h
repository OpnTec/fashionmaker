/************************************************************************
 **
 **  @file   vpatternconverter.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   11 12, 2014
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentine project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2013-2015 Valentina project
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

#ifndef VPATTERNCONVERTER_H
#define VPATTERNCONVERTER_H

#include <qcompilerdetection.h>
#include <QCoreApplication>
#include <QDomElement>
#include <QMap>
#include <QSet>
#include <QString>
#include <QStringList>
#include <QVector>
#include <QtGlobal>

#include "vabstractconverter.h"

class QDomElement;

class VPatternConverter : public VAbstractConverter
{
    Q_DECLARE_TR_FUNCTIONS(VPatternConverter)
public:
    explicit VPatternConverter(const QString &fileName);
    virtual ~VPatternConverter() Q_DECL_OVERRIDE;

    static const QString PatternMaxVerStr;
    static const QString CurrentSchema;
// GCC 4.6 doesn't allow constexpr and const together
#if defined(__GNUC__) && (__GNUC__ * 100 + __GNUC_MINOR__) > 406
    static Q_DECL_CONSTEXPR const int PatternMinVer = CONVERTER_VERSION_CHECK(0, 1, 0);
    static Q_DECL_CONSTEXPR const int PatternMaxVer = CONVERTER_VERSION_CHECK(0, 3, 3);
#else
    static Q_DECL_CONSTEXPR int PatternMinVer = CONVERTER_VERSION_CHECK(0, 1, 0);
    static Q_DECL_CONSTEXPR int PatternMaxVer = CONVERTER_VERSION_CHECK(0, 3, 3);
#endif

protected:
    virtual int     MinVer() const Q_DECL_OVERRIDE;
    virtual int     MaxVer() const Q_DECL_OVERRIDE;

    virtual QString MinVerStr() const Q_DECL_OVERRIDE;
    virtual QString MaxVerStr() const Q_DECL_OVERRIDE;

    virtual QString XSDSchema(int ver) const Q_DECL_OVERRIDE;
    virtual void    ApplyPatches() Q_DECL_OVERRIDE;
    virtual void    DowngradeToCurrentMaxVersion() Q_DECL_OVERRIDE;

private:
    Q_DISABLE_COPY(VPatternConverter)
    static const QString PatternMinVerStr;

    void ToV0_1_1();
    void ToV0_1_2();
    void ToV0_1_3();
    void ToV0_1_4();
    void ToV0_2_0();
    void ToV0_2_1();
    void ToV0_2_2();
    void ToV0_2_3();
    void ToV0_2_4();
    void ToV0_2_5();
    void ToV0_2_6();
    void ToV0_2_7();
    void ToV0_3_0();
    void ToV0_3_1();
    void ToV0_3_2();
    void ToV0_3_3();

    void          TagUnitToV0_2_0();
    void          TagIncrementToV0_2_0();
    void          ConvertMeasurementsToV0_2_0();
    void          TagMeasurementsToV0_2_0();

    void          ConvertMeasurementsToV0_2_1();

    void          RemoveColorToolCutV0_3_1();

    QSet<QString> FixIncrementsToV0_2_0();
    QString       FixIncrementInFormulaToV0_2_0(const QString &formula, const QSet<QString> &names);
    void          FixPointExpressionsToV0_2_0(const QSet<QString> &names);
    void          FixArcExpressionsToV0_2_0(const QSet<QString> &names);
    void          FixPathPointExpressionsToV0_2_0(const QSet<QString> &names);

    void    ConvertPointExpressionsToV0_2_0(const QMap<QString, QString> &names);
    void    ConvertArcExpressionsToV0_2_0(const QMap<QString, QString> &names);
    void    ConvertPathPointExpressionsToV0_2_0(const QMap<QString, QString> &names);
    QString FixMeasurementInFormulaToV0_2_0(const QString &formula, const QMap<QString, QString> &names);

    QString MUnitV0_1_4() const;
    QDomElement TagMeasurementsV0_1_4() const;
    QDomElement TagIncrementsV0_1_4() const;
    QStringList ListPathPointExpressionsV0_1_4() const;

    void FixToolUnionToV0_2_4();
    void ParseModelingToV0_2_4(const QDomElement &modeling);
    void SaveChildrenToolUnionToV0_2_4(quint32 id, const QVector<quint32> &children);

    static QMap<QString, QString> OldNamesToNewNames_InV0_2_0();
    static QMap<QString, QString> OldNamesToNewNames_InV0_2_1();

    void FixCutPoint();
    void FixSubPaths(int i, quint32 id, quint32 baseCurve);
};

//---------------------------------------------------------------------------------------------------------------------
inline int VPatternConverter::MinVer() const
{
    return PatternMinVer;
}

//---------------------------------------------------------------------------------------------------------------------
inline int VPatternConverter::MaxVer() const
{
    return PatternMaxVer;
}

//---------------------------------------------------------------------------------------------------------------------
inline QString VPatternConverter::MinVerStr() const
{
    return PatternMinVerStr;
}

//---------------------------------------------------------------------------------------------------------------------
inline QString VPatternConverter::MaxVerStr() const
{
    return PatternMaxVerStr;
}

#endif // VPATTERNCONVERTER_H
