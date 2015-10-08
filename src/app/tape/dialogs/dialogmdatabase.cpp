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

#include <QKeyEvent>

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

    ui->treeWidget->installEventFilter(this);

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

    ui->treeWidget->installEventFilter(this);

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
    RetranslateGroup(groupD, "D. " + tr("Hand", "Measurement section"),                       ListGroupD());
    RetranslateGroup(groupE, "E. " + tr("Foot", "Measurement section"),                       ListGroupE());
    RetranslateGroup(groupF, "F. " + tr("Head", "Measurement section"),                       ListGroupF());
    RetranslateGroup(groupG, "G. " + tr("Circumference and Arc", "Measurement section"),      ListGroupG());
    RetranslateGroup(groupH, "H. " + tr("Vertical", "Measurement section"),                   ListGroupH());
    RetranslateGroup(groupI, "I. " + tr("Horizontal", "Measurement section"),                 ListGroupI());
    RetranslateGroup(groupJ, "J. " + tr("Bust", "Measurement section"),                       ListGroupJ());
    RetranslateGroup(groupK, "K. " + tr("Balance", "Measurement section"),                    ListGroupK());
    RetranslateGroup(groupL, "L. " + tr("Arm", "Measurement section"),                        ListGroupL());
    RetranslateGroup(groupM, "M. " + tr("Leg", "Measurement section"),                        ListGroupM());
    RetranslateGroup(groupN, "N. " + tr("Crotch and Rise", "Measurement section"),            ListGroupN());
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
bool DialogMDataBase::eventFilter(QObject *target, QEvent *event)
{
    if (target == ui->treeWidget)
    {
        if (event->type() == QEvent::KeyPress)
        {
            QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
            switch(keyEvent->key())
            {
                case Qt::Key_Up:
                {
                    const QModelIndex model = ui->treeWidget->indexAbove(ui->treeWidget->currentIndex());
                    QTreeWidgetItem *item = ui->treeWidget->itemAbove(ui->treeWidget->currentItem());
                    emit ShowDescription(item, model.column());
                    break;
                }
                case Qt::Key_Down:
                {
                    const QModelIndex model = ui->treeWidget->indexBelow(ui->treeWidget->currentIndex());
                    QTreeWidgetItem *item = ui->treeWidget->itemBelow(ui->treeWidget->currentItem());
                    emit ShowDescription(item, model.column());
                    break;
                }
                default:
                    break;
            }
        }
    }
    return QDialog::eventFilter(target, event);
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
    InitGroup(groupA, "A. " + tr("Direct Height", "Measurement section"),              ListGroupA(), list);
    InitGroup(groupB, "B. " + tr("Direct Width", "Measurement section"),               ListGroupB(), list);
    InitGroup(groupC, "C. " + tr("Indentation", "Measurement section"),                ListGroupC(), list);
    InitGroup(groupD, "D. " + tr("Hand", "Measurement section"),                       ListGroupD(), list);
    InitGroup(groupE, "E. " + tr("Foot", "Measurement section"),                       ListGroupE(), list);
    InitGroup(groupF, "F. " + tr("Head", "Measurement section"),                       ListGroupF(), list);
    InitGroup(groupG, "G. " + tr("Circumference and Arc", "Measurement section"),      ListGroupG(), list);
    InitGroup(groupH, "H. " + tr("Vertical", "Measurement section"),                   ListGroupH(), list);
    InitGroup(groupI, "I. " + tr("Horizontal", "Measurement section"),                 ListGroupI(), list);
    InitGroup(groupJ, "J. " + tr("Bust", "Measurement section"),                       ListGroupJ(), list);
    InitGroup(groupK, "K. " + tr("Balance", "Measurement section"),                    ListGroupK(), list);
    InitGroup(groupL, "L. " + tr("Arm", "Measurement section"),                        ListGroupL(), list);
    InitGroup(groupM, "M. " + tr("Leg", "Measurement section"),                        ListGroupM(), list);
    InitGroup(groupN, "N. " + tr("Crotch and Rise", "Measurement section"),            ListGroupN(), list);
    InitGroup(groupO, "O. " + tr("Men & Tailoring", "Measurement section"),            ListGroupO(), list);
    InitGroup(groupP, "P. " + tr("Historical & Specialty", "Measurement section"),     ListGroupP(), list);
    InitGroup(groupQ, "Q. " + tr("Patternmaking measurements", "Measurement section"), ListGroupQ(), list);
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
    group->setExpanded(true);
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
            return QStringLiteral("Ap1");
        case 17: // A18
            V_FALLTHROUGH
        case 18: // A19
            V_FALLTHROUGH
        case 19: // A20
            V_FALLTHROUGH
        case 20: // A21
            V_FALLTHROUGH
        case 21: // A22
            return QStringLiteral("Ap2");
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
            return QStringLiteral("Bp1");
        case 4: // B05
            return QStringLiteral("Bp2");
        default:
            break;
    }

    switch (ListNumbers(ListGroupC()).indexOf(number))
    {
        // C
        case 0: // C01
            return QStringLiteral("Cp1");
        case 1: // C02
            V_FALLTHROUGH
        case 2: // C03
            return QStringLiteral("Cp2");
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
            return QStringLiteral("Dp1");
        case 3: // D04
            return QStringLiteral("Dp2");
        case 4: // D05
            return QStringLiteral("Dp3");
        default:
            break;
    }

    switch (ListNumbers(ListGroupE()).indexOf(number))
    {
        // E
        case 0: // E01
            return QStringLiteral("Ep1");
        case 1: // E02
            V_FALLTHROUGH
        case 2: // E03
            V_FALLTHROUGH
        case 3: // E04
            return QStringLiteral("Ep2");
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
            return QStringLiteral("Fp1");
        case 3: // F04
            return QStringLiteral("Fp2");
        case 4: // F05
            V_FALLTHROUGH
        case 5: // F06
            return QStringLiteral("Fp3");
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
            V_FALLTHROUGH
        case 4: // G05
            V_FALLTHROUGH
        case 5: // G06
            V_FALLTHROUGH
        case 6: // G07
            V_FALLTHROUGH
        case 7: // G08
            V_FALLTHROUGH
        case 8: // G09
            return QStringLiteral("Gp1");
        case 9: // G10
            V_FALLTHROUGH
        case 10: // G11
            V_FALLTHROUGH
        case 11: // G12
            V_FALLTHROUGH
        case 12: // G13
            V_FALLTHROUGH
        case 13: // G14
            V_FALLTHROUGH
        case 14: // G15
            V_FALLTHROUGH
        case 15: // G16
            V_FALLTHROUGH
        case 16: // G17
            return QStringLiteral("Gp2");
        case 17: // G18
            V_FALLTHROUGH
        case 18: // G19
            V_FALLTHROUGH
        case 19: // G20
            V_FALLTHROUGH
        case 20: // G21
            V_FALLTHROUGH
        case 21: // G22
            V_FALLTHROUGH
        case 22: // G23
            V_FALLTHROUGH
        case 23: // G24
            V_FALLTHROUGH
        case 24: // G25
            return QStringLiteral("Gp3");
        case 25: // G26
            V_FALLTHROUGH
        case 26: // G27
            V_FALLTHROUGH
        case 27: // G28
            V_FALLTHROUGH
        case 28: // G29
            V_FALLTHROUGH
        case 29: // G30
            V_FALLTHROUGH
        case 30: // G31
            V_FALLTHROUGH
        case 31: // G32
            V_FALLTHROUGH
        case 32: // G33
            return QStringLiteral("Gp4");
        case 33: // G34
            V_FALLTHROUGH
        case 34: // G35
            V_FALLTHROUGH
        case 35: // G36
            V_FALLTHROUGH
        case 36: // G37
            V_FALLTHROUGH
        case 37: // G38
            V_FALLTHROUGH
        case 38: // G39
            V_FALLTHROUGH
        case 39: // G40
            V_FALLTHROUGH
        case 40: // G41
            return QStringLiteral("Gp5");
        case 41: // G42
            return QStringLiteral("Gp6");
        case 42: // G43
            V_FALLTHROUGH
        case 43: // G44
            return QStringLiteral("Gp7");
        case 44: // G45
            return QStringLiteral("Gp8");
        default:
            break;
    }

    switch (ListNumbers(ListGroupH()).indexOf(number))
    {
        // H
        case 0: // H01
            return QStringLiteral("Hp1");
        case 1: // H02
            return QStringLiteral("Hp2");
        case 2: // H03
            V_FALLTHROUGH
        case 3: // H04
            V_FALLTHROUGH
        case 4: // H05
            V_FALLTHROUGH
        case 5: // H06
            return QStringLiteral("Hp3");
        case 6: // H07
            V_FALLTHROUGH
        case 7: // H08
            V_FALLTHROUGH
        case 8: // H09
            V_FALLTHROUGH
        case 9: // H10
            V_FALLTHROUGH
        case 10: // H11
            V_FALLTHROUGH
        case 11: // H12
            return QStringLiteral("Hp4");
        case 12: // H13
            V_FALLTHROUGH
        case 13: // H14
            V_FALLTHROUGH
        case 14: // H15
            V_FALLTHROUGH
        case 15: // H16
            return QStringLiteral("Hp5");
        case 16: // H17
            V_FALLTHROUGH
        case 17: // H18
            V_FALLTHROUGH
        case 18: // H19
            V_FALLTHROUGH
        case 19: // H20
            return QStringLiteral("Hp6");
        case 20: // H21
            V_FALLTHROUGH
        case 21: // H22
            V_FALLTHROUGH
        case 22: // H23
            V_FALLTHROUGH
        case 23: // H24
            V_FALLTHROUGH
        case 24: // H25
            return QStringLiteral("Hp7");
        case 25: // H26
            V_FALLTHROUGH
        case 26: // H27
            V_FALLTHROUGH
        case 27: // H28
            V_FALLTHROUGH
        case 28: // H29
            return QStringLiteral("Hp8");
        case 29: // H30
            V_FALLTHROUGH
        case 30: // H31
            V_FALLTHROUGH
        case 31: // H32
            return QStringLiteral("Hp9");
        case 32: // H33
            V_FALLTHROUGH
        case 33: // H34
            V_FALLTHROUGH
        case 34: // H35
            return QStringLiteral("Hp10");
        case 35: // H36
            V_FALLTHROUGH
        case 36: // H37
            V_FALLTHROUGH
        case 37: // H38
            V_FALLTHROUGH
        case 38: // H39
            return QStringLiteral("Hp11");
        case 39: // H40
            return QStringLiteral("Hp12");
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
            return QStringLiteral("Ip1");
        case 4: // I05
            V_FALLTHROUGH
        case 5: // I06
            return QStringLiteral("Ip2");
        case 6: // I07
            V_FALLTHROUGH
        case 7: // I08
            V_FALLTHROUGH
        case 8: // I09
            return QStringLiteral("Ip3");
        case 9: // I10
            V_FALLTHROUGH
        case 10: // I11
            return QStringLiteral("Ip4");
        case 11: // I12
            return QStringLiteral("Ip5");
        case 12: // I13
            return QStringLiteral("Ip6");
        case 13: // I14
            return QStringLiteral("Ip7");
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
            V_FALLTHROUGH
        case 3: // J04
            return QStringLiteral("Jp1");
        case 4: // J05
            return QStringLiteral("Jp2");
        case 5: // J06
            return QStringLiteral("Jp3");
        case 6: // J07
            V_FALLTHROUGH
        case 7: // J08
            return QStringLiteral("Jp4");
        case 8: // J09
            return QStringLiteral("Jp5");
        default:
            break;
    }

    switch (ListNumbers(ListGroupK()).indexOf(number))
    {
        // K
        case 0: // K01
            return QStringLiteral("Kp1");
        case 1: // K02
            V_FALLTHROUGH
        case 2: // K03
            return QStringLiteral("Kp2");
        case 3: // K04
            return QStringLiteral("Kp3");
        case 4: // K05
            return QStringLiteral("Kp4");
        case 5: // K06
            V_FALLTHROUGH
        case 6: // K07
            return QStringLiteral("Kp5");
        case 7: // K08
            return QStringLiteral("Kp6");
        case 8: // K09
            return QStringLiteral("Kp7");
        case 9: // K10
            return QStringLiteral("Kp8");
        case 10: // K11
            return QStringLiteral("Kp9");
        case 11: // K12
            return QStringLiteral("Kp10");
        case 12: // K13
            return QStringLiteral("Kp11");
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
            V_FALLTHROUGH
        case 3: // L04
            return QStringLiteral("Lp1");
        case 4: // L05
            V_FALLTHROUGH
        case 5: // L06
            V_FALLTHROUGH
        case 6: // L07
            return QStringLiteral("Lp2");
        case 7: // L08
            V_FALLTHROUGH
        case 8: // L09
            V_FALLTHROUGH
        case 9: // L10
            return QStringLiteral("Lp3");
        case 10: // L11
            V_FALLTHROUGH
        case 11: // L12
            V_FALLTHROUGH
        case 12: // L13
            V_FALLTHROUGH
        case 13: // L14
            V_FALLTHROUGH
        case 14: // L15
            return QStringLiteral("Lp4");
        case 15: // L16
            return QStringLiteral("Lp5");
        case 16: // L17
            return QStringLiteral("Lp6");
        case 17: // L18
            return QStringLiteral("Lp7");
        case 18: // L19
            V_FALLTHROUGH
        case 19: // L20
            return QStringLiteral("Lp8");
        case 20: // L21
            return QStringLiteral("Lp9");
        default:
            break;
    }

    switch (ListNumbers(ListGroupM()).indexOf(number))
    {
        // M
        case 0: // M01
            V_FALLTHROUGH
        case 1: // M02
            return QStringLiteral("Mp1");
        case 2: // M03
            V_FALLTHROUGH
        case 3: // M04
            V_FALLTHROUGH
        case 4: // M05
            V_FALLTHROUGH
        case 5: // M06
            V_FALLTHROUGH
        case 6: // M07
            V_FALLTHROUGH
        case 7: // M08
            V_FALLTHROUGH
        case 8: // M09
            V_FALLTHROUGH
        case 9: // M10
            V_FALLTHROUGH
        case 10: // M11
            return QStringLiteral("Mp2");
        case 11: // M12
            V_FALLTHROUGH
        case 12: // M13
            V_FALLTHROUGH
        case 13: // M14
            return QStringLiteral("Mp3");
        default:
            break;
    }

    switch (ListNumbers(ListGroupN()).indexOf(number))
    {
        // N
        case 0: // N01
            return QStringLiteral("Np1");
        case 1: // N02
            V_FALLTHROUGH
        case 2: // N03
            return QStringLiteral("Np2");
        case 3: // N04
            V_FALLTHROUGH
        case 4: // N05
            return QStringLiteral("Np3");
        case 5: // N06
            V_FALLTHROUGH
        case 6: // N07
            return QStringLiteral("Np4");
        default:
            break;
    }

    switch (ListNumbers(ListGroupO()).indexOf(number))
    {
        // O
        case 0: // O01
            return QStringLiteral("Op1");
        case 1: // O02
            return QStringLiteral("Op2");
        case 2: // O03
            return QStringLiteral("Op3");
        case 3: // O04
            return QStringLiteral("Op4");
        case 4: // O05
            return QStringLiteral("Op5");
        case 5: // O06
            return QStringLiteral("Op6");
        case 6: // O07
            return QStringLiteral("Op7");
        case 7: // O08
            V_FALLTHROUGH
        case 8: // O09
            return QStringLiteral("Op8");
        case 9: // O10
            V_FALLTHROUGH
        case 10: // O11
            return QStringLiteral("Op9");
        case 11: // O12
            V_FALLTHROUGH
        case 12: // O13
            return QStringLiteral("Op10");
        case 13: // O14
            return QStringLiteral("Op11");
        default:
            break;
    }

    switch (ListNumbers(ListGroupP()).indexOf(number))
    {
        // P
        case 0: // P01
            return QStringLiteral("Pp1");
        case 1: // P02
            return QStringLiteral("Pp2");
        case 2: // P03
            return QStringLiteral("Pp3");
        case 3: // P04
            return QStringLiteral("Pp4");
        case 4: // P05
            return QStringLiteral("Pp5");
        case 5: // P06
            return QStringLiteral("Pp6");
        case 6: // P07
            return QStringLiteral("Pp7");
        case 7: // P08
            return QStringLiteral("Pp8");
        case 8: // P09
            return QStringLiteral("Pp9");
        case 9: // P10
            return QStringLiteral("Pp10");
        case 10: // P11
            return QStringLiteral("Pp11");
        case 11: // P12
            return QStringLiteral("Pp12");
        default:
            break;
    }

    switch (ListNumbers(ListGroupQ()).indexOf(number))
    {
        // Q
        case 0: // Q01
            return QStringLiteral("Qp1");
        case 1: // Q02
            return QStringLiteral("Qp2");
        case 2: // Q03
            return QStringLiteral("Qp3");
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
