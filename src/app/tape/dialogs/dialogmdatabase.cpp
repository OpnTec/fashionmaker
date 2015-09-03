/************************************************************************
 **
 **  @file   dialogmdatabase.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   26 7, 2015
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

#include "dialogmdatabase.h"
#include "ui_dialogmdatabase.h"
#include "../mapplication.h"

//---------------------------------------------------------------------------------------------------------------------
DialogMDataBase::DialogMDataBase(const QStringList &list, QWidget *parent)
    :QDialog(parent),
      ui(new Ui::DialogMDataBase),
      selectMode(true),
      list(list),
      groupA(nullptr),
      groupB(nullptr),
      groupC(nullptr),
      groupD(nullptr),
      groupE(nullptr),
      groupF(nullptr),
      groupG(nullptr),
      groupH(nullptr),
      groupI(nullptr),
      groupJ(nullptr),
      groupK(nullptr),
      groupL(nullptr),
      groupM(nullptr),
      groupN(nullptr),
      groupO(nullptr),
      groupP(nullptr),
      groupQ(nullptr)
{
    ui->setupUi(this);
    InitDataBase(list);

    connect(ui->treeWidget, &QTreeWidget::itemChanged, this, &DialogMDataBase::UpdateChecks);
    connect(ui->treeWidget, &QTreeWidget::itemClicked, this, &DialogMDataBase::ShowDescription);

    ReadSettings();
}

//---------------------------------------------------------------------------------------------------------------------
DialogMDataBase::DialogMDataBase(QWidget *parent)
    :QDialog(parent),
      ui(new Ui::DialogMDataBase),
      selectMode(false),
      list(),
      groupA(nullptr),
      groupB(nullptr),
      groupC(nullptr),
      groupD(nullptr),
      groupE(nullptr),
      groupF(nullptr),
      groupG(nullptr),
      groupH(nullptr),
      groupI(nullptr),
      groupJ(nullptr),
      groupK(nullptr),
      groupL(nullptr),
      groupM(nullptr),
      groupN(nullptr),
      groupO(nullptr),
      groupP(nullptr),
      groupQ(nullptr)

{
    ui->setupUi(this);
    InitDataBase();

    connect(ui->treeWidget, &QTreeWidget::itemClicked, this, &DialogMDataBase::ShowDescription);

    ReadSettings();
}

//---------------------------------------------------------------------------------------------------------------------
DialogMDataBase::~DialogMDataBase()
{
    WriteSettings();
    delete ui;
}

//---------------------------------------------------------------------------------------------------------------------
QStringList DialogMDataBase::GetNewNames() const
{
    if (selectMode)
    {
        QStringList newNames;
        QTreeWidgetItemIterator it(ui->treeWidget,
                                   QTreeWidgetItemIterator::NoChildren | QTreeWidgetItemIterator::Checked );
        while (*it)
        {
            const QString name = (*it)->data(0, Qt::UserRole).toString();
            if (not list.contains(name))
            {
                newNames.append(name);
            }
            ++it;
        }
        return newNames;
    }
    else
    {
        return QStringList();
    }
}

//---------------------------------------------------------------------------------------------------------------------
void DialogMDataBase::RetranslateGroups()
{
    RetranslateGroup(groupA, "A. " + tr("Direct Height", "Measurement section"),              ListGroupA());
    RetranslateGroup(groupB, "B. " + tr("Direct Width", "Measurement section"),               ListGroupB());
    RetranslateGroup(groupC, "C. " + tr("Indentation", "Measurement section"),                ListGroupC());
    RetranslateGroup(groupD, "D. " + tr("Circumference and Arc", "Measurement section"),      ListGroupD());
    RetranslateGroup(groupE, "E. " + tr("Vertical", "Measurement section"),                   ListGroupE());
    RetranslateGroup(groupF, "F. " + tr("Horizontal", "Measurement section"),                 ListGroupF());
    RetranslateGroup(groupG, "G. " + tr("Bust", "Measurement section"),                       ListGroupG());
    RetranslateGroup(groupH, "H. " + tr("Balance", "Measurement section"),                    ListGroupH());
    RetranslateGroup(groupI, "I. " + tr("Arm", "Measurement section"),                        ListGroupI());
    RetranslateGroup(groupJ, "J. " + tr("Leg", "Measurement section"),                        ListGroupJ());
    RetranslateGroup(groupK, "K. " + tr("Crotch and Rise", "Measurement section"),            ListGroupK());
    RetranslateGroup(groupL, "L. " + tr("Hand", "Measurement section"),                       ListGroupL());
    RetranslateGroup(groupM, "M. " + tr("Foot", "Measurement section"),                       ListGroupM());
    RetranslateGroup(groupN, "N. " + tr("Head", "Measurement section"),                       ListGroupN());
    RetranslateGroup(groupO, "O. " + tr("Men & Tailoring", "Measurement section"),            ListGroupO());
    RetranslateGroup(groupP, "P. " + tr("Historical & Specialty", "Measurement section"),     ListGroupP());
    RetranslateGroup(groupQ, "Q. " + tr("Patternmaking measurements", "Measurement section"), ListGroupQ());

    ShowDescription(ui->treeWidget->currentItem(), 0);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogMDataBase::changeEvent(QEvent *event)
{
    if (event->type() == QEvent::LanguageChange)
    {
        // retranslate designer form (single inheritance approach)
        ui->retranslateUi(this);
    }

    // remember to call base class implementation
    QDialog::changeEvent(event);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogMDataBase::UpdateChecks(QTreeWidgetItem *item, int column)
{
    bool diff = false;
    if (column != 0 && column != -1)
    {
        return;
    }

    if (item->childCount() != 0 && item->checkState(0) != Qt::PartiallyChecked && column != -1)
    {
        bool flag = false; // Check if we could change atleast one children
        Qt::CheckState checkState = item->checkState(0);
        for (int i = 0; i < item->childCount(); ++i)
        {
            if (not list.contains(item->child(i)->data(0, Qt::UserRole).toString()))
            {
                item->child(i)->setCheckState(0, checkState);
                flag = true;
            }

            if (flag == false) // All child in the list
            {
                item->setCheckState(0, Qt::Checked);
            }
        }
    }
    else if (item->childCount() == 0 || column == -1)
    {
        if (item->parent() == nullptr)
        {
            return;
        }
        for (int j = 0; j < item->parent()->childCount(); ++j)
        {
            if (j != item->parent()->indexOfChild(item)
                    && item->checkState(0) != item->parent()->child(j)->checkState(0))
            {
                diff = true;
            }
        }
        if (diff)
        {
            item->parent()->setCheckState(0, Qt::PartiallyChecked);
        }
        else
        {
            item->parent()->setCheckState(0, item->checkState(0));
        }

        if (item->parent() != nullptr)
        {
            UpdateChecks(item->parent(), -1);
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
void DialogMDataBase::ShowDescription(QTreeWidgetItem *item, int column)
{
    if (column != 0 && column != -1)
    {
        ui->textEdit->clear();
        return;
    }

    if (item == nullptr)
    {
        ui->textEdit->clear();
        return;
    }

    if (item->childCount() != 0)
    {
        ui->textEdit->clear();
        return;
    }

    const QString name = item->data(0, Qt::UserRole).toString();
    const QString number = qApp->TrVars()->MNumber(name);
    const QString iconPath = QString("://diagrams/%1.png").arg(MapDiagrams(number));

    QString text = QString("<p align=\"center\" style=\"font-variant: normal; font-style: normal; font-weight: "
                           "normal\"> <img src=\"%1\" align=\"center\">"
                           "<br clear=\"left\"><b>%2</b>. <i>%3</i></p>"
                           "<p align=\"left\" style=\"font-variant: normal; font-style: normal; font-weight: normal\">"
                           "%5</p>")
            .arg(iconPath)
            .arg(number)
            .arg(qApp->TrVars()->GuiText(name))
            .arg(qApp->TrVars()->Description(name));

    ui->textEdit->setHtml(text);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogMDataBase::InitDataBase(const QStringList &list)
{
    InitGroup(groupA, "A. " + tr("Direct Height", "Measurement section"), ListGroupA(), list);
    InitGroup(groupB, "B. " + tr("Direct Width", "Measurement section"), ListGroupB(), list);
    InitGroup(groupC, "C. " + tr("Indentation", "Measurement section"), ListGroupC(), list);
    InitGroup(groupD, "D. " + tr("Circumference and Arc", "Measurement section"), ListGroupD(), list);
    InitGroup(groupE, "E. " + tr("Vertical", "Measurement section"), ListGroupE(), list);
    InitGroup(groupF, "F. " + tr("Horizontal", "Measurement section"), ListGroupF(), list);
    InitGroup(groupG, "G. " + tr("Bust", "Measurement section"), ListGroupG(), list);
    InitGroup(groupH, "H. " + tr("Balance", "Measurement section"), ListGroupH(), list);
    InitGroup(groupI, "I. " + tr("Arm", "Measurement section"), ListGroupI(), list);
    InitGroup(groupJ, "J. " + tr("Leg", "Measurement section"), ListGroupJ(), list);
    InitGroup(groupK, "K. " + tr("Crotch and Rise", "Measurement section"), ListGroupK(), list);
    InitGroup(groupL, "L. " + tr("Hand", "Measurement section"), ListGroupL(), list);
    InitGroup(groupM, "M. " + tr("Foot", "Measurement section"), ListGroupM(), list);
    InitGroup(groupN, "N. " + tr("Head", "Measurement section"), ListGroupN(), list);
    InitGroup(groupO, "O. " + tr("Men & Tailoring", "Measurement section"), ListGroupO(), list);
    InitGroup(groupP, "P. " + tr("Historical & Specialty", "Measurement section"), ListGroupP(), list);
    InitGroup(groupQ, "Q. " + tr("Patternmaking measurements", "Measurement section"), ListGroupQ(), list);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogMDataBase::InitDataBase()
{
    InitGroup(groupA, "A. " + tr("Direct Height", "Measurement section"), ListGroupA());
    InitGroup(groupB, "B. " + tr("Direct Width", "Measurement section"), ListGroupB());
    InitGroup(groupC, "C. " + tr("Indentation", "Measurement section"), ListGroupC());
    InitGroup(groupD, "D. " + tr("Circumference and Arc", "Measurement section"), ListGroupD());
    InitGroup(groupE, "E. " + tr("Vertical", "Measurement section"), ListGroupE());
    InitGroup(groupF, "F. " + tr("Horizontal", "Measurement section"), ListGroupF());
    InitGroup(groupG, "G. " + tr("Bust", "Measurement section"), ListGroupG());
    InitGroup(groupH, "H. " + tr("Balance", "Measurement section"), ListGroupH());
    InitGroup(groupI, "I. " + tr("Arm", "Measurement section"), ListGroupI());
    InitGroup(groupJ, "J. " + tr("Leg", "Measurement section"), ListGroupJ());
    InitGroup(groupK, "K. " + tr("Crotch and Rise", "Measurement section"), ListGroupK());
    InitGroup(groupL, "L. " + tr("Hand", "Measurement section"), ListGroupL());
    InitGroup(groupM, "M. " + tr("Foot", "Measurement section"), ListGroupM());
    InitGroup(groupN, "N. " + tr("Head", "Measurement section"), ListGroupN());
    InitGroup(groupO, "O. " + tr("Men & Tailoring", "Measurement section"), ListGroupO());
    InitGroup(groupP, "P. " + tr("Historical & Specialty", "Measurement section"), ListGroupP());
    InitGroup(groupQ, "Q. " + tr("Patternmaking measurements", "Measurement section"), ListGroupQ());
}

//---------------------------------------------------------------------------------------------------------------------
void DialogMDataBase::InitGroup(QTreeWidgetItem *group, const QString &groupName, const QStringList &mList,
                                const QStringList &list)
{
    group = AddGroup(groupName);

    for (int i=0; i < mList.size(); ++i)
    {
        AddMeasurement(group, mList.at(i), list);
    }
}

//---------------------------------------------------------------------------------------------------------------------
QTreeWidgetItem *DialogMDataBase::AddGroup(const QString &text)
{
    QTreeWidgetItem *group = new QTreeWidgetItem(ui->treeWidget);
    group->setText(0, text);
    group->setToolTip(0, text);
    if (selectMode)
    {
        group->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
        group->setCheckState(0, Qt::Unchecked);
        group->setBackground(0, QBrush(Qt::lightGray));
    }
    return group;
}

//---------------------------------------------------------------------------------------------------------------------
void DialogMDataBase::AddMeasurement(QTreeWidgetItem *group, const QString &name, const QStringList &list)
{
    QTreeWidgetItem *m = new QTreeWidgetItem(group);

    if (selectMode)
    {
        if (list.contains(name))
        {
            m->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
            m->setCheckState(0, Qt::Checked);
            m->setBackground(0, QBrush(Qt::yellow));
        }
        else
        {
            m->setCheckState(0, Qt::Unchecked);
        }

        UpdateChecks(m, 0);
    }

    const QString text = qApp->TrVars()->MNumber(name) + ". " + qApp->TrVars()->MToUser(name);
    m->setText(0, text);
    m->setToolTip(0, text);
    m->setData(0, Qt::UserRole, name);
}

//---------------------------------------------------------------------------------------------------------------------
QStringList DialogMDataBase::ListNumbers(const QStringList &listMeasurements) const
{
    QStringList numbers;
    for (int i=0; i < listMeasurements.size(); ++i)
    {
        numbers.append(qApp->TrVars()->MNumber(listMeasurements.at(i)));
    }
    return numbers;
}

//---------------------------------------------------------------------------------------------------------------------
QString DialogMDataBase::MapDiagrams(const QString &number) const
{
    switch (ListNumbers(ListGroupA()).indexOf(number))
    {
        // A
        case 0: // A01
            V_FALLTHROUGH
        case 1: // A02
            V_FALLTHROUGH
        case 2: // A03
            V_FALLTHROUGH
        case 3: // A04
            V_FALLTHROUGH
        case 4: // A05
            V_FALLTHROUGH
        case 5: // A06
            V_FALLTHROUGH
        case 6: // A07
            V_FALLTHROUGH
        case 7: // A08
            V_FALLTHROUGH
        case 8: // A09
            V_FALLTHROUGH
        case 9: // A10
            V_FALLTHROUGH
        case 10: // A11
            V_FALLTHROUGH
        case 11: // A12
            V_FALLTHROUGH
        case 12: // A13
            V_FALLTHROUGH
        case 13: // A14
            V_FALLTHROUGH
        case 14: // A15
            V_FALLTHROUGH
        case 15: // A16
            V_FALLTHROUGH
        case 16: // A17
            return QString("Ap1");
        case 17: // A18
            V_FALLTHROUGH
        case 18: // A19
            V_FALLTHROUGH
        case 19: // A20
            V_FALLTHROUGH
        case 20: // A21
            return QString("Ap2");
        default:
            break;
    }

    switch (ListNumbers(ListGroupB()).indexOf(number))
    {
        // B
        case 0: // B01
            V_FALLTHROUGH
        case 1: // B02
            V_FALLTHROUGH
        case 2: // B03
            V_FALLTHROUGH
        case 3: // B04
            return QString("Bp1");
        case 4: // B05
            return QString("Bp2");
        default:
            break;
    }

    switch (ListNumbers(ListGroupC()).indexOf(number))
    {
        // C
        case 0: // C01
            return QString("Cp1");
        case 1: // C02
            V_FALLTHROUGH
        case 2: // C03
            return QString("Cp2");
        default:
            break;
    }

    switch (ListNumbers(ListGroupD()).indexOf(number))
    {
        // D
        case 0: // D01
            V_FALLTHROUGH
        case 1: // D02
            V_FALLTHROUGH
        case 2: // D03
            V_FALLTHROUGH
        case 3: // D04
            V_FALLTHROUGH
        case 4: // D05
            V_FALLTHROUGH
        case 5: // D06
            V_FALLTHROUGH
        case 6: // D07
            V_FALLTHROUGH
        case 7: // D08
            V_FALLTHROUGH
        case 8: // D09
            return QString("Dp1");
        case 9: // D10
            V_FALLTHROUGH
        case 10: // D11
            V_FALLTHROUGH
        case 11: // D12
            V_FALLTHROUGH
        case 12: // D13
            V_FALLTHROUGH
        case 13: // D14
            V_FALLTHROUGH
        case 14: // D15
            V_FALLTHROUGH
        case 15: // D16
            V_FALLTHROUGH
        case 16: // D17
            return QString("Dp2");
        case 17: // D18
            V_FALLTHROUGH
        case 18: // D19
            V_FALLTHROUGH
        case 19: // D20
            V_FALLTHROUGH
        case 20: // D21
            V_FALLTHROUGH
        case 21: // D22
            V_FALLTHROUGH
        case 22: // D23
            V_FALLTHROUGH
        case 23: // D24
            V_FALLTHROUGH
        case 24: // D25
            return QString("Dp3");
        case 25: // D26
            V_FALLTHROUGH
        case 26: // D27
            V_FALLTHROUGH
        case 27: // D28
            V_FALLTHROUGH
        case 28: // D29
            V_FALLTHROUGH
        case 29: // D30
            V_FALLTHROUGH
        case 30: // D31
            V_FALLTHROUGH
        case 31: // D32
            V_FALLTHROUGH
        case 32: // D33
            return QString("Dp4");
        case 33: // D34
            V_FALLTHROUGH
        case 34: // D35
            V_FALLTHROUGH
        case 35: // D36
            V_FALLTHROUGH
        case 36: // D37
            V_FALLTHROUGH
        case 37: // D38
            V_FALLTHROUGH
        case 38: // D39
            V_FALLTHROUGH
        case 39: // D40
            V_FALLTHROUGH
        case 40: // D41
            return QString("Dp5");
        case 41: // D42
            return QString("Dp6");
        case 42: // D43
            V_FALLTHROUGH
        case 43: // D44
            return QString("Dp7");
        case 44: // D45
            return QString("Dp8");
        default:
            break;
    }

    switch (ListNumbers(ListGroupE()).indexOf(number))
    {
        // E
        case 0: // E01
            return QString("Ep1");
        case 1: // E02
            return QString("Ep2");
        case 2: // E03
            V_FALLTHROUGH
        case 3: // E04
            V_FALLTHROUGH
        case 4: // E05
            V_FALLTHROUGH
        case 5: // E06
            return QString("Ep3");
        case 6: // E07
            V_FALLTHROUGH
        case 7: // E08
            V_FALLTHROUGH
        case 8: // E09
            V_FALLTHROUGH
        case 9: // E10
            return QString("Ep4");
        case 10: // E11
            V_FALLTHROUGH
        case 11: // E12
            V_FALLTHROUGH
        case 12: // E13
            V_FALLTHROUGH
        case 13: // E14
            V_FALLTHROUGH
        case 14: // E15
            V_FALLTHROUGH
        case 15: // E16
            return QString("Ep5");
        case 16: // E17
            V_FALLTHROUGH
        case 17: // E18
            V_FALLTHROUGH
        case 18: // E19
            V_FALLTHROUGH
        case 19: // E20
            V_FALLTHROUGH
        case 20: // E21
            return QString("Ep6");
        case 21: // E22
            V_FALLTHROUGH
        case 22: // E23
            V_FALLTHROUGH
        case 23: // E24
            V_FALLTHROUGH
        case 24: // E25
            return QString("Ep7");
        case 25: // E26
            V_FALLTHROUGH
        case 26: // E27
            V_FALLTHROUGH
        case 27: // E28
            V_FALLTHROUGH
        case 28: // E29
            return QString("Ep8");
        case 29: // E30
            V_FALLTHROUGH
        case 30: // E31
            V_FALLTHROUGH
        case 31: // E32
            return QString("Ep9");
        case 32: // E33
            V_FALLTHROUGH
        case 33: // E34
            V_FALLTHROUGH
        case 34: // E35
            return QString("Ep10");
        case 35: // E36
            V_FALLTHROUGH
        case 36: // E37
            V_FALLTHROUGH
        case 37: // E38
            V_FALLTHROUGH
        case 38: // E39
            return QString("Ep11");
        case 39: // E40
            return QString("Ep12");
        default:
            break;
    }

    switch (ListNumbers(ListGroupF()).indexOf(number))
    {
        // F
        case 0: // F01
            V_FALLTHROUGH
        case 1: // F02
            V_FALLTHROUGH
        case 2: // F03
            V_FALLTHROUGH
        case 3: // F04
            return QString("Fp1");
        case 4: // F05
            V_FALLTHROUGH
        case 5: // F06
            V_FALLTHROUGH
        case 6: // F07
            return QString("Fp2");
        case 7: // F08
            V_FALLTHROUGH
        case 8: // F09
            return QString("Fp3");
        case 9: // F10
            V_FALLTHROUGH
        case 10: // F11
            return QString("Fp4");
        case 11: // F12
            return QString("Fp5");
        case 12: // F13
            return QString("Fp6");
        case 13: // F14
            return QString("Fp7");
        default:
            break;
    }

    switch (ListNumbers(ListGroupG()).indexOf(number))
    {
        // G
        case 0: // G01
            V_FALLTHROUGH
        case 1: // G02
            V_FALLTHROUGH
        case 2: // G03
            V_FALLTHROUGH
        case 3: // G04
            return QString("Gp1");
        case 4: // G05
            return QString("Gp2");
        case 5: // G06
            return QString("Gp3");
        case 6: // G07
            V_FALLTHROUGH
        case 7: // G08
            return QString("Gp4");
        default:
            break;
    }

    switch (ListNumbers(ListGroupH()).indexOf(number))
    {
        // H
        case 0: // H01
            return QString("Hp1");
        case 1: // H02
            V_FALLTHROUGH
        case 2: // H03
            return QString("Hp2");
        case 3: // H04
            return QString("Hp3");
        case 4: // H05
            return QString("Hp4");
        case 5: // H06
            return QString("Hp5");
        case 6: // H07
            return QString("Hp6");
        case 7: // H08
            V_FALLTHROUGH
        case 8: // H09
            return QString("Hp7");
        case 9: // H10
            return QString("Hp8");
        case 10: // H11
            return QString("Hp9");
        case 11: // H12
            return QString("Hp10");
        case 12: // H13
            return QString("Hp11");
        default:
            break;
    }

    switch (ListNumbers(ListGroupI()).indexOf(number))
    {
        // I
        case 0: // I01
            V_FALLTHROUGH
        case 1: // I02
            V_FALLTHROUGH
        case 2: // I03
            V_FALLTHROUGH
        case 3: // I04
            return QString("Ip1");
        case 4: // I05
            V_FALLTHROUGH
        case 5: // I06
            V_FALLTHROUGH
        case 6: // I07
            return QString("Ip2");
        case 7: // I08
            V_FALLTHROUGH
        case 8: // I09
            V_FALLTHROUGH
        case 9: // I10
            return QString("Ip3");
        case 10: // I11
            V_FALLTHROUGH
        case 11: // I12
            V_FALLTHROUGH
        case 12: // I13
            V_FALLTHROUGH
        case 13: // I14
            V_FALLTHROUGH
        case 14: // I15
            return QString("Ip4");
        case 15: // I16
            return QString("Ip5");
        case 16: // I17
            V_FALLTHROUGH
        case 17: // I18
            return QString("Ip6");
        case 18: // I19
            return QString("Ip7");
        case 19: // I20
            return QString("Ip8");
        case 20: // I21
            return QString("Ip9");
        default:
            break;
    }

    switch (ListNumbers(ListGroupJ()).indexOf(number))
    {
        // J
        case 0: // J01
            V_FALLTHROUGH
        case 1: // J02
            V_FALLTHROUGH
        case 2: // J03
            return QString("Jp1");
        case 3: // J04
            V_FALLTHROUGH
        case 4: // J05
            V_FALLTHROUGH
        case 5: // J06
            V_FALLTHROUGH
        case 6: // J07
            V_FALLTHROUGH
        case 7: // J08
            V_FALLTHROUGH
        case 8: // J09
            V_FALLTHROUGH
        case 9: // J10
            V_FALLTHROUGH
        case 10: // J11
            V_FALLTHROUGH
        case 11: // J12
            return QString("Jp2");
        case 12: // J13
            V_FALLTHROUGH
        case 13: // J14
            return QString("Jp3");
        default:
            break;
    }

    switch (ListNumbers(ListGroupK()).indexOf(number))
    {
        // K
        case 0: // K01
            return QString("Kp1");
        case 1: // K02
            V_FALLTHROUGH
        case 2: // K03
            return QString("Kp2");
        case 3: // K04
            V_FALLTHROUGH
        case 4: // K05
            return QString("Kp3");
        case 5: // K06
            V_FALLTHROUGH
        case 6: // K07
            return QString("Kp4");
        default:
            break;
    }

    switch (ListNumbers(ListGroupL()).indexOf(number))
    {
        // L
        case 0: // L01
            V_FALLTHROUGH
        case 1: // L02
            V_FALLTHROUGH
        case 2: // L03
            return QString("Lp1");
        case 3: // L04
            return QString("Lp2");
        case 4: // L05
            return QString("Lp3");
        default:
            break;
    }

    switch (ListNumbers(ListGroupM()).indexOf(number))
    {
        // M
        case 0: // M01
            return QString("Mp1");
        case 1: // M02
            V_FALLTHROUGH
        case 2: // M03
            V_FALLTHROUGH
        case 3: // M04
            return QString("Mp2");
        default:
            break;
    }

    switch (ListNumbers(ListGroupN()).indexOf(number))
    {
        // N
        case 0: // N01
            V_FALLTHROUGH
        case 1: // N02
            V_FALLTHROUGH
        case 2: // N03
            return QString("Np1");
        case 3: // N04
            return QString("Np2");
        case 4: // N05
            V_FALLTHROUGH
        case 5: // N06
            return QString("Np3");
        default:
            break;
    }

    switch (ListNumbers(ListGroupO()).indexOf(number))
    {
        // O
        case 0: // O01
            return QString("Op1");
        case 1: // O02
            return QString("Op2");
        case 2: // O03
            return QString("Op3");
        case 3: // O04
            return QString("Op4");
        case 4: // O05
            return QString("Op5");
        case 5: // O06
            return QString("Op6");
        case 6: // O07
            return QString("Op7");
        case 7: // O08
            V_FALLTHROUGH
        case 8: // O09
            return QString("Op8");
        case 9: // O10
            V_FALLTHROUGH
        case 10: // O11
            return QString("Op9");
        case 11: // O12
            V_FALLTHROUGH
        case 12: // O13
            return QString("Op10");
        case 13: // O14
            return QString("Op11");
        default:
            break;
    }

    switch (ListNumbers(ListGroupP()).indexOf(number))
    {
        // P
        case 0: // P01
            return QString("Pp1");
        case 1: // P02
            return QString("Pp2");
        case 2: // P03
            return QString("Pp3");
        case 3: // P04
            return QString("Pp4");
        case 4: // P05
            return QString("Pp5");
        case 5: // P06
            return QString("Pp6");
        case 6: // P07
            return QString("Pp7");
        case 7: // P08
            return QString("Pp8");
        case 8: // P09
            return QString("Pp9");
        case 9: // P10
            return QString("Pp10");
        case 10: // P11
            return QString("Pp11");
        case 11: // P12
            return QString("Pp12");
        default:
            break;
    }

    switch (ListNumbers(ListGroupQ()).indexOf(number))
    {
        // Q
        case 0: // Q01
            return QString("Qp1");
        case 1: // Q02
            return QString("Qp2");
        case 2: // Q03
            return QString("Qp3");
        default:
            break;
    }

    return QString();
}

//---------------------------------------------------------------------------------------------------------------------
void DialogMDataBase::ReadSettings()
{
    restoreGeometry(qApp->TapeSettings()->GetDataBaseGeometry());
}

//---------------------------------------------------------------------------------------------------------------------
void DialogMDataBase::WriteSettings()
{
    qApp->TapeSettings()->SetDataBaseGeometry(saveGeometry());
}

//---------------------------------------------------------------------------------------------------------------------
void DialogMDataBase::RetranslateGroup(QTreeWidgetItem *group, const QString &groupText, const QStringList &list)
{
    group->setText(0, groupText);
    group->setToolTip(0, groupText);

    for (int i=0; i<list.size(); ++i)
    {
        RetranslateMeasurement(group, i, list.at(i));
    }
}

//---------------------------------------------------------------------------------------------------------------------
void DialogMDataBase::RetranslateMeasurement(QTreeWidgetItem *group, int index, const QString &name)
{
    const QString text = qApp->TrVars()->MNumber(name) + ". " + qApp->TrVars()->MToUser(name);

    QTreeWidgetItem *m = group->child(index);
    m->setText(0, text);
    m->setToolTip(0, text);
}
