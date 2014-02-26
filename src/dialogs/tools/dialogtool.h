/************************************************************************
 **
 **  @file   dialogtool.h
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

#ifndef DIALOGTOOL_H
#define DIALOGTOOL_H

#include <QComboBox>
#include <QDialog>
#include <QDoubleSpinBox>
#include <QLabel>
#include <QListWidgetItem>
#include <QRadioButton>
#include "../../container/vcontainer.h"

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
                     /**
                      * @brief DialogTool create dialog
                      * @param data container with data
                      * @param parent parent widget
                      */
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
    /**
     * @brief ChoosedObject gets id and type of selected object. Save right data and ignore wrong.
     * @param id id of point or detail
     * @param type type of object
     */
    virtual void     ChoosedObject(quint32 id, const Scene::Scenes &type);
    /**
     * @brief NamePointChanged check name of point
     */
    void             NamePointChanged();
    /**
     * @brief DialogAccepted save data and emit signal about closed dialog.
     */
    virtual void     DialogAccepted();
    /**
     * @brief DialogRejected emit signal dialog rejected
     */
    virtual void     DialogRejected();
    /**
     * @brief formula check formula
     */
    void             FormulaChanged();
    /**
     * @brief ArrowUp set angle value 90 degree
     */
    void             ArrowUp();
    /**
     * @brief ArrowDown set angle value 270 degree
     */
    void             ArrowDown();
    /**
     * @brief ArrowLeft set angle value 180 degree
     */
    void             ArrowLeft();
    /**
     * @brief ArrowRight set angle value 0 degree
     */
    void             ArrowRight();
    /**
     * @brief ArrowLeftUp set angle value 135 degree
     */
    void             ArrowLeftUp();
    /**
     * @brief ArrowLeftDown set angle value 225 degree
     */
    void             ArrowLeftDown();
    /**
     * @brief ArrowRightUp set angle value 45 degree
     */
    void             ArrowRightUp();
    /**
     * @brief ArrowRightDown set angle value 315 degree
     */
    void             ArrowRightDown();
    /**
     * @brief EvalFormula evaluate formula
     */
    void             EvalFormula();
    /**
     * @brief SizeGrowth show in list base variables
     */
    void             SizeGrowth();
    /**
     * @brief StandardTable show in list standard table variables
     */
    void             StandardTable();
    /**
     * @brief LengthLines show in list lengths of lines variables
     */
    void             LengthLines();
    /**
     * @brief LengthArcs show in list lengths of arcs variables
     */
    void             LengthArcs();
    /**
     * @brief LengthCurves show in list lengths of curves variables
     */
    void             LengthCurves();
    /**
     * @brief Increments show in list increment variables
     */
    void             Increments();
    /**
     * @brief PutHere put variable into edit
     */
    void             PutHere();
    /**
     * @brief PutVal put variable into edit
     * @param item chosen item of list widget
     */
    void             PutVal(QListWidgetItem * item);
    /**
     * @brief ValChenged show description when current variable changed
     * @param row number of row
     */
    virtual void     ValChenged(int row);
    /**
     * @brief UpdateList update lists of variables
     */
    void             UpdateList();
