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
#include <QMenu>

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
    connect(ui->treeWidget, &QTreeWidget::customContextMenuRequested, this, &DialogMDataBase::TreeMenu);

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
    connect(ui->treeWidget, &QTreeWidget::customContextMenuRequested, this, &DialogMDataBase::TreeMenu);
    connect(ui->treeWidget, &QTreeWidget::itemActivated, this, &DialogMDataBase::ShowDescription);

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
                    ShowDescription(item, model.column());
                    break;
                }
                case Qt::Key_Down:
                {
                    const QModelIndex model = ui->treeWidget->indexBelow(ui->treeWidget->currentIndex());
                    QTreeWidgetItem *item = ui->treeWidget->itemBelow(ui->treeWidget->currentItem());
                    ShowDescription(item, model.column());
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
        QTreeWidgetItem *parent = item->parent();
        if (parent == nullptr)
        {
            return;
        }
        for (int j = 0; j < parent->childCount(); ++j)
        {
            if (j != parent->indexOfChild(item)
                    && item->checkState(0) != parent->child(j)->checkState(0))
            {
                diff = true;
            }
        }
        if (diff)
        {
            parent->setCheckState(0, Qt::PartiallyChecked);
        }
        else
        {
            parent->setCheckState(0, item->checkState(0));
        }

        UpdateChecks(parent, -1);
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
    const VTranslateVars *trv = qApp->TrVars();
    const QString number = trv->MNumber(name);
    const QString iconPath = QString("://diagrams/%1.png").arg(MapDiagrams(number));

    QString text = QString("<p align=\"center\" style=\"font-variant: normal; font-style: normal; font-weight: "
                           "normal\"> <img src=\"%1\" align=\"center\">"
                           "<br clear=\"left\"><b>%2</b>. <i>%3</i></p>"
                           "<p align=\"left\" style=\"font-variant: normal; font-style: normal; font-weight: normal\">"
                           "%5</p>")
            .arg(iconPath)
            .arg(number)
            .arg(trv->GuiText(name))
            .arg(trv->Description(name));

    ui->textEdit->setHtml(text);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogMDataBase::TreeMenu(const QPoint &pos)
{
    // Because item also will be selected need to show description
    const QModelIndex model = ui->treeWidget->currentIndex();
    QTreeWidgetItem *item = ui->treeWidget->currentItem();
    ShowDescription(item, model.column());

    QAction *actionCollapseAll = new QAction(tr("Collapse All"), this);
    connect(actionCollapseAll, &QAction::triggered, ui->treeWidget, &QTreeWidget::collapseAll);

    QAction *actionExpandeAll = new QAction(tr("Expand All"), this);
    connect(actionExpandeAll, &QAction::triggered, ui->treeWidget, &QTreeWidget::expandAll);

    QMenu menu(this);
    menu.addAction(actionCollapseAll);
    menu.addAction(actionExpandeAll);
    menu.exec(ui->treeWidget->mapToGlobal(pos));
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
