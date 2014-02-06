/************************************************************************
 **
 **  @file   dialogtool.cpp
 **  @author Roman Telezhinsky <dismine@gmail.com>
 **  @date   November 15, 2013
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentine project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2013 Valentina project
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

#include "dialogtool.h"
#include "../container/calculator.h"
#include "../geometry/vgobject.h"
#include "../tools/vabstracttool.h"

#include <QtWidgets>

DialogTool::DialogTool(const VContainer *data, QWidget *parent)
    :QDialog(parent), data(data), isInitialized(false), flagName(true), flagFormula(true), timerFormula(0), bOk(0),
      spinBoxAngle(0), lineEditFormula(0), listWidget(0), labelResultCalculation(0), labelDescription(0),
      labelEditNamePoint(0), labelEditFormula(0), radioButtonSizeGrowth(0), radioButtonStandardTable(0),
      radioButtonIncrements(0), radioButtonLengthLine(0), radioButtonLengthArc(0), radioButtonLengthCurve(0),
      lineStyles(QStringList())
{
    Q_CHECK_PTR(data);
    timerFormula = new QTimer(this);
    connect(timerFormula, &QTimer::timeout, this, &DialogTool::EvalFormula);
    //Keep synchronize with VAbstractTool styles list!!!
    lineStyles<<tr("No line")<<tr("Line")<<tr("Dash Line")<<tr("Dot Line")<<tr("Dash Dot Line")
             <<tr("Dash Dot Dot Line");
    Qt::WindowFlags flags = windowFlags();
    setWindowFlags(flags | Qt::WindowStaysOnTopHint);
}

void DialogTool::closeEvent(QCloseEvent *event)
{
    DialogClosed(QDialog::Rejected);
    event->accept();
}

void DialogTool::showEvent(QShowEvent *event)
{
    QDialog::showEvent( event );
    if ( event->spontaneous() )
    {
        return;
    }
    if (isInitialized)
    {
        return;
    }
    isInitialized = true;//first show windows are held
}

void DialogTool::FillComboBoxPoints(QComboBox *box, const qint64 &id) const
{
    Q_CHECK_PTR(box);
    const QHash<qint64, VGObject*> *objs = data->DataGObjects();
    QHashIterator<qint64, VGObject*> i(*objs);
    QMap<QString, qint64> list;
    while (i.hasNext())
    {
        i.next();
        if (i.key() != id)
        {
            VGObject *obj = i.value();
            if (obj->getType() == GObject::Point && obj->getMode() == Draw::Calculation)
            {
                const VPointF *point = data->GeometricObject<const VPointF *>(i.key());
                list[point->name()] = i.key();
            }
        }
    }
    FillList(box, list);
}

void DialogTool::FillComboBoxArcs(QComboBox *box, const qint64 &id, ComboMode::ComboBoxCutArc cut) const
{
    Q_CHECK_PTR(box);
    const QHash<qint64, VGObject *> *objs = data->DataGObjects();
    QHashIterator<qint64, VGObject*> i(*objs);
    QMap<QString, qint64> list;
    while (i.hasNext())
    {
        i.next();
        if (cut == ComboMode::CutArc)
        {
            if (i.key() != id + 1 && i.key() != id + 2)
            {
                VGObject *obj = i.value();
                if (obj->getType() == GObject::Arc && obj->getMode() == Draw::Calculation)
                {
                    const VArc *arc = data->GeometricObject<const VArc *>(i.key());
                    list[arc->name()] = i.key();
                }
            }
        }
        else
        {
            if (i.key() != id)
            {
                VGObject *obj = i.value();
                if (obj->getType() == GObject::Arc && obj->getMode() == Draw::Calculation)
                {
                    const VArc *arc = data->GeometricObject<const VArc *>(i.key());
                    list[arc->name()] = i.key();
                }
            }
        }
    }
    FillList(box, list);
}

void DialogTool::FillComboBoxSplines(QComboBox *box, const qint64 &id, ComboMode::ComboBoxCutSpline cut) const
{
    Q_CHECK_PTR(box);
    const QHash<qint64, VGObject *> *objs = data->DataGObjects();
    QHashIterator<qint64, VGObject*> i(*objs);
    QMap<QString, qint64> list;
    while (i.hasNext())
    {
        i.next();
        if (cut == ComboMode::CutSpline)
        {
            if (i.key() != id + 1 && i.key() != id + 2)
            {
                VGObject *obj = i.value();
                if (obj->getType() == GObject::Spline && obj->getMode() == Draw::Calculation)
                {
                    const VSpline *spl = data->GeometricObject<const VSpline *>(i.key());
                    list[spl->name()] = i.key();
                }
            }
        }
        else
        {
            if (i.key() != id)
            {
                VGObject *obj = i.value();
                if (obj->getType() == GObject::Spline && obj->getMode() == Draw::Calculation)
                {
                    const VSpline *spl = data->GeometricObject<const VSpline *>(i.key());
                    list[spl->name()] = i.key();
                }
            }
        }
    }
    FillList(box, list);
}

void DialogTool::FillComboBoxSplinesPath(QComboBox *box, const qint64 &id, ComboMode::ComboBoxCutSpline cut) const
{
    Q_CHECK_PTR(box);
    const QHash<qint64, VGObject *> *objs = data->DataGObjects();
    QHashIterator<qint64, VGObject *> i(*objs);
    QMap<QString, qint64> list;
    while (i.hasNext())
    {
        i.next();
        if (cut == ComboMode::CutSpline)
        {
            if (i.key() != id + 1 && i.key() != id + 2)
            {
                VGObject *obj = i.value();
                if (obj->getType() == GObject::SplinePath && obj->getMode() == Draw::Calculation)
                {
                    const VSplinePath *splPath = data->GeometricObject<const VSplinePath *>(i.key());
                    list[splPath->name()] = i.key();
                }
            }
        }
        else
        {
            if (i.key() != id)
            {
                VGObject *obj = i.value();
                if (obj->getType() == GObject::SplinePath && obj->getMode() == Draw::Calculation)
                {
                    const VSplinePath *splPath = data->GeometricObject<const VSplinePath *>(i.key());
                    list[splPath->name()] = i.key();
                }
            }
        }
    }
    FillList(box, list);
}

void DialogTool::FillComboBoxTypeLine(QComboBox *box) const
{
    Q_CHECK_PTR(box);
    box->addItems(lineStyles);
    box->setCurrentIndex(1);
}

QString DialogTool::GetTypeLine(const QComboBox *box) const
{
    switch(lineStyles.indexOf(box->currentText()))
    {
        case 0:
            return VAbstractTool::TypeLineNone;
            break;
        case 1:
            return VAbstractTool::TypeLineLine;
            break;
        case 2:
            return VAbstractTool::TypeLineDashLine;
            break;
        case 3:
            return VAbstractTool::TypeLineDotLine;
            break;
        case 4:
            return VAbstractTool::TypeLineDashDotLine;
            break;
        case 5:
            return VAbstractTool::TypeLineDashDotDotLine;
            break;
        default:
            return VAbstractTool::TypeLineLine;
            break;
    }
}

void DialogTool::SetupTypeLine(QComboBox *box, const QString &value)
{
    QStringList styles = VAbstractTool::Styles();
    qint32 index = box->findText(lineStyles.at(styles.indexOf(value)));
    if (index != -1)
    {
        box->setCurrentIndex(index);
    }
}

void DialogTool::ChangeCurrentText(QComboBox *box, const QString &value)
{
    qint32 index = box->findText(value);
    if (index != -1)
    {
        box->setCurrentIndex(index);
    }
    else
    {
        qWarning()<<tr("Can't find object by name")<<value;
    }
}

void DialogTool::ChangeCurrentData(QComboBox *box, const qint64 &value) const
{
    qint32 index = box->findData(value);
    if (index != -1)
    {
        box->setCurrentIndex(index);
    }
}

void DialogTool::PutValHere(QLineEdit *lineEdit, QListWidget *listWidget)
{
    Q_CHECK_PTR(lineEdit);
    Q_CHECK_PTR(listWidget);
    QListWidgetItem *item = listWidget->currentItem();
    int pos = lineEdit->cursorPosition();
    lineEdit->setText(lineEdit->text().insert(lineEdit->cursorPosition(), item->text()));
    lineEdit->setFocus();
    lineEdit->setCursorPosition(pos + item->text().size());
}

void DialogTool::ValFormulaChanged(bool &flag, QLineEdit *edit, QTimer *timer)
{
    Q_CHECK_PTR(edit);
    Q_CHECK_PTR(timer);
    Q_CHECK_PTR(labelEditFormula);
    if (edit->text().isEmpty())
    {
        flag = false;
        CheckState();
        QPalette palette = labelEditFormula->palette();
        palette.setColor(labelEditFormula->foregroundRole(), Qt::red);
        labelEditFormula->setPalette(palette);
        return;
    }
    timer->start(1000);
}

void DialogTool::Eval(QLineEdit *edit, bool &flag, QTimer *timer, QLabel *label)
{
    Q_CHECK_PTR(edit);
    Q_CHECK_PTR(timer);
    Q_CHECK_PTR(label);
    Q_CHECK_PTR(labelEditFormula);
    QPalette palette = labelEditFormula->palette();
    if (edit->text().isEmpty())
    {
        flag = false;
        palette.setColor(labelEditFormula->foregroundRole(), Qt::red);
    }
    else
    {
        Calculator cal(data);
        QString errorMsg;
        qreal result = cal.eval(edit->text(), &errorMsg);
        if (errorMsg.isEmpty() == false)
        {
            label->setText(tr("Error"));
            flag = false;
            palette.setColor(labelEditFormula->foregroundRole(), Qt::red);
        }
        else
        {
            label->setText(QString().setNum(result));
            flag = true;
            palette.setColor(labelEditFormula->foregroundRole(), QColor(76, 76, 76));
        }
    }
    CheckState();
    timer->stop();
    labelEditFormula->setPalette(palette);
}

void DialogTool::setCurrentPointId(QComboBox *box, qint64 &pointId, const qint64 &value, const qint64 &id) const
{
    Q_CHECK_PTR(box);
    FillComboBoxPoints(box, id);
    pointId = value;
    ChangeCurrentData(box, value);
}

void DialogTool::setCurrentSplineId(QComboBox *box, qint64 &splineId, const qint64 &value, const qint64 &id,
                                    ComboMode::ComboBoxCutSpline cut) const
{
    Q_CHECK_PTR(box);
    FillComboBoxSplines(box, id, cut);
    splineId = value;
    ChangeCurrentData(box, value);
}

void DialogTool::setCurrentArcId(QComboBox *box, qint64 &arcId, const qint64 &value, const qint64 &id,
                                 ComboMode::ComboBoxCutArc cut) const
{
    Q_CHECK_PTR(box);
    FillComboBoxArcs(box, id, cut);
    arcId = value;
    ChangeCurrentData(box, value);
}

void DialogTool::setCurrentSplinePathId(QComboBox *box, qint64 &splinePathId, const qint64 &value,
                                        const qint64 &id, ComboMode::ComboBoxCutSpline cut) const
{
    Q_CHECK_PTR(box);
    FillComboBoxSplinesPath(box, id, cut);
    splinePathId = value;
    ChangeCurrentData(box, value);
}

qint64 DialogTool::getCurrentObjectId(QComboBox *box) const
{
    Q_CHECK_PTR(box);
    qint32 index = box->currentIndex();
    Q_ASSERT(index != -1);
    if (index != -1)
    {
        return qvariant_cast<qint64>(box->itemData(index));
    }
    else
    {
        return -1;
    }
}

void DialogTool::FillList(QComboBox *box, const QMap<QString, qint64> &list) const
{
    Q_CHECK_PTR(box);
    box->clear();

    QMapIterator<QString, qint64> iter(list);
    while (iter.hasNext())
    {
        iter.next();
        box->addItem(iter.key(), iter.value());
    }
}

void DialogTool::CheckState()
{
    Q_CHECK_PTR(bOk);
    bOk->setEnabled(flagFormula && flagName);
}

void DialogTool::ChoosedObject(qint64 id, const Scene::Scenes &type)
{
    Q_UNUSED(id);
    Q_UNUSED(type);
}

void DialogTool::NamePointChanged()
{
    Q_CHECK_PTR(labelEditNamePoint);
    QLineEdit* edit = qobject_cast<QLineEdit*>(sender());
    if (edit)
    {
        QString name = edit->text();
        if (name.isEmpty() || name.contains(" "))
        {
            flagName = false;
            QPalette palette = labelEditNamePoint->palette();
            palette.setColor(labelEditNamePoint->foregroundRole(), Qt::red);
            labelEditNamePoint->setPalette(palette);
        }
        else
        {
            flagName = true;
            QPalette palette = labelEditNamePoint->palette();
            palette.setColor(labelEditNamePoint->foregroundRole(), QColor(76, 76, 76));
            labelEditNamePoint->setPalette(palette);
        }
    }
    CheckState();
}

void DialogTool::DialogAccepted()
{
    emit DialogClosed(QDialog::Accepted);
}

void DialogTool::DialogRejected()
{
    emit DialogClosed(QDialog::Rejected);
}

void DialogTool::FormulaChanged()
{
    QLineEdit* edit = qobject_cast<QLineEdit*>(sender());
    if (edit)
    {
        ValFormulaChanged(flagFormula, edit, timerFormula);
    }
}

void DialogTool::ArrowUp()
{
    Q_CHECK_PTR(spinBoxAngle);
    spinBoxAngle->setValue(90);
}

void DialogTool::ArrowDown()
{
    Q_CHECK_PTR(spinBoxAngle);
    spinBoxAngle->setValue(270);
}

void DialogTool::ArrowLeft()
{
    Q_CHECK_PTR(spinBoxAngle);
    spinBoxAngle->setValue(180);
}

void DialogTool::ArrowRight()
{
    Q_CHECK_PTR(spinBoxAngle);
    spinBoxAngle->setValue(0);
}

void DialogTool::ArrowLeftUp()
{
    Q_CHECK_PTR(spinBoxAngle);
    spinBoxAngle->setValue(135);
}

void DialogTool::ArrowLeftDown()
{
    Q_CHECK_PTR(spinBoxAngle);
    spinBoxAngle->setValue(225);
}

void DialogTool::ArrowRightUp()
{
    Q_CHECK_PTR(spinBoxAngle);
    spinBoxAngle->setValue(45);
}

void DialogTool::ArrowRightDown()
{
    Q_CHECK_PTR(spinBoxAngle);
    spinBoxAngle->setValue(315);
}

void DialogTool::EvalFormula()
{
    Q_CHECK_PTR(lineEditFormula);
    Q_CHECK_PTR(labelResultCalculation);
    Eval(lineEditFormula, flagFormula, timerFormula, labelResultCalculation);
}

void DialogTool::SizeGrowth()
{
    ShowVariable(data->DataBase());
}

void DialogTool::StandardTable()
{
    ShowVariable(data->DataStandardTable());
}

void DialogTool::LengthLines()
{
    ShowVariable(data->DataLengthLines());
}

void DialogTool::LengthArcs()
{
    ShowVariable(data->DataLengthArcs());
}

void DialogTool::LengthCurves()
{
    ShowVariable(data->DataLengthSplines());
}

void DialogTool::Increments()
{
    ShowVariable(data->DataIncrementTable());
}

void DialogTool::PutHere()
{
    PutValHere(lineEditFormula, listWidget);
}

void DialogTool::PutVal(QListWidgetItem *item)
{
    Q_CHECK_PTR(lineEditFormula);
    Q_CHECK_PTR(item);
    int pos = lineEditFormula->cursorPosition();
    lineEditFormula->setText(lineEditFormula->text().insert(lineEditFormula->cursorPosition(),
                                                            item->text()));
    lineEditFormula->setFocus();
    lineEditFormula->setCursorPosition(pos + item->text().size());
}

void DialogTool::ValChenged(int row)
{
    Q_CHECK_PTR(listWidget);
    Q_CHECK_PTR(labelDescription);
    Q_CHECK_PTR(radioButtonSizeGrowth);
    Q_CHECK_PTR(radioButtonStandardTable);
    Q_CHECK_PTR(radioButtonIncrements);
    Q_CHECK_PTR(radioButtonLengthLine);
    Q_CHECK_PTR(radioButtonLengthArc);
    Q_CHECK_PTR(radioButtonLengthCurve);
    if (listWidget->count() == 0)
    {
        return;
    }
    QListWidgetItem *item = listWidget->item( row );
    if (radioButtonSizeGrowth->isChecked())
    {
        if (item->text()=="Р")
        {
            QString desc = QString("%1(%2) - %3").arg(item->text()).arg(data->growth()).arg(tr("Height"));
            labelDescription->setText(desc);
        }
        if (item->text()=="Сг")
        {
            QString desc = QString("%1(%2) - %3").arg(item->text()).arg(data->size()).arg(tr("Size"));
            labelDescription->setText(desc);
        }
        return;
    }
    if (radioButtonStandardTable->isChecked())
    {
        VStandardTableRow stable = data->GetStandardTableCell(item->text());
        QString desc = QString("%1(%2) - %3").arg(item->text()).arg(data->GetValueStandardTableCell(item->text()))
                .arg(stable.GetDescription());
        labelDescription->setText(desc);
        return;
    }
    if (radioButtonIncrements->isChecked())
    {
        VIncrementTableRow itable = data->GetIncrementTableRow(item->text());
        QString desc = QString("%1(%2) - %3").arg(item->text()).arg(data->GetValueIncrementTableRow(item->text()))
                .arg(itable.getDescription());
        labelDescription->setText(desc);
        return;
    }
    if (radioButtonLengthLine->isChecked())
    {
        QString desc = QString("%1(%2) - %3").arg(item->text()).arg(data->GetLine(item->text()))
                .arg(tr("Line length"));
        labelDescription->setText(desc);
        return;
    }
    if (radioButtonLengthArc->isChecked())
    {
        QString desc = QString("%1(%2) - %3").arg(item->text()).arg(data->GetLengthArc(item->text()))
                .arg(tr("Arc length"));
        labelDescription->setText(desc);
        return;
    }
    if (radioButtonLengthCurve->isChecked())
    {
        QString desc = QString("%1(%2) - %3").arg(item->text()).arg(data->GetLengthSpline(item->text()))
                .arg(tr("Curve length"));
        labelDescription->setText(desc);
        return;
    }
}

void DialogTool::UpdateList()
{
    Q_CHECK_PTR(radioButtonSizeGrowth);
    Q_CHECK_PTR(radioButtonStandardTable);
    Q_CHECK_PTR(radioButtonIncrements);
    Q_CHECK_PTR(radioButtonLengthLine);
    Q_CHECK_PTR(radioButtonLengthArc);
    Q_CHECK_PTR(radioButtonLengthCurve);

    if (radioButtonSizeGrowth->isChecked())
    {
        ShowVariable(data->DataBase());
    }
    if (radioButtonStandardTable->isChecked())
    {
        ShowVariable(data->DataStandardTable());
    }
    if (radioButtonIncrements->isChecked())
    {
        ShowVariable(data->DataIncrementTable());
    }
    if (radioButtonLengthLine->isChecked())
    {
        ShowVariable(data->DataLengthLines());
    }
    if (radioButtonLengthArc->isChecked())
    {
        ShowVariable(data->DataLengthArcs());
    }
    if (radioButtonLengthCurve->isChecked())
    {
        ShowVariable(data->DataLengthSplines());
    }
}

template <class key, class val>
void DialogTool::ShowVariable(const QHash<key, val> *var)
{
    Q_CHECK_PTR(listWidget);
    disconnect(listWidget, &QListWidget::currentRowChanged, this, &DialogTool::ValChenged);
    listWidget->clear();

    QHashIterator<key, val> i(*var);
    QMap<key, val> map;
    while (i.hasNext())
    {
        i.next();
        map.insert(i.key(), i.value());
    }

    QMapIterator<key, val> iMap(map);
    while (iMap.hasNext())
    {
        iMap.next();
        QListWidgetItem *item = new QListWidgetItem(iMap.key());
        item->setFont(QFont("Times", 12, QFont::Bold));
        listWidget->addItem(item);
    }
    connect(listWidget, &QListWidget::currentRowChanged, this, &DialogTool::ValChenged);
    listWidget->setCurrentRow (0);
}
