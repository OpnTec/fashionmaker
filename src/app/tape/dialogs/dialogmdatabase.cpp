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
      list(list)
{
    ui->setupUi(this);
    InitDataBase(list);

    connect(ui->treeWidget, &QTreeWidget::itemChanged, this, &DialogMDataBase::UpdateChecks);
    connect(ui->treeWidget, &QTreeWidget::itemClicked, this, &DialogMDataBase::ShowDescription);
}

//---------------------------------------------------------------------------------------------------------------------
DialogMDataBase::DialogMDataBase(QWidget *parent)
    :QDialog(parent),
      ui(new Ui::DialogMDataBase),
      selectMode(false),
      list()

{
    InitDataBase();

    connect(ui->treeWidget, &QTreeWidget::itemChanged, this, &DialogMDataBase::UpdateChecks);
    connect(ui->treeWidget, &QTreeWidget::itemClicked, this, &DialogMDataBase::ShowDescription);
}

//---------------------------------------------------------------------------------------------------------------------
DialogMDataBase::~DialogMDataBase()
{
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
void DialogMDataBase::UpdateChecks(QTreeWidgetItem *item, int column)
{
    bool diff = false;
    if (column != 0 && column != -1)
    {
        return;
    }

    if (item->childCount() != 0 && item->checkState(0) != Qt::PartiallyChecked && column != -1)
    {
        Qt::CheckState checkState = item->checkState(0);
        for (int i = 0; i < item->childCount(); ++i)
        {
            if (not list.contains(item->child(i)->data(0, Qt::UserRole).toString()))
            {
                item->child(i)->setCheckState(0, checkState);
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
            item->parent()->setCheckState(0,item->checkState(0));
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
    InitGroupA(list);
    InitGroupB(list);
    InitGroupC(list);
    InitGroupD(list);
    InitGroupE(list);
    InitGroupF(list);
    InitGroupG(list);
    InitGroupH(list);
    InitGroupI(list);
    InitGroupJ(list);
    InitGroupK(list);
    InitGroupL(list);
    InitGroupM(list);
    InitGroupN(list);
    InitGroupO(list);
    InitGroupP(list);
    InitGroupQ(list);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogMDataBase::InitDataBase()
{
    InitGroupA();
    InitGroupB();
    InitGroupC();
    InitGroupD();
    InitGroupE();
    InitGroupF();
    InitGroupG();
    InitGroupH();
    InitGroupI();
    InitGroupJ();
    InitGroupK();
    InitGroupL();
    InitGroupM();
    InitGroupN();
    InitGroupO();
    InitGroupP();
    InitGroupQ();
}

//---------------------------------------------------------------------------------------------------------------------
void DialogMDataBase::InitGroupA(const QStringList &list)
{
    QTreeWidgetItem *group = new QTreeWidgetItem(ui->treeWidget);
    group->setText(0, "A. " + tr("Direct Height"));
    if (selectMode)
    {
        group->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
        group->setCheckState(0, Qt::Unchecked);
    }

    AddMeasurement(group, height_M, list);                // A01
    AddMeasurement(group, heightNeckBack_M, list);        // A02
    AddMeasurement(group, heightScapula_M, list);         // A03
    AddMeasurement(group, heightArmpit_M, list);          // A04
    AddMeasurement(group, heightWaistSide_M, list);       // A05
    AddMeasurement(group, heightHip_M, list);             // A06
    AddMeasurement(group, heightGlutealFold_M, list);     // A07
    AddMeasurement(group, heightKnee_M, list);            // A08
    AddMeasurement(group, heightCalf_M, list);            // A09
    AddMeasurement(group, heightAnkleHigh_M, list);       // A10
    AddMeasurement(group, heightAnkle_M, list);           // A11
    AddMeasurement(group, heightHighhip_M, list);         // A12
    AddMeasurement(group, heightWaistFront_M, list);      // A13
    AddMeasurement(group, heightBustpoint_M, list);       // A14
    AddMeasurement(group, heightShoulderTip_M, list);     // A15
    AddMeasurement(group, heightNeckFront_M, list);       // A16
    AddMeasurement(group, heightNeckSide_M, list);        // A17
    AddMeasurement(group, heightNeckBackToKnee_M, list);  // A18
    AddMeasurement(group, heightWaistSideToKnee_M, list); // A19
    AddMeasurement(group, heightWaistSideToHip_M, list);  // A20

    ui->treeWidget->setItemExpanded(group, true);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogMDataBase::InitGroupB(const QStringList &list)
{
    QTreeWidgetItem *group = new QTreeWidgetItem(ui->treeWidget);
    group->setText(0, "B. " + tr("Direct Width"));
    if (selectMode)
    {
        group->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
        group->setCheckState(0, Qt::Unchecked);
    }

    AddMeasurement(group, widthShoulder_M, list);     // B01
    AddMeasurement(group, widthBust_M, list);         // B02
    AddMeasurement(group, widthWaist_M, list);        // B03
    AddMeasurement(group, widthHip_M, list);          // B04
    AddMeasurement(group, widthAbdomenToHip_M, list); // B05

    ui->treeWidget->setItemExpanded(group, true);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogMDataBase::InitGroupC(const QStringList &list)
{
    QTreeWidgetItem *group = new QTreeWidgetItem(ui->treeWidget);
    group->setText(0, "C. " + tr("Indentation"));
    if (selectMode)
    {
        group->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
        group->setCheckState(0, Qt::Unchecked);
    }

    AddMeasurement(group, indentNeckBack_M, list);  // C01
    AddMeasurement(group, indentWaistBack_M, list); // C02
    AddMeasurement(group, indentAnkleHigh_M, list); // C03

    ui->treeWidget->setItemExpanded(group, true);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogMDataBase::InitGroupD(const QStringList &list)
{
    QTreeWidgetItem *group = new QTreeWidgetItem(ui->treeWidget);
    group->setText(0, "D. " + tr("Circumference and Arc"));
    if (selectMode)
    {
        group->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
        group->setCheckState(0, Qt::Unchecked);
    }

    AddMeasurement(group, neckMidCirc_M, list);        // D01
    AddMeasurement(group, neckCirc_M, list);           // D02
    AddMeasurement(group, highbustCirc_M, list);       // D03
    AddMeasurement(group, bustCirc_M, list);           // D04
    AddMeasurement(group, lowbustCirc_M, list);        // D05
    AddMeasurement(group, ribCirc_M, list);            // D06
    AddMeasurement(group, waistCirc_M, list);          // D07
    AddMeasurement(group, highhipCirc_M, list);        // D08
    AddMeasurement(group, hipCirc_M, list);            // D09
    AddMeasurement(group, neckArcF_M, list);           // D10
    AddMeasurement(group, highbustArcF_M, list);       // D11
    AddMeasurement(group, bustArcF_M, list);           // D12
    AddMeasurement(group, lowbustArcF_M, list);        // D13
    AddMeasurement(group, ribArcF_M, list);            // D14
    AddMeasurement(group, waistArcF_M, list);          // D15
    AddMeasurement(group, highhipArcF_M, list);        // D16
    AddMeasurement(group, hipArcF_M, list);            // D17
    AddMeasurement(group, neckArcB_M, list);           // D18
    AddMeasurement(group, highbustArcB_M, list);       // D19
    AddMeasurement(group, bustArcB_M, list);           // D20
    AddMeasurement(group, lowbustArcB_M, list);        // D21
    AddMeasurement(group, ribArcB_M, list);            // D22
    AddMeasurement(group, waistArcB_M, list);          // D23
    AddMeasurement(group, highhipArcB_M, list);        // D24
    AddMeasurement(group, hipArcB_M, list);            // D25
    AddMeasurement(group, neckArcHalfF_M, list);       // D26
    AddMeasurement(group, highbustArcHalfF_M, list);   // D27
    AddMeasurement(group, bustArcHalfF_M, list);       // D28
    AddMeasurement(group, lowbustArcHalfF_M, list);    // D29
    AddMeasurement(group, ribArcHalfF_M, list);        // D30
    AddMeasurement(group, waistArcHalfF_M, list);      // D31
    AddMeasurement(group, highhipArcHalfF_M, list);    // D32
    AddMeasurement(group, hipArcHalfF_M, list);        // D33
    AddMeasurement(group, neckArcHalfB_M, list);       // D34
    AddMeasurement(group, highbustArcHalfB_M, list);   // D35
    AddMeasurement(group, bustArcHalfB_M, list);       // D36
    AddMeasurement(group, lowbustArcHalfB_M, list);    // D37
    AddMeasurement(group, ribArcHalfB_M, list);        // D38
    AddMeasurement(group, waistArcHalfB_M, list);      // D39
    AddMeasurement(group, highhipArcHalfB_M, list);    // D40
    AddMeasurement(group, hipArcHalfB_M, list);        // D41
    AddMeasurement(group, hipWithAbdomenArcF_M, list); // D42
    AddMeasurement(group, bodyArmfoldCirc_M, list);    // D43
    AddMeasurement(group, bodyBustCirc_M, list);       // D44
    AddMeasurement(group, bodyTorsoCirc_M, list);      // D45

    ui->treeWidget->setItemExpanded(group, true);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogMDataBase::InitGroupE(const QStringList &list)
{
    QTreeWidgetItem *group = new QTreeWidgetItem(ui->treeWidget);
    group->setText(0, "E. " + tr("Vertical"));
    if (selectMode)
    {
        group->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
        group->setCheckState(0, Qt::Unchecked);
    }

    AddMeasurement(group, neckFrontToWaistF_M, list);             // E01
    AddMeasurement(group, neckFrontToWaistFlatF_M, list);         // E02
    AddMeasurement(group, armpitToWaistSide_M, list);             // E03
    AddMeasurement(group, shoulderTipToWaistSideF_M, list);       // E04
    AddMeasurement(group, neckSideToWaistF_M, list);              // E05
    AddMeasurement(group, neckSideToWaistBustpointF_M, list);     // E06
    AddMeasurement(group, shoulderTipToWaistSideB_M, list);       // E07
    AddMeasurement(group, neckSideToWaistB_M, list);              // E08
    AddMeasurement(group, neckBackToWaistB_M, list);              // E09
    AddMeasurement(group, neckSideToWaistBladepointB_M, list);    // E10
    AddMeasurement(group, shoulderTipToArmfoldF_M, list);         // E11
    AddMeasurement(group, neckSideToBustF_M, list);               // E12
    AddMeasurement(group, neckSideToHighbustF_M, list);           // E13
    AddMeasurement(group, shoulderCenterToHighbustF_M, list);     // E14
    AddMeasurement(group, neckFrontToHighbustF_M, list);          // E15
    AddMeasurement(group, neckFrontToBustF_M, list);              // E16
    AddMeasurement(group, lowbustToWaistF_M, list);               // E17
    AddMeasurement(group, shoulderTipToArmfoldB_M, list);         // E18
    AddMeasurement(group, neckSideToBustB_M, list);               // E19
    AddMeasurement(group, neckSideToHighbustB_M, list);           // E20
    AddMeasurement(group, shoulderCenterToHighbustB_M, list);     // E21
    AddMeasurement(group, neckBackToHighbustB_M, list);           // E22
    AddMeasurement(group, neckBackToBustB_M, list);               // E23
    AddMeasurement(group, lowbustToWaistB_M, list);               // E24
    AddMeasurement(group, waistToHighhipF_M, list);               // E25
    AddMeasurement(group, waistToHipF_M, list);                   // E26
    AddMeasurement(group, waistToHighhipSide_M, list);            // E27
    AddMeasurement(group, waistToHighhipB_M, list);               // E28
    AddMeasurement(group, waistToHipB_M, list);                   // E29
    AddMeasurement(group, waistToHipSide_M, list);                // E30
    AddMeasurement(group, shoulderSlopeNeckSideLength_M, list);   // E31
    AddMeasurement(group, shoulderSlopeNeckSideAngle_M, list);    // E32
    AddMeasurement(group, shoulderSlopeNeckBackHeight_M, list);   // E33
    AddMeasurement(group, shoulderSlopeNeckBackAngle_M, list);    // E34
    AddMeasurement(group, shoulderSlopeShoulderTipAngle_M, list); // E35
    AddMeasurement(group, highbustToWaistF_M, list);              // E36
    AddMeasurement(group, highbustToWaistB_M, list);              // E37

    ui->treeWidget->setItemExpanded(group, true);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogMDataBase::InitGroupF(const QStringList &list)
{
    QTreeWidgetItem *group = new QTreeWidgetItem(ui->treeWidget);
    group->setText(0, "F." + tr("Horizontal"));
    if (selectMode)
    {
        group->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
        group->setCheckState(0, Qt::Unchecked);
    }

    AddMeasurement(group, shoulderLength_M, list);                // F01
    AddMeasurement(group, shoulderWidthF_M, list);                // F02
    AddMeasurement(group, acrossChestF_M, list);                  // F03
    AddMeasurement(group, armfoldToArmfoldF_M, list);             // F04
    AddMeasurement(group, shoulderWidthB_M, list);                // F05
    AddMeasurement(group, acrossBackB_M, list);                   // F06
    AddMeasurement(group, armfoldToArmfoldB_M, list);             // F07
    AddMeasurement(group, shoulderTipToShoulderTipHalfF_M, list); // F08
    AddMeasurement(group, acrossChestHalfF_M, list);              // F09
    AddMeasurement(group, shoulderTipToShoulderTipHalfB_M, list); // F10
    AddMeasurement(group, acrossBackHalfB_M, list);               // F11
    AddMeasurement(group, neckFrontToShoulderTipF_M, list);       // F12
    AddMeasurement(group, neckBackToShoulderTipB_M, list);        // F13
    AddMeasurement(group, neckWidth_M, list);                     // F14

    ui->treeWidget->setItemExpanded(group, true);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogMDataBase::InitGroupG(const QStringList &list)
{
    QTreeWidgetItem *group = new QTreeWidgetItem(ui->treeWidget);
    group->setText(0, "G. " + tr("Bust"));
    if (selectMode)
    {
        group->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
        group->setCheckState(0, Qt::Unchecked);
    }

    AddMeasurement(group, bustpointToBustpoint_M, list);       // G01
    AddMeasurement(group, bustpointToNeckSide_M, list);        // G02
    AddMeasurement(group, bustpointToLowbust_M, list);         // G03
    AddMeasurement(group, bustpointToWaist_M, list);           // G04
    AddMeasurement(group, bustpointToBustpointHalf_M, list);   // G05
    AddMeasurement(group, bustpointToBustpointHalter_M, list); // G06
    AddMeasurement(group, bustpointToShoulderTip_M, list);     // G07
    AddMeasurement(group, bustpointToWaistFront_M, list);      // G08

    ui->treeWidget->setItemExpanded(group, true);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogMDataBase::InitGroupH(const QStringList &list)
{
    QTreeWidgetItem *group = new QTreeWidgetItem(ui->treeWidget);
    group->setText(0, "H. " + tr("Balance"));
    if (selectMode)
    {
        group->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
        group->setCheckState(0, Qt::Unchecked);
    }

    AddMeasurement(group, shoulderTipToWaistFront_M, list); // H01
    AddMeasurement(group, neckFrontToWaistSide_M, list);    // H02
    AddMeasurement(group, neckSideToWaistSideF_M, list);    // H03
    AddMeasurement(group, neckSideToArmfoldF_M, list);      // H04
    AddMeasurement(group, neckSideToArmpitF_M, list);       // H05
    AddMeasurement(group, neckSideToBustSideF_M, list);     // H06
    AddMeasurement(group, shoulderTipToWaistBack_M, list);  // H07
    AddMeasurement(group, neckBackToWaistSide_M, list);     // H08
    AddMeasurement(group, neckSideToWaistSideB_M, list);    // H09
    AddMeasurement(group, neckSideToArmfoldB_M, list);      // H10
    AddMeasurement(group, neckSideToArmpitB_M, list);       // H11
    AddMeasurement(group, neckSideToBustSideB_M, list);     // H12

    ui->treeWidget->setItemExpanded(group, true);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogMDataBase::InitGroupI(const QStringList &list)
{
    QTreeWidgetItem *group = new QTreeWidgetItem(ui->treeWidget);
    group->setText(0, "I. " + tr("Arm"));
    if (selectMode)
    {
        group->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
        group->setCheckState(0, Qt::Unchecked);
    }

    AddMeasurement(group, armShoulderTipToWristBent_M, list);   // I01
    AddMeasurement(group, armShoulderTipToElbowBent_M, list);   // I02
    AddMeasurement(group, armElbowToWristBent_M, list);         // I03
    AddMeasurement(group, armElbowCircBent_M, list);            // I04
    AddMeasurement(group, armShoulderTipToWrist_M, list);       // I05
    AddMeasurement(group, armShoulderTipToElbow_M, list);       // I06
    AddMeasurement(group, armElbowToWrist_M, list);             // I07
    AddMeasurement(group, armArmpitToWrist_M, list);            // I08
    AddMeasurement(group, armArmpitToElbow_M, list);            // I09
    AddMeasurement(group, armElbowToWristInside_M, list);       // I10
    AddMeasurement(group, armUpperCirc_M, list);                // I11
    AddMeasurement(group, armAboveElbowCirc_M, list);           // I12
    AddMeasurement(group, armElbowCirc_M, list);                // I13
    AddMeasurement(group, armLowerCirc_M, list);                // I14
    AddMeasurement(group, armWristCirc_M, list);                // I15
    AddMeasurement(group, armShoulderTipToArmfoldLine_M, list); // I16
    AddMeasurement(group, armscyeCirc_M, list);                 // I17
    AddMeasurement(group, armscyeLength_M, list);               // I18
    AddMeasurement(group, armscyeWidth_M, list);                // I19
    AddMeasurement(group, armNeckSideToFingerTip_M, list);      // I20
    AddMeasurement(group, armNeckSideToWrist_M, list);          // I21

    ui->treeWidget->setItemExpanded(group, true);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogMDataBase::InitGroupJ(const QStringList &list)
{
    QTreeWidgetItem *group = new QTreeWidgetItem(ui->treeWidget);
    group->setText(0, "J. " + tr("Leg"));
    if (selectMode)
    {
        group->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
        group->setCheckState(0, Qt::Unchecked);
    }

    AddMeasurement(group, legCrotchToFloor_M, list);    // J01
    AddMeasurement(group, legWaistSideToFloor_M, list); // J02
    AddMeasurement(group, legWaistSideToKnee_M, list);  // J03
    AddMeasurement(group, legThighUpperCirc_M, list);	// J04
    AddMeasurement(group, legThighMidCirc_M, list);     // J05
    AddMeasurement(group, legKneeCirc_M, list);	        // J06
    AddMeasurement(group, legKneeSmallCirc_M, list);    // J07
    AddMeasurement(group, legCalfCirc_M, list);	        // J08
    AddMeasurement(group, legAnkleHighCirc_M, list);    // J09
    AddMeasurement(group, legAnkleCirc_M, list);        // J10
    AddMeasurement(group, legKneeCircBent_M, list);	    // J11
    AddMeasurement(group, legAnkleDiagCirc_M, list);    // J12

    ui->treeWidget->setItemExpanded(group, true);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogMDataBase::InitGroupK(const QStringList &list)
{
    QTreeWidgetItem *group = new QTreeWidgetItem(ui->treeWidget);
    group->setText(0, "K. " + tr("Crotch and Rise"));
    if (selectMode)
    {
        group->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
        group->setCheckState(0, Qt::Unchecked);
    }

    AddMeasurement(group, crotchLength_M, list);   // K01
    AddMeasurement(group, crotchLengthB_M, list);  // K02
    AddMeasurement(group, crotchLengthF_M, list);  // K03
    AddMeasurement(group, riseLengthSide_M, list); // K04
    AddMeasurement(group, riseLengthDiag_M, list); // K05
    AddMeasurement(group, riseLengthB_M, list);    // K06
    AddMeasurement(group, riseLengthF_M, list);    // K07

    ui->treeWidget->setItemExpanded(group, true);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogMDataBase::InitGroupL(const QStringList &list)
{
    QTreeWidgetItem *group = new QTreeWidgetItem(ui->treeWidget);
    group->setText(0, "L. " + tr("Hand"));
    if (selectMode)
    {
        group->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
        group->setCheckState(0, Qt::Unchecked);
    }

    AddMeasurement(group, handPalmLength_M, list); // L01
    AddMeasurement(group, handLength_M, list);     // L02
    AddMeasurement(group, handPalmWidth_M, list);  // L03
    AddMeasurement(group, handPalmCirc_M, list);   // L04
    AddMeasurement(group, handCirc_M, list);	   // L05

    ui->treeWidget->setItemExpanded(group, true);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogMDataBase::InitGroupM(const QStringList &list)
{
    QTreeWidgetItem *group = new QTreeWidgetItem(ui->treeWidget);
    group->setText(0, "M. " + tr("Foot"));
    if (selectMode)
    {
        group->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
        group->setCheckState(0, Qt::Unchecked);
    }

    AddMeasurement(group, footWidth_M, list);      // M01
    AddMeasurement(group, footLength_M, list);     // M02
    AddMeasurement(group, footCirc_M, list);       // M03
    AddMeasurement(group, footInstepCirc_M, list); // M04

    ui->treeWidget->setItemExpanded(group, true);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogMDataBase::InitGroupN(const QStringList &list)
{
    QTreeWidgetItem *group = new QTreeWidgetItem(ui->treeWidget);
    group->setText(0, "N. " + tr("Head"));
    if (selectMode)
    {
        group->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
        group->setCheckState(0, Qt::Unchecked);
    }

    AddMeasurement(group, headCirc_M, list);	        // N01
    AddMeasurement(group, headLength_M, list);          // N02
    AddMeasurement(group, headDepth_M, list);	        // N03
    AddMeasurement(group, headWidth_M, list);	        // N04
    AddMeasurement(group, headCrownToNeckBack_M, list); // N05
    AddMeasurement(group, headChinToNeckBack_M, list);  // N06

    ui->treeWidget->setItemExpanded(group, true);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogMDataBase::InitGroupO(const QStringList &list)
{
    QTreeWidgetItem *group = new QTreeWidgetItem(ui->treeWidget);
    group->setText(0, "O. " + tr("Men & Tailoring"));
    if (selectMode)
    {
        group->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
        group->setCheckState(0, Qt::Unchecked);
    }

    AddMeasurement(group, neckBackToWaistFront_M, list);	        // O01
    AddMeasurement(group, waistToWaistHalter_M, list);	            // O02
    AddMeasurement(group, waistNaturalCirc_M, list);	            // O03
    AddMeasurement(group, waistNaturalArcF_M, list);                // O04
    AddMeasurement(group, waistNaturalArcB_M, list);                // O05
    AddMeasurement(group, waistToNaturalWaistF_M, list);            // O06
    AddMeasurement(group, waistToNaturalWaistB_M, list);            // O07
    AddMeasurement(group, armNeckBackToElbowBent_M, list);          // O08
    AddMeasurement(group, armNeckBackToWristBent_M, list);          // O09
    AddMeasurement(group, armNeckSideToElbowBent_M, list);	        // O10
    AddMeasurement(group, armNeckSideToWristBent_M, list);	        // O11
    AddMeasurement(group, armAcrossBackCenterToElbowBent_M, list);  // O12
    AddMeasurement(group, armAcrossBackCenterToWristBent_M, list);  // O13
    AddMeasurement(group, armArmscyeBackCenterToWristBent_M, list); // O14

    ui->treeWidget->setItemExpanded(group, true);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogMDataBase::InitGroupP(const QStringList &list)
{
    QTreeWidgetItem *group = new QTreeWidgetItem(ui->treeWidget);
    group->setText(0, "P. " + tr("Historical & Specialty"));
    if (selectMode)
    {
        group->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
        group->setCheckState(0, Qt::Unchecked);
    }

    AddMeasurement(group, armfoldToArmfoldBust_M, list);                             // P01
    AddMeasurement(group, armfoldToBustFront_M, list);	                             // P02
    AddMeasurement(group, neckBackToBustFront_M, list);	                             // P03
    AddMeasurement(group, neckBackToArmfoldFront_M, list);	                         // P04
    AddMeasurement(group, neckBackToArmfoldFrontToWaistSide_M, list);                // P05
    AddMeasurement(group, highbustBackOverShoulderToArmfoldFront_M, list);           // P06
    AddMeasurement(group, highbustBackOverShoulderToWaistFront_M, list);             // P07
    AddMeasurement(group, neckBackToArmfoldFrontToNeckBack_M, list);                 // P08
    AddMeasurement(group, acrossBackCenterToArmfoldFrontToAcrossBackCenter_M, list); // P09
    AddMeasurement(group, neckBackToArmfoldFrontToHighbustBack_M, list);             // P10
    AddMeasurement(group, highbustBOverShoulderToHighbustF_M, list);	             // P11
    AddMeasurement(group, armscyeArc_M, list);	                                     // P12

    ui->treeWidget->setItemExpanded(group, true);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogMDataBase::InitGroupQ(const QStringList &list)
{
    QTreeWidgetItem *group = new QTreeWidgetItem(ui->treeWidget);
    group->setText(0, "Q. " + tr("Patternmaking measurements"));
    if (selectMode)
    {
        group->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
        group->setCheckState(0, Qt::Unchecked);
    }

    AddMeasurement(group, dartWidthShoulder_M, list); // Q01
    AddMeasurement(group, dartWidthBust_M, list);	  // Q02
    AddMeasurement(group, dartWidthWaist_M, list);    // Q03

    ui->treeWidget->setItemExpanded(group, true);
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
        }
        else
        {
            m->setCheckState(0, Qt::Unchecked);
        }
    }

    m->setText(0, qApp->TrVars()->MNumber(name) + ". " + qApp->TrVars()->MToUser(name));
    m->setData(0, Qt::UserRole, name);
}

//---------------------------------------------------------------------------------------------------------------------
QString DialogMDataBase::MapDiagrams(const QString &number) const
{
    const QStringList ANumbers = QStringList()
            // A
            << qApp->TrVars()->MNumber(height_M)                // A01
            << qApp->TrVars()->MNumber(heightNeckBack_M)        // A02
            << qApp->TrVars()->MNumber(heightScapula_M)         // A03
            << qApp->TrVars()->MNumber(heightArmpit_M)          // A04
            << qApp->TrVars()->MNumber(heightWaistSide_M)       // A05
            << qApp->TrVars()->MNumber(heightHip_M)             // A06
            << qApp->TrVars()->MNumber(heightGlutealFold_M)     // A07
            << qApp->TrVars()->MNumber(heightKnee_M)            // A08
            << qApp->TrVars()->MNumber(heightCalf_M)            // A09
            << qApp->TrVars()->MNumber(heightAnkleHigh_M)       // A10
            << qApp->TrVars()->MNumber(heightAnkle_M)           // A11
            << qApp->TrVars()->MNumber(heightHighhip_M)         // A12
            << qApp->TrVars()->MNumber(heightWaistFront_M)      // A13
            << qApp->TrVars()->MNumber(heightBustpoint_M)       // A14
            << qApp->TrVars()->MNumber(heightShoulderTip_M)     // A15
            << qApp->TrVars()->MNumber(heightNeckFront_M)       // A16
            << qApp->TrVars()->MNumber(heightNeckSide_M)        // A17
            << qApp->TrVars()->MNumber(heightNeckBackToKnee_M)  // A18
            << qApp->TrVars()->MNumber(heightWaistSideToKnee_M) // A19
            << qApp->TrVars()->MNumber(heightWaistSideToHip_M); // A20

    const QStringList BNumbers = QStringList()
            // B
            << qApp->TrVars()->MNumber(widthShoulder_M)      // B01
            << qApp->TrVars()->MNumber(widthBust_M)          // B02
            << qApp->TrVars()->MNumber(widthWaist_M)         // B03
            << qApp->TrVars()->MNumber(widthHip_M)           // B04
            << qApp->TrVars()->MNumber(widthAbdomenToHip_M); // B05

    const QStringList CNumbers = QStringList()
            // C
            << qApp->TrVars()->MNumber(indentNeckBack_M)   // C01
            << qApp->TrVars()->MNumber(indentWaistBack_M)  // C02
            << qApp->TrVars()->MNumber(indentAnkleHigh_M); // C03

    const QStringList DNumbers = QStringList()
            // D
            << qApp->TrVars()->MNumber(neckMidCirc_M)        // D01
            << qApp->TrVars()->MNumber(neckCirc_M)           // D02
            << qApp->TrVars()->MNumber(highbustCirc_M)       // D03
            << qApp->TrVars()->MNumber(bustCirc_M)           // D04
            << qApp->TrVars()->MNumber(lowbustCirc_M)        // D05
            << qApp->TrVars()->MNumber(ribCirc_M)            // D06
            << qApp->TrVars()->MNumber(waistCirc_M)          // D07
            << qApp->TrVars()->MNumber(highhipCirc_M)        // D08
            << qApp->TrVars()->MNumber(hipCirc_M)            // D09
            << qApp->TrVars()->MNumber(neckArcF_M)           // D10
            << qApp->TrVars()->MNumber(highbustArcF_M)       // D11
            << qApp->TrVars()->MNumber(bustArcF_M)           // D12
            << qApp->TrVars()->MNumber(lowbustArcF_M)        // D13
            << qApp->TrVars()->MNumber(ribArcF_M)            // D14
            << qApp->TrVars()->MNumber(waistArcF_M)          // D15
            << qApp->TrVars()->MNumber(highhipArcF_M)        // D16
            << qApp->TrVars()->MNumber(hipArcF_M)            // D17
            << qApp->TrVars()->MNumber(neckArcB_M)           // D18
            << qApp->TrVars()->MNumber(highbustArcB_M)       // D19
            << qApp->TrVars()->MNumber(bustArcB_M)           // D20
            << qApp->TrVars()->MNumber(lowbustArcB_M)        // D21
            << qApp->TrVars()->MNumber(ribArcB_M)            // D22
            << qApp->TrVars()->MNumber(waistArcB_M)          // D23
            << qApp->TrVars()->MNumber(highhipArcB_M)        // D24
            << qApp->TrVars()->MNumber(hipArcB_M)            // D25
            << qApp->TrVars()->MNumber(neckArcHalfF_M)       // D26
            << qApp->TrVars()->MNumber(highbustArcHalfF_M)   // D27
            << qApp->TrVars()->MNumber(bustArcHalfF_M)       // D28
            << qApp->TrVars()->MNumber(lowbustArcHalfF_M)    // D29
            << qApp->TrVars()->MNumber(ribArcHalfF_M)        // D30
            << qApp->TrVars()->MNumber(waistArcHalfF_M)      // D31
            << qApp->TrVars()->MNumber(highhipArcHalfF_M)    // D32
            << qApp->TrVars()->MNumber(hipArcHalfF_M)        // D33
            << qApp->TrVars()->MNumber(neckArcHalfB_M)       // D34
            << qApp->TrVars()->MNumber(highbustArcHalfB_M)   // D35
            << qApp->TrVars()->MNumber(bustArcHalfB_M)       // D36
            << qApp->TrVars()->MNumber(lowbustArcHalfB_M)    // D37
            << qApp->TrVars()->MNumber(ribArcHalfB_M)        // D38
            << qApp->TrVars()->MNumber(waistArcHalfB_M)      // D39
            << qApp->TrVars()->MNumber(highhipArcHalfB_M)    // D40
            << qApp->TrVars()->MNumber(hipArcHalfB_M)        // D41
            << qApp->TrVars()->MNumber(hipWithAbdomenArcF_M) // D42
            << qApp->TrVars()->MNumber(bodyArmfoldCirc_M)    // D43
            << qApp->TrVars()->MNumber(bodyBustCirc_M)       // D44
            << qApp->TrVars()->MNumber(bodyTorsoCirc_M);     // D45

    const QStringList ENumbers = QStringList()
            // E
            << qApp->TrVars()->MNumber(neckFrontToWaistF_M)             // E01
            << qApp->TrVars()->MNumber(neckFrontToWaistFlatF_M)         // E02
            << qApp->TrVars()->MNumber(armpitToWaistSide_M)             // E03
            << qApp->TrVars()->MNumber(shoulderTipToWaistSideF_M)       // E04
            << qApp->TrVars()->MNumber(neckSideToWaistF_M)              // E05
            << qApp->TrVars()->MNumber(neckSideToWaistBustpointF_M)     // E06
            << qApp->TrVars()->MNumber(shoulderTipToWaistSideB_M)       // E07
            << qApp->TrVars()->MNumber(neckSideToWaistB_M)              // E08
            << qApp->TrVars()->MNumber(neckBackToWaistB_M)              // E09
            << qApp->TrVars()->MNumber(neckSideToWaistBladepointB_M)    // E10
            << qApp->TrVars()->MNumber(shoulderTipToArmfoldF_M)         // E11
            << qApp->TrVars()->MNumber(neckSideToBustF_M)               // E12
            << qApp->TrVars()->MNumber(neckSideToHighbustF_M)           // E13
            << qApp->TrVars()->MNumber(shoulderCenterToHighbustF_M)     // E14
            << qApp->TrVars()->MNumber(neckFrontToHighbustF_M)          // E15
            << qApp->TrVars()->MNumber(neckFrontToBustF_M)              // E16
            << qApp->TrVars()->MNumber(lowbustToWaistF_M)               // E17
            << qApp->TrVars()->MNumber(shoulderTipToArmfoldB_M)         // E18
            << qApp->TrVars()->MNumber(neckSideToBustB_M)               // E19
            << qApp->TrVars()->MNumber(neckSideToHighbustB_M)           // E20
            << qApp->TrVars()->MNumber(shoulderCenterToHighbustB_M)     // E21
            << qApp->TrVars()->MNumber(neckBackToHighbustB_M)           // E22
            << qApp->TrVars()->MNumber(neckBackToBustB_M)               // E23
            << qApp->TrVars()->MNumber(lowbustToWaistB_M)               // E24
            << qApp->TrVars()->MNumber(waistToHighhipF_M)               // E25
            << qApp->TrVars()->MNumber(waistToHipF_M)                   // E26
            << qApp->TrVars()->MNumber(waistToHighhipSide_M)            // E27
            << qApp->TrVars()->MNumber(waistToHighhipB_M)               // E28
            << qApp->TrVars()->MNumber(waistToHipB_M)                   // E29
            << qApp->TrVars()->MNumber(waistToHipSide_M)                // E30
            << qApp->TrVars()->MNumber(shoulderSlopeNeckSideLength_M)   // E31
            << qApp->TrVars()->MNumber(shoulderSlopeNeckSideAngle_M)    // E32
            << qApp->TrVars()->MNumber(shoulderSlopeNeckBackHeight_M)   // E33
            << qApp->TrVars()->MNumber(shoulderSlopeNeckBackAngle_M)    // E34
            << qApp->TrVars()->MNumber(shoulderSlopeShoulderTipAngle_M) // E35
            << qApp->TrVars()->MNumber(highbustToWaistF_M)              // E36
            << qApp->TrVars()->MNumber(highbustToWaistB_M);             // E37

    const QStringList FNumbers = QStringList()
            // F
            << qApp->TrVars()->MNumber(shoulderLength_M)                // F01
            << qApp->TrVars()->MNumber(shoulderWidthF_M)                // F02
            << qApp->TrVars()->MNumber(acrossChestF_M)                  // F03
            << qApp->TrVars()->MNumber(armfoldToArmfoldF_M)             // F04
            << qApp->TrVars()->MNumber(shoulderWidthB_M)                // F05
            << qApp->TrVars()->MNumber(acrossBackB_M)                   // F06
            << qApp->TrVars()->MNumber(armfoldToArmfoldB_M)             // F07
            << qApp->TrVars()->MNumber(shoulderTipToShoulderTipHalfF_M) // F08
            << qApp->TrVars()->MNumber(acrossChestHalfF_M)              // F09
            << qApp->TrVars()->MNumber(shoulderTipToShoulderTipHalfB_M) // F10
            << qApp->TrVars()->MNumber(acrossBackHalfB_M)               // F11
            << qApp->TrVars()->MNumber(neckFrontToShoulderTipF_M)       // F12
            << qApp->TrVars()->MNumber(neckBackToShoulderTipB_M)        // F13
            << qApp->TrVars()->MNumber(neckWidth_M);                    // F14

    const QStringList GNumbers = QStringList()
            // G
            << qApp->TrVars()->MNumber(bustpointToBustpoint_M)       // G01
            << qApp->TrVars()->MNumber(bustpointToNeckSide_M)        // G02
            << qApp->TrVars()->MNumber(bustpointToLowbust_M)         // G03
            << qApp->TrVars()->MNumber(bustpointToWaist_M)           // G04
            << qApp->TrVars()->MNumber(bustpointToBustpointHalf_M)   // G05
            << qApp->TrVars()->MNumber(bustpointToBustpointHalter_M) // G06
            << qApp->TrVars()->MNumber(bustpointToShoulderTip_M)     // G07
            << qApp->TrVars()->MNumber(bustpointToWaistFront_M);     // G08

    const QStringList HNumbers = QStringList()
            // H
            << qApp->TrVars()->MNumber(shoulderTipToWaistFront_M) // H01
            << qApp->TrVars()->MNumber(neckFrontToWaistSide_M)    // H02
            << qApp->TrVars()->MNumber(neckSideToWaistSideF_M)    // H03
            << qApp->TrVars()->MNumber(neckSideToArmfoldF_M)      // H04
            << qApp->TrVars()->MNumber(neckSideToArmpitF_M)       // H05
            << qApp->TrVars()->MNumber(neckSideToBustSideF_M)     // H06
            << qApp->TrVars()->MNumber(shoulderTipToWaistBack_M)  // H07
            << qApp->TrVars()->MNumber(neckBackToWaistSide_M)     // H08
            << qApp->TrVars()->MNumber(neckSideToWaistSideB_M)    // H09
            << qApp->TrVars()->MNumber(neckSideToArmfoldB_M)      // H10
            << qApp->TrVars()->MNumber(neckSideToArmpitB_M)       // H11
            << qApp->TrVars()->MNumber(neckSideToBustSideB_M);    // H12

    const QStringList INumbers = QStringList()
            // I
            << qApp->TrVars()->MNumber(armShoulderTipToWristBent_M)   // I01
            << qApp->TrVars()->MNumber(armShoulderTipToElbowBent_M)   // I02
            << qApp->TrVars()->MNumber(armElbowToWristBent_M)         // I03
            << qApp->TrVars()->MNumber(armElbowCircBent_M)            // I04
            << qApp->TrVars()->MNumber(armShoulderTipToWrist_M)       // I05
            << qApp->TrVars()->MNumber(armShoulderTipToElbow_M)       // I06
            << qApp->TrVars()->MNumber(armElbowToWrist_M)             // I07
            << qApp->TrVars()->MNumber(armArmpitToWrist_M)            // I08
            << qApp->TrVars()->MNumber(armArmpitToElbow_M)            // I09
            << qApp->TrVars()->MNumber(armElbowToWristInside_M)       // I10
            << qApp->TrVars()->MNumber(armUpperCirc_M)                // I11
            << qApp->TrVars()->MNumber(armAboveElbowCirc_M)           // I12
            << qApp->TrVars()->MNumber(armElbowCirc_M)                // I13
            << qApp->TrVars()->MNumber(armLowerCirc_M)                // I14
            << qApp->TrVars()->MNumber(armWristCirc_M)                // I15
            << qApp->TrVars()->MNumber(armShoulderTipToArmfoldLine_M) // I16
            << qApp->TrVars()->MNumber(armscyeCirc_M)                 // I17
            << qApp->TrVars()->MNumber(armscyeLength_M)               // I18
            << qApp->TrVars()->MNumber(armscyeWidth_M)                // I19
            << qApp->TrVars()->MNumber(armNeckSideToFingerTip_M)      // I20
            << qApp->TrVars()->MNumber(armNeckSideToWrist_M);         // I21

    const QStringList JNumbers = QStringList()
            // J
            << qApp->TrVars()->MNumber(legCrotchToFloor_M)     // J01
            << qApp->TrVars()->MNumber(legWaistSideToFloor_M)  // J02
            << qApp->TrVars()->MNumber(legWaistSideToKnee_M)   // J03
            << qApp->TrVars()->MNumber(legThighUpperCirc_M)	   // J04
            << qApp->TrVars()->MNumber(legThighMidCirc_M)      // J05
            << qApp->TrVars()->MNumber(legKneeCirc_M)          // J06
            << qApp->TrVars()->MNumber(legKneeSmallCirc_M)     // J07
            << qApp->TrVars()->MNumber(legCalfCirc_M)          // J08
            << qApp->TrVars()->MNumber(legAnkleHighCirc_M)     // J09
            << qApp->TrVars()->MNumber(legAnkleCirc_M)         // J10
            << qApp->TrVars()->MNumber(legKneeCircBent_M)      // J11
            << qApp->TrVars()->MNumber(legAnkleDiagCirc_M)     // J12
            << qApp->TrVars()->MNumber(legCrotchToAnkle_M)     // J13
            << qApp->TrVars()->MNumber(legWaistSideToAnkle_M); // J14

    const QStringList KNumbers = QStringList()
            // K
            << qApp->TrVars()->MNumber(crotchLength_M)   // K01
            << qApp->TrVars()->MNumber(crotchLengthB_M)  // K02
            << qApp->TrVars()->MNumber(crotchLengthF_M)  // K03
            << qApp->TrVars()->MNumber(riseLengthSide_M) // K04
            << qApp->TrVars()->MNumber(riseLengthDiag_M) // K05
            << qApp->TrVars()->MNumber(riseLengthB_M)    // K06
            << qApp->TrVars()->MNumber(riseLengthF_M);   // K07

    const QStringList LNumbers = QStringList()
            // L
            << qApp->TrVars()->MNumber(handPalmLength_M) // L01
            << qApp->TrVars()->MNumber(handLength_M)     // L02
            << qApp->TrVars()->MNumber(handPalmWidth_M)  // L03
            << qApp->TrVars()->MNumber(handPalmCirc_M)   // L04
            << qApp->TrVars()->MNumber(handCirc_M);      // L05

    const QStringList MNumbers = QStringList()
            // M
            << qApp->TrVars()->MNumber(footWidth_M)       // M01
            << qApp->TrVars()->MNumber(footLength_M)      // M02
            << qApp->TrVars()->MNumber(footCirc_M)        // M03
            << qApp->TrVars()->MNumber(footInstepCirc_M); // M04

    const QStringList NNumbers = QStringList()
            // N
            << qApp->TrVars()->MNumber(headCirc_M)	          // N01
            << qApp->TrVars()->MNumber(headLength_M)          // N02
            << qApp->TrVars()->MNumber(headDepth_M)	          // N03
            << qApp->TrVars()->MNumber(headWidth_M)	          // N04
            << qApp->TrVars()->MNumber(headCrownToNeckBack_M) // N05
            << qApp->TrVars()->MNumber(headChinToNeckBack_M); // N06

    const QStringList ONumbers = QStringList()
            // O
            << qApp->TrVars()->MNumber(neckBackToWaistFront_M)	           // O01
            << qApp->TrVars()->MNumber(waistToWaistHalter_M)	           // O02
            << qApp->TrVars()->MNumber(waistNaturalCirc_M)	               // O03
            << qApp->TrVars()->MNumber(waistNaturalArcF_M)                 // O04
            << qApp->TrVars()->MNumber(waistNaturalArcB_M)                 // O05
            << qApp->TrVars()->MNumber(waistToNaturalWaistF_M)             // O06
            << qApp->TrVars()->MNumber(waistToNaturalWaistB_M)             // O07
            << qApp->TrVars()->MNumber(armNeckBackToElbowBent_M)           // O08
            << qApp->TrVars()->MNumber(armNeckBackToWristBent_M)           // O09
            << qApp->TrVars()->MNumber(armNeckSideToElbowBent_M)	       // O10
            << qApp->TrVars()->MNumber(armNeckSideToWristBent_M)	       // O11
            << qApp->TrVars()->MNumber(armAcrossBackCenterToElbowBent_M)   // O12
            << qApp->TrVars()->MNumber(armAcrossBackCenterToWristBent_M)   // O13
            << qApp->TrVars()->MNumber(armArmscyeBackCenterToWristBent_M); // O14

    const QStringList PNumbers = QStringList()
            // P
            << qApp->TrVars()->MNumber(armfoldToArmfoldBust_M)                             // P01
            << qApp->TrVars()->MNumber(armfoldToBustFront_M)	                           // P02
            << qApp->TrVars()->MNumber(neckBackToBustFront_M)	                           // P03
            << qApp->TrVars()->MNumber(neckBackToArmfoldFront_M)	                       // P04
            << qApp->TrVars()->MNumber(neckBackToArmfoldFrontToWaistSide_M)                // P05
            << qApp->TrVars()->MNumber(highbustBackOverShoulderToArmfoldFront_M)           // P06
            << qApp->TrVars()->MNumber(highbustBackOverShoulderToWaistFront_M)             // P07
            << qApp->TrVars()->MNumber(neckBackToArmfoldFrontToNeckBack_M)                 // P08
            << qApp->TrVars()->MNumber(acrossBackCenterToArmfoldFrontToAcrossBackCenter_M) // P09
            << qApp->TrVars()->MNumber(neckBackToArmfoldFrontToHighbustBack_M)             // P10
            << qApp->TrVars()->MNumber(highbustBOverShoulderToHighbustF_M)	               // P11
            << qApp->TrVars()->MNumber(armscyeArc_M);	                                   // P12

    const QStringList QNumbers = QStringList()
            // Q
            << qApp->TrVars()->MNumber(dartWidthShoulder_M) // Q01
            << qApp->TrVars()->MNumber(dartWidthBust_M)	    // Q02
            << qApp->TrVars()->MNumber(dartWidthWaist_M);   // Q03

    switch (ANumbers.indexOf(number))
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
            return QString("Ap2");
        default:
            break;
    }

    switch (BNumbers.indexOf(number))
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

    switch (CNumbers.indexOf(number))
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

    switch (DNumbers.indexOf(number))
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

    switch (ENumbers.indexOf(number))
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
            return QString("Ep5");
        case 14: // E15
            V_FALLTHROUGH
        case 15: // E16
            V_FALLTHROUGH
        case 16: // E17
            return QString("Ep6");
        case 17: // E18
            V_FALLTHROUGH
        case 18: // E19
            V_FALLTHROUGH
        case 19: // E20
            V_FALLTHROUGH
        case 20: // E21
            return QString("Ep7");
        case 21: // E22
            V_FALLTHROUGH
        case 22: // E23
            V_FALLTHROUGH
        case 23: // E24
            return QString("Ep8");
        case 24: // E25
            V_FALLTHROUGH
        case 25: // E26
            V_FALLTHROUGH
        case 26: // E27
            return QString("Ep9");
        case 27: // E28
            V_FALLTHROUGH
        case 28: // E29
            V_FALLTHROUGH
        case 29: // E30
            return QString("Ep10");
        case 30: // E31
            V_FALLTHROUGH
        case 31: // E32
            V_FALLTHROUGH
        case 32: // E33
            V_FALLTHROUGH
        case 33: // E34
            return QString("Ep11");
        case 34: // E35
            return QString("Ep12");
        case 35: // E36
            return QString("Ep13");
        case 36: // E37
            return QString("Ep14");
        default:
            break;
    }

    switch (FNumbers.indexOf(number))
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

    switch (GNumbers.indexOf(number))
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

    switch (HNumbers.indexOf(number))
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
        default:
            break;
    }

    switch (INumbers.indexOf(number))
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

    switch (JNumbers.indexOf(number))
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

    switch (KNumbers.indexOf(number))
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

    switch (LNumbers.indexOf(number))
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

    switch (MNumbers.indexOf(number))
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

    switch (NNumbers.indexOf(number))
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

    switch (ONumbers.indexOf(number))
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

    switch (PNumbers.indexOf(number))
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

    switch (QNumbers.indexOf(number))
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
