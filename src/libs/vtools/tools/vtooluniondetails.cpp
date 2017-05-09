/************************************************************************
 **
 **  @file   vtooluniondetails.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   26 12, 2013
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

#include "vtooluniondetails.h"

#include <QByteArray>
#include <QDomNodeList>
#include <QHash>
#include <QLineF>
#include <QMessageLogger>
#include <QSharedPointer>
#include <QStaticStringData>
#include <QStringData>
#include <QStringDataPtr>
#include <QUndoStack>
#include <QtDebug>
#include <new>

#include "../dialogs/tools/dialoguniondetails.h"
#include "../ifc/xml/vabstractconverter.h"
#include "../ifc/xml/vdomdocument.h"
#include "../ifc/xml/vpatternconverter.h"
#include "../vgeometry/varc.h"
#include "../vgeometry/vellipticalarc.h"
#include "../vgeometry/vsplinepath.h"
#include "../vgeometry/vabstractcubicbezier.h"
#include "../vgeometry/vabstractcubicbezierpath.h"
#include "../vgeometry/vgeometrydef.h"
#include "../vgeometry/vpointf.h"
#include "../vgeometry/vspline.h"
#include "../vgeometry/vsplinepoint.h"
#include "../vmisc/diagnostic.h"
#include "../vmisc/logging.h"
#include "../vmisc/vabstractapplication.h"
#include "../vpatterndb/vcontainer.h"
#include "../vpatterndb/vpiecepath.h"
#include "../vpatterndb/vpiecenode.h"
#include "../dialogs/tools/dialogtool.h"
#include "nodeDetails/vnodearc.h"
#include "nodeDetails/vnodeellipticalarc.h"
#include "nodeDetails/vnodepoint.h"
#include "nodeDetails/vnodespline.h"
#include "nodeDetails/vnodesplinepath.h"
#include "nodeDetails/vtoolpiecepath.h"
#include "nodeDetails/vtoolpin.h"
#include "vdatatool.h"
#include "vnodedetail.h"
#include "vtoolseamallowance.h"

const QString VToolUnionDetails::ToolType         = QStringLiteral("unionDetails");
const QString VToolUnionDetails::TagDetail        = QStringLiteral("det");
const QString VToolUnionDetails::TagNode          = QStringLiteral("node");
const QString VToolUnionDetails::TagChildren      = QStringLiteral("children");
const QString VToolUnionDetails::TagChild         = QStringLiteral("child");
const QString VToolUnionDetails::AttrIndexD1      = QStringLiteral("indexD1");
const QString VToolUnionDetails::AttrIndexD2      = QStringLiteral("indexD2");
const QString VToolUnionDetails::AttrIdObject     = QStringLiteral("idObject");
const QString VToolUnionDetails::AttrNodeType     = QStringLiteral("nodeType");
const QString VToolUnionDetails::NodeTypeContour  = QStringLiteral("Contour");
const QString VToolUnionDetails::NodeTypeModeling = QStringLiteral("Modeling");

QT_WARNING_PUSH
QT_WARNING_DISABLE_CLANG("-Wmissing-prototypes")
QT_WARNING_DISABLE_INTEL(1418)

Q_LOGGING_CATEGORY(vToolUnion, "v.toolUnion")

QT_WARNING_POP

namespace
{
//---------------------------------------------------------------------------------------------------------------------
VPiecePath GetPiecePath(int piece, VAbstractPattern *doc, quint32 id)
{
    const QDomElement tool = doc->elementById(id);
    if (tool.isNull())
    {
        VException e(QString("Can't get tool by id='%1'.").arg(id));
        throw e;
    }

    const QDomNodeList nodesList = tool.childNodes();
    for (qint32 i = 0; i < nodesList.size(); ++i)
    {
        const QDomElement element = nodesList.at(i).toElement();
        if (not element.isNull() && element.tagName() == VToolUnionDetails::TagDetail && i+1 == piece)
        {
            const QDomNodeList detList = element.childNodes();
            for (qint32 j = 0; j < detList.size(); ++j)
            {
                const QDomElement element = detList.at(j).toElement();
                if (not element.isNull() && element.tagName() == VAbstractPattern::TagNodes)
                {
                    return VAbstractPattern::ParsePieceNodes(element);
                }
            }
        }
    }

    return VPiecePath();
}

//---------------------------------------------------------------------------------------------------------------------
VPiecePath GetPiece1MainPath(VAbstractPattern *doc, quint32 id)
{
    return GetPiecePath(1, doc, id);
}

//---------------------------------------------------------------------------------------------------------------------
VPiecePath GetPiece2MainPath(VAbstractPattern *doc, quint32 id)
{
    return GetPiecePath(2, doc, id);
}

//---------------------------------------------------------------------------------------------------------------------
QVector<CustomSARecord> GetPiece2CSAPaths(VAbstractPattern *doc, quint32 id)
{
    const QDomElement tool = doc->elementById(id);
    if (tool.isNull())
    {
        VException e(QString("Can't get tool by id='%1'.").arg(id));
        throw e;
    }

    const QDomNodeList nodesList = tool.childNodes();
    for (qint32 i = 0; i < nodesList.size(); ++i)
    {
        const QDomElement element = nodesList.at(i).toElement();
        if (not element.isNull() && element.tagName() == VToolUnionDetails::TagDetail && i+1 == 2)
        {
            const QDomNodeList detList = element.childNodes();
            for (qint32 j = 0; j < detList.size(); ++j)
            {
                const QDomElement element = detList.at(j).toElement();
                if (not element.isNull() && element.tagName() == VToolSeamAllowance::TagCSA)
                {
                    return VAbstractPattern::ParsePieceCSARecords(element);
                }
            }
        }
    }

    return QVector<CustomSARecord>();
}

//---------------------------------------------------------------------------------------------------------------------
QVector<quint32> GetPiece2InternalPaths(VAbstractPattern *doc, quint32 id)
{
    const QDomElement tool = doc->elementById(id);
    if (tool.isNull())
    {
        VException e(QString("Can't get tool by id='%1'.").arg(id));
        throw e;
    }

    const QDomNodeList nodesList = tool.childNodes();
    for (qint32 i = 0; i < nodesList.size(); ++i)
    {
        const QDomElement element = nodesList.at(i).toElement();
        if (not element.isNull() && element.tagName() == VToolUnionDetails::TagDetail && i+1 == 2)
        {
            const QDomNodeList detList = element.childNodes();
            for (qint32 j = 0; j < detList.size(); ++j)
            {
                const QDomElement element = detList.at(j).toElement();
                if (not element.isNull() && element.tagName() == VToolSeamAllowance::TagIPaths)
                {
                    return VAbstractPattern::ParsePieceInternalPaths(element);
                }
            }
        }
    }

    return QVector<quint32>();
}

//---------------------------------------------------------------------------------------------------------------------
QVector<quint32> GetPiece2Pins(VAbstractPattern *doc, quint32 id)
{
    const QDomElement tool = doc->elementById(id);
    if (tool.isNull())
    {
        VException e(QString("Can't get tool by id='%1'.").arg(id));
        throw e;
    }

    const QDomNodeList nodesList = tool.childNodes();
    for (qint32 i = 0; i < nodesList.size(); ++i)
    {
        const QDomElement element = nodesList.at(i).toElement();
        if (not element.isNull() && element.tagName() == VToolUnionDetails::TagDetail && i+1 == 2)
        {
            const QDomNodeList detList = element.childNodes();
            for (qint32 j = 0; j < detList.size(); ++j)
            {
                const QDomElement element = detList.at(j).toElement();
                if (not element.isNull() && element.tagName() == VToolSeamAllowance::TagPins)
                {
                    return VAbstractPattern::ParsePiecePins(element);
                }
            }
        }
    }

    return QVector<quint32>();
}

//---------------------------------------------------------------------------------------------------------------------
QString DrawName(VAbstractPattern *doc, quint32 d1id, quint32 d2id)
{
    const QDomElement detail1 = doc->elementById(d1id);
    if (detail1.isNull())
    {
        return QString();
    }

    const QDomElement detail2 = doc->elementById(d2id);
    if (detail2.isNull())
    {
        return QString();
    }

    const QDomElement draw1 = detail1.parentNode().parentNode().toElement();
    if (draw1.isNull() || not draw1.hasAttribute(VAbstractPattern::AttrName))
    {
        return QString();
    }

    const QDomElement draw2 = detail2.parentNode().parentNode().toElement();
    if (draw2.isNull() || not draw2.hasAttribute(VAbstractPattern::AttrName))
    {
        return QString();
    }

    const QString draw1Name = draw1.attribute(VAbstractPattern::AttrName);
    const QString draw2Name = draw2.attribute(VAbstractPattern::AttrName);

    if (draw1Name == draw2Name)
    {
        return draw1Name;
    }

    const QDomElement pattern = draw1.parentNode().toElement();
    if (pattern.isNull())
    {
        return QString();
    }

    int indexD1 = 0;
    int indexD2 = 0;
    const QDomNodeList listDraws = pattern.elementsByTagName(VAbstractPattern::TagDraw);
    for (int i=0; i < listDraws.size(); ++i)
    {
        const QDomElement draw = listDraws.at(i).toElement();
        if (draw == draw1)
        {
            indexD1 = i;
        }

        if (draw == draw2)
        {
            indexD2 = i;
        }
    }

    if (indexD1 >= indexD2)
    {
        return draw1Name;
    }
    else
    {
        return draw2Name;
    }
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief BiasRotatePoint bias and rotate point.
 * @param point point.
 * @param dx bias x axis.
 * @param dy bias y axis.
 * @param pRotate point rotation.
 * @param angle angle rotation.
 */
