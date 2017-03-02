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

#ifndef DIALOGTOOL_H
#define DIALOGTOOL_H

#include <qcompilerdetection.h>
#include <QCheckBox>
#include <QColor>
#include <QComboBox>
#include <QDialog>
#include <QDialogButtonBox>
#include <QLineEdit>
#include <QList>
#include <QListWidget>
#include <QLocale>
#include <QMap>
#include <QMetaObject>
#include <QObject>
#include <QPointer>
#include <QPushButton>
#include <QRadioButton>
#include <QString>
#include <QVariant>
#include <QtGlobal>

#include "../vtools/visualization/visualization.h" // Issue on Windows
#include "../ifc/xml/vabstractpattern.h"
#include "../ifc/ifcdef.h"
#include "../vgeometry/vgeometrydef.h"
#include "../vmisc/def.h"
#include "../vmisc/logging.h"
#include "../vmisc/vabstractapplication.h"
#include "../vmisc/vcommonsettings.h"
#include "../vwidgets/vmaingraphicsscene.h"

template <class T> class QSharedPointer;

Q_DECLARE_LOGGING_CATEGORY(vDialog)

class QDoubleSpinBox;
class QLabel;
class QPlainTextEdit;
class VAbstractTool;

enum class FillComboBox : char { Whole, NoChildren};

/**
 * @brief The DialogTool class parent for all dialog of tools.
 */
class DialogTool : public QDialog
{
    Q_OBJECT
public:
    DialogTool(const VContainer *data, const quint32 &toolId, QWidget *parent = nullptr);
    virtual          ~DialogTool() Q_DECL_OVERRIDE;

    VAbstractTool*   GetAssociatedTool();
    void             SetAssociatedTool(VAbstractTool* tool);

    virtual void     ShowDialog(bool click);
    virtual void     Build(const Tool &type);
    virtual void     SetPiecesList(const QVector<quint32> &list);

    quint32          GetToolId() const;
    void             SetToolId(const quint32 &value);

    QString          getPointName() const;
signals:
    /**
     * @brief DialogClosed signal dialog closed
     * @param result keep result
     */
    void             DialogClosed(int result);
    /**
     * @brief DialogApplied emit signal dialog apply changes
     */
    void             DialogApplied();
    /**
     * @brief ToolTip emit tooltipe for tool
     * @param toolTip text tooltipe
     */
    void             ToolTip(const QString &toolTip);
public slots:
    virtual void     ChosenObject(quint32 id, const SceneObject &type);
    virtual void     SelectedObject(bool selected, quint32 object, quint32 tool);
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
    virtual void     EvalFormula();

    virtual void     PointNameChanged() {}
protected:
    Q_DISABLE_COPY(DialogTool)

    /** @brief data container with data */
    // cppcheck-suppress duplInheritedMember
    const VContainer *data;

    /** @brief isInitialized true if window is initialized */
    bool             isInitialized;

    /** @brief flagName true if name is correct */
    bool             flagName;

    /** @brief flagFormula true if formula correct */
    bool             flagFormula;

    /** @brief flagError use this flag if for you do not enought @see flagName and @see flagFormula.
     *
     * In many cases you will need more flags fore checking if all data are valid.
     * By default this flag is true.
     */
    bool             flagError;

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

    /** @brief labelResultCalculation label with result of calculation */
    QLabel           *labelResultCalculation;

    /** @brief labelEditNamePoint label used when need show wrong name of point */
    QLabel           *labelEditNamePoint;

    /** @brief labelEditFormula label used when need show wrong formula */
    QLabel           *labelEditFormula;

    const QColor     okColor;
    const QColor     errorColor;
    /**
     * @brief associatedTool vdrawtool associated with opened dialog.
     */
    VAbstractTool    *associatedTool;
    quint32          toolId;

    /** @brief prepare show if we prepare. Show dialog after finish working with visual part of tool*/
    bool             prepare;

    /** @brief pointName name of point */
    QString          pointName;

    /** @brief number number of handled objects */
    qint32           number;

    QPointer<Visualization> vis;

