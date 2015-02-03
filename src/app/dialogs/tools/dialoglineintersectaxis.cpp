/************************************************************************
 **
 **  @file   dialoglineintersectaxis.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   19 10, 2014
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentine project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2014 Valentina project
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

#include "dialoglineintersectaxis.h"
#include "ui_dialoglineintersectaxis.h"

#include "../../geometry/vpointf.h"
#include "../../container/vcontainer.h"
#include "../../visualization/vistoollineintersectaxis.h"
#include "../../widgets/vmaingraphicsscene.h"
#include "../../tools/vabstracttool.h"
#include <QTimer>

//---------------------------------------------------------------------------------------------------------------------
DialogLineIntersectAxis::DialogLineIntersectAxis(const VContainer *data, const quint32 &toolId, QWidget *parent)
    :DialogTool(data, toolId, parent), ui(new Ui::DialogLineIntersectAxis), formulaAngle(QString()),
      formulaBaseHeightAngle(0), line(nullptr)
{
    ui->setupUi(this);
    InitVariables(ui);
    InitFormulaUI(ui);
    ui->lineEditNamePoint->setText(qApp->getCurrentDocument()->GenerateLabel(LabelType::NewLabel));
    labelEditNamePoint = ui->labelEditNamePoint;
    this->formulaBaseHeightAngle = ui->plainTextEditFormula->height();
    ui->plainTextEditFormula->installEventFilter(this);

    InitOkCancelApply(ui);
    flagFormula = false;
    CheckState();

    FillComboBoxPoints(ui->comboBoxAxisPoint);
    FillComboBoxPoints(ui->comboBoxFirstLinePoint);
    FillComboBoxPoints(ui->comboBoxSecondLinePoint);
    FillComboBoxTypeLine(ui->comboBoxLineType, VAbstractTool::LineStylesPics());
    FillComboBoxLineColors(ui->comboBoxLineColor);

    connect(ui->toolButtonPutHereAngle, &QPushButton::clicked, this, &DialogLineIntersectAxis::PutAngle);
    connect(listWidget, &QListWidget::itemDoubleClicked, this, &DialogLineIntersectAxis::PutVal);
    connect(ui->toolButtonEqualAngle, &QPushButton::clicked, this, &DialogLineIntersectAxis::EvalAngle);
    connect(ui->lineEditNamePoint, &QLineEdit::textChanged, this, &DialogLineIntersectAxis::NamePointChanged);
    connect(ui->plainTextEditFormula, &QPlainTextEdit::textChanged, this, &DialogLineIntersectAxis::AngleTextChanged);
    connect(ui->pushButtonGrowLengthAngle, &QPushButton::clicked, this, &DialogLineIntersectAxis::DeployAngleTextEdit);
    connect(timerFormula, &QTimer::timeout, this, &DialogLineIntersectAxis::EvalAngle);
    connect(ui->comboBoxFirstLinePoint,
            static_cast<void (QComboBox::*)(const QString &)>(&QComboBox::currentIndexChanged),
            this, &DialogLineIntersectAxis::PointNameChanged);
    connect(ui->comboBoxSecondLinePoint,
            static_cast<void (QComboBox::*)(const QString &)>(&QComboBox::currentIndexChanged),
            this, &DialogLineIntersectAxis::PointNameChanged);
    connect(ui->comboBoxAxisPoint, static_cast<void (QComboBox::*)(const QString &)>(&QComboBox::currentIndexChanged),
            this, &DialogLineIntersectAxis::PointNameChanged);

    line = new VisToolLineIntersectAxis(data);
}

//---------------------------------------------------------------------------------------------------------------------
DialogLineIntersectAxis::~DialogLineIntersectAxis()
{
    if (qApp->getCurrentScene()->items().contains(line))
    { // In some cases scene delete object yourself. If not make check program will crash.
        delete line;
    }
    delete ui;
}

//---------------------------------------------------------------------------------------------------------------------
void DialogLineIntersectAxis::setPointName(const QString &value)
{
    pointName = value;
    ui->lineEditNamePoint->setText(pointName);
}

//---------------------------------------------------------------------------------------------------------------------
QString DialogLineIntersectAxis::getTypeLine() const
{
    return GetTypeLine(ui->comboBoxLineType);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogLineIntersectAxis::setTypeLine(const QString &value)
{
    ChangeCurrentData(ui->comboBoxLineType, value);
    line->setLineStyle(VAbstractTool::LineStyleToPenStyle(value));
}

//---------------------------------------------------------------------------------------------------------------------
QString DialogLineIntersectAxis::getAngle() const
{
    return qApp->FormulaFromUser(formulaAngle);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogLineIntersectAxis::setAngle(const QString &value)
{
    formulaAngle = qApp->FormulaToUser(value);
    // increase height if needed. TODO : see if I can get the max number of caracters in one line
    // of this PlainTextEdit to change 80 to this value
    if (formulaAngle.length() > 80)
    {
        this->DeployAngleTextEdit();
    }
    ui->plainTextEditFormula->setPlainText(formulaAngle);
    line->setAngle(formulaAngle);
    MoveCursorToEnd(ui->plainTextEditFormula);
}

//---------------------------------------------------------------------------------------------------------------------
quint32 DialogLineIntersectAxis::getBasePointId() const
{
    return getCurrentObjectId(ui->comboBoxAxisPoint);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogLineIntersectAxis::setBasePointId(const quint32 &value)
{
    setCurrentPointId(ui->comboBoxAxisPoint, value);
    line->setAxisPointId(value);
}

//---------------------------------------------------------------------------------------------------------------------
quint32 DialogLineIntersectAxis::getFirstPointId() const
{
    return getCurrentObjectId(ui->comboBoxFirstLinePoint);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogLineIntersectAxis::setFirstPointId(const quint32 &value)
{
    setCurrentPointId(ui->comboBoxFirstLinePoint, value);
    line->setPoint1Id(value);
}

//---------------------------------------------------------------------------------------------------------------------
quint32 DialogLineIntersectAxis::getSecondPointId() const
{
    return getCurrentObjectId(ui->comboBoxSecondLinePoint);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogLineIntersectAxis::setSecondPointId(const quint32 &value)
{
    setCurrentPointId(ui->comboBoxSecondLinePoint, value);
    line->setPoint2Id(value);
}

//---------------------------------------------------------------------------------------------------------------------
QString DialogLineIntersectAxis::getLineColor() const
{
    return GetLineColor(ui->comboBoxLineColor);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogLineIntersectAxis::setLineColor(const QString &value)
{
    ChangeCurrentData(ui->comboBoxLineColor, value);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogLineIntersectAxis::ShowDialog(bool click)
{
    if (prepare)
    {
        if (click)
        {
            /*We will ignore click if poinet is in point circle*/
            VMainGraphicsScene *scene = qApp->getCurrentScene();
            SCASSERT(scene != nullptr);
            const QSharedPointer<VPointF> point = data->GeometricObject<VPointF>(getBasePointId());
            QLineF line = QLineF(point->toQPointF(), scene->getScenePos());

            //Radius of point circle, but little bigger. Need handle with hover sizes.
            qreal radius = qApp->toPixel(DefPointRadius/*mm*/, Unit::Mm)*1.5;
            if (line.length() <= radius)
            {
                return;
            }
        }
        this->setModal(true);
        this->setAngle(line->Angle());//Show in dialog angle what user choose
        emit ToolTip("");
        timerFormula->start();
        this->show();
    }
}

