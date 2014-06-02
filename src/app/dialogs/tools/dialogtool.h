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

#include <QComboBox>
#include <QDialog>
#include <QDoubleSpinBox>
#include <QLabel>
#include <QListWidgetItem>
#include <QRadioButton>
#include <QDialogButtonBox>
#include <QPushButton>
#include "../../container/vcontainer.h"
#include "../../widgets/vapplication.h"

namespace ComboMode
{
    /**
     * @brief The ComboBoxCutSpline enum
     */
    enum ComboBoxCutSpline { CutSpline, NoCutSpline };
    Q_DECLARE_FLAGS(ComboBoxCutSplines, ComboBoxCutSpline)

    enum ComboBoxCutArc { CutArc, NoCutArc};
    Q_DECLARE_FLAGS(ComboBoxCutArcs, ComboBoxCutArc)
}
Q_DECLARE_OPERATORS_FOR_FLAGS( ComboMode::ComboBoxCutSplines )
Q_DECLARE_OPERATORS_FOR_FLAGS( ComboMode::ComboBoxCutArcs )

/**
 * @brief The DialogTool class parent for all dialog of tools.
 */
class DialogTool : public QDialog
{
    Q_OBJECT
public:
    DialogTool(const VContainer *data, QWidget *parent = nullptr);
    virtual          ~DialogTool() {}
signals:
    /**
     * @brief DialogClosed signal dialog closed
     * @param result keep result
     */
    void             DialogClosed(int result);
    /**
     * @brief ToolTip emit tooltipe for tool
     * @param toolTip text tooltipe
     */
    void             ToolTip(const QString &toolTip);
public slots:
    virtual void     ChoosedObject(quint32 id, const Valentina::Scenes &type);
    void             NamePointChanged();
    virtual void     DialogAccepted();
    virtual void     DialogRejected();
    void             FormulaChanged();
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

    /** @brief spinBoxAngle spinbox for angle */
    QDoubleSpinBox   *spinBoxAngle;

    /** @brief lineEditFormula linEdit for formula */
    QLineEdit        *lineEditFormula;

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
                                      ComboMode::ComboBoxCutArc cut = ComboMode::NoCutArc)const;
    void             FillComboBoxSplines(QComboBox *box, const quint32 &id = 0,
                                         ComboMode::ComboBoxCutSpline cut = ComboMode::NoCutSpline)const;
    void             FillComboBoxSplinesPath(QComboBox *box, const quint32 &id = 0,
                                             ComboMode::ComboBoxCutSpline cut = ComboMode::NoCutSpline)const;
    void             FillComboBoxTypeLine(QComboBox *box) const;
    virtual void     CheckState();
    QString          GetTypeLine(const QComboBox *box)const;
    template <class key, class val>
    void             ShowVariable(const QHash<key, val> *var);
    void             SetupTypeLine(QComboBox *box, const QString &value);
    void             ChangeCurrentText(QComboBox *box, const QString &value);
    void             ChangeCurrentData(QComboBox *box, const quint32 &value) const;
    void             PutValHere(QLineEdit *lineEdit, QListWidget *listWidget);
    void             ValFormulaChanged(bool &flag, QLineEdit *edit, QTimer * timer);
    void             Eval(QLineEdit *edit, bool &flag, QTimer *timer, QLabel *label);
    void             setCurrentPointId(QComboBox *box, quint32 &pointId, const quint32 &value, const quint32 &id) const;
    void             setCurrentSplineId(QComboBox *box, quint32 &splineId, const quint32 &value, const quint32 &id,
                                        ComboMode::ComboBoxCutSpline cut = ComboMode::NoCutSpline) const;
    void             setCurrentArcId(QComboBox *box, quint32 &arcId, const quint32 &value, const quint32 &id,
                                        ComboMode::ComboBoxCutArc cut = ComboMode::NoCutArc) const;
    void             setCurrentSplinePathId(QComboBox *box, quint32 &splinePathId, const quint32 &value,
                                            const quint32 &id,
                                            ComboMode::ComboBoxCutSpline cut = ComboMode::NoCutSpline) const;
    quint32          getCurrentObjectId(QComboBox *box) const;
    bool             ChoosedPoint(const quint32 &id, QComboBox *box, const QString &toolTip);
    template <typename T>
    void             InitArrow(T *ui)
    {
        Q_CHECK_PTR(ui);
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

        if (qApp->patternType() == Pattern::Standard)
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
    void             InitOkCansel(T *ui)
    {
        bOk = ui->buttonBox->button(QDialogButtonBox::Ok);
        Q_CHECK_PTR(bOk);
        connect(bOk, &QPushButton::clicked, this, &DialogTool::DialogAccepted);

        QPushButton *bCansel = ui->buttonBox->button(QDialogButtonBox::Cancel);
        Q_CHECK_PTR(bCansel);
        connect(bCansel, &QPushButton::clicked, this, &DialogTool::DialogRejected);
    }
private:
    void             FillList(QComboBox *box, const QMap<QString, quint32> &list)const;
};

#endif // DIALOGTOOL_H
