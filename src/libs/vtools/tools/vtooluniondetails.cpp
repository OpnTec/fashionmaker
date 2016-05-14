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
#include "nodeDetails/nodedetails.h"
#include "vtooldetail.h"
#include "../../vgeometry/vpointf.h"
#include "../../vgeometry/varc.h"
#include "../../vgeometry/vsplinepath.h"
#include "../dialogs/tools/dialoguniondetails.h"

#include <QUndoStack>

const QString VToolUnionDetails::TagName          = QStringLiteral("tools");
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

#if defined(Q_CC_CLANG)
    #pragma clang diagnostic push
    #pragma clang diagnostic ignored "-Wmissing-prototypes"
#elif defined(Q_CC_INTEL)
    #pragma warning( push )
    #pragma warning( disable: 1418 )
#endif

Q_LOGGING_CATEGORY(vToolUnion, "v.toolUnion")

#if defined(Q_CC_CLANG)
    #pragma clang diagnostic pop
#elif defined(Q_CC_INTEL)
    #pragma warning( pop )
#endif

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief VToolUnionDetails costructor.
 * @param doc dom document container.
 * @param data dom document container.
 * @param id object id in container.
 * @param d1 first detail.
 * @param d2 second detail.
 * @param indexD1 index edge in first detail.
 * @param indexD2 index edge in second detail.
 * @param typeCreation way we create this tool.
 * @param parent parent object.
 */
