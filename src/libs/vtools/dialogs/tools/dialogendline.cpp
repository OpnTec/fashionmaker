/************************************************************************
 **
 **  @file   dialogendline.cpp
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

#include "dialogendline.h"

#include <QDialog>
#include <QLineEdit>
#include <QLineF>
#include <QPlainTextEdit>
#include <QPointF>
#include <QPointer>
#include <QPushButton>
#include <QSharedPointer>
#include <QTimer>
#include <QToolButton>
#include <new>

#include "../vgeometry/vpointf.h"
#include "../vpatterndb/vcontainer.h"
#include "../vpatterndb/vtranslatevars.h"
#include "../vwidgets/vmaingraphicsscene.h"
#include "../vwidgets/vabstractmainwindow.h"
#include "../../tools/vabstracttool.h"
#include "../../visualization/line/vistoolendline.h"
#include "../../visualization/visualization.h"
#include "../ifc/xml/vabstractpattern.h"
#include "../ifc/xml/vdomdocument.h"
#include "../support/dialogeditwrongformula.h"
#include "../vmisc/vabstractapplication.h"
#include "../vmisc/vcommonsettings.h"
#include "ui_dialogendline.h"

class QCloseEvent;
class QWidget;

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief DialogEndLine create dialog
 * @param data container with data
 * @param parent parent widget
 */
