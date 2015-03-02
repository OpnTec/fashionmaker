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
#include <QGraphicsView>
#include <QIcon>
#include <QStyle>
#include <QMessageBox>
#include <QtCore/qmath.h>
#include "../../libs/vpropertyexplorer/checkablemessagebox.h"
#include "../undocommands/deltool.h"
#include "../core/vapplication.h"
#include "../geometry/vpointf.h"
#include "../undocommands/savetooloptions.h"
#include "../widgets/vmaingraphicsview.h"
#include "../core/vsettings.h"

const QString VAbstractTool::AttrType        = QStringLiteral("type");
const QString VAbstractTool::AttrMx          = QStringLiteral("mx");
const QString VAbstractTool::AttrMy          = QStringLiteral("my");
const QString VAbstractTool::AttrName        = QStringLiteral("name");
const QString VAbstractTool::AttrX           = QStringLiteral("x");
const QString VAbstractTool::AttrY           = QStringLiteral("y");
const QString VAbstractTool::AttrTypeLine    = QStringLiteral("typeLine");
const QString VAbstractTool::AttrLength      = QStringLiteral("length");
const QString VAbstractTool::AttrBasePoint   = QStringLiteral("basePoint");
const QString VAbstractTool::AttrFirstPoint  = QStringLiteral("firstPoint");
const QString VAbstractTool::AttrSecondPoint = QStringLiteral("secondPoint");
const QString VAbstractTool::AttrThirdPoint  = QStringLiteral("thirdPoint");
const QString VAbstractTool::AttrCenter      = QStringLiteral("center");
const QString VAbstractTool::AttrRadius      = QStringLiteral("radius");
const QString VAbstractTool::AttrAngle       = QStringLiteral("angle");
const QString VAbstractTool::AttrAngle1      = QStringLiteral("angle1");
const QString VAbstractTool::AttrAngle2      = QStringLiteral("angle2");
const QString VAbstractTool::AttrP1Line      = QStringLiteral("p1Line");
const QString VAbstractTool::AttrP2Line      = QStringLiteral("p2Line");
const QString VAbstractTool::AttrP1Line1     = QStringLiteral("p1Line1");
const QString VAbstractTool::AttrP2Line1     = QStringLiteral("p2Line1");
const QString VAbstractTool::AttrP1Line2     = QStringLiteral("p1Line2");
const QString VAbstractTool::AttrP2Line2     = QStringLiteral("p2Line2");
const QString VAbstractTool::AttrPShoulder   = QStringLiteral("pShoulder");
const QString VAbstractTool::AttrPoint1      = QStringLiteral("point1");
const QString VAbstractTool::AttrPoint4      = QStringLiteral("point4");
const QString VAbstractTool::AttrKAsm1       = QStringLiteral("kAsm1");
const QString VAbstractTool::AttrKAsm2       = QStringLiteral("kAsm2");
const QString VAbstractTool::AttrKCurve      = QStringLiteral("kCurve");
const QString VAbstractTool::AttrPathPoint   = QStringLiteral("pathPoint");
const QString VAbstractTool::AttrPSpline     = QStringLiteral("pSpline");
const QString VAbstractTool::AttrAxisP1      = QStringLiteral("axisP1");
const QString VAbstractTool::AttrAxisP2      = QStringLiteral("axisP2");
const QString VAbstractTool::AttrCurve       = QStringLiteral("curve");
const QString VAbstractTool::AttrLineColor   = QStringLiteral("lineColor");
const QString VAbstractTool::AttrColor       = QStringLiteral("color");

const QString VAbstractTool::TypeLineNone           = QStringLiteral("none");
const QString VAbstractTool::TypeLineLine           = QStringLiteral("hair");
const QString VAbstractTool::TypeLineDashLine       = QStringLiteral("dashLine");
const QString VAbstractTool::TypeLineDotLine        = QStringLiteral("dotLine");
const QString VAbstractTool::TypeLineDashDotLine    = QStringLiteral("dashDotLine");
const QString VAbstractTool::TypeLineDashDotDotLine = QStringLiteral("dashDotDotLine");

const QString VAbstractTool::ColorBlack     = QStringLiteral("black");
const QString VAbstractTool::ColorGreen     = QStringLiteral("green");
const QString VAbstractTool::ColorBlue      = QStringLiteral("blue");
const QString VAbstractTool::ColorDarkRed   = QStringLiteral("darkRed");
const QString VAbstractTool::ColorDarkGreen = QStringLiteral("darkGreen");
const QString VAbstractTool::ColorDarkBlue  = QStringLiteral("darkBlue");
const QString VAbstractTool::ColorYellow    = QStringLiteral("yellow");

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief VAbstractTool container.
 * @param doc dom document container.
 * @param data container with data.
 * @param id object id in container.
 * @param parent parent object.
 */