    virtual void     closeEvent ( QCloseEvent * event ) Q_DECL_OVERRIDE;
    virtual void     showEvent( QShowEvent *event ) Q_DECL_OVERRIDE;

    void             FillComboBoxPoints(QComboBox *box, FillComboBox rule = FillComboBox::Whole,
                                        const quint32 &ch1 = NULL_ID, const quint32 &ch2 = NULL_ID)const;
    void             FillComboBoxArcs(QComboBox *box, FillComboBox rule = FillComboBox::Whole,
                                      const quint32 &ch1 = NULL_ID, const quint32 &ch2 = NULL_ID)const;
    void             FillComboBoxSplines(QComboBox *box)const;
    void             FillComboBoxSplinesPath(QComboBox *box)const;
    void             FillComboBoxCurves(QComboBox *box)const;
    void             FillComboBoxTypeLine(QComboBox *box, const QMap<QString, QIcon> &stylesPics) const;
    void             FillComboBoxLineColors(QComboBox *box)const;
    void             FillComboBoxCrossCirclesPoints(QComboBox *box) const;
    void             FillComboBoxVCrossCurvesPoint(QComboBox *box) const;
    void             FillComboBoxHCrossCurvesPoint(QComboBox *box) const;

    virtual void     CheckState();
    QString          GetComboBoxCurrentData(const QComboBox *box, const QString &def)const;
    void             ChangeCurrentData(QComboBox *box, const QVariant &value) const;
    void             ValFormulaChanged(bool &flag, QLineEdit *edit, QTimer * timer, const QString &postfix = QString());
    void             ValFormulaChanged(bool &flag, QPlainTextEdit *edit, QTimer * timer,
                                       const QString &postfix = QString());
    qreal            Eval(const QString &text, bool &flag, QLabel *label, const QString &postfix,
                          bool checkZero = true, bool checkLessThanZero = false);

    void             setCurrentPointId(QComboBox *box, const quint32 &value,
                                       FillComboBox rule = FillComboBox::NoChildren,
                                       const quint32 &ch1 = NULL_ID, const quint32 &ch2 = NULL_ID) const;
    void             setCurrentSplineId(QComboBox *box, const quint32 &value) const;
    void             setCurrentArcId(QComboBox *box, const quint32 &value,
                                     FillComboBox rule = FillComboBox::NoChildren,
                                     const quint32 &ch1 = NULL_ID, const quint32 &ch2 = NULL_ID) const;
    void             setCurrentSplinePathId(QComboBox *box, const quint32 &value) const;
    void             setCurrentCurveId(QComboBox *box, const quint32 &value) const;

    quint32          getCurrentObjectId(QComboBox *box) const;

    template <typename T>
    T                getCurrentCrossPoint(QComboBox *box) const;

    bool             SetObject(const quint32 &id, QComboBox *box, const QString &toolTip);
    void             DeployFormula(QPlainTextEdit *formula, QPushButton *buttonGrowLength, int formulaBaseHeight);

    template <typename T>
    void             InitArrow(T *ui);

    template <typename T>
    void             InitOkCancelApply(T *ui);

    template <typename T>
    void             InitOkCancel(T *ui);

    template <typename T>
    void             InitFormulaUI(T *ui);

    template <typename T>
    void             AddVisualization();

    void             ChangeColor(QWidget *widget, const QColor &color);
    virtual void     ShowVisualization() {}
    /**
     * @brief SaveData Put dialog data in local variables
     */
    virtual void     SaveData() {}
    void             MoveCursorToEnd(QPlainTextEdit *plainTextEdit) const;
    virtual bool     eventFilter(QObject *object, QEvent *event) Q_DECL_OVERRIDE;
    quint32          DNumber(const QString &baseName) const;

    static quint32   RowId(QListWidget *listWidget, int i);
    static bool      FirstPointEqualLast(QListWidget *listWidget);
    static bool      DoublePoints(QListWidget *listWidget);
    static QString   DialogWarningIcon();

    QString          GetNodeName(const VPieceNode &node) const;
    void             NewNodeItem(QListWidget *listWidget, const VPieceNode &node);