void BiasRotatePoint(VPointF *point, qreal dx, qreal dy, const QPointF &pRotate, qreal angle)
{
    point->setX(point->x()+dx);
    point->setY(point->y()+dy);
    QLineF line(pRotate, static_cast<QPointF>(*point));
    line.setAngle(line.angle()+angle);
    point->setX(line.p2().x());
    point->setY(line.p2().y());
}

//---------------------------------------------------------------------------------------------------------------------
void PointsOnEdge(const VPiecePath &path, quint32 index, VPointF &p1, VPointF &p2, VContainer *data)
{
    VPieceNode det2p1;
    VPieceNode det2p2;
    path.NodeOnEdge(index, det2p1, det2p2);
    p1 = VPointF(*data->GeometricObject<VPointF>(det2p1.GetId()));
    p2 = VPointF(*data->GeometricObject<VPointF>(det2p2.GetId()));
}

//---------------------------------------------------------------------------------------------------------------------
void UnionInitParameters(const VToolUnionDetailsInitData &initData, const VPiecePath &d1Path, const VPiecePath &d2Path,
                         VPieceNode &det1p1, qreal &dx, qreal &dy, qreal &angle)
{
    VPieceNode det1p2;
    d1Path.NodeOnEdge(initData.indexD1, det1p1, det1p2);
    Q_UNUSED(det1p2)

    VPointF point1;
    VPointF point2;
    PointsOnEdge(d1Path, initData.indexD1, point1, point2, initData.data);

    VPointF point3;
    VPointF point4;
    PointsOnEdge(d2Path, initData.indexD2, point3, point4, initData.data);

    dx = point1.x() - point4.x();
    dy = point1.y() - point4.y();

    point3.setX(point3.x()+dx);
    point3.setY(point3.y()+dy);

    point4.setX(point4.x()+dx);
    point4.setY(point4.y()+dy);

    const QLineF p4p3 = QLineF(static_cast<QPointF>(point4), static_cast<QPointF>(point3));
    const QLineF p1p2 = QLineF(static_cast<QPointF>(point1), static_cast<QPointF>(point2));

    angle = p4p3.angleTo(p1p2);
}

//---------------------------------------------------------------------------------------------------------------------
quint32 AddNodePoint(const VPieceNode &node, const VToolUnionDetailsInitData &initData, quint32 idTool,
                     QVector<quint32> &children, const QString &drawName, qreal dx, qreal dy,
                     quint32 pRotate, qreal angle)
{
    QScopedPointer<VPointF> point(new VPointF(*initData.data->GeometricObject<VPointF>(node.GetId())));
    point->setMode(Draw::Modeling);

    if (not qFuzzyIsNull(dx) || not qFuzzyIsNull(dy) || pRotate != NULL_ID)
    {
        BiasRotatePoint(point.data(), dx, dy, static_cast<QPointF>(*initData.data->GeometricObject<VPointF>(pRotate)),
                        angle);
    }

    QScopedPointer<VPointF> point1(new VPointF(*point));

    const quint32 idObject = initData.data->AddGObject(point.take());
    children.append(idObject);
    point1->setMode(Draw::Modeling);
    const quint32 id = initData.data->AddGObject(point1.take());
    VNodePoint::Create(initData.doc, initData.data, initData.scene, id, idObject, Document::FullParse, Source::FromTool,
                       drawName, idTool);
    return id;
}

//---------------------------------------------------------------------------------------------------------------------
quint32 AddPin(quint32 id, const VToolUnionDetailsInitData &initData, quint32 idTool, QVector<quint32> &children,
               const QString &drawName, qreal dx, qreal dy, quint32 pRotate, qreal angle)
{
    QScopedPointer<VPointF> point(new VPointF(*initData.data->GeometricObject<VPointF>(id)));
    point->setMode(Draw::Modeling);

    if (not qFuzzyIsNull(dx) || not qFuzzyIsNull(dy) || pRotate != NULL_ID)
    {
        BiasRotatePoint(point.data(), dx, dy, static_cast<QPointF>(*initData.data->GeometricObject<VPointF>(pRotate)),
                        angle);
    }

    QScopedPointer<VPointF> point1(new VPointF(*point));

    const quint32 idObject = initData.data->AddGObject(point.take());
    children.append(idObject);
    point1->setMode(Draw::Modeling);
    const quint32 idPin = initData.data->AddGObject(point1.take());
    VToolPin::Create(idPin, idObject, 0, initData.doc, initData.data, Document::FullParse, Source::FromTool, drawName,
                     idTool);
    return idPin;
}

//---------------------------------------------------------------------------------------------------------------------
quint32 AddNodeArc(const VPieceNode &node, const VToolUnionDetailsInitData &initData, quint32 idTool,
                   QVector<quint32> &children, const QString &drawName, qreal dx, qreal dy,
                   quint32 pRotate, qreal angle)
{
    const QSharedPointer<VArc> arc = initData.data->GeometricObject<VArc>(node.GetId());
    VPointF p1 = VPointF(arc->GetP1(), "A", 0, 0);
    VPointF p2 = VPointF(arc->GetP2(), "A", 0, 0);
    QScopedPointer<VPointF> center(new VPointF(arc->GetCenter()));

    if (not qFuzzyIsNull(dx) || not qFuzzyIsNull(dy) || pRotate != NULL_ID)
    {
        const QPointF p = static_cast<QPointF>(*initData.data->GeometricObject<VPointF>(pRotate));

        BiasRotatePoint(&p1, dx, dy, p, angle);
        BiasRotatePoint(&p2, dx, dy, p, angle);
        BiasRotatePoint(center.data(), dx, dy, p, angle);
    }

    QLineF l1(static_cast<QPointF>(*center), static_cast<QPointF>(p1));
    QLineF l2(static_cast<QPointF>(*center), static_cast<QPointF>(p2));
    center->setMode(Draw::Modeling);
    VPointF *tmpCenter = center.take();
    const quint32 idCenter = initData.data->AddGObject(tmpCenter);
    Q_UNUSED(idCenter)
    QScopedPointer<VArc> arc1(new VArc(*tmpCenter, arc->GetRadius(), arc->GetFormulaRadius(), l1.angle(),
                                       QString().setNum(l1.angle()), l2.angle(), QString().setNum(l2.angle())));
    arc1->setMode(Draw::Modeling);

    QScopedPointer<VArc>arc2(new VArc(*arc1));

    const quint32 idObject = initData.data->AddGObject(arc1.take());
    children.append(idObject);

    arc2->setMode(Draw::Modeling);
    const quint32 id = initData.data->AddGObject(arc2.take());

    VNodeArc::Create(initData.doc, initData.data, id, idObject, Document::FullParse, Source::FromTool, drawName,
                     idTool);
    return id;
}

