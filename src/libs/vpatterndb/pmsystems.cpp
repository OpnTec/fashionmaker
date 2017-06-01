/************************************************************************
 **
 **  @file
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   1 6, 2017
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentine project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2017 Valentina project
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

#include "pmsystems.h"
#include "../vmisc/def.h"
#include "../vmisc/vabstractapplication.h"

#include <QComboBox>

// pattern making systems codes
const QString p0_S = QStringLiteral("p0");
const QString p1_S = QStringLiteral("p1");
const QString p2_S = QStringLiteral("p2");
const QString p3_S = QStringLiteral("p3");
const QString p4_S = QStringLiteral("p4");
const QString p5_S = QStringLiteral("p5");
const QString p6_S = QStringLiteral("p6");
const QString p7_S = QStringLiteral("p7");
const QString p8_S = QStringLiteral("p8");
const QString p9_S = QStringLiteral("p9");
const QString p10_S = QStringLiteral("p10");
const QString p11_S = QStringLiteral("p11");
const QString p12_S = QStringLiteral("p12");
const QString p13_S = QStringLiteral("p13");
const QString p14_S = QStringLiteral("p14");
const QString p15_S = QStringLiteral("p15");
const QString p16_S = QStringLiteral("p16");
const QString p17_S = QStringLiteral("p17");
const QString p18_S = QStringLiteral("p18");
const QString p19_S = QStringLiteral("p19");
const QString p20_S = QStringLiteral("p20");
const QString p21_S = QStringLiteral("p21");
const QString p22_S = QStringLiteral("p22");
const QString p23_S = QStringLiteral("p23");
const QString p24_S = QStringLiteral("p24");
const QString p25_S = QStringLiteral("p25");
const QString p26_S = QStringLiteral("p26");
const QString p27_S = QStringLiteral("p27");
const QString p28_S = QStringLiteral("p28");
const QString p29_S = QStringLiteral("p29");
const QString p30_S = QStringLiteral("p30");
const QString p31_S = QStringLiteral("p31");
const QString p32_S = QStringLiteral("p32");
const QString p33_S = QStringLiteral("p33");
const QString p34_S = QStringLiteral("p34");
const QString p35_S = QStringLiteral("p35");
const QString p36_S = QStringLiteral("p36");
const QString p37_S = QStringLiteral("p37");
const QString p38_S = QStringLiteral("p38");
const QString p39_S = QStringLiteral("p39");
const QString p40_S = QStringLiteral("p40");
const QString p41_S = QStringLiteral("p41");
const QString p42_S = QStringLiteral("p42");
const QString p43_S = QStringLiteral("p43");
const QString p44_S = QStringLiteral("p44");
const QString p45_S = QStringLiteral("p45");
const QString p46_S = QStringLiteral("p46");
const QString p47_S = QStringLiteral("p47");
const QString p48_S = QStringLiteral("p48");
const QString p49_S = QStringLiteral("p49");
const QString p50_S = QStringLiteral("p50");
const QString p51_S = QStringLiteral("p51");
const QString p52_S = QStringLiteral("p52");
const QString p53_S = QStringLiteral("p53");
const QString p54_S = QStringLiteral("p54");
const QString p998_S = QStringLiteral("p998");

//---------------------------------------------------------------------------------------------------------------------
QStringList ListPMSystems()
{
    QStringList list;
    list << p0_S
         << p1_S
         << p2_S
         << p3_S
         << p4_S
         << p5_S
         << p6_S
         << p7_S
         << p8_S
         << p9_S
         << p10_S
         << p11_S
         << p12_S
         << p13_S
         << p14_S
         << p15_S
         << p16_S
         << p17_S
         << p18_S
         << p19_S
         << p20_S
         << p21_S
         << p22_S
         << p23_S
         << p24_S
         << p25_S
         << p26_S
         << p27_S
         << p28_S
         << p29_S
         << p30_S
         << p31_S
         << p32_S
         << p33_S
         << p34_S
         << p35_S
         << p36_S
         << p37_S
         << p38_S
         << p39_S
         << p40_S
         << p41_S
         << p42_S
         << p43_S
         << p44_S
         << p45_S
         << p46_S
         << p47_S
         << p48_S
         << p49_S
         << p50_S
         << p51_S
         << p52_S
         << p53_S
         << p54_S
         << p998_S;

    return list;
}

//---------------------------------------------------------------------------------------------------------------------
void InitPMSystems(QComboBox *systemCombo)
{
    const QStringList listSystems = ListPMSystems();
    QMap<QString, QString> systems;
    for (int i = 0; i < listSystems.size()-1; ++i)
    {
        systems.insert(qApp->TrVars()->PMSystemName(listSystems.at(i)) + " ("+listSystems.at(i)+")", listSystems.at(i));
    }

// * The default option (blank field or 'None') should appear at the top of the list.
// * The list should be sorted alphabetically so users can find their system easily.

    SCASSERT(systemCombo != nullptr)
    systemCombo->addItem(qApp->TrVars()->PMSystemName(listSystems.at(listSystems.size()-1)),
                         listSystems.at(listSystems.size()-1));

    QMap<QString, QString>::const_iterator i = systems.constBegin();
    while (i != systems.constEnd())
    {
        systemCombo->addItem(i.key(), i.value());
        ++i;
    }
}