VAbstractTool::VAbstractTool(VPattern *doc, VContainer *data, quint32 id, QObject *parent)
    :VDataTool(data, parent), doc(doc), id(id), baseColor(Qt::black), vis(nullptr)
{
    SCASSERT(doc != nullptr);
    connect(this, &VAbstractTool::toolhaveChange, this->doc, &VPattern::haveLiteChange);
    connect(this->doc, &VPattern::FullUpdateFromFile, this, &VAbstractTool::FullUpdateFromFile);
    connect(this, &VAbstractTool::LiteUpdateTree, this->doc, &VPattern::LiteParseTree);
}

//---------------------------------------------------------------------------------------------------------------------
VAbstractTool::~VAbstractTool()
{}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief NewSceneRect calculate scene rect what contains all items and doesn't less that size of scene view.
 * @param sc scene.
 * @param view view.
 */
void VAbstractTool::NewSceneRect(QGraphicsScene *sc, QGraphicsView *view)
{
    SCASSERT(sc != nullptr);
    SCASSERT(view != nullptr);

    QRectF rect = sc->itemsBoundingRect();

    QRect  rec0 = view->rect();
    rec0 = QRect(0, 0, rec0.width()-2, rec0.height()-2);

    QTransform t = view->transform();

    QRectF rec1;
    if (t.m11() < 1)
    {
        qreal width = rec0.width()/t.m11();
        qreal height = rec0.height()/t.m22();
        rec1 = QRect(0, 0, static_cast<qint32>(width), static_cast<qint32>(height));

        rec1.translate(rec0.center().x()-rec1.center().x(), rec0.center().y()-rec1.center().y());
        QPolygonF polygone =  view->mapToScene(rec1.toRect());
        rec1 = polygone.boundingRect();
    }
    else
    {
        rec1 = rec0;
    }
    rec1 = rec1.united(rect.toRect());
    sc->setSceneRect(rec1);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief DeleteTool full delete object form scene and file.
 */
void VAbstractTool::DeleteTool(bool ask)
{
    if (_referens <= 1)
    {
        qApp->getSceneView()->itemClicked(nullptr);
        if (ask)
        {
            if (ConfirmDeletion() == QMessageBox::No)
            {
                return;
            }
        }
        DelTool *delTool = new DelTool(doc, id);
        connect(delTool, &DelTool::NeedFullParsing, doc, &VPattern::NeedFullParsing);
        qApp->getUndoStack()->push(delTool);
    }
}

//---------------------------------------------------------------------------------------------------------------------
int VAbstractTool::ConfirmDeletion()
{
    if (false == qApp->getSettings()->GetConfirmItemDelete())
    {
        return QMessageBox::Yes;
    }

    Utils::CheckableMessageBox msgBox(qApp->getMainWindow());
    msgBox.setWindowTitle(tr("Confirm deletion"));
    msgBox.setText(tr("Do you really want to delete?"));
    msgBox.setStandardButtons(QDialogButtonBox::Yes | QDialogButtonBox::No);
    msgBox.setDefaultButton(QDialogButtonBox::No);
    msgBox.setIconPixmap(qApp->style()->standardIcon(QStyle::SP_MessageBoxQuestion).pixmap(32,32) );

    int dialogResult = msgBox.exec();

    if (dialogResult == QDialog::Accepted)
    {
        qApp->getSettings()->SetConfirmItemDelete(not msgBox.isChecked());
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
    const QStringList colors = QStringList() << ColorBlack << ColorGreen << ColorBlue << ColorDarkRed << ColorDarkGreen
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
void VAbstractTool::SaveOption(QSharedPointer<VGObject> &obj)
{
    qCDebug(vTool)<<"Saving tool options";
    QDomElement oldDomElement = doc->elementById(id);
    if (oldDomElement.isElement())
    {
        QDomElement newDomElement = oldDomElement.cloneNode().toElement();

        SaveOptions(newDomElement, obj);

        SaveToolOptions *saveOptions = new SaveToolOptions(oldDomElement, newDomElement, doc, id);
        connect(saveOptions, &SaveToolOptions::NeedLiteParsing, doc, &VPattern::LiteParseTree);
        qApp->getUndoStack()->push(saveOptions);
    }
    else
    {
        qCDebug(vTool)<<"Can't find tool with id ="<< id << Q_FUNC_INFO;
    }
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief Styles return list of all line styles.
 * @return list of all line styles.
 */
const QStringList VAbstractTool::StylesList()
{
    const QStringList styles = QStringList() << TypeLineNone << TypeLineLine << TypeLineDashLine << TypeLineDotLine
                                             << TypeLineDashDotLine << TypeLineDashDotDotLine;
    return styles;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief AddRecord add record about tool in history.
 * @param id object id in container
 * @param toolType tool type
 * @param doc dom document container
 */
void VAbstractTool::AddRecord(const quint32 id, const Tool &toolType, VPattern *doc)
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
