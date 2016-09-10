/************************************************************************
 **
 **  @file   dialogdetail.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   November 15, 2013
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

#include "dialogdetail.h"

#include <QBuffer>
#include <QByteArray>
#include <QCheckBox>
#include <QComboBox>
#include <QDialogButtonBox>
#include <QDoubleSpinBox>
#include <QFlags>
#include <QFont>
#include <QIcon>
#include <QLabel>
#include <QLatin1String>
#include <QLineEdit>
#include <QList>
#include <QListWidget>
#include <QListWidgetItem>
#include <QMessageLogger>
#include <QPixmap>
#include <QPointF>
#include <QPushButton>
#include <QSharedPointer>
#include <QSize>
#include <QSpinBox>
#include <QTabWidget>
#include <QToolButton>
#include <QVariant>
#include <QVector>
#include <Qt>
#include <QtDebug>
#include <new>

#include "../ifc/xml/vdomdocument.h"
#include "../vpatterndb/vcontainer.h"
#include "../vpatterndb/calculator.h"
#include "../vgeometry/vgobject.h"
#include "../vmisc/vabstractapplication.h"
#include "dialogtool.h"
#include "vnodedetail.h"
#include "../support/dialogeditwrongformula.h"

class QPointF;
class QWidget;

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief DialogDetail create dialog
 * @param data container with data
 * @param parent parent widget
 */
