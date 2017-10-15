/************************************************************************
 **
 **  @file   dialogplacelabel.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   14 10, 2017
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentina project, a pattern making
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
#include "dialogplacelabel.h"
#include "ui_dialogplacelabel.h"

#include "visualization/line/vistoolspecialpoint.h"
#include "../../tools/vabstracttool.h"
#include "../../tools/vtoolseamallowance.h"
#include "../support/dialogeditwrongformula.h"
#include "../vgeometry/vplacelabelitem.h"

#include <QTimer>

//---------------------------------------------------------------------------------------------------------------------
DialogPlaceLabel::DialogPlaceLabel(const VContainer *data, quint32 toolId, QWidget *parent)
    : DialogTool(data, toolId, parent),
      ui(new Ui::DialogPlaceLabel),
      m_showMode(false),
      m_flagPoint(false),
      m_flagWidth(false),
      m_flagHeight(false),
      m_flagAngle(false),
      m_formulaBaseHeightWidth(0),
      m_formulaBaseHeightHeight(0),
      m_formulaBaseHeightAngle(0),
      timerAngle(new QTimer(this)),
      timerWidth(new QTimer(this)),
      timerHeight(new QTimer(this))
{
    ui->setupUi(this);
    InitOkCancel(ui);

    FillComboBoxPoints(ui->comboBoxPoint);
    FillPlaceLabelTypes();

    m_formulaBaseHeightWidth = ui->plainTextEditFormulaWidth->height();
    m_formulaBaseHeightHeight = ui->plainTextEditFormulaHeight->height();
    m_formulaBaseHeightAngle = ui->plainTextEditFormulaAngle->height();

    ui->plainTextEditFormulaWidth->installEventFilter(this);
    ui->plainTextEditFormulaHeight->installEventFilter(this);
    ui->plainTextEditFormulaAngle->installEventFilter(this);

    ui->plainTextEditFormulaWidth->setPlainText(QString::number(UnitConvertor(1, Unit::Cm, qApp->patternUnit())));
    ui->plainTextEditFormulaHeight->setPlainText(QString::number(UnitConvertor(1, Unit::Cm, qApp->patternUnit())));

    flagError = false;
    CheckState();

    connect(ui->toolButtonExprWidth, &QPushButton::clicked, this, &DialogPlaceLabel::FXWidth);
    connect(ui->toolButtonExprHeight, &QPushButton::clicked, this, &DialogPlaceLabel::FXHeight);
    connect(ui->toolButtonExprAngle, &QPushButton::clicked, this, &DialogPlaceLabel::FXAngle);

    connect(ui->plainTextEditFormulaWidth, &QPlainTextEdit::textChanged, this, &DialogPlaceLabel::FormulaWidthChanged);
    connect(ui->plainTextEditFormulaHeight, &QPlainTextEdit::textChanged, this,
            &DialogPlaceLabel::FormulaHeightChanged);
    connect(ui->plainTextEditFormulaAngle, &QPlainTextEdit::textChanged, this, &DialogPlaceLabel::FormulaAngleChanged);

    connect(ui->pushButtonGrowWidth, &QPushButton::clicked, this, &DialogPlaceLabel::DeployFormulaWidthEdit);
    connect(ui->pushButtonGrowHeight, &QPushButton::clicked, this, &DialogPlaceLabel::DeployFormulaHeightEdit);
    connect(ui->pushButtonGrowAngle, &QPushButton::clicked, this, &DialogPlaceLabel::DeployFormulaAngleEdit);

    connect(timerWidth, &QTimer::timeout, this, &DialogPlaceLabel::EvalWidth);
    connect(timerHeight, &QTimer::timeout, this, &DialogPlaceLabel::EvalHeight);
    connect(timerAngle, &QTimer::timeout, this, &DialogPlaceLabel::EvalAngle);

    connect(ui->comboBoxPiece, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this, [this]()
    {
        CheckPieces();
    });

    vis = new VisToolSpecialPoint(data);

    FormulaWidthChanged();
    FormulaHeightChanged();
    FormulaAngleChanged();
}

//---------------------------------------------------------------------------------------------------------------------
DialogPlaceLabel::~DialogPlaceLabel()
{
    delete ui;
}

//---------------------------------------------------------------------------------------------------------------------
void DialogPlaceLabel::EnbleShowMode(bool disable)
{
    m_showMode = disable;
    ui->comboBoxPiece->setDisabled(m_showMode);
}

//---------------------------------------------------------------------------------------------------------------------
quint32 DialogPlaceLabel::GetCenterPoint() const
{
    return getCurrentObjectId(ui->comboBoxPoint);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogPlaceLabel::SetCenterPoint(quint32 id)
{
    setCurrentPointId(ui->comboBoxPoint, id);

    if (not m_showMode)
    {
        VisToolSpecialPoint *point = qobject_cast<VisToolSpecialPoint *>(vis);
        SCASSERT(point != nullptr)
        point->setObject1Id(id);
    }

    CheckPoint();
}

//---------------------------------------------------------------------------------------------------------------------
PlaceLabelType DialogPlaceLabel::GetLabelType() const
{
    return static_cast<PlaceLabelType>(ui->comboBoxLabelType->currentData().toInt());
}

//---------------------------------------------------------------------------------------------------------------------
void DialogPlaceLabel::SetLabelType(PlaceLabelType type)
{
    const qint32 index = ui->comboBoxLabelType->findData(static_cast<int>(type));
    if (index != -1)
    {
        ui->comboBoxLabelType->setCurrentIndex(index);
    }
}

//---------------------------------------------------------------------------------------------------------------------
QString DialogPlaceLabel::GetWidth() const
{
    return qApp->TrVars()->TryFormulaFromUser(ui->plainTextEditFormulaWidth->toPlainText().replace("\n", " "),
                                              qApp->Settings()->GetOsSeparator());
}

//---------------------------------------------------------------------------------------------------------------------
void DialogPlaceLabel::SetWidth(const QString &value)
{
    const QString formula = qApp->TrVars()->FormulaToUser(value, qApp->Settings()->GetOsSeparator());
    // increase height if needed. TODO : see if I can get the max number of caracters in one line
    // of this PlainTextEdit to change 80 to this value
    if (formula.length() > 80)
    {
        this->DeployFormulaWidthEdit();
    }
    ui->plainTextEditFormulaWidth->setPlainText(formula);

//    VisToolPlaceLabel *point = qobject_cast<VisToolPlaceLabel *>(vis);
//    SCASSERT(point != nullptr)
//    point->setObject1Id(id);

    MoveCursorToEnd(ui->plainTextEditFormulaWidth);
}

//---------------------------------------------------------------------------------------------------------------------
QString DialogPlaceLabel::GetHeight() const
{
    return qApp->TrVars()->TryFormulaFromUser(ui->plainTextEditFormulaHeight->toPlainText().replace("\n", " "),
                                              qApp->Settings()->GetOsSeparator());
}

//---------------------------------------------------------------------------------------------------------------------
void DialogPlaceLabel::SetHeight(const QString &value)
{
    const QString formula = qApp->TrVars()->FormulaToUser(value, qApp->Settings()->GetOsSeparator());
    // increase height if needed. TODO : see if I can get the max number of caracters in one line
    // of this PlainTextEdit to change 80 to this value
    if (formula.length() > 80)
    {
        this->DeployFormulaHeightEdit();
    }
    ui->plainTextEditFormulaHeight->setPlainText(formula);

//    VisToolPlaceLabel *point = qobject_cast<VisToolPlaceLabel *>(vis);
//    SCASSERT(point != nullptr)
//    point->setObject1Id(id);

    MoveCursorToEnd(ui->plainTextEditFormulaHeight);
}

//---------------------------------------------------------------------------------------------------------------------
QString DialogPlaceLabel::GetAngle() const
{
    return qApp->TrVars()->TryFormulaFromUser(ui->plainTextEditFormulaAngle->toPlainText().replace("\n", " "),
                                              qApp->Settings()->GetOsSeparator());
}

//---------------------------------------------------------------------------------------------------------------------
void DialogPlaceLabel::SetAngle(const QString &value)
{
    const QString formula = qApp->TrVars()->FormulaToUser(value, qApp->Settings()->GetOsSeparator());
    // increase height if needed. TODO : see if I can get the max number of caracters in one line
    // of this PlainTextEdit to change 80 to this value
    if (formula.length() > 80)
    {
        this->DeployFormulaAngleEdit();
    }
    ui->plainTextEditFormulaAngle->setPlainText(formula);

//    VisToolPlaceLabel *point = qobject_cast<VisToolPlaceLabel *>(vis);
//    SCASSERT(point != nullptr)
//    point->setObject1Id(id);

    MoveCursorToEnd(ui->plainTextEditFormulaAngle);
}

//---------------------------------------------------------------------------------------------------------------------
quint32 DialogPlaceLabel::GetPieceId() const
{
    return getCurrentObjectId(ui->comboBoxPiece);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogPlaceLabel::SetPieceId(quint32 id)
{
    if (ui->comboBoxPiece->count() <= 0)
    {
        ui->comboBoxPiece->addItem(data->GetPiece(id).GetName(), id);
    }
    else
    {
        const qint32 index = ui->comboBoxPiece->findData(id);
        if (index != -1)
        {
            ui->comboBoxPiece->setCurrentIndex(index);
        }
        else
        {
            ui->comboBoxPiece->setCurrentIndex(0);
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
void DialogPlaceLabel::SetPiecesList(const QVector<quint32> &list)
{
    FillComboBoxPiecesList(ui->comboBoxPiece, list);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogPlaceLabel::ChosenObject(quint32 id, const SceneObject &type)
{
    if (not prepare)
    {
        if (type == SceneObject::Point)
        {
            if (SetObject(id, ui->comboBoxPoint, ""))
            {
                vis->VisualMode(id);
                CheckPoint();
                prepare = true;
                this->setModal(true);
                this->show();
            }
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
void DialogPlaceLabel::CheckState()
{
    SCASSERT(bOk != nullptr);
    bOk->setEnabled(m_flagPoint && flagError && m_flagWidth && m_flagHeight && m_flagAngle);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogPlaceLabel::ShowVisualization()
{
    if (not m_showMode)
    {
        AddVisualization<VisToolSpecialPoint>();
    }
}

//---------------------------------------------------------------------------------------------------------------------
void DialogPlaceLabel::closeEvent(QCloseEvent *event)
{
    ui->plainTextEditFormulaWidth->blockSignals(true);
    ui->plainTextEditFormulaHeight->blockSignals(true);
    ui->plainTextEditFormulaAngle->blockSignals(true);
    DialogTool::closeEvent(event);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogPlaceLabel::DeployFormulaWidthEdit()
{
    DeployFormula(ui->plainTextEditFormulaWidth, ui->pushButtonGrowWidth, m_formulaBaseHeightWidth);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogPlaceLabel::DeployFormulaHeightEdit()
{
    DeployFormula(ui->plainTextEditFormulaHeight, ui->pushButtonGrowHeight, m_formulaBaseHeightHeight);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogPlaceLabel::DeployFormulaAngleEdit()
{
    DeployFormula(ui->plainTextEditFormulaAngle, ui->pushButtonGrowAngle, m_formulaBaseHeightAngle);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogPlaceLabel::FormulaWidthChanged()
{
    labelEditFormula = ui->labelEditFormulaWidth;
    labelResultCalculation = ui->labelResultCalculationWidth;
    const QString postfix = UnitsToStr(qApp->patternUnit(), true);
    ValFormulaChanged(m_flagWidth, ui->plainTextEditFormulaWidth, timerWidth, postfix);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogPlaceLabel::FormulaHeightChanged()
{
    labelEditFormula = ui->labelEditFormulaHeight;
    labelResultCalculation = ui->labelResultCalculationHeight;
    const QString postfix = UnitsToStr(qApp->patternUnit(), true);
    ValFormulaChanged(m_flagHeight, ui->plainTextEditFormulaHeight, timerHeight, postfix);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogPlaceLabel::FormulaAngleChanged()
{
    labelEditFormula = ui->labelEditFormulaAngle;
    labelResultCalculation = ui->labelResultCalculationAngle;
    ValFormulaChanged(m_flagAngle, ui->plainTextEditFormulaAngle, timerAngle, degreeSymbol);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogPlaceLabel::EvalWidth()
{
    labelEditFormula = ui->labelEditFormulaWidth;
    const QString postfix = UnitsToStr(qApp->patternUnit(), true);
    Eval(ui->plainTextEditFormulaWidth->toPlainText(), m_flagWidth, ui->labelResultCalculationWidth, postfix, true,
         true);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogPlaceLabel::EvalHeight()
{
    labelEditFormula = ui->labelEditFormulaHeight;
    const QString postfix = UnitsToStr(qApp->patternUnit(), true);
    Eval(ui->plainTextEditFormulaHeight->toPlainText(), m_flagHeight, ui->labelResultCalculationHeight, postfix, true,
         true);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogPlaceLabel::EvalAngle()
{
    labelEditFormula = ui->labelEditFormulaAngle;
    Eval(ui->plainTextEditFormulaAngle->toPlainText(), m_flagAngle, ui->labelResultCalculationAngle, degreeSymbol,
         false);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogPlaceLabel::FXWidth()
{
    QScopedPointer<DialogEditWrongFormula> dialog(new DialogEditWrongFormula(data, toolId, this));
    dialog->setWindowTitle(tr("Edit rectangle width"));
    dialog->SetFormula(GetWidth());
    dialog->setPostfix(UnitsToStr(qApp->patternUnit(), true));
    if (dialog->exec() == QDialog::Accepted)
    {
        SetWidth(dialog->GetFormula());
    }
}

//---------------------------------------------------------------------------------------------------------------------
void DialogPlaceLabel::FXHeight()
{
    QScopedPointer<DialogEditWrongFormula> dialog(new DialogEditWrongFormula(data, toolId, this));
    dialog->setWindowTitle(tr("Edit rectangle width"));
    dialog->SetFormula(GetHeight());
    dialog->setPostfix(UnitsToStr(qApp->patternUnit(), true));
    if (dialog->exec() == QDialog::Accepted)
    {
        SetHeight(dialog->GetFormula());
    }
}

//---------------------------------------------------------------------------------------------------------------------
void DialogPlaceLabel::FXAngle()
{
    QScopedPointer<DialogEditWrongFormula> dialog(new DialogEditWrongFormula(data, toolId, this));
    dialog->setWindowTitle(tr("Edit angle"));
    dialog->SetFormula(GetAngle());
    dialog->setPostfix(degreeSymbol);
    if (dialog->exec() == QDialog::Accepted)
    {
        SetAngle(dialog->GetFormula());
    }
}

//---------------------------------------------------------------------------------------------------------------------
void DialogPlaceLabel::FillPlaceLabelTypes()
{
    ui->comboBoxLabelType->addItem(tr("Segment"), static_cast<int>(PlaceLabelType::Segment));
    ui->comboBoxLabelType->addItem(tr("Rectangle"), static_cast<int>(PlaceLabelType::Rectangle));
    ui->comboBoxLabelType->addItem(tr("Cross"), static_cast<int>(PlaceLabelType::Cross));
    ui->comboBoxLabelType->addItem(tr("T-shaped"), static_cast<int>(PlaceLabelType::Tshaped));
    ui->comboBoxLabelType->addItem(tr("Doubletree"), static_cast<int>(PlaceLabelType::Doubletree));
    ui->comboBoxLabelType->addItem(tr("Corner"), static_cast<int>(PlaceLabelType::Corner));
    ui->comboBoxLabelType->addItem(tr("Triangle"), static_cast<int>(PlaceLabelType::Triangle));
    ui->comboBoxLabelType->addItem(tr("H-shaped"), static_cast<int>(PlaceLabelType::Hshaped));
    ui->comboBoxLabelType->addItem(tr("Button"), static_cast<int>(PlaceLabelType::Button));
}

//---------------------------------------------------------------------------------------------------------------------
void DialogPlaceLabel::CheckPieces()
{
    if (not m_showMode)
    {
        QColor color = okColor;
        if (ui->comboBoxPiece->count() <= 0 || ui->comboBoxPiece->currentIndex() == -1)
        {
            flagError = false;
            color = errorColor;
        }
        else
        {
            flagError = true;
            color = okColor;
        }
        ChangeColor(ui->labelPiece, color);
        CheckState();
    }
}

//---------------------------------------------------------------------------------------------------------------------
void DialogPlaceLabel::CheckPoint()
{
    QColor color = okColor;
    if (ui->comboBoxPoint->currentIndex() != -1)
    {
        m_flagPoint = true;
        color = okColor;
    }
    else
    {
        m_flagPoint = false;
        color = errorColor;
    }
    ChangeColor(ui->labelPoint, color);
    CheckState();
}