//---------------------------------------------------------------------------------------------------------------------
quint32 AddNodeElArc(const VPieceNode &node, const VToolUnionDetailsInitData &initData, quint32 idTool,
                     QVector<quint32> &children, const QString &drawName, qreal dx, qreal dy,
                     quint32 pRotate, qreal angle)
{
    const QSharedPointer<VEllipticalArc> arc = initData.data->GeometricObject<VEllipticalArc>(node.GetId());
    VPointF p1 = VPointF(arc->GetP1(), "A", 0, 0);
    VPointF p2 = VPointF(arc->GetP2(), "A", 0, 0);
    QScopedPointer<VPointF> center(new VPointF(arc->GetCenter()));

    if (not qFuzzyIsNull(dx) || not qFuzzyIsNull(dy) || pRotate != NULL_ID)
    {
        const QPointF p = static_cast<QPointF>(*initData.data->GeometricObject<VPointF>(pRotate));

        BiasRotatePoint(&p1, dx, dy, p, angle);
        BiasRotatePoint(&p2, dx, dy, p, angle);
        BiasRotatePoint(center.data(), dx, dy, p, angle);
    }

    QLineF l1(static_cast<QPointF>(*center), static_cast<QPointF>(p1));
    QLineF l2(static_cast<QPointF>(*center), static_cast<QPointF>(p2));
    center->setMode(Draw::Modeling);
    VPointF *tmpCenter = center.take();
    quint32 idCenter = initData.data->AddGObject(tmpCenter);
    Q_UNUSED(idCenter)
    QScopedPointer<VEllipticalArc> arc1(new VEllipticalArc (*tmpCenter, arc->GetRadius1(), arc->GetRadius2(),
                                                            arc->GetFormulaRadius1(), arc->GetFormulaRadius2(),
                                                            l1.angle(), QString().setNum(l1.angle()), l2.angle(),
                                                            QString().setNum(l2.angle()), 0, "0"));
    arc1->setMode(Draw::Modeling);

    QScopedPointer<VEllipticalArc> arc2(new VEllipticalArc(*arc1));

    const quint32 idObject = initData.data->AddGObject(arc1.take());
    children.append(idObject);

    arc2->setMode(Draw::Modeling);
    const quint32 id = initData.data->AddGObject(arc2.take());

    VNodeEllipticalArc::Create(initData.doc, initData.data, id, idObject, Document::FullParse, Source::FromTool,
                               drawName, idTool);
    return id;
}

//---------------------------------------------------------------------------------------------------------------------
quint32 AddNodeSpline(const VPieceNode &node, const VToolUnionDetailsInitData &initData, quint32 idTool,
                      QVector<quint32> &children, const QString &drawName, qreal dx, qreal dy,
                      quint32 pRotate, qreal angle)
{
    const QSharedPointer<VAbstractCubicBezier> spline =
            initData.data->GeometricObject<VAbstractCubicBezier>(node.GetId());

    QScopedPointer<VPointF> p1(new VPointF(spline->GetP1()));
    VPointF p2 = VPointF(spline->GetP2());
    VPointF p3 = VPointF(spline->GetP3());
    QScopedPointer<VPointF> p4(new VPointF(spline->GetP4()));

    if (not qFuzzyIsNull(dx) || not qFuzzyIsNull(dy) || pRotate != NULL_ID)
    {
        const QPointF p = static_cast<QPointF>(*initData.data->GeometricObject<VPointF>(pRotate));

        BiasRotatePoint(p1.data(), dx, dy, p, angle);
        BiasRotatePoint(&p2, dx, dy, p, angle);
        BiasRotatePoint(&p3, dx, dy, p, angle);
        BiasRotatePoint(p4.data(), dx, dy, p, angle);
    }

    VSpline *spl = new VSpline(*p1, static_cast<QPointF>(p2), static_cast<QPointF>(p3), *p4, 0, Draw::Modeling);
    const quint32 idObject = initData.data->AddGObject(spl);
    children.append(idObject);

    VSpline *spl1 = new VSpline(*spl);
    spl1->setMode(Draw::Modeling);
    const quint32 id = initData.data->AddGObject(spl1);
    VNodeSpline::Create(initData.doc, initData.data, id, idObject, Document::FullParse, Source::FromTool, drawName,
                        idTool);
    return id;
}

