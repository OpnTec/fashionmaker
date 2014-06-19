/************************************************************************
 **
 **  @file   dialogtool.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
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

#ifndef DIALOGTOOL_H
#define DIALOGTOOL_H

#include <QDialog>
#include <QDialogButtonBox>
#include "../../widgets/vapplication.h"
#include <QPushButton>

class QDoubleSpinBox;
class QLabel;
class QRadioButton;
class QComboBox;
class QListWidgetItem;
class QListWidget;
class VContainer;
class QPlainTextEdit;
class VAbstractTool;

enum class ComboBoxCutSpline : char { CutSpline, NoCutSpline };
enum class ComboBoxCutArc : char { CutArc, NoCutArc};

/**
 * @brief The DialogTool class parent for all dialog of tools.
 */
class DialogTool : public QDialog
{
    Q_OBJECT
public:
    DialogTool(const VContainer *data, QWidget *parent = nullptr);
    virtual          ~DialogTool() {}
    VAbstractTool* GetAssociatedTool();
    void SetAssociatedTool(VAbstractTool* tool);
signals:
    /**
     * @brief DialogClosed signal dialog closed
     * @param result keep result
     */
    void             DialogClosed(int result);
    /**
     * @brief DialogApplied emit signal dialog apply changes
     */
    void     DialogApplied();
    /**
     * @brief ToolTip emit tooltipe for tool
     * @param toolTip text tooltipe
     */
    void             ToolTip(const QString &toolTip);
public slots:
    virtual void     ChoosedObject(quint32 id, const SceneObject &type);
    void             NamePointChanged();
    virtual void     DialogAccepted();
    /**
     * @brief DialogApply save data and emit signal DialogApplied.
     */
    virtual void     DialogApply();
    virtual void     DialogRejected();
    void             FormulaChanged();
    /**
     * @brief FormulaChangedPlainText check formula (plain text editor editor)
     */
    void             FormulaChangedPlainText();
    void             ArrowUp();
    void             ArrowDown();
    void             ArrowLeft();
    void             ArrowRight();
    void             ArrowLeftUp();
    void             ArrowLeftDown();
    void             ArrowRightUp();
    void             ArrowRightDown();
    void             EvalFormula();
    void             SizeHeight();
    void             Measurements();
    void             LengthLines();
    void             LengthArcs();
    void             LengthCurves();
    void             Increments();
    void             PutHere();
    void             PutVal(QListWidgetItem * item);
    virtual void     ValChenged(int row);
    void             UpdateList();
protected:
    Q_DISABLE_COPY(DialogTool)

    /** @brief data container with data */
    const VContainer *data;

    /** @brief isInitialized true if window is initialized */
    bool             isInitialized;

    /** @brief flagName true if name is correct */
    bool             flagName;

    /** @brief flagFormula true if formula correct */
    bool             flagFormula;

    /** @brief timerFormula timer for check formula */
    QTimer           *timerFormula;

    /** @brief bOk button ok */
    QPushButton      *bOk;

    /** @brief bApply button apply */
    QPushButton      *bApply;

    /** @brief spinBoxAngle spinbox for angle */
    QDoubleSpinBox   *spinBoxAngle;

    /** @brief plainTextEditFormula formula */
    QPlainTextEdit   *plainTextEditFormula;
    /** @brief listWidget listWidget with variables */
    QListWidget      *listWidget;

    /** @brief labelResultCalculation label with result of calculation */
    QLabel           *labelResultCalculation;

    /** @brief labelDescription description of variable */
    QLabel           *labelDescription;

    /** @brief labelEditNamePoint label used when need show wrong name of point */
    QLabel           *labelEditNamePoint;

    /** @brief labelEditFormula label used when need show wrong formula */
    QLabel           *labelEditFormula;

    /** @brief radioButtonSizeGrowth radio button for base variables */
    QRadioButton     *radioButtonSizeGrowth;