DialogDetail::DialogDetail(const VContainer *data, const quint32 &toolId, QWidget *parent)
    :DialogTool(data, toolId, parent), ui(), detail(VDetail()), supplement(true), closed(true), flagWidth(true),
      m_bAddMode(true), m_qslMaterials(), m_qslPlacements(), m_conMCP(), m_oldData(), m_oldGeom(), m_oldGrainline()
{
    ui.setupUi(this);

#if QT_VERSION >= QT_VERSION_CHECK(5, 2, 0)
    ui.lineEditName->setClearButtonEnabled(true);
    ui.lineEditLetter->setClearButtonEnabled(true);
#endif

    ui.checkBoxForbidFlipping->setChecked(qApp->Settings()->GetForbidWorkpieceFlipping());

    labelEditNamePoint = ui.labelEditName;
    ui.labelUnit->setText( VDomDocument::UnitsToStr(qApp->patternUnit(), true));
    ui.labelUnitX->setText(VDomDocument::UnitsToStr(qApp->patternUnit(), true));
    ui.labelUnitY->setText(VDomDocument::UnitsToStr(qApp->patternUnit(), true));

    if(qApp->patternUnit() == Unit::Inch)
    {
        ui.doubleSpinBoxSeams->setDecimals(5);
    }
    // Default value for seam allowence is 1 cm. But pattern have different units, so just set 1 in dialog not enough.
    ui.doubleSpinBoxSeams->setValue(UnitConvertor(1, Unit::Cm, qApp->patternUnit()));

    bOk = ui.buttonBox->button(QDialogButtonBox::Ok);
    SCASSERT(bOk != nullptr);
    connect(bOk, &QPushButton::clicked, this, &DialogTool::DialogAccepted);
    QPushButton *bCancel = ui.buttonBox->button(QDialogButtonBox::Cancel);
    SCASSERT(bCancel != nullptr);
    connect(bCancel, &QPushButton::clicked, this, &DialogTool::DialogRejected);

    flagName = true;//We have default name of detail.
    ChangeColor(labelEditNamePoint, okColor);
    CheckState();

    connect(ui.listWidget, &QListWidget::currentRowChanged, this, &DialogDetail::ObjectChanged);
    connect(ui.doubleSpinBoxBiasX,  static_cast<void (QDoubleSpinBox::*)(qreal)>(&QDoubleSpinBox::valueChanged),
            this, &DialogDetail::BiasXChanged);
    connect(ui.doubleSpinBoxBiasY,  static_cast<void (QDoubleSpinBox::*)(qreal)>(&QDoubleSpinBox::valueChanged),
            this, &DialogDetail::BiasYChanged);
    connect(ui.doubleSpinBoxSeams,  static_cast<void (QDoubleSpinBox::*)(qreal)>(&QDoubleSpinBox::valueChanged),
            this, &DialogDetail::AlowenceChanged);
    connect(ui.checkBoxSeams, &QCheckBox::clicked, this, &DialogDetail::ClickedSeams);
    connect(ui.checkBoxClosed, &QCheckBox::clicked, this, &DialogDetail::ClickedClosed);
    connect(ui.checkBoxReverse, &QCheckBox::clicked, this, &DialogDetail::ClickedReverse);
    connect(ui.lineEditName, &QLineEdit::textChanged, this, &DialogDetail::NameDetailChanged);

    connect(ui.toolButtonDelete, &QToolButton::clicked, this, &DialogDetail::DeleteItem);
    connect(ui.toolButtonUp, &QToolButton::clicked, this, &DialogDetail::ScrollUp);
    connect(ui.toolButtonDown, &QToolButton::clicked, this, &DialogDetail::ScrollDown);

    m_qslMaterials << QApplication::translate("Detail", "Fabric", 0)
                   << QApplication::translate("Detail", "Lining", 0)
                   << QApplication::translate("Detail", "Interfacing", 0)
                   << QApplication::translate("Detail", "Interlining", 0);

    for (int i = 0; i < m_qslMaterials.count(); ++i)
    {
        ui.comboBoxMaterial->addItem(m_qslMaterials[i], i);
    }

    QStringList qsl = qApp->Settings()->GetUserDefinedMaterials();
    for (int i = 0; i < qsl.count(); ++i)
    {
        ui.comboBoxMaterial->addItem(qsl[i], int(MaterialType::mtUserDefined));
    }

    m_qslPlacements << tr("None") << tr("Cut on fold");
    ui.comboBoxPlacement->addItems(m_qslPlacements);
    ui.pushButtonRot->setIcon(QIcon("://icon/16x16/fx.png"));
    ui.pushButtonLen->setIcon(QIcon("://icon/16x16/fx.png"));

    connect(ui.pushButtonAdd, &QPushButton::clicked, this, &DialogDetail::AddUpdate);
    connect(ui.pushButtonCancel, &QPushButton::clicked, this, &DialogDetail::Cancel);
    connect(ui.pushButtonRemove, &QPushButton::clicked, this, &DialogDetail::Remove);
    connect(ui.listWidgetMCP, &QListWidget::itemClicked, this, &DialogDetail::SetEditMode);
    connect(ui.comboBoxMaterial, &QComboBox::currentTextChanged, this, &DialogDetail::MaterialChanged);
    connect(ui.checkBoxGrainline, &QCheckBox::toggled, this, &DialogDetail::EnableGrainlineRotation);
    connect(ui.pushButtonRot, &QPushButton::clicked, this, &DialogDetail::EditFormula);
    connect(ui.pushButtonLen, &QPushButton::clicked, this, &DialogDetail::EditFormula);
    connect(ui.lineEditLenFormula, &QLineEdit::textChanged, this, &DialogDetail::UpdateValue);
    connect(ui.lineEditRotFormula, &QLineEdit::textChanged, this, &DialogDetail::UpdateValue);

    SetAddMode();
    EnableGrainlineRotation();

    ui.tabWidget->setCurrentIndex(0);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief ChoosedObject gets id and type of selected object. Save right data and ignore wrong.
 * @param id id of objects (points, arcs, splines, spline paths)
 * @param type type of object
 */
void DialogDetail::ChosenObject(quint32 id, const SceneObject &type)
{
    if (type != SceneObject::Line && type != SceneObject::Detail)
    {
        switch (type)
        {
            case (SceneObject::Arc):
                NewItem(id, Tool::NodeArc, NodeDetail::Contour);
                break;
            case (SceneObject::Point):
                NewItem(id, Tool::NodePoint, NodeDetail::Contour);
                break;
            case (SceneObject::Spline):
                NewItem(id, Tool::NodeSpline, NodeDetail::Contour);
                break;
            case (SceneObject::SplinePath):
                NewItem(id, Tool::NodeSplinePath, NodeDetail::Contour);
                break;
            case (SceneObject::Line):
            case (SceneObject::Detail):
            case (SceneObject::Unknown):
            default:
                qDebug()<<tr("Got wrong scene object. Ignore.");
                break;
        }

        if (ui.listWidget->count() > 0)
        {
            EnableObjectGUI(true);
        }

        ValidObjects(DetailIsValid());
        // Fix issue #526. Dialog Detail is not on top after selection second object on Mac.
        setWindowFlags(windowFlags() | Qt::WindowStaysOnTopHint);
        this->show();
    }
}

//---------------------------------------------------------------------------------------------------------------------
void DialogDetail::SaveData()
{
    detail.Clear();
    detail = CreateDetail();
}

//---------------------------------------------------------------------------------------------------------------------
void DialogDetail::CheckState()
{
    SCASSERT(bOk != nullptr);
    bOk->setEnabled(flagFormula && flagName && flagError && flagWidth);
    // In case dialog hasn't apply button
    if ( bApply != nullptr)
    {
        bApply->setEnabled(bOk->isEnabled());
    }
}

//---------------------------------------------------------------------------------------------------------------------
void DialogDetail::UpdateList()
{
    ui.listWidgetMCP->clear();
    for (int i = 0; i < m_conMCP.count(); ++i)
    {
        MaterialCutPlacement mcp = m_conMCP.at(i);
        QString qsText = tr("Cut %1 of %2%3").arg(mcp.m_iCutNumber);
        if (mcp.m_eMaterial < MaterialType::mtUserDefined)
        {
            qsText = qsText.arg(m_qslMaterials[int(mcp.m_eMaterial)]);
        }
        else
        {
            qsText = qsText.arg(mcp.m_qsMaterialUserDef);
        }
        if (mcp.m_ePlacement == PlacementType::ptCutOnFold)
        {
            qsText = qsText.arg(QLatin1String(" ") + tr("on Fold"));
        }
        else
        {
            qsText = qsText.arg("");
        }

        ui.listWidgetMCP->addItem(qsText);
    }
}

//---------------------------------------------------------------------------------------------------------------------
void DialogDetail::AddUpdate()
{
    MaterialCutPlacement mcp;
    QStringList qslUserMaterials = qApp->Settings()->GetUserDefinedMaterials();

#if QT_VERSION < QT_VERSION_CHECK(5, 2, 0)
    int i = ui.comboBoxMaterial->itemData(ui.comboBoxMaterial->currentIndex()).toInt();
#else
    int i = ui.comboBoxMaterial->currentData().toInt();
#endif
    QString qsMat = ui.comboBoxMaterial->currentText();
    if (i < m_qslMaterials.count() && qsMat == m_qslMaterials[i])
    {
        mcp.m_eMaterial = MaterialType(i);
        mcp.m_qsMaterialUserDef.clear();
    }
    else
    {
        mcp.m_eMaterial = MaterialType::mtUserDefined;
        mcp.m_qsMaterialUserDef = qsMat;
        // check if we have new user defined material
        bool bFound = false;
        for (int i = 0; i < qslUserMaterials.count() && bFound == false; ++i)
        {
            if (mcp.m_qsMaterialUserDef == qslUserMaterials[i])
            {
                bFound = true;
            }
        }
        if (bFound == false)
        {
            qApp->Settings()->AddUserDefinedMaterial(mcp.m_qsMaterialUserDef);
            qApp->Settings()->sync();
            ui.comboBoxMaterial->addItem(mcp.m_qsMaterialUserDef, int(MaterialType::mtUserDefined));
        }
    }

    mcp.m_iCutNumber = ui.spinBoxCutNumber->value();
    mcp.m_ePlacement = PlacementType(ui.comboBoxPlacement->currentIndex());

    if (m_bAddMode == true)
    {
        m_conMCP << mcp;
    }
    else
    {
        int iR = ui.listWidgetMCP->currentRow();
        SCASSERT(iR >= 0);
        m_conMCP[iR] = mcp;
        SetAddMode();
    }
    UpdateList();
    ClearFields();
}

//---------------------------------------------------------------------------------------------------------------------
void DialogDetail::Cancel()
{
    ClearFields();
    SetAddMode();
}

//---------------------------------------------------------------------------------------------------------------------
void DialogDetail::Remove()
{
    int iR = ui.listWidgetMCP->currentRow();
    SCASSERT(iR >= 0);
    m_conMCP.removeAt(iR);
    UpdateList();
    ClearFields();
    SetAddMode();
}

//---------------------------------------------------------------------------------------------------------------------
void DialogDetail::NameDetailChanged()
{
    SCASSERT(labelEditNamePoint != nullptr);
    QLineEdit* edit = qobject_cast<QLineEdit*>(sender());
    if (edit)
    {
        if (edit->text().isEmpty())
        {
            flagName = false;
            ChangeColor(labelEditNamePoint, Qt::red);
            QIcon icon(":/icons/win.icon.theme/16x16/status/dialog-warning.png");
            ui.tabWidget->setTabIcon(1, icon);
        }
        else
        {
            flagName = true;
            ChangeColor(labelEditNamePoint, okColor);
            QIcon icon;
            ui.tabWidget->setTabIcon(1, icon);
        }
    }
    CheckState();
}

//---------------------------------------------------------------------------------------------------------------------
void DialogDetail::MaterialChanged()
{
    ui.pushButtonAdd->setEnabled(ui.comboBoxMaterial->currentText().isEmpty() == false);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief NewItem add new object (point, arc, spline or spline path) to list
 * @param id id of object
 * @param typeTool type of tool
 * @param typeNode type of node in detail
 * @param mx offset respect to x
 * @param my offset respect to y
 * @param reverse reverse list of points
 */
void DialogDetail::NewItem(quint32 id, const Tool &typeTool, const NodeDetail &typeNode,
                           qreal mx, qreal my, bool reverse)
{
    SCASSERT(id > NULL_ID);
    QString name;
    switch (typeTool)
    {
        case (Tool::NodePoint):
        case (Tool::NodeArc):
        case (Tool::NodeSpline):
        case (Tool::NodeSplinePath):
        {
            const QSharedPointer<VGObject> obj = data->GeometricObject<VGObject>(id);
            name = obj->name();
            break;
        }
        default:
            qDebug()<<"Got wrong tools. Ignore.";
            return;
    }

    bool canAddNewPoint = false;

    if(ui.listWidget->count() == 0)
    {
        canAddNewPoint = true;
        ui.toolButtonUp->setEnabled(false);
        ui.toolButtonDown->setEnabled(false);
    }
    else
    {
        if(RowId(ui.listWidget->count()-1) != id)
        {
            canAddNewPoint = true;
        }
        ui.toolButtonUp->setEnabled(true);
        ui.toolButtonDown->setEnabled(true);
    }

    if(canAddNewPoint)
    {
        QListWidgetItem *item = new QListWidgetItem(name);
        item->setFont(QFont("Times", 12, QFont::Bold));
        VNodeDetail node(id, typeTool, typeNode, mx, my, reverse);
        item->setData(Qt::UserRole, QVariant::fromValue(node));
        ui.listWidget->addItem(item);
        ui.listWidget->setCurrentRow(ui.listWidget->count()-1);

        ui.doubleSpinBoxBiasX->blockSignals(true);
        ui.doubleSpinBoxBiasY->blockSignals(true);

        ui.doubleSpinBoxBiasX->setValue(qApp->fromPixel(node.getMx()));
        ui.doubleSpinBoxBiasY->setValue(qApp->fromPixel(node.getMy()));
        if (node.getTypeTool() == Tool::NodePoint)
        {
            ui.checkBoxReverse->setChecked(false);
            ui.checkBoxReverse->setEnabled(false);
        }
        else
        {
            ui.checkBoxReverse->setEnabled(true);
            ui.checkBoxReverse->setChecked(node.getReverse());
        }

        ui.doubleSpinBoxBiasX->blockSignals(false);
        ui.doubleSpinBoxBiasY->blockSignals(false);
    }
}

//---------------------------------------------------------------------------------------------------------------------
VDetail DialogDetail::CreateDetail() const
{
    VDetail detail;
    for (qint32 i = 0; i < ui.listWidget->count(); ++i)
    {
        QListWidgetItem *item = ui.listWidget->item(i);
        detail.append( qvariant_cast<VNodeDetail>(item->data(Qt::UserRole)));
    }
    detail.setWidth(ui.doubleSpinBoxSeams->value());
    detail.setName(ui.lineEditName->text());
    detail.setSeamAllowance(supplement);
    detail.setClosed(closed);
    detail.setForbidFlipping(ui.checkBoxForbidFlipping->isChecked());

    detail.GetPatternPieceData().SetLetter(ui.lineEditLetter->text());

    for (int i = 0; i < m_conMCP.count(); ++i)
    {
        detail.GetPatternPieceData().Append(m_conMCP[i]);
    }

    detail.GetPatternPieceData().SetPos(m_oldData.GetPos());
    detail.GetPatternPieceData().SetLabelWidth(m_oldData.GetLabelWidth());
    detail.GetPatternPieceData().SetLabelHeight(m_oldData.GetLabelHeight());
    detail.GetPatternPieceData().SetFontSize(m_oldData.GetFontSize());
    detail.GetPatternPieceData().SetRotation(m_oldData.GetRotation());
    detail.GetPatternPieceData().SetVisible(ui.checkBoxDetail->isChecked());

    detail.GetPatternInfo() = m_oldGeom;
    detail.GetPatternInfo().SetVisible(ui.checkBoxPattern->isChecked());

    detail.GetGrainlineGeometry() = m_oldGrainline;
    detail.GetGrainlineGeometry().SetVisible(ui.checkBoxGrainline->isChecked());
    if (ui.checkBoxGrainline->isChecked() == true)
    {
        detail.GetGrainlineGeometry().SetRotation(ui.lineEditRotFormula->text());
        detail.GetGrainlineGeometry().SetLength(ui.lineEditLenFormula->text());
    }
    return detail;
}

//---------------------------------------------------------------------------------------------------------------------
void DialogDetail::ValidObjects(bool value)
{
    flagError = value;
    CheckState();
}

//---------------------------------------------------------------------------------------------------------------------
void DialogDetail::EnableObjectGUI(bool value)
{
    ui.toolButtonDelete->setEnabled(value);
    ui.doubleSpinBoxBiasX->setEnabled(value);
    ui.doubleSpinBoxBiasY->setEnabled(value);

    if (value == false)
    {
        ui.checkBoxReverse->setEnabled(value);
    }
}

//---------------------------------------------------------------------------------------------------------------------
quint32 DialogDetail::RowId(int i) const
{
    const QListWidgetItem *rowItem = ui.listWidget->item(i);
    SCASSERT(rowItem != nullptr);
    const VNodeDetail rowNode = qvariant_cast<VNodeDetail>(rowItem->data(Qt::UserRole));
    return rowNode.getId();
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief setDetails set detail
 * @param value detail
 */
void DialogDetail::setDetail(const VDetail &value)
{
    detail = value;
    ui.listWidget->clear();
    for (int i = 0; i < detail.CountNode(); ++i)
    {
        const VNodeDetail &node = detail.at(i);
        NewItem(node.getId(), node.getTypeTool(), node.getTypeNode(), node.getMx(),
                node.getMy(), node.getReverse());
    }
    ui.lineEditName->setText(detail.getName());
    ui.checkBoxSeams->setChecked(detail.getSeamAllowance());
    ui.checkBoxClosed->setChecked(detail.getClosed());
    ui.checkBoxForbidFlipping->setChecked(detail.getForbidFlipping());
    ClickedClosed(detail.getClosed());
    ClickedSeams(detail.getSeamAllowance());
    ui.doubleSpinBoxSeams->setValue(detail.getWidth());
    ui.listWidget->setCurrentRow(0);
    ui.listWidget->setFocus(Qt::OtherFocusReason);
    ui.toolButtonDelete->setEnabled(true);

    ui.lineEditLetter->setText(detail.GetPatternPieceData().GetLetter());
    ui.checkBoxDetail->setChecked(detail.GetPatternPieceData().IsVisible());
    ui.checkBoxPattern->setChecked(detail.GetPatternInfo().IsVisible());

    m_conMCP.clear();
    for (int i = 0; i < detail.GetPatternPieceData().GetMCPCount(); ++i)
    {
        m_conMCP << detail.GetPatternPieceData().GetMCP(i);
    }

    UpdateList();

    ui.checkBoxGrainline->setChecked(detail.GetGrainlineGeometry().IsVisible());
    ui.lineEditRotFormula->setText(detail.GetGrainlineGeometry().GetRotation());
    ui.lineEditLenFormula->setText(detail.GetGrainlineGeometry().GetLength());

    m_oldData = detail.GetPatternPieceData();
    m_oldGeom = detail.GetPatternInfo();
    m_oldGrainline = detail.GetGrainlineGeometry();

    ValidObjects(DetailIsValid());
    EnableGrainlineRotation();
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief BiasXChanged changed value of offset for object respect to x
 * @param d value in mm
 */
void DialogDetail::BiasXChanged(qreal d)
{
    qint32 row = ui.listWidget->currentRow();
    QListWidgetItem *item = ui.listWidget->item( row );
    SCASSERT(item != nullptr);
    VNodeDetail node = qvariant_cast<VNodeDetail>(item->data(Qt::UserRole));
    node.setMx(qApp->toPixel(d));
    item->setData(Qt::UserRole, QVariant::fromValue(node));
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief BiasYChanged changed value of offset for object respect to y
 * @param d value in mm
 */
void DialogDetail::BiasYChanged(qreal d)
{
    qint32 row = ui.listWidget->currentRow();
    QListWidgetItem *item = ui.listWidget->item( row );
    SCASSERT(item != nullptr);
    VNodeDetail node = qvariant_cast<VNodeDetail>(item->data(Qt::UserRole));
    node.setMy(qApp->toPixel(d));
    item->setData(Qt::UserRole, QVariant::fromValue(node));
}

//---------------------------------------------------------------------------------------------------------------------
void DialogDetail::AlowenceChanged(qreal d)
{
    if (ui.doubleSpinBoxSeams->isEnabled())
    {
        if (d <= 0)
        {
            flagWidth = false;
            ChangeColor(ui.labelEditWidth, errorColor);
        }
        else
        {
            flagWidth = true;
            ChangeColor(ui.labelEditWidth, okColor);
        }
        CheckState();
    }
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief ClickedSeams save supplement of seams for detail
 * @param checked 1 - need supplement, 0 - don't need supplement
 */
void DialogDetail::ClickedSeams(bool checked)
{
    supplement = checked;
    ui.checkBoxClosed->setEnabled(checked);
    ui.doubleSpinBoxSeams->setEnabled(checked);

    if (checked && ui.doubleSpinBoxSeams->value() <= 0)
    {
        flagWidth = false;
        ChangeColor(ui.labelEditWidth, errorColor);
    }
    else
    {
        flagWidth = true;
        ChangeColor(ui.labelEditWidth, okColor);
    }
    CheckState();
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief ClickedClosed save closed equdistant or not
 * @param checked 1 - closed, 0 - don't closed
 */
void DialogDetail::ClickedClosed(bool checked)
{
    closed = checked;
}

//---------------------------------------------------------------------------------------------------------------------
void DialogDetail::ClickedReverse(bool checked)
{
    qint32 row = ui.listWidget->currentRow();
    QListWidgetItem *item = ui.listWidget->item( row );
    SCASSERT(item != nullptr);
    VNodeDetail node = qvariant_cast<VNodeDetail>(item->data(Qt::UserRole));
    node.setReverse(checked);
    item->setData(Qt::UserRole, QVariant::fromValue(node));
    ValidObjects(DetailIsValid());
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief ObjectChanged changed new object (point, arc, spline or spline path) form list
 * @param row number of row
 */
void DialogDetail::ObjectChanged(int row)
{
    if (ui.listWidget->count() == 0 || row == -1 || row >= ui.listWidget->count())
    {
        return;
    }
    const QListWidgetItem *item = ui.listWidget->item( row );
    SCASSERT(item != nullptr);
    const VNodeDetail node = qvariant_cast<VNodeDetail>(item->data(Qt::UserRole));
    ui.doubleSpinBoxBiasX->setValue(qApp->fromPixel(node.getMx()));
    ui.doubleSpinBoxBiasY->setValue(qApp->fromPixel(node.getMy()));
    if (node.getTypeTool() == Tool::NodePoint)
    {
        ui.checkBoxReverse->setChecked(false);
        ui.checkBoxReverse->setEnabled(false);
    }
    else
    {
        ui.checkBoxReverse->setEnabled(true);
        ui.checkBoxReverse->setChecked(node.getReverse());
    }
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief DeleteItem delete item from list
 */
void DialogDetail::DeleteItem()
{
    if (ui.listWidget->count() == 1)
    {
        EnableObjectGUI(false);
    }

    delete ui.listWidget->item(ui.listWidget->currentRow());
    ValidObjects(DetailIsValid());

    if(ui.listWidget->count() < 2)
    {
        ui.toolButtonUp->setEnabled(false);
        ui.toolButtonDown->setEnabled(false);
    }
    else
    {
        ui.toolButtonUp->setEnabled(true);
        ui.toolButtonDown->setEnabled(true);
    }
}

//---------------------------------------------------------------------------------------------------------------------
void DialogDetail::ScrollUp()
{
    if (ui.listWidget->count() > 1)
    {
        QListWidgetItem *item = ui.listWidget->takeItem(0);
        ui.listWidget->addItem(item);
        ValidObjects(DetailIsValid());
    }
}

//---------------------------------------------------------------------------------------------------------------------
void DialogDetail::ScrollDown()
{
    if (ui.listWidget->count() > 1)
    {
        QListWidgetItem *item = ui.listWidget->takeItem(ui.listWidget->count()-1);
        ui.listWidget->insertItem(0, item);
        ValidObjects(DetailIsValid());
    }
}

//---------------------------------------------------------------------------------------------------------------------
bool DialogDetail::DetailIsValid() const
{
    const QIcon icon = QIcon::fromTheme("dialog-warning",
                                  QIcon(":/icons/win.icon.theme/16x16/status/dialog-warning.png"));

    const QPixmap pixmap = icon.pixmap(QSize(16, 16));
    QByteArray byteArray;
    QBuffer buffer(&byteArray);
    pixmap.save(&buffer, "PNG");
    QString url = QString("<img src=\"data:image/png;base64,") + byteArray.toBase64() + QLatin1String("\"/> ");

    if(CreateDetail().ContourPoints(data).count() < 3)
    {
        url += tr("You need more points!");
        ui.helpLabel->setText(url);
        return false;
    }
    else
    {
        if(not DetailIsClockwise())
        {
            url += tr("You have to choose points in a clockwise direction!");
            ui.helpLabel->setText(url);
            return false;
        }
        if (FirstPointEqualLast())
        {
            url += tr("First point cannot be equal to the last point!");
            ui.helpLabel->setText(url);
            return false;
        }
        else
        {
            for (int i=0, sz = ui.listWidget->count()-1; i<sz; ++i)
            {
                if (RowId(i) == RowId(i+1))
                {
                    url += tr("You have double points!");
                    ui.helpLabel->setText(url);
                    return false;
                }
            }
        }
    }
    ui.helpLabel->setText(tr("Ready!"));
    return true;
}

//---------------------------------------------------------------------------------------------------------------------
bool DialogDetail::FirstPointEqualLast() const
{
    if (ui.listWidget->count() > 1)
    {
        if (RowId(0) == RowId(ui.listWidget->count()-1))
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    return false;
}

//---------------------------------------------------------------------------------------------------------------------
bool DialogDetail::DetailIsClockwise() const
{
    const QVector<QPointF> points = CreateDetail().ContourPoints(data);

    if(points.count() < 3)
    {
        return false;
    }

    const qreal res = VDetail::SumTrapezoids(points);
    if (res < 0)
    {
        return true;
    }
    return false;
}

//---------------------------------------------------------------------------------------------------------------------
void DialogDetail::ClearFields()
{
    ui.comboBoxMaterial->setCurrentIndex(0);
    ui.spinBoxCutNumber->setValue(0);
    ui.comboBoxPlacement->setCurrentIndex(0);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogDetail::SetValue(QObject *pobjSender)
{
    QLineEdit* pleSender = dynamic_cast<QLineEdit*>(pobjSender);
    SCASSERT(pleSender != 0);

    QString qsFormula = pleSender->text().simplified();
    Calculator cal;
    QString qsVal;
    try
    {
        qsFormula.replace("\n", " ");
        qsFormula = qApp->TrVars()->FormulaFromUser(qsFormula, qApp->Settings()->GetOsSeparator());
        qreal dVal;
        dVal = cal.EvalFormula(data->PlainVariables(), qsFormula);
        if (qIsInf(dVal) == true || qIsNaN(dVal) == true)
        {
            qsVal.clear();
        }
        else
        {
            qsVal.setNum(dVal, 'f', 5);
        }
    }
    catch (...)
    {
        qsVal.clear();
    }

    QLineEdit* pleVal;
    QLabel* plbVal;
    QString qsUnit;
    if (pobjSender == ui.lineEditLenFormula)
    {
        pleVal = ui.lineEditLenValue;
        plbVal = ui.labelLen;
        qsUnit = " " + VDomDocument::UnitsToStr(qApp->patternUnit());
    }
    else if (pobjSender == ui.lineEditRotFormula)
    {
        pleVal = ui.lineEditRotValue;
        plbVal = ui.labelRot;
        QChar ch(0x00b0);
        qsUnit = ch;
    }
    else
    {
        // should not get here
        return;
    }

    if (qsVal.isEmpty() == false)
    {
        qsVal += qsUnit;
    }
    pleVal->setText(qsVal);
    plbVal->setText(qsVal);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogDetail::SetAddMode()
{
    ui.pushButtonAdd->setText(tr("Add"));
    ui.pushButtonCancel->hide();
    ui.pushButtonRemove->hide();
    ui.listWidgetMCP->setCurrentRow(-1);
    m_bAddMode = true;
}

//---------------------------------------------------------------------------------------------------------------------
void DialogDetail::SetEditMode()
{
    int iR = ui.listWidgetMCP->currentRow();
    // this method can be called by clicking on item or by update. In the latter case there is nothing else to do!
    if (iR < 0 || iR >= m_conMCP.count())
    {
        return;
    }

    ui.pushButtonAdd->setText(tr("Update"));
    ui.pushButtonCancel->show();
    ui.pushButtonRemove->show();

    MaterialCutPlacement mcp = m_conMCP.at(iR);
    if (mcp.m_eMaterial == MaterialType::mtUserDefined)
    {
        int iRow = qApp->Settings()->GetUserDefinedMaterials().indexOf(mcp.m_qsMaterialUserDef);
        if (iRow >= 0)
        {
            ui.comboBoxMaterial->setCurrentIndex(iRow + m_qslMaterials.count());
        }
        else
        {
            ui.comboBoxMaterial->setCurrentText(mcp.m_qsMaterialUserDef);
        }
    }
    else
    {
        ui.comboBoxMaterial->setCurrentIndex(int(mcp.m_eMaterial));
    }
    ui.spinBoxCutNumber->setValue(mcp.m_iCutNumber);
    ui.comboBoxPlacement->setCurrentIndex(int(mcp.m_ePlacement));

    m_bAddMode = false;
}

//---------------------------------------------------------------------------------------------------------------------
void DialogDetail::EnableGrainlineRotation()
{
    ui.lineEditRotValue->setEnabled(ui.checkBoxGrainline->isChecked());
    ui.lineEditRotFormula->setEnabled(ui.checkBoxGrainline->isChecked());
    ui.lineEditLenValue->setEnabled(ui.checkBoxGrainline->isChecked());
    ui.lineEditLenFormula->setEnabled(ui.checkBoxGrainline->isChecked());
    ui.pushButtonRot->setEnabled(ui.checkBoxGrainline->isChecked());
    ui.pushButtonLen->setEnabled(ui.checkBoxGrainline->isChecked());
}

//---------------------------------------------------------------------------------------------------------------------
void DialogDetail::EditFormula()
{
    QLineEdit* pleFormula;
    bool bCheckZero;

    if (sender() == ui.pushButtonLen)
    {
        pleFormula = ui.lineEditLenFormula;
        bCheckZero = true;
    }
    else if (sender() == ui.pushButtonRot)
    {
        pleFormula = ui.lineEditRotFormula;
        bCheckZero = false;
    }
    else
    {
        // should not get here!
        return;
    }

    DialogEditWrongFormula dlg(data, NULL_ID, this);
    dlg.SetFormula(pleFormula->text());
    dlg.setCheckZero(bCheckZero);
    if (dlg.exec() == true)
    {
        QString qsFormula = dlg.GetFormula();
        qsFormula.replace("\n", " ");
        pleFormula->setText(qsFormula);
        SetValue(pleFormula);
    }
}

//---------------------------------------------------------------------------------------------------------------------
void DialogDetail::UpdateValue(const QString& qsText)
{
    Q_UNUSED(qsText);
    SetValue(sender());
}