//---------------------------------------------------------------------------------------------------------------------
quint32 AddNodeSplinePath(const VPieceNode &node, const VToolUnionDetailsInitData &initData, quint32 idTool,
                          QVector<quint32> &children, const QString &drawName, qreal dx, qreal dy,
                          quint32 pRotate, qreal angle)
{
    QScopedPointer<VSplinePath> path(new VSplinePath());
    path->setMode(Draw::Modeling);
    const QSharedPointer<VAbstractCubicBezierPath> splinePath =
            initData.data->GeometricObject<VAbstractCubicBezierPath>(node.GetId());
    for (qint32 i = 1; i <= splinePath->CountSubSpl(); ++i)
    {
        const VSpline spline = splinePath->GetSpline(i);

        QScopedPointer<VPointF> p1(new VPointF(spline.GetP1()));
        VPointF p2 = VPointF(spline.GetP2());
        VPointF p3 = VPointF(spline.GetP3());
        QScopedPointer<VPointF> p4(new VPointF(spline.GetP4()));
        if (not qFuzzyIsNull(dx) || not qFuzzyIsNull(dy) || pRotate != NULL_ID)
        {
            const QPointF p = static_cast<QPointF>(*initData.data->GeometricObject<VPointF>(pRotate));

            BiasRotatePoint(p1.data(), dx, dy, p, angle);
            BiasRotatePoint(&p2, dx, dy, p, angle);
            BiasRotatePoint(&p3, dx, dy, p, angle);
            BiasRotatePoint(p4.data(), dx, dy, p, angle);
        }

        VSpline spl = VSpline(*p1, static_cast<QPointF>(p2), static_cast<QPointF>(p3), *p4);
        if (i==1)
        {
            const qreal angle1 = spl.GetStartAngle()+180;
            const QString angle1F = QString().number(angle1);

            path->append(VSplinePoint(*p1, angle1, angle1F, spl.GetStartAngle(), spl.GetStartAngleFormula(),
                                      0, "0", spline.GetC1Length(), spline.GetC1LengthFormula()));
        }

        const qreal angle2 = spl.GetEndAngle()+180;
        const QString angle2F = QString().number(angle2);
        qreal pL2 = 0;
        QString pL2F("0");
        if (i+1 <= splinePath->CountSubSpl())
        {
            const VSpline nextSpline = splinePath->GetSpline(i+1);
            pL2 = nextSpline.GetC1Length();
            pL2F = nextSpline.GetC1LengthFormula();
        }

        path->append(VSplinePoint(*p4, spl.GetEndAngle(), spl.GetEndAngleFormula(), angle2, angle2F,
                                  spline.GetC2Length(), spline.GetC2LengthFormula(), pL2, pL2F));
    }
    QScopedPointer<VSplinePath> path1(new VSplinePath(*path));

    const quint32 idObject = initData.data->AddGObject(path.take());
    children.append(idObject);

    path1->setMode(Draw::Modeling);
    const quint32 id = initData.data->AddGObject(path1.take());

    VNodeSplinePath::Create(initData.doc, initData.data, id, idObject, Document::FullParse, Source::FromTool, drawName,
                            idTool);
    return id;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief AddToNewDetail create united detail adding one node per time.
 */
void AddNodeToNewPath(const VToolUnionDetailsInitData &initData, VPiecePath &newPath, VPieceNode node,
                      quint32 idTool, QVector<quint32> &children, const QString &drawName, qreal dx = 0, qreal dy = 0,
                      quint32 pRotate = NULL_ID, qreal angle = 0);

void AddNodeToNewPath(const VToolUnionDetailsInitData &initData, VPiecePath &newPath, VPieceNode node,
                      quint32 idTool, QVector<quint32> &children, const QString &drawName, qreal dx, qreal dy,
                      quint32 pRotate, qreal angle)
{
    quint32 id = 0;
    switch (node.GetTypeTool())
    {
        case (Tool::NodePoint):
            id = AddNodePoint(node, initData, idTool, children, drawName, dx, dy, pRotate, angle);
            break;
        case (Tool::NodeArc):
            id = AddNodeArc(node, initData, idTool, children, drawName, dx, dy, pRotate, angle);
            break;
        case (Tool::NodeElArc):
            id = AddNodeElArc(node, initData, idTool, children, drawName, dx, dy, pRotate, angle);
            break;
        case (Tool::NodeSpline):
            id = AddNodeSpline(node, initData, idTool, children, drawName, dx, dy, pRotate, angle);
            break;
        case (Tool::NodeSplinePath):
            id = AddNodeSplinePath(node, initData, idTool, children, drawName, dx, dy, pRotate, angle);
            break;
        default:
            qDebug()<<"May be wrong tool type!!! Ignoring."<<Q_FUNC_INFO;
            break;
    }

    node.SetId(id);
    newPath.Append(node);
}

//---------------------------------------------------------------------------------------------------------------------
void FindIndexJ(qint32 pointsD2, const VPiecePath &d2Path, quint32 indexD2, qint32 &j)
{
    if (pointsD2 == 0)
    {
        VPieceNode node1;
        VPieceNode node2;
        d2Path.NodeOnEdge(indexD2, node1, node2);
        const VPiecePath removedD2 = d2Path.RemoveEdge(indexD2);
        const int k = removedD2.indexOfNode(node2.GetId());
        SCASSERT(k != -1)
        if (k == removedD2.CountNodes()-1)
        {//We have last node in detail, we wil begin from 0
            j = 0;
        }
        else
        {// Continue from next node
            j = k+1;
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
QDomElement GetTagChildren(VAbstractPattern *doc, quint32 id)
{
    QDomElement toolUnion = doc->elementById(id);
    if (toolUnion.isNull())
    {
        VException e(QString("Can't get tool by id='%1'.").arg(id));
        throw e;
    }

    QDomElement tagChildren = toolUnion.firstChildElement(VToolUnionDetails::TagChildren);

    if (tagChildren.isNull())
    {
        tagChildren = doc->createElement(VToolUnionDetails::TagChildren);
        toolUnion.appendChild(tagChildren);
    }

    return tagChildren;
}

//---------------------------------------------------------------------------------------------------------------------
void SaveChildren(VAbstractPattern *doc, quint32 id, QDomElement section, const QVector<quint32> &children)
{
    if (children.size() > 0)
    {
        for (int i=0; i<children.size(); ++i)
        {
            QDomElement tagChild = doc->createElement(VToolUnionDetails::TagChild);
            tagChild.appendChild(doc->createTextNode(QString().setNum(children.at(i))));
            section.appendChild(tagChild);
        }

        GetTagChildren(doc, id).appendChild(section);
    }
}

//---------------------------------------------------------------------------------------------------------------------
void SaveNodesChildren(VAbstractPattern *doc, quint32 id, const QVector<quint32> &children)
{
    SaveChildren(doc, id, doc->createElement(VAbstractPattern::TagNodes), children);
}

//---------------------------------------------------------------------------------------------------------------------
void SaveCSAChildren(VAbstractPattern *doc, quint32 id, const QVector<quint32> &children)
{
    SaveChildren(doc, id, doc->createElement(VToolSeamAllowance::TagCSA), children);
}

//---------------------------------------------------------------------------------------------------------------------
void SaveInternalPathsChildren(VAbstractPattern *doc, quint32 id, const QVector<quint32> &children)
{
    SaveChildren(doc, id, doc->createElement(VToolSeamAllowance::TagIPaths), children);
}

//---------------------------------------------------------------------------------------------------------------------
void SavePinsChildren(VAbstractPattern *doc, quint32 id, const QVector<quint32> &children)
{
    SaveChildren(doc, id, doc->createElement(VToolSeamAllowance::TagPins), children);
}

//---------------------------------------------------------------------------------------------------------------------
QVector<quint32> GetChildren(VAbstractPattern *doc, quint32 id, const QString &tagName)
{
    const QDomElement toolUnion = doc->elementById(id);
    if (toolUnion.isNull())
    {
        return QVector<quint32>();
    }

    const QDomElement tagChildren = toolUnion.firstChildElement(VToolUnionDetails::TagChildren);
    if (tagChildren.isNull())
    {
        return QVector<quint32>();
    }

    const QDomElement tagNodes = tagChildren.firstChildElement(tagName);
    if (tagNodes.isNull())
    {
        return QVector<quint32>();
    }

    QVector<quint32> childrenId;
    const QDomNodeList listChildren = tagNodes.elementsByTagName(VToolUnionDetails::TagChild);
    for (int i=0; i < listChildren.size(); ++i)
    {
        const QDomElement domElement = listChildren.at(i).toElement();
        if (not domElement.isNull())
        {
            childrenId.append(domElement.text().toUInt());
        }
    }
    return childrenId;
}

//---------------------------------------------------------------------------------------------------------------------
QVector<quint32> GetNodesChildren(VAbstractPattern *doc, quint32 id)
{
    return GetChildren(doc, id, VAbstractPattern::TagNodes);
}

//---------------------------------------------------------------------------------------------------------------------
QVector<quint32> GetCSAChildren(VAbstractPattern *doc, quint32 id)
{
    return GetChildren(doc, id, VToolSeamAllowance::TagCSA);
}

//---------------------------------------------------------------------------------------------------------------------
QVector<quint32> GetInternalPathsChildren(VAbstractPattern *doc, quint32 id)
{
    return GetChildren(doc, id, VToolSeamAllowance::TagIPaths);
}

//---------------------------------------------------------------------------------------------------------------------
QVector<quint32> GetPinChildren(VAbstractPattern *doc, quint32 id)
{
    return GetChildren(doc, id, VToolSeamAllowance::TagPins);
}

//---------------------------------------------------------------------------------------------------------------------
quint32 TakeNextId(QVector<quint32> &children)
{
    quint32 idChild = NULL_ID;
    if (not children.isEmpty())
    {
#if QT_VERSION >= QT_VERSION_CHECK(5, 1, 0)
        idChild = children.takeFirst();
#else
        idChild = children.first();
        children.remove(0);
#endif
    }
    else
    {
        idChild = NULL_ID;
    }
    return idChild;
}

//---------------------------------------------------------------------------------------------------------------------
void UpdateNodePoint(VContainer *data, const VPieceNode &node, QVector<quint32> &children, qreal dx, qreal dy,
                     quint32 pRotate, qreal angle)
{
    QScopedPointer<VPointF> point(new VPointF(*data->GeometricObject<VPointF>(node.GetId())));
    point->setMode(Draw::Modeling);
    if (not qFuzzyIsNull(dx) || not qFuzzyIsNull(dy) || pRotate != NULL_ID)
    {
        BiasRotatePoint(point.data(), dx, dy, static_cast<QPointF>(*data->GeometricObject<VPointF>(pRotate)), angle);
    }
    data->UpdateGObject(TakeNextId(children), point.take());
}

//---------------------------------------------------------------------------------------------------------------------
void UpdateNodeArc(VContainer *data, const VPieceNode &node, QVector<quint32> &children, qreal dx, qreal dy,
                   quint32 pRotate, qreal angle)
{
    const QSharedPointer<VArc> arc = data->GeometricObject<VArc>(node.GetId());
    VPointF p1 = VPointF(arc->GetP1());
    VPointF p2 = VPointF(arc->GetP2());
    QScopedPointer<VPointF> center(new VPointF(arc->GetCenter()));

    if (not qFuzzyIsNull(dx) || not qFuzzyIsNull(dy) || pRotate != NULL_ID)
    {
        const QPointF p = static_cast<QPointF>(*data->GeometricObject<VPointF>(pRotate));

        BiasRotatePoint(&p1, dx, dy, p, angle);
        BiasRotatePoint(&p2, dx, dy, p, angle);
        BiasRotatePoint(center.data(), dx, dy, p, angle);
    }

    QLineF l1(static_cast<QPointF>(*center), static_cast<QPointF>(p1));
    QLineF l2(static_cast<QPointF>(*center), static_cast<QPointF>(p2));

    QScopedPointer<VArc> arc1(new VArc(*center, arc->GetRadius(), arc->GetFormulaRadius(), l1.angle(),
                                       QString().setNum(l1.angle()), l2.angle(), QString().setNum(l2.angle())));
    arc1->setMode(Draw::Modeling);
    data->UpdateGObject(TakeNextId(children), arc1.take());
}

//---------------------------------------------------------------------------------------------------------------------
void UpdateNodeElArc(VContainer *data, const VPieceNode &node, QVector<quint32> &children, qreal dx, qreal dy,
                     quint32 pRotate, qreal angle)
{
    const QSharedPointer<VEllipticalArc> arc = data->GeometricObject<VEllipticalArc>(node.GetId());
    VPointF p1 = VPointF(arc->GetP1());
    VPointF p2 = VPointF(arc->GetP2());
    QScopedPointer<VPointF> center(new VPointF(arc->GetCenter()));

    if (not qFuzzyIsNull(dx) || not qFuzzyIsNull(dy) || pRotate != NULL_ID)
    {
        const QPointF p = static_cast<QPointF>(*data->GeometricObject<VPointF>(pRotate));

        BiasRotatePoint(&p1, dx, dy, p, angle);
        BiasRotatePoint(&p2, dx, dy, p, angle);
        BiasRotatePoint(center.data(), dx, dy, p, angle);
    }

    QLineF l1(static_cast<QPointF>(*center), static_cast<QPointF>(p1));
    QLineF l2(static_cast<QPointF>(*center), static_cast<QPointF>(p2));

    QScopedPointer<VEllipticalArc> arc1(new VEllipticalArc (*center, arc->GetRadius1(), arc->GetRadius2(),
                                                            arc->GetFormulaRadius1(), arc->GetFormulaRadius2(),
                                                            l1.angle(), QString().setNum(l1.angle()), l2.angle(),
                                                            QString().setNum(l2.angle()), 0, "0"));
    arc1->setMode(Draw::Modeling);
    data->UpdateGObject(TakeNextId(children), arc1.take());
}

//---------------------------------------------------------------------------------------------------------------------
void UpdateNodeSpline(VContainer *data, const VPieceNode &node, QVector<quint32> &children, qreal dx, qreal dy,
                      quint32 pRotate, qreal angle)
{
    const QSharedPointer<VAbstractCubicBezier> spline =
            data->GeometricObject<VAbstractCubicBezier>(node.GetId());

    QScopedPointer<VPointF> p1(new VPointF(spline->GetP1()));
    VPointF p2 = VPointF(spline->GetP2());
    VPointF p3 = VPointF(spline->GetP3());
    QScopedPointer<VPointF> p4(new VPointF(spline->GetP4()));

    if (not qFuzzyIsNull(dx) || not qFuzzyIsNull(dy) || pRotate != NULL_ID)
    {
        const QPointF p = static_cast<QPointF>(*data->GeometricObject<VPointF>(pRotate));

        BiasRotatePoint(p1.data(), dx, dy, p, angle);
        BiasRotatePoint(&p2, dx, dy, p, angle);
        BiasRotatePoint(&p3, dx, dy, p, angle);
        BiasRotatePoint(p4.data(), dx, dy, p, angle);
    }

    QScopedPointer<VSpline> spl(new VSpline(*p1, static_cast<QPointF>(p2), static_cast<QPointF>(p3), *p4, 0,
                                            Draw::Modeling));
    data->UpdateGObject(TakeNextId(children), spl.take());
}

//---------------------------------------------------------------------------------------------------------------------
void UpdateNodeSplinePath(VContainer *data, const VPieceNode &node, QVector<quint32> &children, qreal dx, qreal dy,
                          quint32 pRotate, qreal angle)
{
    QScopedPointer<VSplinePath> path(new VSplinePath());
    path->setMode(Draw::Modeling);
    const QSharedPointer<VAbstractCubicBezierPath> splinePath =
            data->GeometricObject<VAbstractCubicBezierPath>(node.GetId());
    SCASSERT(splinePath != nullptr)
    for (qint32 i = 1; i <= splinePath->CountSubSpl(); ++i)
    {
        const VSpline spline = splinePath->GetSpline(i);

        QScopedPointer<VPointF> p1(new VPointF(spline.GetP1()));
        VPointF p2 = VPointF(spline.GetP2());
        VPointF p3 = VPointF(spline.GetP3());
        QScopedPointer<VPointF> p4(new VPointF(spline.GetP4()));

        if (not qFuzzyIsNull(dx) || not qFuzzyIsNull(dy) || pRotate != NULL_ID)
        {
            const QPointF p = static_cast<QPointF>(*data->GeometricObject<VPointF>(pRotate));

            BiasRotatePoint(p1.data(), dx, dy, p, angle);
            BiasRotatePoint(&p2, dx, dy, p, angle);
            BiasRotatePoint(&p3, dx, dy, p, angle);
            BiasRotatePoint(p4.data(), dx, dy, p, angle);
        }

        VSpline spl = VSpline(*p1, static_cast<QPointF>(p2), static_cast<QPointF>(p3), *p4);
        if (i==1)
        {
            const qreal angle1 = spl.GetStartAngle()+180;
            const QString angle1F = QString().number(angle1);

            path->append(VSplinePoint(*p1, angle1, angle1F, spl.GetStartAngle(), spl.GetStartAngleFormula(),
                                      0, "0", spline.GetC1Length(), spline.GetC1LengthFormula()));
        }

        const qreal angle2 = spl.GetEndAngle()+180;
        const QString angle2F = QString().number(angle2);

        qreal pL2 = 0;
        QString pL2F("0");
        if (i+1 <= splinePath->CountSubSpl())
        {
            const VSpline nextSpline = splinePath->GetSpline(i+1);
            pL2 = nextSpline.GetC1Length();
            pL2F = nextSpline.GetC1LengthFormula();
        }

        path->append(VSplinePoint(*p4, spl.GetEndAngle(), spl.GetEndAngleFormula(), angle2, angle2F,
                                  spline.GetC2Length(), spline.GetC2LengthFormula(), pL2, pL2F));
    }
    data->UpdateGObject(TakeNextId(children), path.take());
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief UpdateNodes update nodes of united detail.
 * @param data container with variables.
 * @param node detail's node.
 * @param children list ids of all children.
 * @param dx bias node x axis.
 * @param dy bias node y axis.
 * @param pRotate point rotation.
 * @param angle angle rotation.
 */
void UpdatePathNode(VContainer *data, const VPieceNode &node, QVector<quint32> &children,
                 qreal dx = 0, qreal dy = 0, quint32 pRotate = NULL_ID, qreal angle = 0);
void UpdatePathNode(VContainer *data, const VPieceNode &node, QVector<quint32> &children, qreal dx, qreal dy,
                 quint32 pRotate, qreal angle)
{
    switch (node.GetTypeTool())
    {
        case (Tool::NodePoint):
            UpdateNodePoint(data, node, children, dx, dy, pRotate, angle);
            break;
        case (Tool::NodeArc):
            UpdateNodeArc(data, node, children, dx, dy, pRotate, angle);
            break;
        case (Tool::NodeElArc):
            UpdateNodeElArc(data, node, children, dx, dy, pRotate, angle);
            break;
        case (Tool::NodeSpline):
            UpdateNodeSpline(data, node, children, dx, dy, pRotate, angle);
            break;
        case (Tool::NodeSplinePath):
            UpdateNodeSplinePath(data, node, children, dx, dy, pRotate, angle);
            break;
        default:
            qDebug()<<"May be wrong tool type!!! Ignoring."<<Q_FUNC_INFO;
            break;
    }
}

//---------------------------------------------------------------------------------------------------------------------
void CreateUnitedNodes(VPiece &newDetail, const VPiece &d1, const VPiece &d2, quint32 id, const QString &drawName,
                       const VToolUnionDetailsInitData &initData, qreal dx, qreal dy, quint32 pRotate, qreal angle)
{
    const VPiecePath d1Path = d1.GetPath().RemoveEdge(initData.indexD1);
    const VPiecePath d2Path = d2.GetPath().RemoveEdge(initData.indexD2);

    const qint32 countNodeD1 = d1Path.CountNodes();
    const qint32 countNodeD2 = d2Path.CountNodes();

    qint32 pointsD2 = 0; //Keeps number points the second detail, what we have already added.
    qint32 i = 0;
    QVector<quint32> children;
    VPiecePath newPath;
    const int det1P1Index = d1.GetPath().indexOfNode(pRotate);
    do
    {
        AddNodeToNewPath(initData, newPath, d1Path.at(i), id, children, drawName);
        ++i;
        if (i > det1P1Index && pointsD2 < countNodeD2-1)
        {
            qint32 j = 0;
            FindIndexJ(pointsD2, d2.GetPath(), initData.indexD2, j);
            do
            {
                if (j >= countNodeD2)
                {
                    j=0;
                }
                AddNodeToNewPath(initData, newPath, d2Path.at(j), id, children, drawName, dx, dy, pRotate, angle);
                ++pointsD2;
                ++j;
            } while (pointsD2 < countNodeD2-1);
        }
    } while (i < countNodeD1);

    newDetail.SetPath(newPath);

    SCASSERT(not children.isEmpty())
    SaveNodesChildren(initData.doc, id, children);
}

//---------------------------------------------------------------------------------------------------------------------
void CreateUnitedDetailCSA(VPiece &newDetail, const VPiece &d, QVector<quint32> &children, quint32 id,
                           const QString &drawName, const VToolUnionDetailsInitData &initData, qreal dx, qreal dy,
                           quint32 pRotate, qreal angle)
{
    QVector<quint32> nodeChildren;
    for(int i=0; i < d.GetCustomSARecords().size(); ++i)
    {
        CustomSARecord record = d.GetCustomSARecords().at(i);
        const VPiecePath path = initData.data->GetPiecePath(record.path);
        VPiecePath newPath = path;
        newPath.Clear();//Clear nodes
        for (int i=0; i < path.CountNodes(); ++i)
        {
            AddNodeToNewPath(initData, newPath, path.at(i), id, nodeChildren, drawName, dx, dy, pRotate, angle);
        }
        const quint32 idPath = initData.data->AddPiecePath(newPath);
        VToolPiecePath::Create(idPath, newPath, NULL_ID, initData.scene, initData.doc, initData.data, initData.parse,
                               Source::FromTool, drawName, id);
        record.path = idPath;
        newDetail.GetCustomSARecords().append(record);
        nodeChildren.prepend(idPath);
    }
    children += nodeChildren;
}

//---------------------------------------------------------------------------------------------------------------------
void CreateUnitedCSA(VPiece &newDetail, const VPiece &d1, const VPiece &d2, quint32 id, const QString &drawName,
                     const VToolUnionDetailsInitData &initData, qreal dx, qreal dy, quint32 pRotate, qreal angle)
{
    for (int i = 0; i < d1.GetCustomSARecords().size(); ++i)
    {
        newDetail.GetCustomSARecords().append(d1.GetCustomSARecords().at(i));
    }

    QVector<quint32> children;
    CreateUnitedDetailCSA(newDetail, d2, children, id, drawName, initData, dx, dy, pRotate, angle);
    SaveCSAChildren(initData.doc, id, children);
}

//---------------------------------------------------------------------------------------------------------------------
void CreateUnitedDetailInternalPaths(VPiece &newDetail, const VPiece &d, QVector<quint32> &children, quint32 id,
                                     const QString &drawName, const VToolUnionDetailsInitData &initData, qreal dx,
                                     qreal dy, quint32 pRotate, qreal angle)
{
    QVector<quint32> nodeChildren;
    for(int i=0; i < d.GetInternalPaths().size(); ++i)
    {
        const VPiecePath path = initData.data->GetPiecePath(d.GetInternalPaths().at(i));
        VPiecePath newPath = path;
        newPath.Clear();//Clear nodes

        for (int i=0; i < path.CountNodes(); ++i)
        {
            AddNodeToNewPath(initData, newPath, path.at(i), id, nodeChildren, drawName, dx, dy, pRotate, angle);
        }
        const quint32 idPath = initData.data->AddPiecePath(newPath);
        VToolPiecePath::Create(idPath, newPath, NULL_ID, initData.scene, initData.doc, initData.data, initData.parse,
                               Source::FromTool, drawName, id);
        newDetail.GetInternalPaths().append(idPath);
        nodeChildren.prepend(idPath);
    }
    children += nodeChildren;
}

//---------------------------------------------------------------------------------------------------------------------
void CreateUnitedInternalPaths(VPiece &newDetail, const VPiece &d1, const VPiece &d2, quint32 id,
                               const QString &drawName, const VToolUnionDetailsInitData &initData, qreal dx, qreal dy,
                               quint32 pRotate, qreal angle)
{
    for (int i = 0; i < d1.GetInternalPaths().size(); ++i)
    {
        newDetail.GetInternalPaths().append(d1.GetInternalPaths().at(i));
    }

    QVector<quint32> children;
    CreateUnitedDetailInternalPaths(newDetail, d2, children, id, drawName, initData, dx, dy, pRotate, angle);

    SaveInternalPathsChildren(initData.doc, id, children);
}

//---------------------------------------------------------------------------------------------------------------------
void CreateUnitedDetailPins(VPiece &newDetail, const VPiece &d, QVector<quint32> &children, quint32 idTool,
                            const QString &drawName, const VToolUnionDetailsInitData &initData, qreal dx, qreal dy,
                            quint32 pRotate, qreal angle)
{
    QVector<quint32> nodeChildren;
    for(int i=0; i < d.GetPins().size(); ++i)
    {
        const quint32 id = AddPin(d.GetPins().at(i), initData, idTool, children, drawName, dx, dy, pRotate, angle);
        newDetail.GetPins().append(id);
        nodeChildren.prepend(id);
    }
    children += nodeChildren;
}

//---------------------------------------------------------------------------------------------------------------------
void CreateUnitedPins(VPiece &newDetail, const VPiece &d1, const VPiece &d2, quint32 id, const QString &drawName,
                      const VToolUnionDetailsInitData &initData, qreal dx, qreal dy, quint32 pRotate, qreal angle)
{
    for (int i = 0; i < d1.GetPins().size(); ++i)
    {
        newDetail.GetPins().append(d1.GetPins().at(i));
    }

    QVector<quint32> children;
    CreateUnitedDetailPins(newDetail, d2, children, id, drawName, initData, dx, dy, pRotate, angle);
    SavePinsChildren(initData.doc, id, children);
}

//---------------------------------------------------------------------------------------------------------------------
void UpdateUnitedNodes(quint32 id, const VToolUnionDetailsInitData &initData, qreal dx, qreal dy, quint32 pRotate,
                       qreal angle)
{
    const VPiecePath d1Path = GetPiece1MainPath(initData.doc, id);
    const VPiecePath d1REPath = d1Path.RemoveEdge(initData.indexD1);

    const VPiecePath d2Path = GetPiece2MainPath(initData.doc, id);
    const VPiecePath d2REPath = d2Path.RemoveEdge(initData.indexD2);

    const qint32 countNodeD1 = d1REPath.CountNodes();
    const qint32 countNodeD2 = d2REPath.CountNodes();

    QVector<quint32> children = GetNodesChildren(initData.doc, id);
    if (not children.isEmpty())
    {
        // This check need for backward compatibility
        // Remove check and "else" part if min version is 0.3.2
        Q_STATIC_ASSERT_X(VPatternConverter::PatternMinVer < CONVERTER_VERSION_CHECK(0, 3, 2),
                          "Time to refactor the code.");
        if (children.size() == countNodeD1 + countNodeD2-1)
        {
            qint32 pointsD2 = 0; //Keeps number points the second detail, what we have already added.
            qint32 i = 0;
            const int indexOfNode = d1Path.indexOfNode(pRotate);
            do
            {
                UpdatePathNode(initData.data, d1REPath.at(i), children);
                ++i;
                if (i > indexOfNode && pointsD2 < countNodeD2-1)
                {
                    qint32 j = 0;
                    FindIndexJ(pointsD2, d2Path, initData.indexD2, j);
                    do
                    {
                        if (j >= countNodeD2)
                        {
                            j=0;
                        }
                        UpdatePathNode(initData.data, d2REPath.at(j), children, dx, dy, pRotate, angle);
                        ++pointsD2;
                        ++j;
                    } while (pointsD2 < countNodeD2-1);
                }
            } while (i<countNodeD1);
        }
        else // remove if min version is 0.3.2
        {
            qint32 pointsD2 = 0; //Keeps number points the second detail, what we have already added.
            qint32 i = 0;
            const int indexOfNode = d1Path.indexOfNode(pRotate);
            do
            {
                ++i;
                if (i > indexOfNode)
                {
                    const int childrenCount = children.size();
                    qint32 j = 0;
                    FindIndexJ(pointsD2, d2Path, initData.indexD2, j);
                    do
                    {
                        if (j >= countNodeD2)
                        {
                            j=0;
                        }
                        UpdatePathNode(initData.data, d2REPath.at(j), children, dx, dy, pRotate, angle);
                        ++pointsD2;
                        ++j;
                    } while (pointsD2 < childrenCount);
                    break;
                }
            } while (i<countNodeD1);
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
void UpdateUnitedDetailPaths(const VToolUnionDetailsInitData &initData, qreal dx, qreal dy, quint32 pRotate,
                             qreal angle, const QVector<quint32> &records, QVector<quint32> children)
{
    for (int i=0; i < records.size(); ++i)
    {
        const VPiecePath path = initData.data->GetPiecePath(records.at(i));
        const quint32 updatedId = TakeNextId(children);

        VPiecePath updatedPath(path);
        updatedPath.Clear();

        for (int j=0; j < path.CountNodes(); ++j)
        {
            const VPieceNode &node = path.at(j);
            const quint32 id = TakeNextId(children);
            updatedPath.Append(VPieceNode(id, node.GetTypeTool(), node.GetReverse()));
            QVector<quint32> nodeChildren = {id};
            UpdatePathNode(initData.data, path.at(j), nodeChildren, dx, dy, pRotate, angle);
        }
        initData.data->UpdatePiecePath(updatedId, updatedPath);
    }
}

//---------------------------------------------------------------------------------------------------------------------
void UpdateUnitedDetailCSA(quint32 id, const VToolUnionDetailsInitData &initData, qreal dx, qreal dy, quint32 pRotate,
                           qreal angle, const QVector<CustomSARecord> &records)
{
    QVector<quint32> idRecords;
    for (int i = 0; i < records.size(); ++i)
    {
        idRecords.append(records.at(i).path);
    }
    UpdateUnitedDetailPaths(initData, dx, dy, pRotate, angle, idRecords, GetCSAChildren(initData.doc, id));
}

//---------------------------------------------------------------------------------------------------------------------
void UpdateUnitedDetailInternalPaths(quint32 id, const VToolUnionDetailsInitData &initData, qreal dx, qreal dy,
                                     quint32 pRotate, qreal angle, const QVector<quint32> &records)
{
    UpdateUnitedDetailPaths(initData, dx, dy, pRotate, angle, records, GetInternalPathsChildren(initData.doc, id));
}

//---------------------------------------------------------------------------------------------------------------------
void UpdateUnitedDetailPins(quint32 id, const VToolUnionDetailsInitData &initData, qreal dx, qreal dy,
                            quint32 pRotate, qreal angle, const QVector<quint32> &records)
{
    QVector<quint32> children = GetPinChildren(initData.doc, id);

    for (int i = 0; i < records.size(); ++i)
    {
        QScopedPointer<VPointF> point(new VPointF(*initData.data->GeometricObject<VPointF>(records.at(i))));
        point->setMode(Draw::Modeling);
        if (not qFuzzyIsNull(dx) || not qFuzzyIsNull(dy) || pRotate != NULL_ID)
        {
            BiasRotatePoint(point.data(), dx, dy,
                            static_cast<QPointF>(*initData.data->GeometricObject<VPointF>(pRotate)), angle);
        }
        initData.data->UpdateGObject(TakeNextId(children), point.take());
    }
}

//---------------------------------------------------------------------------------------------------------------------
void CreateUnitedDetail(quint32 id, const VToolUnionDetailsInitData &initData, qreal dx, qreal dy, quint32 pRotate,
                        qreal angle)
{
    const QString drawName = DrawName(initData.doc, initData.d1id, initData.d2id);
    SCASSERT(not drawName.isEmpty())

    const VPiece d1 = initData.data->GetPiece(initData.d1id);
    const VPiece d2 = initData.data->GetPiece(initData.d2id);

    VPiece newDetail;

    CreateUnitedNodes(newDetail, d1, d2, id, drawName, initData, dx, dy, pRotate, angle);
    CreateUnitedCSA(newDetail, d1, d2, id, drawName, initData, dx, dy, pRotate, angle);
    CreateUnitedInternalPaths(newDetail, d1, d2, id, drawName, initData, dx, dy, pRotate, angle);
    CreateUnitedPins(newDetail, d1, d2, id, drawName, initData, dx, dy, pRotate, angle);

    newDetail.SetName(QObject::tr("United detail"));
    QString formulaSAWidth = d1.GetFormulaSAWidth();
    newDetail.SetFormulaSAWidth(formulaSAWidth, d1.GetSAWidth());
    newDetail.SetMx(d1.GetMx());
    newDetail.SetMy(d1.GetMy());
    newDetail.SetUnited(true);
    VToolSeamAllowance::Create(0, newDetail, formulaSAWidth, initData.scene, initData.doc, initData.data,
                               initData.parse, Source::FromTool, drawName);

    auto RemoveDetail = [initData](quint32 id)
    {
        VToolSeamAllowance *toolDet = qobject_cast<VToolSeamAllowance*>(VAbstractPattern::getTool(id));
        SCASSERT(toolDet != nullptr);
        bool ask = false;
        toolDet->Remove(ask);
        // We do not call full parse, so will need more to do more cleaning than usually
        initData.doc->RemoveTool(id);
        initData.data->RemovePiece(id);
    };

    if (not initData.retainPieces)
    {
        RemoveDetail(initData.d1id);
        RemoveDetail(initData.d2id);
    }
}

//---------------------------------------------------------------------------------------------------------------------
void UpdateUnitedDetail(quint32 id, const VToolUnionDetailsInitData &initData, qreal dx, qreal dy, quint32 pRotate,
                        qreal angle)
{
    UpdateUnitedNodes(id, initData, dx, dy, pRotate, angle);
    UpdateUnitedDetailCSA(id, initData, dx, dy, pRotate, angle, GetPiece2CSAPaths(initData.doc, id));
    UpdateUnitedDetailInternalPaths(id, initData, dx, dy, pRotate, angle, GetPiece2InternalPaths(initData.doc, id));
    UpdateUnitedDetailPins(id, initData, dx, dy, pRotate, angle, GetPiece2Pins(initData.doc, id));
}

//---------------------------------------------------------------------------------------------------------------------
void UniteDetails(quint32 id, const VToolUnionDetailsInitData &initData)
{
    VPieceNode det1p1;
    qreal dx = 0;
    qreal dy = 0;
    qreal angle = 0;

    if (initData.typeCreation == Source::FromGui)
    {
        const VPiece d1 = initData.data->GetPiece(initData.d1id);
        const VPiece d2 = initData.data->GetPiece(initData.d2id);
        UnionInitParameters(initData, d1.GetPath(), d2.GetPath(), det1p1, dx, dy, angle);
        CreateUnitedDetail(id, initData, dx, dy, det1p1.GetId(), angle);
    }
    else
    {
        const VPiecePath d1Path = GetPiece1MainPath(initData.doc, id);
        const VPiecePath d2Path = GetPiece2MainPath(initData.doc, id);
        UnionInitParameters(initData, d1Path, d2Path, det1p1, dx, dy, angle);
        UpdateUnitedDetail(id, initData, dx, dy, det1p1.GetId(), angle);
    }
}
} // static functions

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief VToolUnionDetails costructor.
 * @param id object id in container.
 * @param initData global init data.
 * @param parent parent object.
 */
VToolUnionDetails::VToolUnionDetails(quint32 id, const VToolUnionDetailsInitData &initData,
                                     QObject *parent)
    : VAbstractTool(initData.doc, initData.data, id, parent),
      d1id(initData.d1id),
      d2id(initData.d2id),
      indexD1(initData.indexD1),
      indexD2(initData.indexD2)
{
    _referens = 0;
    ToolCreation(initData.typeCreation);
}

//---------------------------------------------------------------------------------------------------------------------
QString VToolUnionDetails::getTagName() const
{
    return VAbstractPattern::TagTools;
}

//---------------------------------------------------------------------------------------------------------------------
void VToolUnionDetails::ShowVisualization(bool show)
{
    Q_UNUSED(show)
}

//---------------------------------------------------------------------------------------------------------------------
void VToolUnionDetails::incrementReferens()
{
    VDataTool::incrementReferens();
    if (_referens == 1)
    {
        const QVector<quint32> objects = GetReferenceObjects();
        for(int i = 0; i < objects.size(); ++i)
        {
            doc->IncrementReferens(objects.at(i));
        }

        QDomElement domElement = doc->elementById(id);
        if (domElement.isElement())
        {
            doc->SetParametrUsage(domElement, AttrInUse, NodeUsage::InUse);
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VToolUnionDetails::decrementReferens()
{
    VDataTool::decrementReferens();
    if (_referens == 0)
    {
        const QVector<quint32> objects = GetReferenceObjects();
        for(int i = 0; i < objects.size(); ++i)
        {
            doc->DecrementReferens(objects.at(i));
        }

        QDomElement domElement = doc->elementById(id);
        if (domElement.isElement())
        {
            doc->SetParametrUsage(domElement, AttrInUse, NodeUsage::NotInUse);
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VToolUnionDetails::GroupVisibility(quint32 object, bool visible)
{
    Q_UNUSED(object)
    Q_UNUSED(visible)
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief Create help create tool from GUI.
 * @param dialog dialog.
 * @param doc dom document container.
 * @param data container with variables.
 */
VToolUnionDetails* VToolUnionDetails::Create(QSharedPointer<DialogTool> dialog, VMainGraphicsScene *scene,
                                             VAbstractPattern *doc, VContainer *data)
{
    SCASSERT(not dialog.isNull())
    QSharedPointer<DialogUnionDetails> dialogTool = dialog.objectCast<DialogUnionDetails>();
    SCASSERT(not dialogTool.isNull())

    VToolUnionDetailsInitData initData;
    initData.d1id = dialogTool->getD1();
    initData.d2id = dialogTool->getD2();
    initData.indexD1 = static_cast<quint32>(dialogTool->getIndexD1());
    initData.indexD2 = static_cast<quint32>(dialogTool->getIndexD2());
    initData.scene = scene;
    initData.doc = doc;
    initData.data = data;
    initData.parse = Document::FullParse;
    initData.typeCreation = Source::FromGui;
    initData.retainPieces = dialogTool->RetainPieces();

    qApp->getUndoStack()->beginMacro(tr("union details"));
    VToolUnionDetails* tool = Create(0, initData);
    qApp->getUndoStack()->endMacro();
    return tool;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief Create help create tool.
 * @param _id tool id, 0 if tool doesn't exist yet.
 * @param initData contains all init data.
 */
VToolUnionDetails* VToolUnionDetails::Create(const quint32 _id, const VToolUnionDetailsInitData &initData)
{
    VToolUnionDetails *unionDetails = nullptr;
    quint32 id = _id;
    if (initData.typeCreation == Source::FromGui)
    {
        id = VContainer::getNextId();
    }
    else
    {
        if (initData.parse != Document::FullParse)
        {
            initData.doc->UpdateToolData(id, initData.data);
        }
    }

    //First add tool to file
    if (initData.parse == Document::FullParse)
    {
        VAbstractTool::AddRecord(id, Tool::UnionDetails, initData.doc);
        //Scene doesn't show this tool, so doc will destroy this object.
        unionDetails = new VToolUnionDetails(id, initData);
        VAbstractPattern::AddTool(id, unionDetails);
        // Unfortunatelly doc will destroy all objects only in the end, but we should delete them before each FullParse
        initData.doc->AddToolOnRemove(unionDetails);
    }
    UniteDetails(id, initData);
    return unionDetails;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief AddToFile add tag with informations about tool into file.
 */
void VToolUnionDetails::AddToFile()
{
    QDomElement domElement = doc->createElement(getTagName());

    doc->SetAttribute(domElement, VDomDocument::AttrId, id);
    doc->SetAttribute(domElement, AttrType, ToolType);
    doc->SetAttribute(domElement, AttrIndexD1, indexD1);
    doc->SetAttribute(domElement, AttrIndexD2, indexD2);

    AddDetail(domElement, data.GetPiece(d1id));
    AddDetail(domElement, data.GetPiece(d2id));

    AddToModeling(domElement);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief RefreshDataInFile refresh attributes in file. If attributes don't exist create them.
 */
void VToolUnionDetails::RefreshDataInFile()
{
    // do nothing
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief AddDetail add detail to xml file.
 * @param domElement tag in xml tree.
 * @param d detail.
 */
void VToolUnionDetails::AddDetail(QDomElement &domElement, const VPiece &d) const
{
    QDomElement det = doc->createElement(TagDetail);

    // nodes
    VToolSeamAllowance::AddNodes(doc, det, d);
    //custom seam allowance
    VToolSeamAllowance::AddCSARecords(doc, det, d.GetCustomSARecords());
    VToolSeamAllowance::AddInternalPaths(doc, det, d.GetInternalPaths());
    VToolSeamAllowance::AddPins(doc, det, d.GetPins());

    domElement.appendChild(det);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief AddToModeling add tool to xml tree.
 * @param domElement tag in xml tree.
 */
void VToolUnionDetails::AddToModeling(const QDomElement &domElement)
{
    const QString drawName = DrawName(doc, d1id, d2id);
    SCASSERT(not drawName.isEmpty())

    QDomElement modeling = doc->GetDraw(drawName).firstChildElement(VAbstractPattern::TagModeling);
    if (not modeling.isNull())
    {
        modeling.appendChild(domElement);
    }
    else
    {
        qCCritical(vToolUnion, "Can't find tag %s.", qUtf8Printable(VAbstractPattern::TagModeling));
        return;
    }
}

//---------------------------------------------------------------------------------------------------------------------
QVector<quint32> VToolUnionDetails::GetReferenceObjects() const
{
    QVector<quint32> list;
    const QDomElement tool = doc->elementById(id);
    if (tool.isNull())
    {
        return list;
    }

    const QStringList parts = QStringList() << VAbstractPattern::TagNodes     /*0*/
                                            << VToolSeamAllowance::TagCSA     /*1*/
                                            << VToolSeamAllowance::TagIPaths; /*2*/

    const QDomNodeList nodesList = tool.childNodes();
    for (qint32 i = 0; i < nodesList.size(); ++i)
    {
        const QDomElement element = nodesList.at(i).toElement();
        if (not element.isNull() && element.tagName() == VToolUnionDetails::TagDetail)
        {
            const QDomNodeList detList = element.childNodes();
            for (qint32 j = 0; j < detList.size(); ++j)
            {
                const QDomElement element = detList.at(j).toElement();
                if (not element.isNull())
                {
                    switch (parts.indexOf(element.tagName()))
                    {
                        case 0://VAbstractPattern::TagNodes
                            list += ReferenceObjects(element, TagNode, AttrIdObject);
                            break;
                        case 1://VToolSeamAllowance::TagCSA
                        case 2://VToolSeamAllowance::TagIPaths
                            list += ReferenceObjects(element, VToolSeamAllowance::TagRecord,
                                                     VAbstractPattern::AttrPath);
                            break;
                        default:
                            break;
                    }
                }
            }
        }
    }
    return list;
}

//---------------------------------------------------------------------------------------------------------------------
QVector<quint32> VToolUnionDetails::ReferenceObjects(const QDomElement &root, const QString &tag,
                                                     const QString &attribute) const
{
    QVector<quint32> objects;

    const QDomNodeList list = root.childNodes();
    for (qint32 i = 0; i < list.size(); ++i)
    {
        const QDomElement element = list.at(i).toElement();
        if (not element.isNull() && element.tagName() == tag)
        {
            const quint32 id = doc->GetParametrUInt(element, attribute, NULL_ID_STR);
            if (id > NULL_ID)
            {
                objects.append(id);
            }
        }
    }

    return objects;
}
