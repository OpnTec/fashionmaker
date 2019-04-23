/************************************************************************
 **
 **  @file   dialogtool.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   November 15, 2013
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentina project, a pattern making
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

template <class T> class QSharedPointer;

Q_LOGGING_CATEGORY(vDialog, "v.dialog")

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief DialogTool create dialog
 * @param data container with data
 * @param parent parent widget
 */
DialogTool::DialogTool(const VContainer *data, quint32 toolId, QWidget *parent)
    : QDialog(parent),
      data(data),
      isInitialized(false),
      bOk(nullptr),
      bApply(nullptr),
      associatedTool(nullptr),
      toolId(toolId),
      prepare(false),
      number(0),
      vis(nullptr)
{
    SCASSERT(data != nullptr)      
}

//---------------------------------------------------------------------------------------------------------------------
DialogTool::~DialogTool()
{
    emit ToolTip(QString());

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

    CheckState();
}

//---------------------------------------------------------------------------------------------------------------------
void DialogTool::keyPressEvent(QKeyEvent *event)
{
    switch (event->key())
    {
        case Qt::Key_Escape:
            DialogRejected();
            return; // After reject the dialog will be destroyed, exit imidiately
        default:
            break;
    }
    QDialog::keyPressEvent ( event );
}

//---------------------------------------------------------------------------------------------------------------------
void DialogTool::FillComboBoxPiecesList(QComboBox *box, const QVector<quint32> &list)
{
    SCASSERT(box != nullptr)
    box->blockSignals(true);
    box->clear();
    for (auto id : list)
    {
        box->addItem(data->GetPiece(id).GetName(), id);
    }
    box->blockSignals(false);
    box->setCurrentIndex(-1); // Force a user to choose
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief FillComboBoxPoints fill comboBox list of points
 * @param box comboBox
 */
void DialogTool::FillComboBoxPoints(QComboBox *box, FillComboBox rule, quint32 ch1, quint32 ch2) const
{
    FillCombo<VPointF>(box, GOType::Point, rule, ch1, ch2);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogTool::FillComboBoxArcs(QComboBox *box, FillComboBox rule, quint32 ch1, quint32 ch2) const
{
    FillCombo<VAbstractCurve>(box, GOType::Arc, rule, ch1, ch2);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogTool::FillComboBoxSplines(QComboBox *box) const
{
    SCASSERT(box != nullptr)
    box->blockSignals(true);

    const auto objs = data->CalculationGObjects();
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

    const auto objs = data->CalculationGObjects();
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
    const auto objs = data->CalculationGObjects();
    QMap<QString, quint32> list;
    QHash<quint32, QSharedPointer<VGObject> >::const_iterator i;
    for (i = objs->constBegin(); i != objs->constEnd(); ++i)
    {
        if (i.key() != toolId)
        {
            QSharedPointer<VGObject> obj = i.value();
            if (obj->getType() == GOType::Arc
                || obj->getType() == GOType::EllipticalArc
                || obj->getType() == GOType::Spline
                || obj->getType() == GOType::SplinePath
                || obj->getType() == GOType::CubicBezier
                || obj->getType() == GOType::CubicBezierPath)
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
        box->addItem(i.value(), QString(), QVariant(i.key()));
        ++i;
    }

    const int index = box->findData(QVariant(TypeLineLine));
    if (index != -1)
    {
        box->setCurrentIndex(index);
    }
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
    value = box->currentData().toString();

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
bool DialogTool::eventFilter(QObject *object, QEvent *event)
{
    const bool fitered = FilterObject(object, event);
    if (fitered)
    {
        return fitered;
    }

    return QDialog::eventFilter(object, event);
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
QString DialogTool::GetNodeName(const VPieceNode &node, bool showDetails) const
{
    const QSharedPointer<VGObject> obj = data->GetGObject(node.GetId());
    QString name = obj->name();

    if (node.GetTypeTool() != Tool::NodePoint)
    {
        int bias = 0;
        qApp->TrVars()->VariablesToUser(name, 0, obj->name(), bias);

        if (showDetails && node.GetReverse())
        {
            name = QLatin1String("- ") + name;
        }
    }
    else
    {
        if (showDetails && node.IsPassmark())
        {
            switch(node.GetPassmarkLineType())
            {
                case PassmarkLineType::OneLine:
                    name += QLatin1Char('|');
                    break;
                case PassmarkLineType::TwoLines:
                    name += QLatin1Literal("||");
                    break;
                case PassmarkLineType::ThreeLines:
                    name += QLatin1Literal("|||");
                    break;
                case PassmarkLineType::TMark:
                    name += QStringLiteral("┴");
                    break;
                case PassmarkLineType::VMark:
                    name += QStringLiteral("⊼");
                    break;
                case PassmarkLineType::VMark2:
                    name += QStringLiteral("⊽");
                    break;
                case PassmarkLineType::UMark:
                    name += QStringLiteral("⋃");
                    break;
                case PassmarkLineType::BoxMark:
                    name += QStringLiteral("⎕");
                    break;
                default:
                    break;
            }
        }

        if (showDetails && not node.IsCheckUniqueness())
        {
            name = QLatin1Char('[') + name + QLatin1Char(']');
        }
    }

    return name;
}

//---------------------------------------------------------------------------------------------------------------------
void DialogTool::NewNodeItem(QListWidget *listWidget, const VPieceNode &node, bool showPassmark)
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
            name = GetNodeName(node, showPassmark);
            break;
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
        if(RowNode(listWidget, listWidget->count()-1).GetId() != node.GetId())
        {
            canAddNewPoint = true;
        }
    }

    if(canAddNewPoint)
    {
        QListWidgetItem *item = new QListWidgetItem(name);
        item->setFont(NodeFont(item->font(), node.IsExcluded()));
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
void DialogTool::MoveListRowTop(QListWidget *list)
{
    SCASSERT(list != nullptr)
    const int currentIndex = list->currentRow();
    if (QListWidgetItem *currentItem = list->takeItem(currentIndex))
    {
        list->insertItem(0, currentItem);
        list->setCurrentRow(0);
    }
}

//---------------------------------------------------------------------------------------------------------------------
void DialogTool::MoveListRowUp(QListWidget *list)
{
    SCASSERT(list != nullptr)
    int currentIndex = list->currentRow();
    if (QListWidgetItem *currentItem = list->takeItem(currentIndex--))
    {
        if (currentIndex < 0)
        {
            currentIndex = 0;
        }
        list->insertItem(currentIndex, currentItem);
        list->setCurrentRow(currentIndex);
    }
}

//---------------------------------------------------------------------------------------------------------------------
void DialogTool::MoveListRowDown(QListWidget *list)
{
    SCASSERT(list != nullptr)
    int currentIndex = list->currentRow();
    if (QListWidgetItem *currentItem = list->takeItem(currentIndex++))
    {
        if (currentIndex > list->count())
        {
            currentIndex = list->count();
        }
        list->insertItem(currentIndex, currentItem);
        list->setCurrentRow(currentIndex);
    }
}

//---------------------------------------------------------------------------------------------------------------------
void DialogTool::MoveListRowBottom(QListWidget *list)
{
    SCASSERT(list != nullptr)
    const int currentIndex = list->currentRow();
    if (QListWidgetItem *currentItem = list->takeItem(currentIndex))
    {
        list->insertItem(list->count(), currentItem);
        list->setCurrentRow(list->count()-1);
    }
}

//---------------------------------------------------------------------------------------------------------------------
bool DialogTool::IsSplinePath(const QSharedPointer<VGObject> &obj) const
{
    return obj->getType() == GOType::SplinePath || obj->getType() == GOType::CubicBezierPath;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief Eval evaluate formula and show result
 * @param formulaData options to control parsing
 */
qreal DialogTool::Eval(const FormulaData &formulaData, bool &flag)
{
    const qreal result = EvalToolFormula(this, formulaData, flag);
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
    return obj->getType() == GOType::Spline || obj->getType() == GOType::CubicBezier;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief CheckState enable, when all is correct, or disable, when something wrong, button ok
 */
void DialogTool::CheckState()
{
    SCASSERT(bOk != nullptr)
    bOk->setEnabled(IsValid());
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

    const QHash<quint32, QSharedPointer<VGObject> > *objs = data->CalculationGObjects();
    QHash<quint32, QSharedPointer<VGObject> >::const_iterator i;
    QMap<QString, quint32> list;
    for (i = objs->constBegin(); i != objs->constEnd(); ++i)
    {
        if (rule == FillComboBox::NoChildren)
        {
            if (i.key() != toolId && i.value()->getIdTool() != toolId && i.key() != ch1 && i.key() != ch2)
            {
                QSharedPointer<VGObject> obj = i.value();
                if (obj->getType() == gType)
                {
                    PrepareList<GObject>(list, i.key());
                }
            }
        }
        else
        {
            if (i.key() != toolId && i.value()->getIdTool() != toolId)
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