    void             InitNodeAngles(QComboBox *box);
private:
    void FillList(QComboBox *box, const QMap<QString, quint32> &list)const;

    template <typename T>
    void PrepareList(QMap<QString, quint32> &list, quint32 id) const;

    bool IsSpline(const QSharedPointer<VGObject> &obj) const;
    bool IsSplinePath(const QSharedPointer<VGObject> &obj) const;

    template <typename GObject>
    void FillCombo(QComboBox *box, GOType gType, FillComboBox rule = FillComboBox::Whole,
                   const quint32 &ch1 = NULL_ID, const quint32 &ch2 = NULL_ID) const;


};

//---------------------------------------------------------------------------------------------------------------------
inline VAbstractTool *DialogTool::GetAssociatedTool()
{
    return this->associatedTool;
}

//---------------------------------------------------------------------------------------------------------------------
template <typename T>
inline void DialogTool::InitArrow(T *ui)
{
    SCASSERT(ui != nullptr)
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

//---------------------------------------------------------------------------------------------------------------------
template <typename T>
/**
 * @brief InitOkCancelApply initialise OK / Cancel and Apply buttons
 * @param ui Dialog container
 */
inline void DialogTool::InitOkCancelApply(T *ui)
{
    InitOkCancel(ui);
    bApply = ui->buttonBox->button(QDialogButtonBox::Apply);
    SCASSERT(bApply != nullptr)
    connect(bApply, &QPushButton::clicked, this, &DialogTool::DialogApply);
}

//---------------------------------------------------------------------------------------------------------------------
template <typename T>
/**
 * @brief InitOkCancel initialise OK and Cancel buttons
 * @param ui Dialog container
 */
inline void DialogTool::InitOkCancel(T *ui)
{
    bOk = ui->buttonBox->button(QDialogButtonBox::Ok);
    SCASSERT(bOk != nullptr)
    connect(bOk, &QPushButton::clicked, this, &DialogTool::DialogAccepted);

    QPushButton *bCancel = ui->buttonBox->button(QDialogButtonBox::Cancel);
    SCASSERT(bCancel != nullptr)
    connect(bCancel, &QPushButton::clicked, this, &DialogTool::DialogRejected);

    qApp->Settings()->GetOsSeparator() ? setLocale(QLocale()) : setLocale(QLocale::c());
}

//---------------------------------------------------------------------------------------------------------------------
template <typename T>
/**
 * @brief InitFormulaUI initialise ui object for formula fild
 * @param ui Dialog container
 */
inline void DialogTool::InitFormulaUI(T *ui)
{
    labelResultCalculation = ui->labelResultCalculation;
    plainTextEditFormula = ui->plainTextEditFormula;
    labelEditFormula = ui->labelEditFormula;
}

//---------------------------------------------------------------------------------------------------------------------
template <typename T>
inline void DialogTool::AddVisualization()
{
    if (prepare == false)
    {
        VMainGraphicsScene *scene = qobject_cast<VMainGraphicsScene *>(qApp->getCurrentScene());
        SCASSERT(scene != nullptr)

        T *toolVis = qobject_cast<T *>(vis);
        SCASSERT(toolVis != nullptr)

        if (not scene->items().contains(toolVis))
        {
            connect(scene, &VMainGraphicsScene::NewFactor, toolVis, &Visualization::SetFactor);
            scene->addItem(toolVis);
        }

        toolVis->SetMode(Mode::Show);
        toolVis->RefreshGeometry();
    }
}

//---------------------------------------------------------------------------------------------------------------------
template <typename T>
inline T DialogTool::getCurrentCrossPoint(QComboBox *box) const
{
    int value;
    bool ok = false;
#if QT_VERSION < QT_VERSION_CHECK(5, 2, 0)
    value = box->itemData(box->currentIndex()).toInt(&ok);
#else
    value = box->currentData().toInt(&ok);
#endif
    if (not ok)
    {
        return static_cast<T>(1);
    }

    switch(value)
    {
        case 1:
        case 2:
            return static_cast<T>(value);
        default:
            return static_cast<T>(1);
    }
}

#endif // DIALOGTOOL_H