//---------------------------------------------------------------------------------------------------------------------
void DialogLineIntersectAxis::ChosenObject(quint32 id, const SceneObject &type)
{
    if (prepare == false)// After first choose we ignore all objects
    {
        if (type == SceneObject::Point)
        {
            switch (number)
            {
                case (0):
                    if (SetObject(id, ui->comboBoxFirstLinePoint, tr("Select second point of line")))
                    {
                        number++;
                        line->VisualMode(id);
                        connect(line, &VisToolLineIntersectAxis::ToolTip, this, &DialogTool::ShowVisToolTip);
                    }
                    break;
                case (1):
                    if (getCurrentObjectId(ui->comboBoxFirstLinePoint) != id)
                    {
                        if (SetObject(id, ui->comboBoxSecondLinePoint, tr("Select axis point")))
                        {
                            number++;
                            line->setPoint2Id(id);
                            line->RefreshGeometry();
                        }
                    }
                    break;
                case (2):
                {
                    QSet<quint32> set;
                    set.insert(getCurrentObjectId(ui->comboBoxFirstLinePoint));
                    set.insert(getCurrentObjectId(ui->comboBoxSecondLinePoint));
                    set.insert(id);

                    if (set.size() == 3)
                    {
                        if (SetObject(id, ui->comboBoxAxisPoint, ""))
                        {
                            line->setAxisPointId(id);
                            line->RefreshGeometry();
                            prepare = true;
                        }
                    }
                }
                    break;
                default:
                    break;
            }
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
void DialogLineIntersectAxis::PutAngle()
{
    PutValHere(ui->plainTextEditFormula, ui->listWidget);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogLineIntersectAxis::EvalAngle()
{
    Eval(ui->plainTextEditFormula->toPlainText(), flagError, ui->labelResultCalculation, degreeSymbol, false);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogLineIntersectAxis::AngleTextChanged()
{
    ValFormulaChanged(flagError, ui->plainTextEditFormula, timerFormula);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogLineIntersectAxis::DeployAngleTextEdit()
{
    DeployFormula(ui->plainTextEditFormula, ui->pushButtonGrowLengthAngle, formulaBaseHeightAngle);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogLineIntersectAxis::PointNameChanged()
{
    QSet<quint32> set;
    set.insert(getCurrentObjectId(ui->comboBoxFirstLinePoint));
    set.insert(getCurrentObjectId(ui->comboBoxSecondLinePoint));
    set.insert(getCurrentObjectId(ui->comboBoxAxisPoint));

    QColor color = okColor;
    if (set.size() != 3)
    {
        flagError = false;
        color = errorColor;
    }
    else
    {
        flagError = true;
        color = okColor;
    }
    ChangeColor(ui->labelFirstLinePoint, color);
    ChangeColor(ui->labelSecondLinePoint, color);
    ChangeColor(ui->labelAxisPoint, color);
    CheckState();
}

//---------------------------------------------------------------------------------------------------------------------
void DialogLineIntersectAxis::ShowVisualization()
{
    if (prepare == false)
    {
        //TODO move to parent class!
        VMainGraphicsScene *scene = qApp->getCurrentScene();
        connect(scene, &VMainGraphicsScene::NewFactor, line, &VisToolLineIntersectAxis::SetFactor);
        scene->addItem(line);
        line->RefreshGeometry();
    }
}

//---------------------------------------------------------------------------------------------------------------------
void DialogLineIntersectAxis::SaveData()
{
    pointName = ui->lineEditNamePoint->text();

    formulaAngle = ui->plainTextEditFormula->toPlainText();
    formulaAngle.replace("\n", " ");

    line->setPoint1Id(getFirstPointId());
    line->setPoint2Id(getSecondPointId());
    line->setAxisPointId(getBasePointId());
    line->setAngle(formulaAngle);
    line->setLineStyle(VAbstractTool::LineStyleToPenStyle(getTypeLine()));
    line->RefreshGeometry();
}

//---------------------------------------------------------------------------------------------------------------------
void DialogLineIntersectAxis::closeEvent(QCloseEvent *event)
{
    ui->plainTextEditFormula->blockSignals(true);
    DialogTool::closeEvent(event);
}
