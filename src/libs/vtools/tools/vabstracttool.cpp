/************************************************************************
 **
 **  @file   vabstracttool.cpp
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

#include "vabstracttool.h"
#include "../../vpropertyexplorer/checkablemessagebox.h"
#include "../../vgeometry/vpointf.h"
#include "../../vwidgets/vmaingraphicsview.h"
#include "../../vmisc/vsettings.h"
#include "../undocommands/deltool.h"
#include "../undocommands/savetooloptions.h"
#include "../vwidgets/vgraphicssimpletextitem.h"

#include <QGraphicsView>
#include <QIcon>
#include <QStyle>
#include <QMessageBox>
#include <QtCore/qmath.h>

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief VAbstractTool container.
 * @param doc dom document container.
 * @param data container with data.
 * @param id object id in container.
 * @param parent parent object.
 */
VAbstractTool::VAbstractTool(VAbstractPattern *doc, VContainer *data, quint32 id, QObject *parent)
    :VDataTool(data, parent), doc(doc), id(id), baseColor(Qt::black), vis(nullptr)
{
    SCASSERT(doc != nullptr);
    connect(this, &VAbstractTool::toolhaveChange, this->doc, &VAbstractPattern::haveLiteChange);
    connect(this->doc, &VAbstractPattern::FullUpdateFromFile, this, &VAbstractTool::FullUpdateFromFile);
    connect(this, &VAbstractTool::LiteUpdateTree, this->doc, &VAbstractPattern::LiteParseTree);
}

//---------------------------------------------------------------------------------------------------------------------
VAbstractTool::~VAbstractTool()
{}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief DeleteTool full delete object form scene and file.
 */
void VAbstractTool::DeleteTool(bool ask)
{
    qCDebug(vTool, "Deleting abstract tool.");
    if (_referens <= 1)
    {
        qCDebug(vTool, "No children.");
        qApp->getSceneView()->itemClicked(nullptr);
        if (ask)
        {
            qCDebug(vTool, "Asking.");
            if (ConfirmDeletion() == QMessageBox::No)
            {
                qCDebug(vTool, "User said no.");
                return;
            }
        }

        qCDebug(vTool, "Begin deleting.");
        DelTool *delTool = new DelTool(doc, id);
        connect(delTool, &DelTool::NeedFullParsing, doc, &VAbstractPattern::NeedFullParsing);
        qApp->getUndoStack()->push(delTool);

        // Throw exception, this will help prevent case when we forget to immediately quit function.
        VExceptionToolWasDeleted e("Tool was used after deleting.");
        throw e;
    }
    else
    {
        qCDebug(vTool, "Can't delete, tool has children.");
    }
}

