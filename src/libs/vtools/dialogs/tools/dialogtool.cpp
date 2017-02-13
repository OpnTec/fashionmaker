/************************************************************************
 **
 **  @file   dialogtool.cpp
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

#include "dialogtool.h"

#include <limits.h>
#include <qiterator.h>
#include <qnumeric.h>
#include <QCloseEvent>
#include <QComboBox>
#include <QDoubleSpinBox>
#include <QEvent>
#include <QHash>
#include <QIcon>
#include <QKeyEvent>
#include <QLabel>
#include <QLineEdit>
#include <QMapIterator>
#include <QMessageLogger>
#include <QPalette>
#include <QPixmap>
#include <QPlainTextEdit>
#include <QPushButton>
#include <QRegularExpression>
#include <QRegularExpressionMatch>
#include <QScopedPointer>
#include <QSharedPointer>
#include <QShowEvent>
#include <QSize>
#include <QTextCursor>
#include <QTimer>
#include <QWidget>
#include <Qt>
#include <QtDebug>
#include <new>
#include <QBuffer>

#include "../ifc/xml/vdomdocument.h"
#include "../qmuparser/qmudef.h"
#include "../qmuparser/qmuparsererror.h"
#include "../vgeometry/vpointf.h"
#include "../vpatterndb/calculator.h"
#include "../vpatterndb/vcontainer.h"
#include "../vpatterndb/vtranslatevars.h"
#include "../vpatterndb/vpiecenode.h"
#include "../../tools/vabstracttool.h"
#include "../ifc/xml/vabstractpattern.h"
#include "../vgeometry/vabstractcurve.h"
#include "../vgeometry/vgobject.h"

class QCloseEvent;
class QKeyEvent;
class QShowEvent;
template <class T> class QSharedPointer;

Q_LOGGING_CATEGORY(vDialog, "v.dialog")

#define DIALOG_MAX_FORMULA_HEIGHT 64

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief DialogTool create dialog
 * @param data container with data
 * @param parent parent widget
 */
DialogTool::DialogTool(const VContainer *data, const quint32 &toolId, QWidget *parent)
    :QDialog(parent), data(data), isInitialized(false), flagName(true), flagFormula(true), flagError(true),
      timerFormula(nullptr), bOk(nullptr), bApply(nullptr), spinBoxAngle(nullptr), plainTextEditFormula(nullptr),
      labelResultCalculation(nullptr), labelEditNamePoint(nullptr), labelEditFormula(nullptr),
      okColor(QColor(76, 76, 76)), errorColor(Qt::red), associatedTool(nullptr),
      toolId(toolId), prepare(false), pointName(QString()), number(0), vis(nullptr)
{
    SCASSERT(data != nullptr)
    timerFormula = new QTimer(this);
    connect(timerFormula, &QTimer::timeout, this, &DialogTool::EvalFormula);
}