DialogEndLine::DialogEndLine(const VContainer *data, const quint32 &toolId, QWidget *parent)
    :DialogTool(data, toolId, parent), ui(new Ui::DialogEndLine),
      formulaLength(QString()), formulaAngle(QString()), formulaBaseHeight(0), formulaBaseHeightAngle(0)
{
    ui->setupUi(this);

#if QT_VERSION >= QT_VERSION_CHECK(5, 2, 0)
    ui->lineEditNamePoint->setClearButtonEnabled(true);
#endif

    InitFormulaUI(ui);
    ui->lineEditNamePoint->setText(qApp->getCurrentDocument()->GenerateLabel(LabelType::NewLabel));
    labelEditNamePoint = ui->labelEditNamePoint;
    this->formulaBaseHeight = ui->plainTextEditFormula->height();
    this->formulaBaseHeightAngle = ui->plainTextEditAngle->height();

    ui->plainTextEditFormula->installEventFilter(this);
    ui->plainTextEditAngle->installEventFilter(this);

    InitOkCancelApply(ui);
    flagFormula = false;
    CheckState();

    FillComboBoxPoints(ui->comboBoxBasePoint);
    FillComboBoxTypeLine(ui->comboBoxLineType, VAbstractTool::LineStylesPics());
    FillComboBoxLineColors(ui->comboBoxLineColor);

    connect(ui->toolButtonExprLength, &QPushButton::clicked, this, &DialogEndLine::FXLength);
    connect(ui->toolButtonExprAngle, &QPushButton::clicked, this, &DialogEndLine::FXAngle);

    connect(ui->lineEditNamePoint, &QLineEdit::textChanged, this, &DialogEndLine::NamePointChanged);

    connect(ui->plainTextEditFormula, &QPlainTextEdit::textChanged, this, &DialogEndLine::FormulaTextChanged);
    connect(ui->plainTextEditAngle, &QPlainTextEdit::textChanged, this, &DialogEndLine::AngleTextChanged);

    connect(ui->pushButtonGrowLength, &QPushButton::clicked, this, &DialogEndLine::DeployFormulaTextEdit);
    connect(ui->pushButtonGrowLengthAngle, &QPushButton::clicked, this, &DialogEndLine::DeployAngleTextEdit);

    connect(timerFormula, &QTimer::timeout, this, &DialogEndLine::EvalAngle);

    vis = new VisToolEndLine(data);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief EvalAngle calculate value of angle
 */
void DialogEndLine::EvalAngle()
{
    labelEditFormula = ui->labelEditAngle;
    Eval(ui->plainTextEditAngle->toPlainText(), flagError, ui->labelResultCalculationAngle, degreeSymbol, false);
    labelEditFormula = ui->labelEditFormula;
}

//---------------------------------------------------------------------------------------------------------------------
void DialogEndLine::FormulaTextChanged()
{
    this->FormulaChangedPlainText();
}

//---------------------------------------------------------------------------------------------------------------------
void DialogEndLine::AngleTextChanged()
{
    labelEditFormula = ui->labelEditAngle;
    ValFormulaChanged(flagError, ui->plainTextEditAngle, timerFormula, degreeSymbol);
    labelEditFormula = ui->labelEditFormula;
}

//---------------------------------------------------------------------------------------------------------------------
void DialogEndLine::DeployFormulaTextEdit()
{
    DeployFormula(ui->plainTextEditFormula, ui->pushButtonGrowLength, formulaBaseHeight);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogEndLine::DeployAngleTextEdit()
{
    DeployFormula(ui->plainTextEditAngle, ui->pushButtonGrowLengthAngle, formulaBaseHeightAngle);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogEndLine::FXAngle()
{
    DialogEditWrongFormula *dialog = new DialogEditWrongFormula(data, toolId, this);
    dialog->setWindowTitle(tr("Edit angle"));
    dialog->SetFormula(GetAngle());
    dialog->setPostfix(degreeSymbol);
    if (dialog->exec() == QDialog::Accepted)
    {
        SetAngle(dialog->GetFormula());
    }
    delete dialog;
}

//---------------------------------------------------------------------------------------------------------------------
void DialogEndLine::FXLength()
{
    DialogEditWrongFormula *dialog = new DialogEditWrongFormula(data, toolId, this);
    dialog->setWindowTitle(tr("Edit length"));
    dialog->SetFormula(GetFormula());
    dialog->setPostfix(VDomDocument::UnitsToStr(qApp->patternUnit(), true));
    if (dialog->exec() == QDialog::Accepted)
    {
        SetFormula(dialog->GetFormula());
    }
    delete dialog;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief ChoosedObject gets id and type of selected object. Save right data and ignore wrong.
 * @param id id of point or detail
 * @param type type of object
 */
void DialogEndLine::ChosenObject(quint32 id, const SceneObject &type)
{
    if (prepare == false)// After first choose we ignore all objects
    {
        if (type == SceneObject::Point)
        {
            if (SetObject(id, ui->comboBoxBasePoint, ""))
            {
                vis->VisualMode(id);
                VAbstractMainWindow *window = qobject_cast<VAbstractMainWindow *>(qApp->getMainWindow());
                SCASSERT(window != nullptr);
                connect(vis.data(), &Visualization::ToolTip, window, &VAbstractMainWindow::ShowToolTip);
                prepare = true;
            }
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief SetPointName set name of point
 * @param value name
 */
void DialogEndLine::SetPointName(const QString &value)
{
    pointName = value;
    ui->lineEditNamePoint->setText(pointName);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief SetTypeLine set type of line
 * @param value type
 */
void DialogEndLine::SetTypeLine(const QString &value)
{
    ChangeCurrentData(ui->comboBoxLineType, value);
    vis->setLineStyle(VAbstractTool::LineStyleToPenStyle(value));
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief SetFormula set string of formula
 * @param value formula
 */
void DialogEndLine::SetFormula(const QString &value)
{
    formulaLength = qApp->TrVars()->FormulaToUser(value, qApp->Settings()->GetOsSeparator());
    // increase height if needed. TODO : see if I can get the max number of caracters in one line
    // of this PlainTextEdit to change 80 to this value
    if (formulaLength.length() > 80)
    {
        this->DeployFormulaTextEdit();
    }
    ui->plainTextEditFormula->setPlainText(formulaLength);

    VisToolEndLine *line = qobject_cast<VisToolEndLine *>(vis);
    SCASSERT(line != nullptr);
    line->setLength(formulaLength);

    MoveCursorToEnd(ui->plainTextEditFormula);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief SetAngle set angle of line
 * @param value angle in degree
 */
void DialogEndLine::SetAngle(const QString &value)
{
    formulaAngle = qApp->TrVars()->FormulaToUser(value, qApp->Settings()->GetOsSeparator());
    // increase height if needed. TODO : see if I can get the max number of caracters in one line
    // of this PlainTextEdit to change 80 to this value
    if (formulaAngle.length() > 80)
    {
        this->DeployAngleTextEdit();
    }
    ui->plainTextEditAngle->setPlainText(formulaAngle);

    VisToolEndLine *line = qobject_cast<VisToolEndLine *>(vis);
    SCASSERT(line != nullptr);
    line->SetAngle(formulaAngle);

    MoveCursorToEnd(ui->plainTextEditAngle);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief SetBasePointId set id base point of line
 * @param value id
 */
void DialogEndLine::SetBasePointId(const quint32 &value)
{
    setCurrentPointId(ui->comboBoxBasePoint, value);

    VisToolEndLine *line = qobject_cast<VisToolEndLine *>(vis);
    SCASSERT(line != nullptr);
    line->setObject1Id(value);
}

//---------------------------------------------------------------------------------------------------------------------
QString DialogEndLine::GetLineColor() const
{
    return GetComboBoxCurrentData(ui->comboBoxLineColor, ColorBlack);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogEndLine::SetLineColor(const QString &value)
{
    ChangeCurrentData(ui->comboBoxLineColor, value);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief DialogEndLine::ShowDialog show dialog after finish working with visual part
 * @param click true if need show dialog after click mouse
 */
void DialogEndLine::ShowDialog(bool click)
{
    if (prepare)
    {
        if (click)
        {
            /*We will ignore click if pointer is in point circle*/
            VMainGraphicsScene *scene = qobject_cast<VMainGraphicsScene *>(qApp->getCurrentScene());
            SCASSERT(scene != nullptr);
            const QSharedPointer<VPointF> point = data->GeometricObject<VPointF>(GetBasePointId());
            QLineF line = QLineF(*point, scene->getScenePos());

            //Radius of point circle, but little bigger. Need handle with hover sizes.
            const qreal radius = ToPixel(DefPointRadius/*mm*/, Unit::Mm)*1.5;
            if (line.length() <= radius)
            {
                return;
            }
        }
        this->setModal(true);

        VisToolEndLine *line = qobject_cast<VisToolEndLine *>(vis);
        SCASSERT(line != nullptr);

        this->SetAngle(line->Angle());//Show in dialog angle what user choose
        this->SetFormula(line->Length());
        emit ToolTip("");
        timerFormula->start();
        this->show();
    }
}

//---------------------------------------------------------------------------------------------------------------------
void DialogEndLine::ShowVisualization()
{
    AddVisualization<VisToolEndLine>();
}

//---------------------------------------------------------------------------------------------------------------------
void DialogEndLine::SaveData()
{
    pointName = ui->lineEditNamePoint->text();

    formulaLength = ui->plainTextEditFormula->toPlainText();
    formulaLength.replace("\n", " ");

    formulaAngle = ui->plainTextEditAngle->toPlainText();
    formulaAngle.replace("\n", " ");

    VisToolEndLine *line = qobject_cast<VisToolEndLine *>(vis);
    SCASSERT(line != nullptr);

    line->setObject1Id(GetBasePointId());
    line->setLength(formulaLength);
    line->SetAngle(formulaAngle);
    line->setLineStyle(VAbstractTool::LineStyleToPenStyle(GetTypeLine()));
    line->RefreshGeometry();
}

//---------------------------------------------------------------------------------------------------------------------
void DialogEndLine::closeEvent(QCloseEvent *event)
{
    ui->plainTextEditFormula->blockSignals(true);
    ui->plainTextEditAngle->blockSignals(true);
    DialogTool::closeEvent(event);
}

//---------------------------------------------------------------------------------------------------------------------
DialogEndLine::~DialogEndLine()
{
    delete ui;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief GetTypeLine return type of line
 * @return type
 */
QString DialogEndLine::GetTypeLine() const
{
    return GetComboBoxCurrentData(ui->comboBoxLineType, TypeLineLine);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief GetFormula return string of formula
 * @return formula
 */
QString DialogEndLine::GetFormula() const
{
    return qApp->TrVars()->TryFormulaFromUser(formulaLength, qApp->Settings()->GetOsSeparator());
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief GetAngle return formula angle of line
 * @return angle formula
 */
QString DialogEndLine::GetAngle() const
{
    return qApp->TrVars()->TryFormulaFromUser(formulaAngle, qApp->Settings()->GetOsSeparator());
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief GetBasePointId return id base point of line
 * @return id
 */
quint32 DialogEndLine::GetBasePointId() const
{
    return getCurrentObjectId(ui->comboBoxBasePoint);
}
