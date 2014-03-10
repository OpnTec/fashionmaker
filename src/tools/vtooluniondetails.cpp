/************************************************************************
 **
 **  @file   vtooluniondetails.cpp
 **  @author Roman Telezhinsky <dismine@gmail.com>
 **  @date   26 12, 2013
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

#include "vtooluniondetails.h"
#include "nodeDetails/nodedetails.h"
#include "vtooldetail.h"

const QString VToolUnionDetails::TagName          = QStringLiteral("tools");
const QString VToolUnionDetails::ToolType         = QStringLiteral("unionDetails");
const QString VToolUnionDetails::TagDetail        = QStringLiteral("det");
const QString VToolUnionDetails::TagNode          = QStringLiteral("node");
const QString VToolUnionDetails::AttrIndexD1      = QStringLiteral("indexD1");
const QString VToolUnionDetails::AttrIndexD2      = QStringLiteral("indexD2");
const QString VToolUnionDetails::AttrIdObject     = QStringLiteral("idObject");
const QString VToolUnionDetails::AttrNodeType     = QStringLiteral("nodeType");
const QString VToolUnionDetails::NodeTypeContour  = QStringLiteral("Contour");
const QString VToolUnionDetails::NodeTypeModeling = QStringLiteral("Modeling");

VToolUnionDetails::VToolUnionDetails(VPattern *doc, VContainer *data, const quint32 &id, const VDetail &d1,
                                     const VDetail &d2, const quint32 &indexD1, const quint32 &indexD2,
                                     const Tool::Sources &typeCreation, QObject *parent)
    :VAbstractTool(doc, data, id, parent), d1(d1), d2(d2), indexD1(indexD1), indexD2(indexD2)
{
    if (typeCreation == Tool::FromGui)
    {
        AddToFile();
    }
    else
    {
        RefreshDataInFile();
    }
}

void VToolUnionDetails::AddToNewDetail(QObject *tool, VPattern *doc, VContainer *data, VDetail &newDetail,
                                       const VDetail &det, const ptrdiff_t &i, const quint32 &idTool, const qreal &dx,
                                       const qreal &dy, const quint32 &pRotate, const qreal &angle)
{
    quint32 id = 0, idObject = 0;
    switch (det.at(i).getTypeTool())
    {
        case (Tool::NodePoint):
        {
            if ( qFuzzyCompare(dx+1, 1) && qFuzzyCompare(dy+1, 1) && (pRotate == 0))
            {
                id = det.at(i).getId();
            }
            else
            {
                VPointF *point = new VPointF(*data->GeometricObject<const VPointF *>(det.at(i).getId()));
                point->setMode(Draw::Modeling);
                BiasRotatePoint(point, dx, dy, data->GeometricObject<const VPointF *>(pRotate)->toQPointF(),
                                angle);
                idObject = data->AddGObject(point);
                VPointF *point1 = new VPointF(*point);
                point1->setMode(Draw::Modeling);
                id = data->AddGObject(point1);
                VNodePoint::Create(doc, data, id, idObject, Document::FullParse, Tool::FromGui, idTool, tool);
            }
        }
        break;
        case (Tool::NodeArc):
        {
            if (qFuzzyCompare(dx+1, 1) && qFuzzyCompare(dy+1, 1) && pRotate == 0)
            {
                id = det.at(i).getId();
            }
            else
            {
                const VArc *arc = data->GeometricObject<const VArc *>(det.at(i).getId());
                VPointF p1 = VPointF(arc->GetP1().x(), arc->GetP1().y(), "A", 0, 0);
                BiasRotatePoint(&p1, dx, dy, data->GeometricObject<const VPointF *>(pRotate)->toQPointF(), angle);
                VPointF p2 = VPointF(arc->GetP2().x(), arc->GetP2().y(), "A", 0, 0);
                BiasRotatePoint(&p2, dx, dy, data->GeometricObject<const VPointF *>(pRotate)->toQPointF(), angle);
                VPointF *center = new VPointF(arc->GetCenter());
                BiasRotatePoint(center, dx, dy, data->GeometricObject<const VPointF *>(pRotate)->toQPointF(),
                                angle);

                QLineF l1(center->toQPointF(), p1.toQPointF());
                QLineF l2(center->toQPointF(), p2.toQPointF());
                center->setMode(Draw::Modeling);
                quint32 idCenter = data->AddGObject(center);
                Q_UNUSED(idCenter);
                VArc *arc1 = new VArc(*center, arc->GetRadius(), arc->GetFormulaRadius(),
                                 l1.angle(), QString().setNum(l1.angle()), l2.angle(),
                                 QString().setNum(l2.angle()));
                arc1->setMode(Draw::Modeling);
                idObject = data->AddGObject(arc1);

                VArc *arc2 = new VArc(*arc1);
                arc2->setMode(Draw::Modeling);
                id = data->AddGObject(arc2);

                VNodeArc::Create(doc, data, id, idObject, Document::FullParse, Tool::FromGui, idTool, tool);
            }
        }
        break;
        case (Tool::NodeSpline):
        {
            if (qFuzzyCompare(dx+1, 1) && qFuzzyCompare(dy+1, 1) && pRotate == 0)
            {
                id = det.at(i).getId();
            }
            else
            {
                const VSpline *spline = data->GeometricObject<const VSpline *>(det.at(i).getId());

                VPointF *p1 = new VPointF(spline->GetP1());
                BiasRotatePoint(p1, dx, dy, data->GeometricObject<const VPointF *>(pRotate)->toQPointF(), angle);
                quint32 idP1 = data->AddGObject(p1);

                VPointF p2 = VPointF(spline->GetP2());
                BiasRotatePoint(&p2, dx, dy, data->GeometricObject<const VPointF *>(pRotate)->toQPointF(), angle);

                VPointF p3 = VPointF(spline->GetP3());
                BiasRotatePoint(&p3, dx, dy, data->GeometricObject<const VPointF *>(pRotate)->toQPointF(), angle);

                VPointF *p4 = new VPointF(spline->GetP4());
                BiasRotatePoint(p4, dx, dy, data->GeometricObject<const VPointF *>(pRotate)->toQPointF(), angle);
                quint32 idP4 = data->AddGObject(p4);

                VSpline *spl = new VSpline(*p1, p2.toQPointF(), p3.toQPointF(), *p4, spline->GetKcurve(), 0,
                Draw::Modeling);
                idObject = data->AddGObject(spl);

                VSpline *spl1 = new VSpline(*spl);
                spl1->setMode(Draw::Modeling);
                id = data->AddGObject(spl1);
                VNodeSpline::Create(doc, data, id, idObject, Document::FullParse, Tool::FromGui, idTool, tool);
            }
        }
        break;
        case (Tool::NodeSplinePath):
        {
            if (qFuzzyCompare(dx+1, 1) && qFuzzyCompare(dy+1, 1) && pRotate == 0)
            {
                id = det.at(i).getId();
            }
            else
            {
                VSplinePath *path = new VSplinePath();
                path->setMode(Draw::Modeling);
                const VSplinePath *splinePath = data->GeometricObject<const VSplinePath *>(det.at(i).getId());
                qint32 k = splinePath->getMaxCountPoints();
                for (qint32 i = 1; i <= splinePath->Count(); ++i)
                {
                    VSpline spline(splinePath->at(i-1).P(), splinePath->at(i).P(),
                            splinePath->at(i-1).Angle2(), splinePath->at(i).Angle1(), splinePath->at(i-1).KAsm2(),
                            splinePath->at(i).KAsm1(), splinePath->getKCurve());

                    VPointF *p1 = new VPointF(spline.GetP1());
                    BiasRotatePoint(p1, dx, dy, data->GeometricObject<const VPointF *>(pRotate)->toQPointF(),
                                    angle);
                    quint32 idP1 = data->AddGObject(p1);
                    --k;

                    VPointF p2 = VPointF(spline.GetP2());
                    BiasRotatePoint(&p2, dx, dy, data->GeometricObject<const VPointF *>(pRotate)->toQPointF(),
                                    angle);

                    VPointF p3 = VPointF(spline.GetP3());
                    BiasRotatePoint(&p3, dx, dy, data->GeometricObject<const VPointF *>(pRotate)->toQPointF(),
                                    angle);

                    VPointF *p4 = new VPointF(spline.GetP4());
                    BiasRotatePoint(p4, dx, dy, data->GeometricObject<const VPointF *>(pRotate)->toQPointF(),
                                    angle);
                    quint32 idP4 = data->AddGObject(p4);
                    --k;

                    VSpline spl = VSpline(*p1, p2.toQPointF(), p3.toQPointF(), *p4, spline.GetKcurve());
                    if (i==1)
                    {
                        path->append(VSplinePoint(*p1, splinePath->at(i-1).KAsm1(), spl.GetAngle1()+180,
                                                  splinePath->at(i-1).KAsm2(), spl.GetAngle1()));
                    }
                        path->append(VSplinePoint(*p4, splinePath->at(i).KAsm1(), spl.GetAngle2(),
                                                  splinePath->at(i).KAsm2(), spl.GetAngle2()+180));
                }
                while (k>=0)
                {
                    data->getNextId();
                    --k;
                }
                idObject = data->AddGObject(path);

                VSplinePath *path1 = new VSplinePath(*path);
                path1->setMode(Draw::Modeling);
                id = data->AddGObject(path1);
                VNodeSplinePath::Create(doc, data, id, idObject, Document::FullParse, Tool::FromGui, idTool, tool);
            }
        }
        break;
        default:
            qWarning()<<"May be wrong tool type!!! Ignoring."<<Q_FUNC_INFO;
            break;
    }
    newDetail.append(VNodeDetail(id, det.at(i).getTypeTool(), NodeDetail::Contour));
}

void VToolUnionDetails::UpdatePoints(const quint32 &idDetail, VContainer *data, const VDetail &det, const ptrdiff_t &i,
                                     quint32 &idCount, const qreal &dx, const qreal &dy, const quint32 &pRotate,
                                     const qreal &angle)
{
    switch (det.at(i).getTypeTool())
    {
        case (Tool::NodePoint):
        {
            if (qFuzzyCompare(dx+1, 1) == false || qFuzzyCompare(dy+1, 1) == false || pRotate != 0)
            {
                VPointF *point = new VPointF(*data->GeometricObject<const VPointF *>(det.at(i).getId()));
                point->setMode(Draw::Modeling);
                BiasRotatePoint(point, dx, dy, data->GeometricObject<const VPointF *>(pRotate)->toQPointF(), angle);
                ++idCount;
                data->UpdateGObject(idDetail+idCount, point);

                ++idCount;
            }
        }
        break;
        case (Tool::NodeArc):
        {
            if (qFuzzyCompare(dx+1, 1) == false || qFuzzyCompare(dy+1, 1) == false || pRotate != 0)
            {
                const VArc *arc = data->GeometricObject<const VArc *>(det.at(i).getId());
                VPointF p1 = VPointF(arc->GetP1());
                BiasRotatePoint(&p1, dx, dy, data->GeometricObject<const VPointF *>(pRotate)->toQPointF(), angle);
                VPointF p2 = VPointF(arc->GetP2());
                BiasRotatePoint(&p2, dx, dy, data->GeometricObject<const VPointF *>(pRotate)->toQPointF(), angle);
                VPointF *center = new VPointF(arc->GetCenter());
                BiasRotatePoint(center, dx, dy, data->GeometricObject<const VPointF *>(pRotate)->toQPointF(),
                                angle);

                QLineF l1(center->toQPointF(), p1.toQPointF());
                QLineF l2(center->toQPointF(), p2.toQPointF());
                ++idCount;
                center->setMode(Draw::Modeling);
                data->UpdateGObject(idDetail+idCount, center);
                VArc *arc1 = new VArc(*center, arc->GetRadius(), arc->GetFormulaRadius(), l1.angle(),
                                     QString().setNum(l1.angle()), l2.angle(), QString().setNum(l2.angle()));
                arc1->setMode(Draw::Modeling);
                ++idCount;
                data->UpdateGObject(idDetail+idCount, arc1);

                ++idCount;
            }
        }
        break;
        case (Tool::NodeSpline):
        {
            if (qFuzzyCompare(dx+1, 1) == false || qFuzzyCompare(dy+1, 1) == false || pRotate != 0)
            {
                const VSpline *spline = data->GeometricObject<const VSpline *>(det.at(i).getId());

                VPointF *p1 = new VPointF(spline->GetP1());
                BiasRotatePoint(p1, dx, dy, data->GeometricObject<const VPointF *>(pRotate)->toQPointF(), angle);
                ++idCount;
                data->UpdateGObject(idDetail+idCount, p1);

                VPointF p2 = VPointF(spline->GetP2());
                BiasRotatePoint(&p2, dx, dy, data->GeometricObject<const VPointF *>(pRotate)->toQPointF(), angle);

                VPointF p3 = VPointF(spline->GetP3());
                BiasRotatePoint(&p3, dx, dy, data->GeometricObject<const VPointF *>(pRotate)->toQPointF(), angle);

                VPointF *p4 = new VPointF(spline->GetP4());
                BiasRotatePoint(p4, dx, dy, data->GeometricObject<const VPointF *>(pRotate)->toQPointF(), angle);
                ++idCount;
                data->UpdateGObject(idDetail+idCount, p4);

                VSpline *spl = new VSpline(*p1, p2.toQPointF(), p3.toQPointF(), *p4, spline->GetKcurve(), 0,
                Draw::Modeling);

                ++idCount;
                data->UpdateGObject(idDetail+idCount, spl);

                ++idCount;
            }
        }
        break;
        case (Tool::NodeSplinePath):
        {
            if (qFuzzyCompare(dx+1, 1) == false || qFuzzyCompare(dy+1, 1) == false || pRotate != 0)
            {
                VSplinePath *path = new VSplinePath();
                path->setMode(Draw::Modeling);
                const VSplinePath *splinePath = data->GeometricObject<const VSplinePath *>(det.at(i).getId());
                Q_CHECK_PTR(splinePath);
                qint32 k = splinePath->getMaxCountPoints();
                for (qint32 i = 1; i <= splinePath->Count(); ++i)
                {
                    VSpline spline(splinePath->at(i-1).P(), splinePath->at(i).P(),
                            splinePath->at(i-1).Angle2(), splinePath->at(i).Angle1(), splinePath->at(i-1).KAsm2(),
                            splinePath->at(i).KAsm1(), splinePath->getKCurve());

                    VPointF *p1 = new VPointF(spline.GetP1());
                    BiasRotatePoint(p1, dx, dy, data->GeometricObject<const VPointF *>(pRotate)->toQPointF(),
                                    angle);
                    ++idCount;
                    data->UpdateGObject(idDetail+idCount, p1);
                    --k;

                    VPointF p2 = VPointF(spline.GetP2());
                    BiasRotatePoint(&p2, dx, dy, data->GeometricObject<const VPointF *>(pRotate)->toQPointF(),
                                    angle);

                    VPointF p3 = VPointF(spline.GetP3());
                    BiasRotatePoint(&p3, dx, dy, data->GeometricObject<const VPointF *>(pRotate)->toQPointF(),
                                    angle);

                    VPointF *p4 = new VPointF(spline.GetP4());
                    BiasRotatePoint(p4, dx, dy, data->GeometricObject<const VPointF *>(pRotate)->toQPointF(),
                                    angle);
                    ++idCount;
                    data->UpdateGObject(idDetail+idCount, p4);
                    --k;

                    VSpline spl = VSpline(*p1, p2.toQPointF(), p3.toQPointF(), *p4, spline.GetKcurve());
                    if (i==1)
                    {
                        path->append(VSplinePoint(*p1, splinePath->at(i-1).KAsm1(), spl.GetAngle1()+180,
                                                  splinePath->at(i-1).KAsm2(), spl.GetAngle1()));
                    }
                        path->append(VSplinePoint(*p4, splinePath->at(i).KAsm1(), spl.GetAngle2(),
                                                  splinePath->at(i).KAsm2(), spl.GetAngle2()+180));
                }

                while (k>=0)
                {
                    data->getNextId();
                    --k;
                    ++idCount;
                }

                ++idCount;
                data->UpdateGObject(idDetail+idCount, path);

                ++idCount;
            }
        }
        break;
        default:
            qWarning()<<"May be wrong tool type!!! Ignoring."<<Q_FUNC_INFO;
            break;
    }
}

void VToolUnionDetails::BiasRotatePoint(VPointF *point, const qreal &dx, const qreal &dy, const QPointF &pRotate,
                                        const qreal angle)
{
    point->setX(point->x()+dx);
    point->setY(point->y()+dy);
    QLineF line(pRotate, point->toQPointF());
    line.setAngle(line.angle()+angle);
    point->setX(line.p2().x());
    point->setY(line.p2().y());
}

void VToolUnionDetails::Create(DialogTool *dialog, VMainGraphicsScene *scene, VPattern *doc, VContainer *data)
{
    Q_CHECK_PTR(dialog);
    DialogUnionDetails *dialogTool = qobject_cast<DialogUnionDetails*>(dialog);
    Q_CHECK_PTR(dialogTool);
    VDetail d1 = data->GetDetail(dialogTool->getD1());
    VDetail d2 = data->GetDetail(dialogTool->getD2());
    ptrdiff_t indexD1 = dialogTool->getIndexD1();
    ptrdiff_t indexD2 = dialogTool->getIndexD2();
    Create(0, d1, d2, dialogTool->getD1(), dialogTool->getD2(), indexD1, indexD2, scene, doc, data, Document::FullParse,
           Tool::FromGui);
}

void VToolUnionDetails::Create(const quint32 _id, const VDetail &d1, const VDetail &d2, const quint32 &d1id,
                               const quint32 &d2id, const quint32 &indexD1, const quint32 &indexD2,
                               VMainGraphicsScene *scene, VPattern *doc, VContainer *data,
                               const Document::Documents &parse, const Tool::Sources &typeCreation)
{
    VToolUnionDetails *unionDetails = 0;
    quint32 id = _id;
    if (typeCreation == Tool::FromGui)
    {
        id = data->getNextId();
    }
    else
    {
        if (parse != Document::FullParse)
        {
            doc->UpdateToolData(id, data);
        }
    }
    VAbstractTool::AddRecord(id, Tool::UnionDetails, doc);
    if (parse == Document::FullParse)
    {
        //Scene doesn't show this tool, so doc will destroy this object.
        unionDetails = new VToolUnionDetails(doc, data, id, d1, d2, indexD1, indexD2, typeCreation, doc);
        QHash<quint32, VDataTool*>* tools = doc->getTools();
        tools->insert(id, unionDetails);
        for (ptrdiff_t i = 0; i < d1.CountNode(); ++i)
        {
            doc->IncrementReferens(d1.at(i).getId());
        }
        for (ptrdiff_t i = 0; i < d2.CountNode(); ++i)
        {
            doc->IncrementReferens(d2.at(i).getId());
        }

    }
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

    const qreal angle = QLineF(point4.toQPointF(), point3.toQPointF()).angleTo(QLineF(point1.toQPointF(),
                                                                                      point2.toQPointF()));
    qint32 pointsD2 = 0; //Keeps count points second detail, what we already add.

    if (typeCreation == Tool::FromGui)
    {
        qint32 j = 0, i = 0;
        VDetail newDetail;
        do
        {
            AddToNewDetail(unionDetails, doc, data, newDetail, d1.RemoveEdge(indexD1), i, id);
            ++i;
            if (i > d1.indexOfNode(det1p1.getId()) && pointsD2 < d2.RemoveEdge(indexD2).CountNode()-2)
            {
                do
                {
                    FindJ(pointsD2, d2, indexD2, j);
                    if (pointsD2 == d2.RemoveEdge(indexD2).CountNode() -2)
                    {
                        break;
                    }
                    if (j >= d2.RemoveEdge(indexD2).CountNode())
                    {
                        j=0;
                    }
                    AddToNewDetail(unionDetails, doc, data, newDetail, d2.RemoveEdge(indexD2), j, id, dx, dy,
                                   det1p1.getId(), angle);
                    ++pointsD2;
                    ++j;
                } while (pointsD2 < d2.RemoveEdge(indexD2).CountNode());
            }
        } while(i < d1.RemoveEdge(indexD1).CountNode());

        newDetail.setName("Detail");
        VToolDetail::Create(0, newDetail, scene, doc, data, parse, Tool::FromTool);
        QHash<quint32, VDataTool*>* tools = doc->getTools();
        Q_CHECK_PTR(tools);

        {
            VToolDetail *toolDet = qobject_cast<VToolDetail*>(tools->value(d1id));
            Q_CHECK_PTR(toolDet);
            toolDet->Remove();
        }

        VToolDetail *toolDet = qobject_cast<VToolDetail*>(tools->value(d2id));
        Q_CHECK_PTR(toolDet);
        toolDet->Remove();
    }
    else
    {
        quint32 idCount = 0;
        qint32 j = 0, i = 0;
        do
        {
            UpdatePoints(id, data, d1.RemoveEdge(indexD1), i, idCount);
            ++i;
            if (i > d1.indexOfNode(det1p1.getId()) && pointsD2 < d2.RemoveEdge(indexD2).CountNode()-2)
            {
                do
                {
                    FindJ(pointsD2, d2, indexD2, j);
                    if (pointsD2 == d2.RemoveEdge(indexD2).CountNode()-2)
                    {
                        break;
                    }
                    if (j >= d2.RemoveEdge(indexD2).CountNode())
                    {
                        j=0;
                    }
                    UpdatePoints(id, data, d2.RemoveEdge(indexD2), j, idCount, dx, dy, det1p1.getId(), angle);
                    ++pointsD2;
                    ++j;
                } while (pointsD2 < d2.RemoveEdge(indexD2).CountNode());
            }
        } while (i<d1.RemoveEdge(indexD1).CountNode());
    }
}

void VToolUnionDetails::PointsOnEdge(const VDetail &d, const qint32 &index, VPointF &p1, VPointF &p2, VContainer *data)
{
    VNodeDetail det2p1;
    VNodeDetail det2p2;
    d.NodeOnEdge(index, det2p1, det2p2);
    p1 = VPointF(*data->GeometricObject<const VPointF *>(det2p1.getId()));
    p2 = VPointF(*data->GeometricObject<const VPointF *>(det2p2.getId()));
}

void VToolUnionDetails::FindJ(const qint32 &pointsD2, const VDetail &d2, const qint32 &indexD2, qint32 &j)
{
    if (pointsD2 == 0)
    {
        VNodeDetail node1;
        VNodeDetail node2;
        d2.NodeOnEdge(indexD2, node1, node2);
        ptrdiff_t k = d2.RemoveEdge(indexD2).indexOfNode(node2.getId());
        if (k == d2.RemoveEdge(indexD2).CountNode()-1)
        {
            j = 0;
        }
        else
        {
            j = d2.RemoveEdge(indexD2).indexOfNode(node2.getId())+1;
        }
    }
}

QVector<VDetail> VToolUnionDetails::GetDetailFromFile(VPattern *doc, const QDomElement &domElement)
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
                            quint32 id = doc->GetParametrUInt(element, VToolDetail::AttrIdObject, "0");
                            qreal mx = toPixel(doc->GetParametrDouble(element, VAbstractTool::AttrMx, "0.0"));
                            qreal my = toPixel(doc->GetParametrDouble(element, VAbstractTool::AttrMy, "0.0"));
                            Tool::Tools tool;
                            NodeDetail::NodeDetails nodeType = NodeDetail::Contour;
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
                            d.append(VNodeDetail(id, tool, nodeType, mx, my));
                        }
                    }
                }
                vector.append(d);
            }
        }
    }
    return vector;
}

void VToolUnionDetails::AddToFile()
{
    QDomElement domElement = doc->createElement(TagName);

    doc->SetAttribute(domElement, AttrId, id);
    doc->SetAttribute(domElement, AttrType, ToolType);
    doc->SetAttribute(domElement, AttrIndexD1, indexD1);
    doc->SetAttribute(domElement, AttrIndexD2, indexD2);

    AddDetail(domElement, d1);
    AddDetail(domElement, d2);

    AddToModeling(domElement);
}

void VToolUnionDetails::RefreshDataInFile()
{
    QDomElement domElement = doc->elementById(QString().setNum(id));
    if (domElement.isElement())
    {
        doc->SetAttribute(domElement, AttrIndexD1, indexD1);
        doc->SetAttribute(domElement, AttrIndexD2, indexD2);

        QDomNode domNode = domElement.firstChild();
        domNode = UpdateDetail(domNode, d1);
        UpdateDetail(domNode, d2);
    }
}

void VToolUnionDetails::AddDetail(QDomElement &domElement, VDetail &d)
{
    QDomElement det = doc->createElement(TagDetail);

    for (ptrdiff_t i = 0; i < d.CountNode(); ++i)
    {
        AddNode(det, d[i]);
    }

    domElement.appendChild(det);
}

void VToolUnionDetails::AddNode(QDomElement &domElement, const VNodeDetail &node)
{
    QDomElement nod = doc->createElement(TagNode);

    doc->SetAttribute(nod, AttrIdObject, node.getId());
    doc->SetAttribute(nod, AttrMx, toMM(node.getMx()));
    doc->SetAttribute(nod, AttrMy, toMM(node.getMy()));
    if (node.getTypeNode() == NodeDetail::Contour)
    {
        doc->SetAttribute(nod, AttrNodeType, NodeTypeContour);
    }
    else
    {
        doc->SetAttribute(nod, AttrNodeType, NodeTypeModeling);
    }
    switch (node.getTypeTool())
    {
        case (Tool::NodeArc):
            doc->SetAttribute(nod, AttrType, QStringLiteral("NodeArc"));
            break;
        case (Tool::NodePoint):
            doc->SetAttribute(nod, AttrType, QStringLiteral("NodePoint"));
            break;
        case (Tool::NodeSpline):
            doc->SetAttribute(nod, AttrType, QStringLiteral("NodeSpline"));
            break;
        case (Tool::NodeSplinePath):
            doc->SetAttribute(nod, AttrType, QStringLiteral("NodeSplinePath"));
            break;
        default:
            qWarning()<<"May be wrong tool type!!! Ignoring."<<Q_FUNC_INFO;
            break;
    }
    domElement.appendChild(nod);
}

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
                    doc->removeAllChilds(domElement);//delete all nodes in detail
                    for (ptrdiff_t i = 0; i < d.CountNode(); ++i)
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

void VToolUnionDetails::AddToModeling(const QDomElement &domElement)
{
    QDomElement modelingElement;
    bool ok = doc->GetActivNodeElement(VPattern::TagModeling, modelingElement);
    if (ok)
    {
        modelingElement.appendChild(domElement);
    }
    else
    {
        qCritical()<<tr("Can't find tag Modeling")<< Q_FUNC_INFO;
    }
    emit toolhaveChange();
}
