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

VToolUnionDetails::VToolUnionDetails(VDomDocument *doc, VContainer *data, const qint64 &id, const VDetail &d1,
                                     const VDetail &d2, const ptrdiff_t &indexD1, const ptrdiff_t &indexD2,
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

void VToolUnionDetails::AddToNewDetail(QObject *tool, VDomDocument *doc, VContainer *data, VDetail &newDetail,
                                       const VDetail &det, const ptrdiff_t &i, const qint64 &idTool, const qreal &dx,
                                       const qreal &dy, const qint64 &pRotate, const qreal &angle)
{
    qint64 id = 0, idObject = 0;
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
                Q_ASSERT(point != 0);
                point->setMode(Draw::Modeling);
                BiasRotatePoint(point, dx, dy, data->GeometricObject<const VPointF *>(pRotate)->toQPointF(),
                                angle);
                idObject = data->AddGObject(point);
                VPointF *point1 = new VPointF(*point);
                Q_ASSERT(point1 != 0);
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
                Q_ASSERT(center != 0);
                BiasRotatePoint(center, dx, dy, data->GeometricObject<const VPointF *>(pRotate)->toQPointF(),
                                angle);

                QLineF l1(center->toQPointF(), p1.toQPointF());
                QLineF l2(center->toQPointF(), p2.toQPointF());
                qint64 idCenter = data->AddGObject(center);
                Q_UNUSED(idCenter);
                VArc *arc1 = new VArc(*center, arc->GetRadius(), arc->GetFormulaRadius(),
                                 l1.angle(), QString().setNum(l1.angle()), l2.angle(),
                                 QString().setNum(l2.angle()));
                Q_ASSERT(arc1 != 0);
                arc1->setMode(Draw::Modeling);
                id = data->AddGObject(arc1);

                VArc *arc2 = new VArc(*arc1);
                Q_ASSERT(arc2 != 0);
                arc2->setMode(Draw::Modeling);
                idObject = data->AddGObject(arc2);

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
                Q_ASSERT(p1 != 0);
                BiasRotatePoint(p1, dx, dy, data->GeometricObject<const VPointF *>(pRotate)->toQPointF(), angle);
                qint64 idP1 = data->AddGObject(p1);

                VPointF p2 = VPointF(spline->GetP2().x(), spline->GetP2().y(), "A", 0, 0);
                BiasRotatePoint(&p2, dx, dy, data->GeometricObject<const VPointF *>(pRotate)->toQPointF(), angle);

                VPointF p3 = VPointF(spline->GetP3().x(), spline->GetP3().y(), "A", 0, 0);
                BiasRotatePoint(&p3, dx, dy, data->GeometricObject<const VPointF *>(pRotate)->toQPointF(), angle);

                VPointF *p4 = new VPointF(spline->GetP4());
                Q_ASSERT(p4 != 0);
                BiasRotatePoint(p4, dx, dy, data->GeometricObject<const VPointF *>(pRotate)->toQPointF(), angle);
                qint64 idP4 = data->AddGObject(p4);

                VSpline *spl = new VSpline(*p1, p2.toQPointF(), p3.toQPointF(), *p4, spline->GetKcurve());
                Q_ASSERT(spl != 0);
                spl->setMode(Draw::Modeling);
                idObject = data->AddGObject(spl);

                VSpline *spl1 = new VSpline(*spl);
                Q_ASSERT(spl1 != 0);
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
                Q_ASSERT(path != 0);
                path->setMode(Draw::Modeling);
                const VSplinePath *splinePath = data->GeometricObject<const VSplinePath *>(det.at(i).getId());
                for (qint32 i = 1; i <= splinePath->Count(); ++i)
                {
                    VSpline spline(splinePath->at(i-1).P(), splinePath->at(i).P(),
                            splinePath->at(i-1).Angle2(), splinePath->at(i).Angle1(), splinePath->at(i-1).KAsm2(),
                            splinePath->at(i).KAsm1(), splinePath->getKCurve());

                    VPointF *p1 = new VPointF(spline.GetP1());
                    Q_ASSERT(p1 != 0);
                    BiasRotatePoint(p1, dx, dy, data->GeometricObject<const VPointF *>(pRotate)->toQPointF(),
                                    angle);
                    qint64 idP1 = data->AddGObject(p1);

                    VPointF p2 = VPointF(spline.GetP2());
                    BiasRotatePoint(&p2, dx, dy, data->GeometricObject<const VPointF *>(pRotate)->toQPointF(),
                                    angle);

                    VPointF p3 = VPointF(spline.GetP3());
                    BiasRotatePoint(&p3, dx, dy, data->GeometricObject<const VPointF *>(pRotate)->toQPointF(),
                                    angle);

                    VPointF *p4 = new VPointF(spline.GetP4());
                    Q_ASSERT(p4 != 0);
                    BiasRotatePoint(p4, dx, dy, data->GeometricObject<const VPointF *>(pRotate)->toQPointF(),
                                    angle);
                    qint64 idP4 = data->AddGObject(p4);

                    VSpline spl = VSpline(*p1, p2.toQPointF(), p3.toQPointF(), *p4, spline.GetKcurve());
                    if (i==1)
                    {
                        path->append(VSplinePoint(*p1, splinePath->at(i-1).KAsm1(), spl.GetAngle1(),
                                                  splinePath->at(i-1).KAsm1()));
                    }
                        path->append(VSplinePoint(*p4, splinePath->at(i).KAsm1(), spl.GetAngle1(),
                                                  splinePath->at(i).KAsm1()));
                }
                idObject = data->AddGObject(path);

                VSplinePath *path1 = new VSplinePath(*path);
                Q_ASSERT(path1 != 0);
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

void VToolUnionDetails::UpdatePoints(const qint64 &idDetail, VContainer *data, const VDetail &det, const ptrdiff_t &i,
                                     qint64 &idCount, const qreal &dx, const qreal &dy, const qint64 &pRotate,
                                     const qreal &angle)
{
    switch (det.at(i).getTypeTool())
    {
        case (Tool::NodePoint):
        {
            if (qFuzzyCompare(dx+1, 1) == false && qFuzzyCompare(dy+1, 1) == false && pRotate != 0)
            {
                VPointF *point = new VPointF(*data->GeometricObject<const VPointF *>(det.at(i).getId()));
                Q_ASSERT(point != 0);
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
            if (qFuzzyCompare(dx+1, 1) == false && qFuzzyCompare(dy+1, 1) == false && pRotate != 0)
            {
                const VArc *arc = data->GeometricObject<const VArc *>(det.at(i).getId());
                VPointF p1 = VPointF(arc->GetP1());
                BiasRotatePoint(&p1, dx, dy, data->GeometricObject<const VPointF *>(pRotate)->toQPointF(), angle);
                VPointF p2 = VPointF(arc->GetP2());
                BiasRotatePoint(&p2, dx, dy, data->GeometricObject<const VPointF *>(pRotate)->toQPointF(), angle);
                VPointF *center = new VPointF(arc->GetCenter());
                Q_ASSERT(center != 0);
                BiasRotatePoint(center, dx, dy, data->GeometricObject<const VPointF *>(pRotate)->toQPointF(),
                                angle);

                QLineF l1(center->toQPointF(), p1.toQPointF());
                QLineF l2(center->toQPointF(), p2.toQPointF());
                ++idCount;
                data->UpdateGObject(idDetail+idCount, center);
                VArc *arc1 = new VArc(*center, arc->GetRadius(), arc->GetFormulaRadius(), l1.angle(),
                                     QString().setNum(l1.angle()), l2.angle(), QString().setNum(l2.angle()));
                Q_ASSERT(arc1);
                arc1->setMode(Draw::Modeling);
                ++idCount;
                data->UpdateGObject(idDetail+idCount, arc1);

                ++idCount;
            }
        }
        break;
        case (Tool::NodeSpline):
        {
            if (qFuzzyCompare(dx+1, 1) == false && qFuzzyCompare(dy+1, 1) == false && pRotate != 0)
            {
                const VSpline *spline = data->GeometricObject<const VSpline *>(det.at(i).getId());

                VPointF *p1 = new VPointF(spline->GetP1());
                Q_ASSERT(p1 != 0);
                BiasRotatePoint(p1, dx, dy, data->GeometricObject<const VPointF *>(pRotate)->toQPointF(), angle);
                ++idCount;
                data->UpdateGObject(idDetail+idCount, p1);

                VPointF p2 = VPointF(spline->GetP2());
                BiasRotatePoint(&p2, dx, dy, data->GeometricObject<const VPointF *>(pRotate)->toQPointF(), angle);

                VPointF p3 = VPointF(spline->GetP3());
                BiasRotatePoint(&p3, dx, dy, data->GeometricObject<const VPointF *>(pRotate)->toQPointF(), angle);

                VPointF *p4 = new VPointF(spline->GetP4());
                Q_ASSERT(p4 != 0);
                BiasRotatePoint(p4, dx, dy, data->GeometricObject<const VPointF *>(pRotate)->toQPointF(), angle);
                ++idCount;
                data->UpdateGObject(idDetail+idCount, p4);

                VSpline *spl = new VSpline(*p1, p2.toQPointF(), p3.toQPointF(), *p4, spline->GetKcurve());
                Q_ASSERT(spl != 0);
                spl->setMode(Draw::Modeling);
                ++idCount;
                data->UpdateGObject(idDetail+idCount, spl);

                ++idCount;
            }
        }
        break;
        case (Tool::NodeSplinePath):
        {
            if (qFuzzyCompare(dx+1, 1) == false && qFuzzyCompare(dy+1, 1) == false && pRotate != 0)
            {
                VSplinePath *path = new VSplinePath();
                Q_ASSERT(path != 0);
                path->setMode(Draw::Modeling);
                const VSplinePath *splinePath = data->GeometricObject<const VSplinePath *>(det.at(i).getId());
                Q_ASSERT(splinePath != 0);
                qDebug()<<splinePath->Count();
                qDebug()<<det.at(i).getId();
                qDebug()<<"Початок циклу.";
                for (qint32 i = 1; i <= splinePath->Count(); ++i)
                {
                    VSpline spline(splinePath->at(i-1).P(), splinePath->at(i).P(),
                            splinePath->at(i-1).Angle2(), splinePath->at(i).Angle1(), splinePath->at(i-1).KAsm2(),
                            splinePath->at(i).KAsm1(), splinePath->getKCurve());

                    VPointF *p1 = new VPointF(spline.GetP1());
                    Q_ASSERT(p1 != 0);
                    BiasRotatePoint(p1, dx, dy, data->GeometricObject<const VPointF *>(pRotate)->toQPointF(),
                                    angle);
                    ++idCount;
                    data->UpdateGObject(idDetail+idCount, p1);
                    qDebug()<<idDetail+idCount;

                    VPointF p2 = VPointF(spline.GetP2());
                    BiasRotatePoint(&p2, dx, dy, data->GeometricObject<const VPointF *>(pRotate)->toQPointF(),
                                    angle);

                    VPointF p3 = VPointF(spline.GetP3());
                    BiasRotatePoint(&p3, dx, dy, data->GeometricObject<const VPointF *>(pRotate)->toQPointF(),
                                    angle);

                    VPointF *p4 = new VPointF(spline.GetP4());
                    Q_ASSERT(p4 != 0);
                    BiasRotatePoint(p4, dx, dy, data->GeometricObject<const VPointF *>(pRotate)->toQPointF(),
                                    angle);
                    ++idCount;
                    data->UpdateGObject(idDetail+idCount, p4);
                    qDebug()<<idDetail+idCount;

                    VSpline spl = VSpline(*p1, p2.toQPointF(), p3.toQPointF(), *p4, spline.GetKcurve());
                    if (i==1)
                    {
                        path->append(VSplinePoint(*p1, splinePath->at(i-1).KAsm1(), spl.GetAngle1(),
                                                  splinePath->at(i-1).KAsm1()));
                    }
                        path->append(VSplinePoint(*p4, splinePath->at(i).KAsm1(), spl.GetAngle1(),
                                                  splinePath->at(i).KAsm1()));
                }

                ++idCount;
                data->UpdateGObject(idDetail+idCount, path);
                qDebug()<<idDetail+idCount;

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

void VToolUnionDetails::Create(QSharedPointer<DialogUnionDetails> &dialog, VMainGraphicsScene *scene, VDomDocument *doc,
                               VContainer *data)
{
    VDetail d1 = data->GetDetail(dialog->getD1());
    VDetail d2 = data->GetDetail(dialog->getD2());
    ptrdiff_t indexD1 = dialog->getIndexD1();
    ptrdiff_t indexD2 = dialog->getIndexD2();
    Create(0, d1, d2, dialog->getD1(), dialog->getD2(), indexD1, indexD2, scene, doc, data, Document::FullParse,
           Tool::FromGui);
}

void VToolUnionDetails::Create(const qint64 _id, const VDetail &d1, const VDetail &d2, const qint64 &d1id,
                               const qint64 &d2id, const ptrdiff_t &indexD1, const ptrdiff_t &indexD2,
                               VMainGraphicsScene *scene, VDomDocument *doc, VContainer *data,
                               const Document::Documents &parse, const Tool::Sources &typeCreation)
{
    VToolUnionDetails *unionDetails = 0;
    qint64 id = _id;
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
        QHash<qint64, VDataTool*>* tools = doc->getTools();
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

    if (typeCreation == Tool::FromGui)
    {
        VDetail uD1 = d1.RemoveEdge(indexD1);
        VDetail uD2 = d2.RemoveEdge(indexD2);
        qint32 j = 0, i = 0;
        qint32 nD1 = uD1.CountNode();
        qint32 nD2 = uD2.CountNode();
        qint32 pointsD2 = 0; //Keeps count points second detail, what we already add.
        VDetail newDetail;

        VNodeDetail det1p1;
        VNodeDetail det1p2;
        d1.NodeOnEdge(indexD1, det1p1, det1p2);
        const VPointF *point1 = data->GeometricObject<const VPointF *>(det1p1.getId());
        const VPointF *point2 = data->GeometricObject<const VPointF *>(det1p2.getId());

        VNodeDetail det2p1;
        VNodeDetail det2p2;
        d2.NodeOnEdge(indexD2, det2p1, det2p2);
        VPointF point3 = VPointF(*data->GeometricObject<const VPointF *>(det2p1.getId()));
        VPointF point4 = VPointF(*data->GeometricObject<const VPointF *>(det2p2.getId()));

        qreal dx = point1->x() - point4.x();
        qreal dy = point1->y() - point4.y();

        point3.setX(point3.x()+dx);
        point3.setY(point3.y()+dy);

        point4.setX(point4.x()+dx);
        point4.setY(point4.y()+dy);

        QLineF l1(point1->toQPointF(), point2->toQPointF());
        QLineF l2(point4.toQPointF(), point3.toQPointF());
        qreal angle = l2.angleTo(l1);

        ptrdiff_t iD1 = d1.indexOfNode(det1p1.getId());

        do
        {
            AddToNewDetail(unionDetails, doc, data, newDetail, uD1, i, id);
            ++i;
            if (i > iD1 && pointsD2 < nD2-2)
            {
                do
                {
                    if (pointsD2 == 0)
                    {
                        VNodeDetail node1;
                        VNodeDetail node2;
                        d2.NodeOnEdge(indexD2, node1, node2);
                        ptrdiff_t k = uD2.indexOfNode(node2.getId());
                        if (k == uD2.CountNode()-1)
                        {
                            j = 0;
                        }
                        else
                        {
                            j = uD2.indexOfNode(node2.getId())+1;
                        }
                    }
                    if (pointsD2 == nD2 -2)
                    {
                        break;
                    }
                    if (j >= nD2)
                    {
                        j=0;
                    }
                    AddToNewDetail(unionDetails, doc, data, newDetail, uD2, j, id, dx, dy, det1p1.getId(), angle);
                    ++pointsD2;
                    ++j;
                } while (pointsD2 < nD2);
            }
        }while(i<nD1);

        newDetail.setName("Detail");
        VToolDetail::Create(0, newDetail, scene, doc, data, parse, Tool::FromTool);
        QHash<qint64, VDataTool*>* tools = doc->getTools();

        VToolDetail *toolDet = qobject_cast<VToolDetail*>(tools->value(d1id));
        toolDet->Remove();

        toolDet = qobject_cast<VToolDetail*>(tools->value(d2id));
        toolDet->Remove();
    }
    else
    {
        VDetail uD1 = d1.RemoveEdge(indexD1);
        VDetail uD2 = d2.RemoveEdge(indexD2);
        qint64 idCount = 0;
        qint32 j = 0, i = 0;
        qint32 nD1 = uD1.CountNode();
        qint32 nD2 = uD2.CountNode();
        qint32 pointsD2 = 0; //Keeps count points second detail, what we already add.

        VNodeDetail det1p1;
        VNodeDetail det1p2;
        d1.NodeOnEdge(indexD1, det1p1, det1p2);
        const VPointF *point1 = data->GeometricObject<const VPointF *>(det1p1.getId());
        const VPointF *point2 = data->GeometricObject<const VPointF *>(det1p2.getId());

        VNodeDetail det2p1;
        VNodeDetail det2p2;
        d2.NodeOnEdge(indexD2, det2p1, det2p2);
        VPointF point3 = VPointF(*data->GeometricObject<const VPointF *>(det2p1.getId()));
        VPointF point4 = VPointF(*data->GeometricObject<const VPointF *>(det2p2.getId()));

        qreal dx = point1->x() - point4.x();
        qreal dy = point1->y() - point4.y();

        point3.setX(point3.x()+dx);
        point3.setY(point3.y()+dy);

        point4.setX(point4.x()+dx);
        point4.setY(point4.y()+dy);

        QLineF l1(point1->toQPointF(), point2->toQPointF());
        QLineF l2(point4.toQPointF(), point3.toQPointF());
        qreal angle = l2.angleTo(l1);

        ptrdiff_t iD1 = d1.indexOfNode(det1p1.getId());

        do
        {
            UpdatePoints(id, data, uD1, i, idCount);
            ++i;
            if (i > iD1 && pointsD2 < nD2-2)
            {
                do
                {
                    if (pointsD2 == 0)
                    {
                        VNodeDetail node1;
                        VNodeDetail node2;
                        d2.NodeOnEdge(indexD2, node1, node2);
                        ptrdiff_t k = uD2.indexOfNode(node2.getId());
                        if (k == uD2.CountNode()-1)
                        {
                            j = 0;
                        }
                        else
                        {
                            j = uD2.indexOfNode(node2.getId())+1;
                        }
                    }
                    if (pointsD2 == nD2-2)
                    {
                        break;
                    }
                    if (j >= nD2)
                    {
                        j=0;
                    }
                    UpdatePoints(id, data, uD2, j, idCount, dx, dy, det1p1.getId(), angle);
                    ++pointsD2;
                    ++j;
                } while (pointsD2 < nD2);
            }
        }while(i<nD1);
    }
}

QVector<VDetail> VToolUnionDetails::GetDetailFromFile(VDomDocument *doc, const QDomElement &domElement)
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
                            qint64 id = doc->GetParametrLongLong(element, VToolDetail::AttrIdObject, "0");
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

    AddAttribute(domElement, AttrId, id);
    AddAttribute(domElement, AttrType, ToolType);
    AddAttribute(domElement, AttrIndexD1, indexD1);
    AddAttribute(domElement, AttrIndexD2, indexD2);

    AddDetail(domElement, d1);
    AddDetail(domElement, d2);

    AddToModeling(domElement);
}

void VToolUnionDetails::RefreshDataInFile()
{
    QDomElement domElement = doc->elementById(QString().setNum(id));
    if (domElement.isElement())
    {
        domElement.setAttribute(AttrIndexD1, indexD1);
        domElement.setAttribute(AttrIndexD2, indexD2);

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

    AddAttribute(nod, AttrIdObject, node.getId());
    AddAttribute(nod, AttrMx, toMM(node.getMx()));
    AddAttribute(nod, AttrMy, toMM(node.getMy()));
    if (node.getTypeNode() == NodeDetail::Contour)
    {
        AddAttribute(nod, AttrNodeType, NodeTypeContour);
    }
    else
    {
        AddAttribute(nod, AttrNodeType, NodeTypeModeling);
    }
    switch (node.getTypeTool())
    {
        case (Tool::NodeArc):
            AddAttribute(nod, AttrType, QStringLiteral("NodeArc"));
            break;
        case (Tool::NodePoint):
            AddAttribute(nod, AttrType, QStringLiteral("NodePoint"));
            break;
        case (Tool::NodeSpline):
            AddAttribute(nod, AttrType, QStringLiteral("NodeSpline"));
            break;
        case (Tool::NodeSplinePath):
            AddAttribute(nod, AttrType, QStringLiteral("NodeSplinePath"));
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
    bool ok = doc->GetActivModelingElement(modelingElement);
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