//---------------------------------------------------------------------------------------------------------------------
int VAbstractTool::ConfirmDeletion()
{
    if (false == qApp->Settings()->GetConfirmItemDelete())
    {
        return QMessageBox::Yes;
    }

    Utils::CheckableMessageBox msgBox(qApp->getMainWindow());
    msgBox.setWindowTitle(tr("Confirm deletion"));
    msgBox.setText(tr("Do you really want to delete?"));
    msgBox.setStandardButtons(QDialogButtonBox::Yes | QDialogButtonBox::No);
    msgBox.setDefaultButton(QDialogButtonBox::No);
    msgBox.setIconPixmap(qApp->style()->standardIcon(QStyle::SP_MessageBoxQuestion).pixmap(32, 32) );

    int dialogResult = msgBox.exec();

    if (dialogResult == QDialog::Accepted)
    {
        qApp->Settings()->SetConfirmItemDelete(not msgBox.isChecked());
    }

    return dialogResult == QDialog::Accepted ? QMessageBox::Yes : QMessageBox::No;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief LineStyle return pen style for current line style.
 * @return pen style.
 */
Qt::PenStyle VAbstractTool::LineStyleToPenStyle(const QString &typeLine)
{
    const QStringList styles = StylesList();
    switch (styles.indexOf(typeLine))
    {
        case 0: // TypeLineNone
            return Qt::NoPen;
            break;
        case 1: // TypeLineLine
            return Qt::SolidLine;
            break;
        case 2: // TypeLineDashLine
            return Qt::DashLine;
            break;
        case 3: // TypeLineDotLine
            return Qt::DotLine;
            break;
        case 4: // TypeLineDashDotLine
            return Qt::DashDotLine;
            break;
        case 5: // TypeLineDashDotDotLine
            return Qt::DashDotDotLine;
            break;
        default:
            return Qt::SolidLine;
            break;
    }
}

//---------------------------------------------------------------------------------------------------------------------
QMap<QString, QIcon> VAbstractTool::LineStylesPics()
{
    QMap<QString, QIcon> map;
    const QStringList styles = StylesList();

    for (int i=0; i < styles.size(); ++i)
    {
        const Qt::PenStyle style = LineStyleToPenStyle(styles.at(i));
        QPixmap pix(80, 14);
        pix.fill(Qt::white);

        QBrush brush(Qt::black);
        QPen pen(brush, 2.5, style);

        QPainter painter(&pix);
        painter.setPen(pen);
        painter.drawLine(2, 7, 78, 7);

        map.insert(styles.at(i), QIcon(pix));
    }
    return map;
}

//---------------------------------------------------------------------------------------------------------------------
const QStringList VAbstractTool::Colors()
{
    const QStringList colors = QStringList() << ColorBlack    << ColorGreen << ColorBlue
                                             << ColorDarkRed  << ColorDarkGreen
                                             << ColorDarkBlue << ColorYellow;
    return colors;
}

//---------------------------------------------------------------------------------------------------------------------
QMap<QString, QString> VAbstractTool::ColorsList()
{
    QMap<QString, QString> map;

    const QStringList colorNames = Colors();
    for (int i = 0; i < colorNames.size(); ++i)
    {
        QString name;
        switch (i)
        {
            case 0: // ColorBlack
                name = tr("black");
                break;
            case 1: // ColorGreen
                name = tr("green");
                break;
            case 2: // ColorBlue
                name = tr("blue");
                break;
            case 3: // ColorDarkRed
                name = tr("dark red");
                break;
            case 4: // ColorDarkGreen
                name = tr("dark green");
                break;
            case 5: // ColorDarkBlue
                name = tr("dark blue");
                break;
            case 6: // ColorYellow
                name = tr("yellow");
                break;
            default:
                name = tr("black");
                break;
        }

        map.insert(colorNames.at(i), name);
    }
    return map;
}

//---------------------------------------------------------------------------------------------------------------------
// cppcheck-suppress unusedFunction
QMap<QString, quint32> VAbstractTool::PointsList() const
{
    const QHash<quint32, QSharedPointer<VGObject> > *objs = data.DataGObjects();
    QMap<QString, quint32> list;
    QHash<quint32, QSharedPointer<VGObject> >::const_iterator i;
    for (i = objs->constBegin(); i != objs->constEnd(); ++i)
    {
        if (i.key() != id)
        {
            QSharedPointer<VGObject> obj = i.value();
            if (obj->getType() == GOType::Point && obj->getMode() == Draw::Calculation)
            {
                const QSharedPointer<VPointF> point = data.GeometricObject<VPointF>(i.key());
                list[point->name()] = i.key();
            }
        }
    }
    return list;
}

//---------------------------------------------------------------------------------------------------------------------
void VAbstractTool::ToolCreation(const Source &typeCreation)
{
    if (typeCreation == Source::FromGui)
    {
        AddToFile();
    }
    else
    {
        RefreshDataInFile();
    }
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief Styles return list of all line styles.
 * @return list of all line styles.
 */
const QStringList VAbstractTool::StylesList()
{
    const QStringList styles = QStringList() << TypeLineNone    << TypeLineLine << TypeLineDashLine
                                             << TypeLineDotLine << TypeLineDashDotLine
                                             << TypeLineDashDotDotLine;
    return styles;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief AddRecord add record about tool in history.
 * @param id object id in container
 * @param toolType tool type
 * @param doc dom document container
 */
void VAbstractTool::AddRecord(const quint32 id, const Tool &toolType, VAbstractPattern *doc)
{
    QVector<VToolRecord> *history = doc->getHistory();
    VToolRecord record = VToolRecord(id, toolType, doc->GetNameActivPP());
    if (history->contains(record))
    {
        return;
    }

    quint32 cursor = doc->getCursor();
    if (cursor <= 0)
    {
        history->append(record);
    }
    else
    {
        qint32 index = 0;
        for (qint32 i = 0; i<history->size(); ++i)
        {
            VToolRecord rec = history->at(i);
            if (rec.getId() == cursor)
            {
                index = i;
                break;
            }
        }
        history->insert(index+1, record);
    }
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief RefreshLine refresh line to label on scene.
 */
void VAbstractTool::RefreshLine(QGraphicsEllipseItem *point, VGraphicsSimpleTextItem *namePoint, QGraphicsLineItem *lineName,
                                const qreal radius)
{
    SCASSERT(point != nullptr);
    SCASSERT(namePoint != nullptr);
    SCASSERT(lineName != nullptr);

    QRectF nRec = namePoint->sceneBoundingRect();
    nRec.translate(- point->scenePos());
    if (point->rect().intersects(nRec) == false)
    {
        const QRectF nameRec = namePoint->sceneBoundingRect();
        QPointF p1, p2;
        VGObject::LineIntersectCircle(QPointF(), radius, QLineF(QPointF(), nameRec.center() - point->scenePos()), p1, p2);
        const QPointF pRec = VGObject::LineIntersectRect(nameRec, QLineF(point->scenePos(), nameRec.center()));
        lineName->setLine(QLineF(p1, pRec - point->scenePos()));

        if (QLineF(p1, pRec - point->scenePos()).length() <= ToPixel(4, Unit::Mm))
        {
            lineName->setVisible(false);
        }
        else
        {
            lineName->setVisible(true);
        }
    }
    else
    {
        lineName->setVisible(false);
    }
}