VToolUnionDetails::VToolUnionDetails(VAbstractPattern *doc, VContainer *data, const quint32 &id, const VDetail &d1,
                                     const VDetail &d2, const quint32 &indexD1, const quint32 &indexD2,
                                     const Source &typeCreation, const QString &drawName, QObject *parent)
    :VAbstractTool(doc, data, id, parent), d1(d1), d2(d2), indexD1(indexD1), indexD2(indexD2), drawName(drawName)
{
    _referens = 0;
    ToolCreation(typeCreation);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief AddToNewDetail create united detail adding one node per time.
 * @param doc dom document container.
 * @param data container with variables.
 * @param newDetail united detail.
 * @param det detail what we union.
 * @param i index node in detail.
 * @param idTool id tool union details.
 * @param dx bias node x axis.
 * @param dy bias node y axis.
 * @param pRotate point rotation.
 * @param angle angle rotation.
 */
void VToolUnionDetails::AddToNewDetail(VMainGraphicsScene *scene, VAbstractPattern *doc,
                                       VContainer *data, VDetail &newDetail, const VDetail &det, const int &i,
                                       const quint32 &idTool, QVector<quint32> &children, const qreal &dx,
                                       const qreal &dy, const quint32 &pRotate, const qreal &angle)
{
    quint32 id = 0, idObject = 0;
    switch (det.at(i).getTypeTool())
    {
        case (Tool::NodePoint):
        {
            if ( qFuzzyIsNull(dx) && qFuzzyIsNull(dy) && (pRotate == 0))
            {
                id = det.at(i).getId();
            }
            else
            {
                VPointF *point = new VPointF(*data->GeometricObject<VPointF>(det.at(i).getId()));
                point->setMode(Draw::Modeling);
                BiasRotatePoint(point, dx, dy, *data->GeometricObject<VPointF>(pRotate), angle);
                idObject = data->AddGObject(point);
                children.append(idObject);
                VPointF *point1 = new VPointF(*point);
                point1->setMode(Draw::Modeling);
                id = data->AddGObject(point1);
                VNodePoint::Create(doc, data, scene, id, idObject, Document::FullParse, Source::FromGui, idTool);
            }
        }
        break;
        case (Tool::NodeArc):
        {
            if (qFuzzyIsNull(dx) && qFuzzyIsNull(dy) && pRotate == 0)
            {
                id = det.at(i).getId();
            }
            else
            {
                const QPointF p = *data->GeometricObject<VPointF>(pRotate);
                const QSharedPointer<VArc> arc = data->GeometricObject<VArc>(det.at(i).getId());
                VPointF p1 = VPointF(arc->GetP1(), "A", 0, 0);
                BiasRotatePoint(&p1, dx, dy, p, angle);
                VPointF p2 = VPointF(arc->GetP2(), "A", 0, 0);
                BiasRotatePoint(&p2, dx, dy, p, angle);
                VPointF *center = new VPointF(arc->GetCenter());
                BiasRotatePoint(center, dx, dy, p, angle);

                QLineF l1(*center, p1);
                QLineF l2(*center, p2);
                center->setMode(Draw::Modeling);
                quint32 idCenter = data->AddGObject(center);
                Q_UNUSED(idCenter);
                VArc *arc1 = new VArc(*center, arc->GetRadius(), arc->GetFormulaRadius(),
                                 l1.angle(), QString().setNum(l1.angle()), l2.angle(),
                                 QString().setNum(l2.angle()));
                arc1->setMode(Draw::Modeling);
                idObject = data->AddGObject(arc1);
                children.append(idObject);

                VArc *arc2 = new VArc(*arc1);
                arc2->setMode(Draw::Modeling);
                id = data->AddGObject(arc2);

                VNodeArc::Create(doc, data, id, idObject, Document::FullParse, Source::FromGui, idTool);
            }
        }
        break;
        case (Tool::NodeSpline):
        {
            if (qFuzzyIsNull(dx) && qFuzzyIsNull(dy) && pRotate == 0)
            {
                id = det.at(i).getId();
            }
            else
            {
                const QSharedPointer<VSpline> spline = data->GeometricObject<VSpline>(det.at(i).getId());

                const QPointF p = *data->GeometricObject<VPointF>(pRotate);
                VPointF *p1 = new VPointF(spline->GetP1());
                BiasRotatePoint(p1, dx, dy, p, angle);

                VPointF p2 = VPointF(spline->GetP2());
                BiasRotatePoint(&p2, dx, dy, p, angle);

                VPointF p3 = VPointF(spline->GetP3());
                BiasRotatePoint(&p3, dx, dy, p, angle);

                VPointF *p4 = new VPointF(spline->GetP4());
                BiasRotatePoint(p4, dx, dy, p, angle);

                VSpline *spl = new VSpline(*p1, p2, p3, *p4, 0, Draw::Modeling);
                idObject = data->AddGObject(spl);
                children.append(idObject);

                VSpline *spl1 = new VSpline(*spl);
                spl1->setMode(Draw::Modeling);
                id = data->AddGObject(spl1);
                VNodeSpline::Create(doc, data, id, idObject, Document::FullParse, Source::FromGui, idTool);

                delete p4;
                delete p1;
            }
        }
        break;
        case (Tool::NodeSplinePath):
        {
            if (qFuzzyIsNull(dx) && qFuzzyIsNull(dy) && pRotate == 0)
            {
                id = det.at(i).getId();
            }
            else
            {
                VSplinePath *path = new VSplinePath();
                path->setMode(Draw::Modeling);
                const QSharedPointer<VSplinePath> splinePath = data->GeometricObject<VSplinePath>(det.at(i).getId());
                for (qint32 i = 1; i <= splinePath->CountSubSpl(); ++i)
                {
                    const VSplinePoint &point1 = splinePath->at(i-1);
                    const VSplinePoint &point2 = splinePath->at(i);
                    VSpline spline(point1.P(), point2.P(), point1.Angle2(), point1.Angle2Formula(), point2.Angle1(),
                                   point2.Angle1Formula(), point1.Length2(), point1.Length2Formula(), point2.Length1(),
                                   point2.Length1Formula());

                    const QPointF p = *data->GeometricObject<VPointF>(pRotate);
                    VPointF *p1 = new VPointF(spline.GetP1());
                    BiasRotatePoint(p1, dx, dy, p, angle);

                    VPointF p2 = VPointF(spline.GetP2());
                    BiasRotatePoint(&p2, dx, dy, p, angle);

                    VPointF p3 = VPointF(spline.GetP3());
                    BiasRotatePoint(&p3, dx, dy, p, angle);

                    VPointF *p4 = new VPointF(spline.GetP4());
                    BiasRotatePoint(p4, dx, dy, p, angle);

                    VSpline spl = VSpline(*p1, p2, p3, *p4);
                    if (i==1)
                    {
                        const qreal angle1 = spl.GetStartAngle()+180;
                        const QString angle1F = QString().number(angle1);

                        path->append(VSplinePoint(*p1, angle1, angle1F, spl.GetStartAngle(), spl.GetStartAngleFormula(),
                                                  point1.Length1(), point1.Length1Formula(), point1.Length2(),
                                                  point1.Length2Formula()));
                    }

                    const qreal angle2 = spl.GetEndAngle()+180;
                    const QString angle2F = QString().number(angle2);

                    path->append(VSplinePoint(*p4, spl.GetEndAngle(), spl.GetEndAngleFormula(), angle2, angle2F,
                                              point2.Length1(), point2.Length1Formula(), point2.Length2(),
                                              point2.Length2Formula()));

                    delete p4;
                    delete p1;
                }
                idObject = data->AddGObject(path);
                children.append(idObject);

                VSplinePath *path1 = new VSplinePath(*path);
                path1->setMode(Draw::Modeling);
                id = data->AddGObject(path1);
                VNodeSplinePath::Create(doc, data, id, idObject, Document::FullParse, Source::FromGui, idTool);
            }
        }
        break;
        default:
            qDebug()<<"May be wrong tool type!!! Ignoring."<<Q_FUNC_INFO;
            break;
    }
    newDetail.append(VNodeDetail(id, det.at(i).getTypeTool(), NodeDetail::Contour, 0, 0, det.at(i).getReverse()));
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief UpdatePoints update data for united details.
 * @param data container with variables.
 * @param det detail what we union.
 * @param i index node in detail.
 * @param children list ids of all children.
 * @param dx bias node x axis.
 * @param dy bias node y axis.
 * @param pRotate point rotation.
 * @param angle angle rotation.
 */
void VToolUnionDetails::UpdatePoints(VContainer *data, const VDetail &det, const int &i,
                                     QVector<quint32> &children, const qreal &dx, const qreal &dy,
                                     const quint32 &pRotate,
                                     const qreal &angle)
{
    switch (det.at(i).getTypeTool())
    {
        case (Tool::NodePoint):
        {
            if (not qFuzzyIsNull(dx) || not qFuzzyIsNull(dy) || pRotate != 0)
            {
                VPointF *point = new VPointF(*data->GeometricObject<VPointF>(det.at(i).getId()));
                point->setMode(Draw::Modeling);
                BiasRotatePoint(point, dx, dy, *data->GeometricObject<VPointF>(pRotate), angle);
                data->UpdateGObject(TakeNextId(children), point);
            }
        }
        break;
        case (Tool::NodeArc):
        {
            if (not qFuzzyIsNull(dx) || not qFuzzyIsNull(dy) || pRotate != 0)
            {
                const QPointF p = *data->GeometricObject<VPointF>(pRotate);
                const QSharedPointer<VArc> arc = data->GeometricObject<VArc>(det.at(i).getId());
                VPointF p1 = VPointF(arc->GetP1());
                BiasRotatePoint(&p1, dx, dy, p, angle);

                VPointF p2 = VPointF(arc->GetP2());
                BiasRotatePoint(&p2, dx, dy, p, angle);

                VPointF *center = new VPointF(arc->GetCenter());
                BiasRotatePoint(center, dx, dy, p, angle);

                QLineF l1(*center, p1);
                QLineF l2(*center, p2);

                VArc *arc1 = new VArc(*center, arc->GetRadius(), arc->GetFormulaRadius(), l1.angle(),
                                     QString().setNum(l1.angle()), l2.angle(), QString().setNum(l2.angle()));
                arc1->setMode(Draw::Modeling);
                data->UpdateGObject(TakeNextId(children), arc1);
                delete center;
            }
        }
        break;
        case (Tool::NodeSpline):
        {
            if (not qFuzzyIsNull(dx) || not qFuzzyIsNull(dy) || pRotate != 0)
            {
                const QSharedPointer<VSpline> spline = data->GeometricObject<VSpline>(det.at(i).getId());

                const QPointF p = *data->GeometricObject<VPointF>(pRotate);
                VPointF *p1 = new VPointF(spline->GetP1());
                BiasRotatePoint(p1, dx, dy, p, angle);

                VPointF p2 = VPointF(spline->GetP2());
                BiasRotatePoint(&p2, dx, dy, p, angle);

                VPointF p3 = VPointF(spline->GetP3());
                BiasRotatePoint(&p3, dx, dy, p, angle);

                VPointF *p4 = new VPointF(spline->GetP4());
                BiasRotatePoint(p4, dx, dy, p, angle);

                VSpline *spl = new VSpline(*p1, p2, p3, *p4, 0, Draw::Modeling);
                data->UpdateGObject(TakeNextId(children), spl);
                delete p1;
                delete p4;
            }
        }
        break;
        case (Tool::NodeSplinePath):
        {
            if (not qFuzzyIsNull(dx) || not qFuzzyIsNull(dy) || pRotate != 0)
            {
                VSplinePath *path = new VSplinePath();
                path->setMode(Draw::Modeling);
                const QSharedPointer<VSplinePath> splinePath = data->GeometricObject<VSplinePath>(det.at(i).getId());
                SCASSERT(splinePath != nullptr);
                for (qint32 i = 1; i <= splinePath->CountSubSpl(); ++i)
                {
                    const VSplinePoint &point1 = splinePath->at(i-1);
                    const VSplinePoint &point2 = splinePath->at(i);

                    VSpline spline(point1.P(), point2.P(), point1.Angle2(), point1.Angle2Formula(), point2.Angle1(),
                                   point2.Angle1Formula(), point1.Length2(), point1.Length2Formula(), point2.Length1(),
                                   point2.Length1Formula());

                    const QPointF p = *data->GeometricObject<VPointF>(pRotate);
                    VPointF *p1 = new VPointF(spline.GetP1());
                    BiasRotatePoint(p1, dx, dy, p, angle);

                    VPointF p2 = VPointF(spline.GetP2());
                    BiasRotatePoint(&p2, dx, dy, p, angle);

                    VPointF p3 = VPointF(spline.GetP3());
                    BiasRotatePoint(&p3, dx, dy, p, angle);

                    VPointF *p4 = new VPointF(spline.GetP4());
                    BiasRotatePoint(p4, dx, dy, p, angle);

                    VSpline spl = VSpline(*p1, p2, p3, *p4);
                    if (i==1)
                    {
                        const qreal angle1 = spl.GetStartAngle()+180;
                        const QString angle1F = QString().number(angle1);

                        path->append(VSplinePoint(*p1, angle1, angle1F, spl.GetStartAngle(), spl.GetStartAngleFormula(),
                                                  point1.Length1(), point1.Length1Formula(), point1.Length2(),
                                                  point1.Length2Formula()));
                    }

                    const qreal angle2 = spl.GetEndAngle()+180;
                    const QString angle2F = QString().number(angle2);

                    path->append(VSplinePoint(*p4, spl.GetEndAngle(), spl.GetEndAngleFormula(), angle2, angle2F,
                                              point2.Length1(), point2.Length1Formula(), point2.Length2(),
                                              point2.Length2Formula()));

                    delete p1;
                    delete p4;
                }
                data->UpdateGObject(TakeNextId(children), path);
            }
        }
        break;
        default:
            qDebug()<<"May be wrong tool type!!! Ignoring."<<Q_FUNC_INFO;
            break;
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
void VToolUnionDetails::BiasRotatePoint(VPointF *point, const qreal &dx, const qreal &dy, const QPointF &pRotate,
                                        const qreal &angle)
{
    point->setX(point->x()+dx);
    point->setY(point->y()+dy);
    QLineF line(pRotate, *point);
    line.setAngle(line.angle()+angle);
    point->setX(line.p2().x());
    point->setY(line.p2().y());
}

//---------------------------------------------------------------------------------------------------------------------
QString VToolUnionDetails::getTagName() const
{
    return VToolUnionDetails::TagName;
}

//---------------------------------------------------------------------------------------------------------------------
void VToolUnionDetails::ShowVisualization(bool show)
{
    Q_UNUSED(show)
}

//---------------------------------------------------------------------------------------------------------------------
void VToolUnionDetails::incrementReferens()
{
    ++_referens;
    if (_referens == 1)
    {
        IncrementReferences(d1);
        IncrementReferences(d2);

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
    if (_referens > 0)
    {
        --_referens;
    }
    if (_referens == 0)
    {
        DecrementReferences(d1);
        DecrementReferences(d2);

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
    Q_UNUSED(object);
    Q_UNUSED(visible);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief Create help create tool from GUI.
 * @param dialog dialog.
 * @param doc dom document container.
 * @param data container with variables.
 */
VToolUnionDetails* VToolUnionDetails::Create(DialogTool *dialog, VMainGraphicsScene *scene, VAbstractPattern *doc,
                                             VContainer *data)
{
    SCASSERT(dialog != nullptr);
    DialogUnionDetails *dialogTool = qobject_cast<DialogUnionDetails*>(dialog);
    SCASSERT(dialogTool != nullptr);
    VDetail d1 = data->GetDetail(dialogTool->getD1());
    VDetail d2 = data->GetDetail(dialogTool->getD2());
    quint32 indexD1 = static_cast<quint32>(dialogTool->getIndexD1());
    quint32 indexD2 = static_cast<quint32>(dialogTool->getIndexD2());
    qApp->getUndoStack()->beginMacro("union details");
    VToolUnionDetails* tool = Create(0, d1, d2, dialogTool->getD1(), dialogTool->getD2(), indexD1, indexD2, scene,
                                     doc, data, Document::FullParse, Source::FromGui);
    qApp->getUndoStack()->endMacro();
    return tool;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief Create help create tool.
 * @param _id tool id, 0 if tool doesn't exist yet.
 * @param d1 first detail.
 * @param d2 second detail.
 * @param d1id id first detail.
 * @param d2id id second detail.
 * @param indexD1 index edge in first detail.
 * @param indexD2 index edge in second detail.
 * @param scene pointer to scene.
 * @param doc dom document container.
 * @param data container with variables.
 * @param parse parser file mode.
 * @param typeCreation way we create this tool.
 */
VToolUnionDetails* VToolUnionDetails::Create(const quint32 _id, const VDetail &d1, const VDetail &d2,
                                             const quint32 &d1id, const quint32 &d2id, const quint32 &indexD1,
                                             const quint32 &indexD2, VMainGraphicsScene *scene, VAbstractPattern *doc,
                                             VContainer *data, const Document &parse, const Source &typeCreation)
{
    VToolUnionDetails *unionDetails = 0;
    quint32 id = _id;
    QString drawName;
    if (typeCreation == Source::FromGui)
    {
        id = data->getNextId();
        drawName = DrawName(doc, d1id, d2id);
        SCASSERT(not drawName.isEmpty());
    }
    else
    {
        if (parse != Document::FullParse)
        {
            doc->UpdateToolData(id, data);
        }
    }
    //First add tool to file
    VAbstractTool::AddRecord(id, Tool::UnionDetails, doc);
    if (parse == Document::FullParse)
    {
        //Scene doesn't show this tool, so doc will destroy this object.
        unionDetails = new VToolUnionDetails(doc, data, id, d1, d2, indexD1, indexD2, typeCreation, drawName, doc);
        doc->AddTool(id, unionDetails);
        // Unfortunatelly doc will destroy all objects only in the end, but we should delete them before each FullParse
        doc->AddToolOnRemove(unionDetails);
    }
    //Then create new details
    VNodeDetail det1p1;
    VNodeDetail det1p2;
    d1.NodeOnEdge(indexD1, det1p1, det1p2);
    Q_UNUSED(det1p2);

    VPointF point1;
    VPointF point2;
    PointsOnEdge(d1, indexD1, point1, point2, data);

    VPointF point3;
    VPointF point4;
    PointsOnEdge(d2, indexD2, point3, point4, data);

    const qreal dx = point1.x() - point4.x();
    const qreal dy = point1.y() - point4.y();

    point3.setX(point3.x()+dx);
    point3.setY(point3.y()+dy);

    point4.setX(point4.x()+dx);
    point4.setY(point4.y()+dy);

    const QLineF p4p3 = QLineF(point4, point3);
    const QLineF p1p2 = QLineF(point1, point2);

    const qreal angle = p4p3.angleTo(p1p2);
    qint32 pointsD2 = 0; //Keeps number points the second detail, what we have already added.

    const qint32 countNodeD1 = d1.RemoveEdge(indexD1).CountNode();
    const qint32 countNodeD2 = d2.RemoveEdge(indexD2).CountNode();

    if (typeCreation == Source::FromGui)
    {
        qint32 i = 0;
        VDetail newDetail;
        QVector<quint32> children;
        do
        {
            AddToNewDetail(scene, doc, data, newDetail, d1.RemoveEdge(indexD1), i, id, children);
            ++i;
            if (i > d1.indexOfNode(det1p1.getId()) && pointsD2 < countNodeD2-1)
            {
                qint32 j = 0;
                FindIndexJ(pointsD2, d2, indexD2, j);
                do
                {
                    if (j >= countNodeD2)
                    {
                        j=0;
                    }
                    AddToNewDetail(scene, doc, data, newDetail, d2.RemoveEdge(indexD2), j, id, children,
                                   dx, dy, det1p1.getId(), angle);
                    ++pointsD2;
                    ++j;
                } while (pointsD2 < countNodeD2-1);
            }
        } while (i < countNodeD1);

        newDetail.setName("Detail");
        newDetail.setWidth(d1.getWidth());
        VToolDetail::Create(0, newDetail, scene, doc, data, parse, Source::FromTool, drawName);
        QHash<quint32, VDataTool*>* tools = doc->getTools();
        SCASSERT(tools != nullptr);

        {
            VToolDetail *toolDet = qobject_cast<VToolDetail*>(tools->value(d1id));
            SCASSERT(toolDet != nullptr);
            bool ask = false;
            toolDet->Remove(ask);
        }

        VToolDetail *toolDet = qobject_cast<VToolDetail*>(tools->value(d2id));
        SCASSERT(toolDet != nullptr);
        bool ask = false;
        toolDet->Remove(ask);

        SCASSERT(not children.isEmpty())
        SaveChildren(doc, id, children);
    }
    else
    {
        QVector<quint32> children = AllChildren(doc, id);
        if (not children.isEmpty())
        {
            qint32 i = 0;
            do
            {
                //UpdatePoints(data, d1.RemoveEdge(indexD1), i, children);
                ++i;
                if (i > d1.indexOfNode(det1p1.getId()))
                {
                    const int childrenCount = children.size();
                    VDetail d2REdge = d2.RemoveEdge(indexD2);
                    qint32 j = 0;
                    FindIndexJ(pointsD2, d2, indexD2, j);
                    do
                    {
                        if (j >= countNodeD2)
                        {
                            j=0;
                        }
                        UpdatePoints(data, d2REdge, j, children, dx, dy, det1p1.getId(), angle);
                        ++pointsD2;
                        ++j;
                    } while (pointsD2 < childrenCount);
                    break;
                }
            } while (i<countNodeD1);
        }
    }
    return nullptr;
}

//---------------------------------------------------------------------------------------------------------------------
void VToolUnionDetails::PointsOnEdge(const VDetail &d, const quint32 &index, VPointF &p1, VPointF &p2, VContainer *data)
{
    VNodeDetail det2p1;
    VNodeDetail det2p2;
    d.NodeOnEdge(index, det2p1, det2p2);
    p1 = VPointF(*data->GeometricObject<VPointF>(det2p1.getId()));
    p2 = VPointF(*data->GeometricObject<VPointF>(det2p2.getId()));
}

//---------------------------------------------------------------------------------------------------------------------
void VToolUnionDetails::FindIndexJ(const qint32 &pointsD2, const VDetail &d2, const quint32 &indexD2, qint32 &j)
{
    if (pointsD2 == 0)
    {
        VNodeDetail node1;
        VNodeDetail node2;
        d2.NodeOnEdge(indexD2, node1, node2);
        const VDetail removedD2 = d2.RemoveEdge(indexD2);
        const int k = removedD2.indexOfNode(node2.getId());
        SCASSERT(k != -1)
        if (k == removedD2.CountNode()-1)
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
/**
 * @brief GetDetailFromFile parse detail from file.
 * @param doc dom document container.
 * @param domElement tag in xml tree.
 * @return detail stored in file.
 */
QVector<VDetail> VToolUnionDetails::GetDetailFromFile(VAbstractPattern *doc, const QDomElement &domElement)
{
    QVector<VDetail> vector;
    QDomNodeList detailList = domElement.childNodes();
    qint32 num = detailList.size();
    for (qint32 i = 0; i < num; ++i)
    {
        QDomElement element = detailList.at(i).toElement();
        if (element.isNull() == false)
        {
            if (element.tagName() == VToolUnionDetails::TagDetail)
            {
                VDetail d;
                QDomNodeList nodeList = element.childNodes();
                qint32 num = nodeList.size();
                for (qint32 j = 0; j < num; ++j)
                {
                    QDomElement element = nodeList.at(j).toElement();
                    if (element.isNull() == false)
                    {
                        if (element.tagName() == VToolUnionDetails::TagNode)
                        {
                            quint32 id = doc->GetParametrUInt(element, VToolDetail::AttrIdObject, NULL_ID_STR);
                            qreal mx = qApp->toPixel(doc->GetParametrDouble(element, AttrMx, "0.0"));
                            qreal my = qApp->toPixel(doc->GetParametrDouble(element, AttrMy, "0.0"));
                            const bool reversed = doc->GetParametrUInt(element, VToolDetail::AttrReverse, "0");
                            Tool tool = Tool::NodePoint;
                            NodeDetail nodeType = NodeDetail::Contour;
                            QString t = doc->GetParametrString(element, "type", "NodePoint");
                            if (t == "NodePoint")
                            {
                                tool = Tool::NodePoint;
                            }
                            else if (t == "NodeArc")
                            {
                                tool = Tool::NodeArc;
                            }
                            else if (t == "NodeSpline")
                            {
                                tool = Tool::NodeSpline;
                            }
                            else if (t == "NodeSplinePath")
                            {
                                tool = Tool::NodeSplinePath;
                            }
                            d.append(VNodeDetail(id, tool, nodeType, mx, my, reversed));
                        }
                    }
                }
                vector.append(d);
            }
        }
    }
    return vector;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief AddToFile add tag with informations about tool into file.
 */
void VToolUnionDetails::AddToFile()
{
    QDomElement domElement = doc->createElement(TagName);

    doc->SetAttribute(domElement, VDomDocument::AttrId, id);
    doc->SetAttribute(domElement, AttrType, ToolType);
    doc->SetAttribute(domElement, AttrIndexD1, indexD1);
    doc->SetAttribute(domElement, AttrIndexD2, indexD2);

    AddDetail(domElement, d1);
    AddDetail(domElement, d2);

    AddToModeling(domElement);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief RefreshDataInFile refresh attributes in file. If attributes don't exist create them.
 */
void VToolUnionDetails::RefreshDataInFile()
{
    QDomElement domElement = doc->elementById(id);
    if (domElement.isElement())
    {
        doc->SetAttribute(domElement, AttrIndexD1, indexD1);
        doc->SetAttribute(domElement, AttrIndexD2, indexD2);

        QDomNode domNode = domElement.firstChild();
        domNode = UpdateDetail(domNode, d1);
        UpdateDetail(domNode, d2);
    }
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief AddDetail add detail to xml file.
 * @param domElement tag in xml tree.
 * @param d detail.
 */
void VToolUnionDetails::AddDetail(QDomElement &domElement, VDetail &d)
{
    QDomElement det = doc->createElement(TagDetail);

    for (int i = 0; i < d.CountNode(); ++i)
    {
        AddNode(det, d.at(i));
    }

    domElement.appendChild(det);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief AddNode add node to xml file.
 * @param domElement tag in xml tree.
 * @param node node.
 */
void VToolUnionDetails::AddNode(QDomElement &domElement, const VNodeDetail &node)
{
    //Right now method similar to method in class VToolDetail.
    VToolDetail::AddNode(doc, domElement, node);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief UpdateDetail update detail in xml tree.
 * @param domNode dom node.
 * @param d detail.
 * @return return next detail tag in xml tree if exist.
 */
QDomNode VToolUnionDetails::UpdateDetail(const QDomNode &domNode, const VDetail &d)
{
    //QDomNode domNode = domElement.firstChild();
    while (domNode.isNull() == false)
    {
        if (domNode.isElement())
        {
            QDomElement domElement = domNode.toElement();
            if (domElement.isNull() == false)
            {
                if (domElement.tagName() == VToolUnionDetails::TagDetail)
                {
                    VDomDocument::RemoveAllChildren(domElement);//delete all nodes in detail
                    for (int i = 0; i < d.CountNode(); ++i)
                    {
                        AddNode(domElement, d.at(i));//rewrite nodes of detail
                    }
                    break;
                }
            }
        }
    }
    return domNode.nextSibling();
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief AddToModeling add tool to xml tree.
 * @param domElement tag in xml tree.
 */
void VToolUnionDetails::AddToModeling(const QDomElement &domElement)
{
    QDomElement modeling = doc->GetDraw(drawName).firstChildElement(VAbstractPattern::TagModeling);
    if (not modeling.isNull())
    {
        modeling.appendChild(domElement);
    }
    else
    {
        qCDebug(vToolUnion, "Can't find tag %s.", qUtf8Printable(VAbstractPattern::TagModeling));
        return;
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VToolUnionDetails::IncrementReferences(const VDetail &d) const
{
    for (int i = 0; i < d.CountNode(); ++i)
    {
        switch (d.at(i).getTypeTool())
        {
            case (Tool::NodePoint):
            {
                const auto point = VAbstractTool::data.GeometricObject<VPointF>(d.at(i).getId());
                doc->IncrementReferens(point->getIdTool());
                break;
            }
            case (Tool::NodeArc):
            case (Tool::NodeSpline):
            case (Tool::NodeSplinePath):
                doc->IncrementReferens(d.at(i).getId());
                break;
            default:
                qDebug()<<"Get wrong tool type. Ignore.";
                break;
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VToolUnionDetails::DecrementReferences(const VDetail &d) const
{
    for (int i = 0; i < d.CountNode(); ++i)
    {
        switch (d.at(i).getTypeTool())
        {
            case (Tool::NodePoint):
            {
                const auto point = VAbstractTool::data.GeometricObject<VPointF>(d.at(i).getId());
                doc->DecrementReferens(point->getIdTool());
                break;
            }
            case (Tool::NodeArc):
            case (Tool::NodeSpline):
            case (Tool::NodeSplinePath):
                doc->DecrementReferens(d.at(i).getId());
                break;
            default:
                qDebug()<<"Get wrong tool type. Ignore.";
                break;
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VToolUnionDetails::SaveChildren(VAbstractPattern *doc, quint32 id, const QVector<quint32> &children)
{
    QDomElement toolUnion = doc->elementById(id);
    if (toolUnion.isNull())
    {
        return;
    }

    QDomElement tagChildren = doc->createElement(TagChildren);

    for (int i=0; i<children.size(); ++i)
    {
        QDomElement tagChild = doc->createElement(TagChild);
        tagChild.appendChild(doc->createTextNode(QString().setNum(children.at(i))));
        tagChildren.appendChild(tagChild);
    }

    toolUnion.appendChild(tagChildren);
}

//---------------------------------------------------------------------------------------------------------------------
QVector<quint32> VToolUnionDetails::AllChildren(VAbstractPattern *doc, quint32 id)
{
    const QDomElement toolUnion = doc->elementById(id);
    if (toolUnion.isNull())
    {
        return QVector<quint32>();
    }

    const QDomElement tagChildren = toolUnion.firstChildElement(TagChildren);
    if (tagChildren.isNull())
    {
        return QVector<quint32>();
    }

    QVector<quint32> childrenId;
    const QDomNodeList listChildren = tagChildren.elementsByTagName(TagChild);
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
quint32 VToolUnionDetails::TakeNextId(QVector<quint32> &children)
{
    quint32 idChild = NULL_ID;
    if (not children.isEmpty())
    {
        idChild = children.takeFirst();
    }
    else
    {
        idChild = NULL_ID;
    }
    return idChild;
}

//---------------------------------------------------------------------------------------------------------------------
QString VToolUnionDetails::DrawName(VAbstractPattern *doc, quint32 d1id, quint32 d2id)
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