    /** @brief radioButtonStandardTable radio button for standard table variables */
    QRadioButton     *radioButtonStandardTable;

    /** @brief radioButtonIncrements radio button for increments variables */
    QRadioButton     *radioButtonIncrements;

    /** @brief radioButtonLengthLine radio button for lengths od lines variables */
    QRadioButton     *radioButtonLengthLine;

    /** @brief radioButtonLengthArc radio button for lengths of arcs variables */
    QRadioButton     *radioButtonLengthArc;

    /** @brief radioButtonLengthCurve radio button for lengths of curves variables */
    QRadioButton     *radioButtonLengthCurve;

    /** @brief lineStyles list supported line styles. */
    QStringList      lineStyles;

    virtual void     closeEvent ( QCloseEvent * event );
    virtual void     showEvent( QShowEvent *event );
    void             FillComboBoxPoints(QComboBox *box, const quint32 &id = 0)const;
    void             FillComboBoxArcs(QComboBox *box, const quint32 &id = 0,
                                      ComboBoxCutArc cut = ComboBoxCutArc::NoCutArc)const;
    void             FillComboBoxSplines(QComboBox *box, const quint32 &id = 0,
                                         ComboBoxCutSpline cut = ComboBoxCutSpline::NoCutSpline)const;
    void             FillComboBoxSplinesPath(QComboBox *box, const quint32 &id = 0,
                                             ComboBoxCutSpline cut = ComboBoxCutSpline::NoCutSpline)const;
    void             FillComboBoxTypeLine(QComboBox *box) const;
    virtual void     CheckState();
    QString          GetTypeLine(const QComboBox *box)const;
    template <class key, class val>
    void             ShowVariable(const QHash<key, val> *var);
    void             SetupTypeLine(QComboBox *box, const QString &value);
    void             ChangeCurrentText(QComboBox *box, const QString &value);
    void             ChangeCurrentData(QComboBox *box, const quint32 &value) const;
    void             PutValHere(QLineEdit *lineEdit, QListWidget *listWidget);
    void             PutValHere(QPlainTextEdit *plainTextEdit, QListWidget *listWidget);
    void             ValFormulaChanged(bool &flag, QLineEdit *edit, QTimer * timer);
    void             ValFormulaChanged(bool &flag, QPlainTextEdit *edit, QTimer * timer);
    void             Eval(const QString &text, bool &flag, QTimer *timer, QLabel *label);
    void             setCurrentPointId(QComboBox *box, quint32 &pointId, const quint32 &value, const quint32 &id) const;
    void             setCurrentSplineId(QComboBox *box, quint32 &splineId, const quint32 &value, const quint32 &id,
                                        ComboBoxCutSpline cut = ComboBoxCutSpline::NoCutSpline) const;
    void             setCurrentArcId(QComboBox *box, quint32 &arcId, const quint32 &value, const quint32 &id,
                                        ComboBoxCutArc cut = ComboBoxCutArc::NoCutArc) const;
    void             setCurrentSplinePathId(QComboBox *box, quint32 &splinePathId, const quint32 &value,
                                            const quint32 &id,
                                            ComboBoxCutSpline cut = ComboBoxCutSpline::NoCutSpline) const;
    quint32          getCurrentObjectId(QComboBox *box) const;
    bool             ChoosedPoint(const quint32 &id, QComboBox *box, const QString &toolTip);
    void             DeployFormula(QPlainTextEdit *formula, QPushButton *buttonGrowLength, int formulaBaseHeight);
    template <typename T>
    void             InitArrow(T *ui)
    {
        SCASSERT(ui != nullptr);
        spinBoxAngle = ui->doubleSpinBoxAngle;
        connect(ui->toolButtonArrowDown, &QPushButton::clicked, this, &DialogTool::ArrowDown);
        connect(ui->toolButtonArrowUp, &QPushButton::clicked, this, &DialogTool::ArrowUp);
        connect(ui->toolButtonArrowLeft, &QPushButton::clicked, this, &DialogTool::ArrowLeft);
        connect(ui->toolButtonArrowRight, &QPushButton::clicked, this, &DialogTool::ArrowRight);
        connect(ui->toolButtonArrowLeftUp, &QPushButton::clicked, this, &DialogTool::ArrowLeftUp);
        connect(ui->toolButtonArrowLeftDown, &QPushButton::clicked, this, &DialogTool::ArrowLeftDown);
        connect(ui->toolButtonArrowRightUp, &QPushButton::clicked, this, &DialogTool::ArrowRightUp);
        connect(ui->toolButtonArrowRightDown, &QPushButton::clicked, this, &DialogTool::ArrowRightDown);
    }
    template <typename T>
    void             InitVariables(T *ui)
    {
        listWidget = ui->listWidget;
        labelDescription = ui->labelDescription;
        radioButtonSizeGrowth = ui->radioButtonSizeGrowth;
        radioButtonStandardTable = ui->radioButtonStandardTable;
        radioButtonIncrements = ui->radioButtonIncrements;
        radioButtonLengthLine = ui->radioButtonLengthLine;
        radioButtonLengthArc = ui->radioButtonLengthArc;
        radioButtonLengthCurve = ui->radioButtonLengthSpline;

        connect(listWidget, &QListWidget::currentRowChanged, this, &DialogTool::ValChenged);

        if (qApp->patternType() == MeasurementsType::Standard)
        {
            SizeHeight();
            connect(radioButtonSizeGrowth, &QRadioButton::clicked, this, &DialogTool::SizeHeight);
        }
        else
        {
            radioButtonSizeGrowth->setEnabled(false);
            radioButtonStandardTable->setChecked(true);
            Measurements();
        }
        connect(radioButtonStandardTable, &QRadioButton::clicked, this, &DialogTool::Measurements);
        connect(radioButtonIncrements, &QRadioButton::clicked, this, &DialogTool::Increments);
        connect(radioButtonLengthLine, &QRadioButton::clicked, this, &DialogTool::LengthLines);
        connect(radioButtonLengthArc, &QRadioButton::clicked, this, &DialogTool::LengthArcs);
        connect(radioButtonLengthCurve, &QRadioButton::clicked, this, &DialogTool::LengthCurves);
    }
    template <typename T>
    /**
     * @brief InitOkCancelApply initialise OK / Cancel and Apply buttons
     * @param ui Dialog container
     */
    void             InitOkCancelApply(T *ui)
    {
        InitOkCancel(ui);
        bApply = ui->buttonBox->button(QDialogButtonBox::Apply);
        SCASSERT(bApply != nullptr);
        connect(bApply, &QPushButton::clicked, this, &DialogTool::DialogApply);
    }
    template <typename T>
    /**
     * @brief InitOkCancel initialise OK and Cancel buttons
     * @param ui Dialog container
     */
    void             InitOkCancel(T *ui)
    {
        bOk = ui->buttonBox->button(QDialogButtonBox::Ok);
        SCASSERT(bOk != nullptr);
        connect(bOk, &QPushButton::clicked, this, &DialogTool::DialogAccepted);

        QPushButton *bCancel = ui->buttonBox->button(QDialogButtonBox::Cancel);
        SCASSERT(bCancel != nullptr);
        connect(bCancel, &QPushButton::clicked, this, &DialogTool::DialogRejected);
    }
    /**
     * @brief associatedTool vdrawtool associated with opened dialog.
     */
    VAbstractTool* associatedTool;
private:
    void             FillList(QComboBox *box, const QMap<QString, quint32> &list)const;
};

//---------------------------------------------------------------------------------------------------------------------
inline VAbstractTool *DialogTool::GetAssociatedTool()
{
    return this->associatedTool;
}

//---------------------------------------------------------------------------------------------------------------------
inline void DialogTool::SetAssociatedTool(VAbstractTool *tool)
{
    this->associatedTool=tool;
}

#endif // DIALOGTOOL_H