protected:
    Q_DISABLE_COPY(DialogTool)
    /**
     * @brief data container with data
     */
    const VContainer *data;
    /**
     * @brief isInitialized true if window is initialized
     */
    bool             isInitialized;
    /**
     * @brief flagName true if name is correct
     */
    bool             flagName;
    /**
     * @brief flagFormula true if formula correct
     */
    bool             flagFormula;
    /**
     * @brief timerFormula timer for check formula
     */
    QTimer           *timerFormula;
    /**
     * @brief bOk button ok
     */
    QPushButton      *bOk;
    /**
     * @brief spinBoxAngle spinbox for angle
     */
    QDoubleSpinBox   *spinBoxAngle;
    /**
     * @brief lineEditFormula linEdit for formula
     */
    QLineEdit        *lineEditFormula;
    /**
     * @brief listWidget listWidget with variables
     */
    QListWidget      *listWidget;
    /**
     * @brief labelResultCalculation label with result of calculation
     */
    QLabel           *labelResultCalculation;
    /**
     * @brief labelDescription description of variable
     */
    QLabel           *labelDescription;
    /**
     * @brief labelEditNamePoint label used when need show wrong name of point
     */
    QLabel           *labelEditNamePoint;
    /**
     * @brief labelEditFormula label used when need show wrong formula
     */
    QLabel           *labelEditFormula;
    /**
     * @brief radioButtonSizeGrowth radio button for base variables
     */
    QRadioButton     *radioButtonSizeGrowth;
    /**
     * @brief radioButtonStandardTable radio button for standard table variables
     */
    QRadioButton     *radioButtonStandardTable;
    /**
     * @brief radioButtonIncrements radio button for increments variables
     */
    QRadioButton     *radioButtonIncrements;
    /**
     * @brief radioButtonLengthLine radio button for lengths od lines variables
     */
    QRadioButton     *radioButtonLengthLine;
    /**
     * @brief radioButtonLengthArc radio button for lengths of arcs variables
     */
    QRadioButton     *radioButtonLengthArc;
    /**
     * @brief radioButtonLengthCurve radio button for lengths of curves variables
     */
    QRadioButton     *radioButtonLengthCurve;
    /**
     * @brief lineStyles list supported line styles.
     */
    QStringList      lineStyles;
    /**
     * @brief closeEvent handle when dialog cloded
     * @param event event
     */
    virtual void     closeEvent ( QCloseEvent * event );
    /**
     * @brief showEvent handle when window show
     * @param event event
     */
    virtual void     showEvent( QShowEvent *event );
    /**
     * @brief FillComboBoxPoints fill comboBox list of points
     * @param box comboBox
     * @param id don't show this id in list
     */
    void             FillComboBoxPoints(QComboBox *box, const quint32 &id = 0)const;
    void             FillComboBoxArcs(QComboBox *box, const quint32 &id = 0,
                                      ComboMode::ComboBoxCutArc cut = ComboMode::NoCutArc)const;
    /**
     * @brief FillComboBoxSplines fill comboBox list of splines
     * @param box comboBox
     * @param id don't show id in list
     * @param cut if set to ComboMode::CutSpline don't show id+1 and id+2
     */
    void             FillComboBoxSplines(QComboBox *box, const quint32 &id = 0,
                                         ComboMode::ComboBoxCutSpline cut = ComboMode::NoCutSpline)const;
    /**
     * @brief FillComboBoxSplinesPath
     * @param box comboBox
     * @param id don't show id in list
     * @param cut if set to ComboMode::CutSpline don't show id+1 and id+2
     */
    void             FillComboBoxSplinesPath(QComboBox *box, const quint32 &id = 0,
                                             ComboMode::ComboBoxCutSpline cut = ComboMode::NoCutSpline)const;
    /**
     * @brief FillComboBoxTypeLine fill comboBox list of type lines
     * @param box comboBox
     */
    void             FillComboBoxTypeLine(QComboBox *box) const;
    /**
     * @brief CheckState enable, when all is correct, or disable, when something wrong, button ok
     */
    virtual void     CheckState();
    /**
     * @brief getTypeLine return type of line
     * @param box combobox
     * @return type
     */
    QString          GetTypeLine(const QComboBox *box)const;
    template <class key, class val>
    /**
     * @brief ShowVariable show variables in list
     * @param var container with variables
     */
    void             ShowVariable(const QHash<key, val> *var);
    /**
     * @brief SetupTypeLine setupe type of line
     * @param box combobox
     * @param value string from pattern file
     */
    void             SetupTypeLine(QComboBox *box, const QString &value);
    /**
     * @brief ChangeCurrentText select item in combobox by name
     * @param box combobox
     * @param value name of item
     */
    void             ChangeCurrentText(QComboBox *box, const QString &value);
    /**
     * @brief ChangeCurrentData select item in combobox by id
     * @param box combobox
     * @param value id of item
     */
    void             ChangeCurrentData(QComboBox *box, const quint32 &value) const;
    /**
     * @brief PutValHere put variable into line edit from list
     * @param lineEdit lineEdit
     * @param listWidget listWidget
     */
    void             PutValHere(QLineEdit *lineEdit, QListWidget *listWidget);
    /**
     * @brief ValFormulaChanged handle change formula
     * @param flag flag state of formula
     * @param edit LineEdit
     * @param timer timer of formula
     */
    void             ValFormulaChanged(bool &flag, QLineEdit *edit, QTimer * timer);
    /**
     * @brief Eval evaluate formula and show result
     * @param edit lineEdit of formula
     * @param flag flag state of formula
     * @param timer timer of formula
     * @param label label for signal error
     */
    void             Eval(QLineEdit *edit, bool &flag, QTimer *timer, QLabel *label);
    /**
     * @brief setCurrentPointId set current point id in combobox
     * @param box combobox
     * @param pointId save current point id
     * @param value point id
     * @param id don't show this id in list
     */
    void             setCurrentPointId(QComboBox *box, quint32 &pointId, const quint32 &value, const quint32 &id) const;
    /**
     * @brief setCurrentSplineId set current spline id in combobox
     * @param box combobox
     * @param splineId save current spline id
     * @param value spline id
     * @param id don't show this id in list
     * @param cut if set to ComboMode::CutSpline don't show id+1 and id+2
     */
    void             setCurrentSplineId(QComboBox *box, quint32 &splineId, const quint32 &value, const quint32 &id,
                                        ComboMode::ComboBoxCutSpline cut = ComboMode::NoCutSpline) const;
    /**
     * @brief setCurrentArcId
     * @param box combobox
     * @param arcId save current arc id
     * @param value arc id
     * @param id don't show this id in list
     * @param cut if set to ComboMode::CutArc don't show id+1 and id+2
     */
    void             setCurrentArcId(QComboBox *box, quint32 &arcId, const quint32 &value, const quint32 &id,
                                        ComboMode::ComboBoxCutArc cut = ComboMode::NoCutArc) const;
    /**
     * @brief setCurrentSplinePathId set current splinePath id in combobox
     * @param box combobox
     * @param splinePathId save current splinePath id
     * @param value splinePath id
     * @param id don't show this id in list
     * @param cut if set to ComboMode::CutSpline don't show id+1 and id+2
     */
    void             setCurrentSplinePathId(QComboBox *box, quint32 &splinePathId, const quint32 &value, const quint32 &id,
                                            ComboMode::ComboBoxCutSpline cut = ComboMode::NoCutSpline) const;
    /**
     * @brief getCurrentPointId return current point id in combobox
     * @param box combobox
     * @return id or -1 if combobox is empty
     */
    quint32           getCurrentObjectId(QComboBox *box) const;
private:
    /**
     * @brief FillList fill combobox list
     * @param box combobox
     * @param list list with ids and names
     */
    void             FillList(QComboBox *box, const QMap<QString, quint32> &list)const;
};

#endif // DIALOGTOOL_H