//---------------------------------------------------------------------------------------------------------------------
DialogTool::~DialogTool()
{
    emit ToolTip("");

    if (not vis.isNull())
    {
        delete vis;
    }
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief closeEvent handle when dialog cloded
 * @param event event
 */
void DialogTool::closeEvent(QCloseEvent *event)
{
    DialogRejected();
    event->accept();
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief showEvent handle when window show
 * @param event event
 */
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
    // do your init stuff here

    setMaximumSize(size());
    setMinimumSize(size());

    isInitialized = true;//first show windows are held
    ShowVisualization();
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief FillComboBoxPoints fill comboBox list of points
 * @param box comboBox
 */
void DialogTool::FillComboBoxPoints(QComboBox *box, FillComboBox rule, const quint32 &ch1, const quint32 &ch2) const
{
    FillCombo<VPointF>(box, GOType::Point, rule, ch1, ch2);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogTool::FillComboBoxArcs(QComboBox *box, FillComboBox rule, const quint32 &ch1, const quint32 &ch2) const
{
    FillCombo<VAbstractCurve>(box, GOType::Arc, rule, ch1, ch2);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogTool::FillComboBoxSplines(QComboBox *box) const
{
    SCASSERT(box != nullptr)
    box->blockSignals(true);

    const auto objs = data->DataGObjects();
    QHash<quint32, QSharedPointer<VGObject> >::const_iterator i;
    QMap<QString, quint32> list;
    for (i = objs->constBegin(); i != objs->constEnd(); ++i)
    {
        if (i.key() != toolId)
        {
            if (IsSpline(i.value()))
            {
                PrepareList<VAbstractCurve>(list, i.key());
            }
        }
    }
    FillList(box, list);

    box->blockSignals(false);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogTool::FillComboBoxSplinesPath(QComboBox *box) const
{
    SCASSERT(box != nullptr)
    box->blockSignals(true);

    const auto objs = data->DataGObjects();
    QHash<quint32, QSharedPointer<VGObject> >::const_iterator i;
    QMap<QString, quint32> list;
    for (i = objs->constBegin(); i != objs->constEnd(); ++i)
    {
        if (i.key() != toolId)
        {
            if (IsSplinePath(i.value()))
            {
                PrepareList<VAbstractCurve>(list, i.key());
            }
        }
    }
    FillList(box, list);

    box->blockSignals(false);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogTool::FillComboBoxCurves(QComboBox *box) const
{
    SCASSERT(box != nullptr)
    const auto objs = data->DataGObjects();
    QMap<QString, quint32> list;
    QHash<quint32, QSharedPointer<VGObject> >::const_iterator i;
    for (i = objs->constBegin(); i != objs->constEnd(); ++i)
    {
        if (i.key() != toolId)
        {
            QSharedPointer<VGObject> obj = i.value();
            if ((obj->getType() == GOType::Arc
                 || obj->getType() == GOType::EllipticalArc
                 || obj->getType() == GOType::Spline
                 || obj->getType() == GOType::SplinePath
                 || obj->getType() == GOType::CubicBezier
                 || obj->getType() == GOType::CubicBezierPath) && obj->getMode() == Draw::Calculation)
            {
                PrepareList<VAbstractCurve>(list, i.key());
            }
        }
    }
    FillList(box, list);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief FillComboBoxTypeLine fill comboBox list of type lines
 * @param box comboBox
 */
void DialogTool::FillComboBoxTypeLine(QComboBox *box, const QMap<QString, QIcon> &stylesPics) const
{
    SCASSERT(box != nullptr)
    QMap<QString, QIcon>::const_iterator i = stylesPics.constBegin();
    while (i != stylesPics.constEnd())
    {
        box->addItem(i.value(), "", QVariant(i.key()));
        ++i;
    }

    box->setCurrentIndex(4);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogTool::FillComboBoxLineColors(QComboBox *box) const
{
    SCASSERT(box != nullptr)

    box->clear();
    int size = box->iconSize().height();
    // On Mac pixmap should be little bit smaller.
#if defined(Q_OS_MAC)
    size -= 2; // Two pixels should be enough.
#endif //defined(Q_OS_MAC)

    const QMap<QString, QString> map = VAbstractTool::ColorsList();
    QMap<QString, QString>::const_iterator i = map.constBegin();
    while (i != map.constEnd())
    {
        QPixmap pix(size, size);
        pix.fill(QColor(i.key()));
        box->addItem(QIcon(pix), i.value(), QVariant(i.key()));
        ++i;
    }
}

//---------------------------------------------------------------------------------------------------------------------
void DialogTool::FillComboBoxCrossCirclesPoints(QComboBox *box) const
{
    SCASSERT(box != nullptr)

    box->addItem(tr("First point"), QVariant(static_cast<int>(CrossCirclesPoint::FirstPoint)));
    box->addItem(tr("Second point"), QVariant(static_cast<int>(CrossCirclesPoint::SecondPoint)));
}

//---------------------------------------------------------------------------------------------------------------------
void DialogTool::FillComboBoxVCrossCurvesPoint(QComboBox *box) const
{
    SCASSERT(box != nullptr)

    box->addItem(tr("Highest point"), QVariant(static_cast<int>(VCrossCurvesPoint::HighestPoint)));
    box->addItem(tr("Lowest point"), QVariant(static_cast<int>(VCrossCurvesPoint::LowestPoint)));
}

//---------------------------------------------------------------------------------------------------------------------
void DialogTool::FillComboBoxHCrossCurvesPoint(QComboBox *box) const
{
    SCASSERT(box != nullptr)

    box->addItem(tr("Leftmost point"), QVariant(static_cast<int>(HCrossCurvesPoint::LeftmostPoint)));
    box->addItem(tr("Rightmost point"), QVariant(static_cast<int>(HCrossCurvesPoint::RightmostPoint)));
}

//---------------------------------------------------------------------------------------------------------------------
QString DialogTool::GetComboBoxCurrentData(const QComboBox *box, const QString &def) const
{
    SCASSERT(box != nullptr)
    QString value;
#if QT_VERSION < QT_VERSION_CHECK(5, 2, 0)
    value = box->itemData(box->currentIndex()).toString();
#else
    value = box->currentData().toString();
#endif
    if (value.isEmpty())
    {
        value = def;
    }
    return value;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief ChangeCurrentData select item in combobox by id
 * @param box combobox
 * @param value id of item
 */
void DialogTool::ChangeCurrentData(QComboBox *box, const QVariant &value) const
{
    SCASSERT(box != nullptr)
    const qint32 index = box->findData(value);
    if (index != -1)
    {
        box->setCurrentIndex(index);
    }
}

//---------------------------------------------------------------------------------------------------------------------
void DialogTool::MoveCursorToEnd(QPlainTextEdit *plainTextEdit)
{
    SCASSERT(plainTextEdit != nullptr)
    QTextCursor cursor = plainTextEdit->textCursor();
    cursor.movePosition(QTextCursor::End, QTextCursor::MoveAnchor);
    plainTextEdit->setTextCursor(cursor);
}

//---------------------------------------------------------------------------------------------------------------------
bool DialogTool::eventFilter(QObject *object, QEvent *event)
{
    if (QPlainTextEdit *plainTextEdit = qobject_cast<QPlainTextEdit *>(object))
    {
        if (event->type() == QEvent::KeyPress)
        {
            QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
            if ((keyEvent->key() == Qt::Key_Enter) || (keyEvent->key() == Qt::Key_Return))
            {
                // Ignore Enter key
                return true;
            }
            else if ((keyEvent->key() == Qt::Key_Period) && (keyEvent->modifiers() & Qt::KeypadModifier))
            {
                if (qApp->Settings()->GetOsSeparator())
                {
                    plainTextEdit->insertPlainText(QLocale().decimalPoint());
                }
                else
                {
                    plainTextEdit->insertPlainText(QLocale::c().decimalPoint());
                }
                return true;
            }
        }
    }
    else
    {
        // pass the event on to the parent class
        return QDialog::eventFilter(object, event);
    }
    return false;// pass the event to the widget
}

//---------------------------------------------------------------------------------------------------------------------
quint32 DialogTool::DNumber(const QString &baseName) const
{
    quint32 num = 0;
    QString name;
    do
    {
        ++num;
        name = baseName + QString("_%1").arg(num);
    } while (not data->IsUnique(name));

    return num;
}

//---------------------------------------------------------------------------------------------------------------------
quint32 DialogTool::RowId(QListWidget *listWidget, int i)
{
    SCASSERT(listWidget != nullptr);
    const QListWidgetItem *rowItem = listWidget->item(i);
    SCASSERT(rowItem != nullptr);
    const VPieceNode rowNode = qvariant_cast<VPieceNode>(rowItem->data(Qt::UserRole));
    return rowNode.GetId();
}

//---------------------------------------------------------------------------------------------------------------------
bool DialogTool::FirstPointEqualLast(QListWidget *listWidget)
{
    SCASSERT(listWidget != nullptr);
    if (listWidget->count() > 1)
    {
        return RowId(listWidget, 0) == RowId(listWidget, listWidget->count()-1);
    }
    return false;
}

//---------------------------------------------------------------------------------------------------------------------
bool DialogTool::DoublePoints(QListWidget *listWidget)
{
    SCASSERT(listWidget != nullptr);
    for (int i=0, sz = listWidget->count()-1; i<sz; ++i)
    {
        if (RowId(listWidget, i) == RowId(listWidget, i+1))
        {
            return true;
        }
    }
    return false;
}

//---------------------------------------------------------------------------------------------------------------------
QString DialogTool::DialogWarningIcon()
{
    const QIcon icon = QIcon::fromTheme("dialog-warning",
                                  QIcon(":/icons/win.icon.theme/16x16/status/dialog-warning.png"));

    const QPixmap pixmap = icon.pixmap(QSize(16, 16));
    QByteArray byteArray;
    QBuffer buffer(&byteArray);
    pixmap.save(&buffer, "PNG");
    const QString url = QString("<img src=\"data:image/png;base64,") + byteArray.toBase64() + QLatin1String("\"/> ");
    return url;
}

//---------------------------------------------------------------------------------------------------------------------
QString DialogTool::GetNodeName(const VPieceNode &node) const
{
    const QSharedPointer<VGObject> obj = data->GeometricObject<VGObject>(node.GetId());
    QString name = obj->name();

    if (node.GetTypeTool() != Tool::NodePoint && node.GetReverse())
    {
        name = QLatin1String("- ") + name;
    }

    return name;
}

//---------------------------------------------------------------------------------------------------------------------
void DialogTool::NewNodeItem(QListWidget *listWidget, const VPieceNode &node)
{
    SCASSERT(listWidget != nullptr);
    SCASSERT(node.GetId() > NULL_ID);
    QString name;
    switch (node.GetTypeTool())
    {
        case (Tool::NodePoint):
        case (Tool::NodeArc):
        case (Tool::NodeElArc):
        case (Tool::NodeSpline):
        case (Tool::NodeSplinePath):
        {
            name = GetNodeName(node);
            break;
        }
        default:
            qDebug()<<"Got wrong tools. Ignore.";
            return;
    }

    bool canAddNewPoint = false;

    if(listWidget->count() == 0)
    {
        canAddNewPoint = true;
    }
    else
    {
        if(RowId(listWidget, listWidget->count()-1) != node.GetId())
        {
            canAddNewPoint = true;
        }
    }

    if(canAddNewPoint)
    {
        QListWidgetItem *item = new QListWidgetItem(name);
        item->setFont(QFont("Times", 12, QFont::Bold));
        item->setData(Qt::UserRole, QVariant::fromValue(node));
        listWidget->addItem(item);
        listWidget->setCurrentRow(listWidget->count()-1);
    }
}

//---------------------------------------------------------------------------------------------------------------------
void DialogTool::InitNodeAngles(QComboBox *box)
{
    SCASSERT(box != nullptr);
    box->clear();

    box->addItem(tr("by length"), static_cast<unsigned char>(PieceNodeAngle::ByLength));
    box->addItem(tr("by points intersetions"), static_cast<unsigned char>(PieceNodeAngle::ByPointsIntersection));
    box->addItem(tr("by first edge symmetry"), static_cast<unsigned char>(PieceNodeAngle::ByFirstEdgeSymmetry));
    box->addItem(tr("by second edge symmetry"), static_cast<unsigned char>(PieceNodeAngle::BySecondEdgeSymmetry));
    box->addItem(tr("by first edge right angle"), static_cast<unsigned char>(PieceNodeAngle::ByFirstEdgeRightAngle));
    box->addItem(tr("by second edge right angle"), static_cast<unsigned char>(PieceNodeAngle::BySecondEdgeRightAngle));
}

//---------------------------------------------------------------------------------------------------------------------
bool DialogTool::IsSplinePath(const QSharedPointer<VGObject> &obj) const
{
    return (obj->getType() == GOType::SplinePath || obj->getType() == GOType::CubicBezierPath) &&
            obj->getMode() == Draw::Calculation;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief ValFormulaChanged handle change formula
 * @param flag flag state of formula
 * @param edit LineEdit
 * @param timer timer of formula
 */
void DialogTool::ValFormulaChanged(bool &flag, QLineEdit *edit, QTimer *timer, const QString& postfix)
{
    SCASSERT(edit != nullptr)
    SCASSERT(timer != nullptr)
    SCASSERT(labelEditFormula != nullptr)
    SCASSERT(labelResultCalculation != nullptr)
    if (edit->text().isEmpty())
    {
        flag = false;
        CheckState();
        ChangeColor(labelEditFormula, Qt::red);
        if (postfix.isEmpty())
        {
            labelResultCalculation->setText(tr("Error"));
        }
        else
        {
            labelResultCalculation->setText(tr("Error") + " (" + postfix + ")");
        }
        labelResultCalculation->setToolTip(tr("Empty field"));
        return;
    }
    timer->start(1000);
}
//---------------------------------------------------------------------------------------------------------------------
void DialogTool::ValFormulaChanged(bool &flag, QPlainTextEdit *edit, QTimer *timer, const QString& postfix)
{
    SCASSERT(edit != nullptr)
    SCASSERT(timer != nullptr)
    SCASSERT(labelEditFormula != nullptr)
    SCASSERT(labelResultCalculation != nullptr)
    if (edit->toPlainText().isEmpty())
    {
        flag = false;
        CheckState();
        ChangeColor(labelEditFormula, Qt::red);
        if (postfix.isEmpty())
        {
            labelResultCalculation->setText(tr("Error"));
        }
        else
        {
            labelResultCalculation->setText(tr("Error") + " (" + postfix + ")");
        }

        labelResultCalculation->setToolTip(tr("Empty field"));
        return;
    }
    timer->setSingleShot(true);
    timer->start(300);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief Eval evaluate formula and show result
 * @param text expresion that we parse
 * @param flag flag state of eval formula
 * @param label label for signal error
 * @param postfix unit name
 * @param checkZero true - if formula can't be equal zero
 */
qreal DialogTool::Eval(const QString &text, bool &flag, QLabel *label, const QString& postfix, bool checkZero,
                       bool checkLessThanZero)
{
    SCASSERT(label != nullptr)
    SCASSERT(labelEditFormula != nullptr)

    qreal result = INT_MIN;//Value can be 0, so use max imposible value

    if (text.isEmpty())
    {
        flag = false;
        ChangeColor(labelEditFormula, Qt::red);
        label->setText(tr("Error") + " (" + postfix + ")");
        label->setToolTip(tr("Empty field"));
    }
    else
    {
        try
        {
            // Replace line return character with spaces for calc if exist
            QString formula = text;
            formula.replace("\n", " ");
            // Translate to internal look.
            formula = qApp->TrVars()->FormulaFromUser(formula, qApp->Settings()->GetOsSeparator());
            QScopedPointer<Calculator> cal(new Calculator());
            result = cal->EvalFormula(data->PlainVariables(), formula);

            if (qIsInf(result) || qIsNaN(result))
            {
                flag = false;
                ChangeColor(labelEditFormula, Qt::red);
                label->setText(tr("Error") + " (" + postfix + ")");
                label->setToolTip(tr("Invalid value"));
            }
            else
            {
                //if result equal 0
                if (checkZero && qFuzzyIsNull(result))
                {
                    flag = false;
                    ChangeColor(labelEditFormula, Qt::red);
                    label->setText(tr("Error") + " (" + postfix + ")");
                    label->setToolTip(tr("Value can't be 0"));
                }
                else if (checkLessThanZero && result < 0)
                {
                    flag = false;
                    ChangeColor(labelEditFormula, Qt::red);
                    label->setText(tr("Error") + " (" + postfix + ")");
                    label->setToolTip(tr("Value can't be lass than 0"));
                }
                else
                {
                    label->setText(qApp->LocaleToString(result) + " " +postfix);
                    flag = true;
                    ChangeColor(labelEditFormula, okColor);
                    label->setToolTip(tr("Value"));
                    emit ToolTip("");
                }
            }
        }
        catch (qmu::QmuParserError &e)
        {
            label->setText(tr("Error") + " (" + postfix + ")");
            flag = false;
            ChangeColor(labelEditFormula, Qt::red);
            emit ToolTip(tr("Parser error: %1").arg(e.GetMsg()));
            label->setToolTip(tr("Parser error: %1").arg(e.GetMsg()));
            qDebug() << "\nMath parser error:\n"
                     << "--------------------------------------\n"
                     << "Message:     " << e.GetMsg()  << "\n"
                     << "Expression:  " << e.GetExpr() << "\n"
                     << "--------------------------------------";
        }
    }
    CheckState(); // Disable Ok and Apply buttons if something wrong.
    return result;
}

//---------------------------------------------------------------------------------------------------------------------
void DialogTool::setCurrentPointId(QComboBox *box, const quint32 &value, FillComboBox rule,
                                   const quint32 &ch1, const quint32 &ch2) const
{
    SCASSERT(box != nullptr)

    box->blockSignals(true);

    FillComboBoxPoints(box, rule, ch1, ch2);
    ChangeCurrentData(box, value);

    box->blockSignals(false);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief setCurrentSplineId set current spline id in combobox
 */
void DialogTool::setCurrentSplineId(QComboBox *box, const quint32 &value) const
{
    SCASSERT(box != nullptr)
    FillComboBoxSplines(box);
    ChangeCurrentData(box, value);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief setCurrentArcId
 */
void DialogTool::setCurrentArcId(QComboBox *box, const quint32 &value, FillComboBox rule,
                                 const quint32 &ch1, const quint32 &ch2) const
{
    SCASSERT(box != nullptr)
    FillComboBoxArcs(box, rule, ch1, ch2);
    ChangeCurrentData(box, value);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief setCurrentSplinePathId set current splinePath id in combobox
 * @param box combobox
 * @param value splinePath id
 */
void DialogTool::setCurrentSplinePathId(QComboBox *box, const quint32 &value) const
{
    SCASSERT(box != nullptr)
    FillComboBoxSplinesPath(box);
    ChangeCurrentData(box, value);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogTool::setCurrentCurveId(QComboBox *box, const quint32 &value) const
{
    SCASSERT(box != nullptr)
    FillComboBoxCurves(box);
    ChangeCurrentData(box, value);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief getCurrentPointId return current point id stored in combobox
 * @param box combobox
 * @return id or 0 if combobox is empty
 */
quint32 DialogTool::getCurrentObjectId(QComboBox *box) const
{
    SCASSERT(box != nullptr)
    qint32 index = box->currentIndex();
    if (index != -1)
    {
        return qvariant_cast<quint32>(box->itemData(index));
    }
    else
    {
        return 0;
    }
}

//---------------------------------------------------------------------------------------------------------------------
bool DialogTool::SetObject(const quint32 &id, QComboBox *box, const QString &toolTip)
{
    SCASSERT(box != nullptr)
    const qint32 index = box->findData(id);
    if ( index != -1 )
    { // -1 for not found
        box->setCurrentIndex(index);
        emit ToolTip(toolTip);
        return true;
    }
    else
    {
        qWarning()<<"Can't find object by id"<<id;
    }
    return false;
}

//---------------------------------------------------------------------------------------------------------------------
void DialogTool::DeployFormula(QPlainTextEdit *formula, QPushButton *buttonGrowLength, int formulaBaseHeight)
{
    SCASSERT(formula != nullptr)
    SCASSERT(buttonGrowLength != nullptr)

    const QTextCursor cursor = formula->textCursor();

    //Before deploy ned to release dialog size
    //I don't know why, but don't need to fixate again.
    //A dialog will be lefted fixated. That's what we need.
    setMaximumSize(QSize(QWIDGETSIZE_MAX, QWIDGETSIZE_MAX));
    setMinimumSize(QSize(0, 0));

    if (formula->height() < DIALOG_MAX_FORMULA_HEIGHT)
    {
        formula->setFixedHeight(DIALOG_MAX_FORMULA_HEIGHT);
        //Set icon from theme (internal for Windows system)
        buttonGrowLength->setIcon(QIcon::fromTheme("go-next",
                                                   QIcon(":/icons/win.icon.theme/16x16/actions/go-next.png")));
    }
    else
    {
       formula->setFixedHeight(formulaBaseHeight);
       //Set icon from theme (internal for Windows system)
       buttonGrowLength->setIcon(QIcon::fromTheme("go-down",
                                                  QIcon(":/icons/win.icon.theme/16x16/actions/go-down.png")));
    }

    // I found that after change size of formula field, it was filed for angle formula, field for formula became black.
    // This code prevent this.
    setUpdatesEnabled(false);
    repaint();
    setUpdatesEnabled(true);

    formula->setFocus();
    formula->setTextCursor(cursor);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief FillList fill combobox list
 * @param box combobox
 * @param list list with ids and names
 */
void DialogTool::FillList(QComboBox *box, const QMap<QString, quint32> &list) const
{
    SCASSERT(box != nullptr)
    box->clear();

    QMapIterator<QString, quint32> iter(list);
    while (iter.hasNext())
    {
        iter.next();
        box->addItem(iter.key(), iter.value());
    }
}

//---------------------------------------------------------------------------------------------------------------------
template <typename T>
void DialogTool::PrepareList(QMap<QString, quint32> &list, quint32 id) const
{
    const auto obj = data->GeometricObject<T>(id);
    SCASSERT(obj != nullptr)

    QString newName = obj->name();
    int bias = 0;
    if (qApp->TrVars()->VariablesToUser(newName, 0, obj->name(), bias))
    {
        list[newName] = id;
    }
    else
    {
        list[obj->name()] = id;
    }
}

//---------------------------------------------------------------------------------------------------------------------
bool DialogTool::IsSpline(const QSharedPointer<VGObject> &obj) const
{
    return (obj->getType() == GOType::Spline || obj->getType() == GOType::CubicBezier) &&
            obj->getMode() == Draw::Calculation;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief CheckState enable, when all is correct, or disable, when something wrong, button ok
 */
void DialogTool::CheckState()
{
    SCASSERT(bOk != nullptr)
    bOk->setEnabled(flagFormula && flagName && flagError);
    // In case dialog hasn't apply button
    if ( bApply != nullptr)
    {
        bApply->setEnabled(bOk->isEnabled());
    }
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief ChosenObject gets id and type of selected object. Save right data and ignore wrong.
 * @param id id of point or detail
 * @param type type of object
 */
void DialogTool::ChosenObject(quint32 id, const SceneObject &type)
{
    Q_UNUSED(id)
    Q_UNUSED(type)
}

//---------------------------------------------------------------------------------------------------------------------
void DialogTool::SelectedObject(bool selected, quint32 object, quint32 tool)
{
    Q_UNUSED(selected)
    Q_UNUSED(object)
    Q_UNUSED(tool)
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief NamePointChanged check name of point
 */
void DialogTool::NamePointChanged()
{
    SCASSERT(labelEditNamePoint != nullptr)
    QLineEdit* edit = qobject_cast<QLineEdit*>(sender());
    if (edit)
    {
        QString name = edit->text();
        QRegularExpression rx(NameRegExp());
        if (name.isEmpty() || (pointName != name && data->IsUnique(name) == false) ||
            rx.match(name).hasMatch() == false)
        {
            flagName = false;
            ChangeColor(labelEditNamePoint, Qt::red);
        }
        else
        {
            flagName = true;
            ChangeColor(labelEditNamePoint, okColor);
        }
    }
    CheckState();
}

//---------------------------------------------------------------------------------------------------------------------
void DialogTool::ChangeColor(QWidget *widget, const QColor &color)
{
    SCASSERT(widget != nullptr)
    QPalette palette = widget->palette();
    palette.setColor(widget->foregroundRole(), color);
    widget->setPalette(palette);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief DialogAccepted save data and emit signal about closed dialog.
 */
void DialogTool::DialogAccepted()
{
    SaveData();
    emit DialogClosed(QDialog::Accepted);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogTool::DialogApply()
{
    SaveData();
    emit DialogApplied();
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief DialogRejected emit signal dialog rejected
 */
void DialogTool::DialogRejected()
{
    emit DialogClosed(QDialog::Rejected);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief formula check formula
 */
void DialogTool::FormulaChanged()
{
    QPlainTextEdit* edit = qobject_cast<QPlainTextEdit*>(sender());
    if (edit)
    {
        ValFormulaChanged(flagFormula, edit, timerFormula);
    }
}
//---------------------------------------------------------------------------------------------------------------------
void DialogTool::FormulaChangedPlainText() //-V524
{
    QPlainTextEdit* edit = qobject_cast<QPlainTextEdit*>(sender());
    if (edit)
    {
        ValFormulaChanged(flagFormula, edit, timerFormula);
    }
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief ArrowUp set angle value 90 degree
 */
void DialogTool::ArrowUp()
{
    SCASSERT(spinBoxAngle != nullptr)
    spinBoxAngle->setValue(90);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief ArrowDown set angle value 270 degree
 */
void DialogTool::ArrowDown()
{
    SCASSERT(spinBoxAngle != nullptr)
    spinBoxAngle->setValue(270);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief ArrowLeft set angle value 180 degree
 */
void DialogTool::ArrowLeft()
{
    SCASSERT(spinBoxAngle != nullptr)
    spinBoxAngle->setValue(180);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief ArrowRight set angle value 0 degree
 */
void DialogTool::ArrowRight()
{
    SCASSERT(spinBoxAngle != nullptr)
    spinBoxAngle->setValue(0);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief ArrowLeftUp set angle value 135 degree
 */
void DialogTool::ArrowLeftUp()
{
    SCASSERT(spinBoxAngle != nullptr)
    spinBoxAngle->setValue(135);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief ArrowLeftDown set angle value 225 degree
 */
void DialogTool::ArrowLeftDown()
{
    SCASSERT(spinBoxAngle != nullptr)
    spinBoxAngle->setValue(225);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief ArrowRightUp set angle value 45 degree
 */
void DialogTool::ArrowRightUp()
{
    SCASSERT(spinBoxAngle != nullptr)
    spinBoxAngle->setValue(45);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief ArrowRightDown set angle value 315 degree
 */
void DialogTool::ArrowRightDown()
{
    SCASSERT(spinBoxAngle != nullptr)
    spinBoxAngle->setValue(315);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief EvalFormula evaluate formula
 */
void DialogTool::EvalFormula()
{
    SCASSERT(plainTextEditFormula != nullptr)
    SCASSERT(labelResultCalculation != nullptr)
    const QString postfix = VDomDocument::UnitsToStr(qApp->patternUnit());//Show unit in dialog lable (cm, mm or inch)
    Eval(plainTextEditFormula->toPlainText(), flagFormula, labelResultCalculation, postfix, false);
}

//---------------------------------------------------------------------------------------------------------------------
// cppcheck-suppress unusedFunction
quint32 DialogTool::GetToolId() const
{
    return toolId;
}

//---------------------------------------------------------------------------------------------------------------------
void DialogTool::SetToolId(const quint32 &value)
{
    toolId = value;
}

//---------------------------------------------------------------------------------------------------------------------
QString DialogTool::getPointName() const
{
    return pointName;
}

//---------------------------------------------------------------------------------------------------------------------
void DialogTool::ShowDialog(bool click)
{
    Q_UNUSED(click)
}

//---------------------------------------------------------------------------------------------------------------------
void DialogTool::Build(const Tool &type)
{
    Q_UNUSED(type)
}

//---------------------------------------------------------------------------------------------------------------------
void DialogTool::SetPiecesList(const QVector<quint32> &list)
{
    Q_UNUSED(list);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogTool::SetAssociatedTool(VAbstractTool *tool)
{
    if (tool != nullptr)
    {
        associatedTool = tool;
        SetToolId(tool->getId());
        data = tool->getData();
        if (not vis.isNull())
        {
            vis->SetData(data);
        }
    }
    else
    {
        associatedTool = nullptr;
        SetToolId(NULL_ID);
    }
}

//---------------------------------------------------------------------------------------------------------------------
template <typename GObject>
void DialogTool::FillCombo(QComboBox *box, GOType gType, FillComboBox rule, const quint32 &ch1,
                           const quint32 &ch2) const
{
    SCASSERT(box != nullptr)
    box->blockSignals(true);

    const QHash<quint32, QSharedPointer<VGObject> > *objs = data->DataGObjects();
    QHash<quint32, QSharedPointer<VGObject> >::const_iterator i;
    QMap<QString, quint32> list;
    for (i = objs->constBegin(); i != objs->constEnd(); ++i)
    {
        if (rule == FillComboBox::NoChildren)
        {
            if (i.key() != toolId && i.key() != ch1 && i.key() != ch2)
            {
                QSharedPointer<VGObject> obj = i.value();
                if (obj->getType() == gType && obj->getMode() == Draw::Calculation)
                {
                    PrepareList<GObject>(list, i.key());
                }
            }
        }
        else
        {
            if (i.key() != toolId)
            {
                QSharedPointer<VGObject> obj = i.value();
                if (obj->getType() == gType && obj->getMode() == Draw::Calculation)
                {
                    PrepareList<GObject>(list, i.key());
                }
            }
        }
    }
    FillList(box, list);

    box->blockSignals(false);
}
