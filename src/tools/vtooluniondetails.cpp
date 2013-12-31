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
const QString VToolUnionDetails::AttrD1P1         = QStringLiteral("d1P1");
const QString VToolUnionDetails::AttrD1P2         = QStringLiteral("d1P2");
const QString VToolUnionDetails::AttrD2P1         = QStringLiteral("d2P1");
const QString VToolUnionDetails::AttrD2P2         = QStringLiteral("d2P2");
const QString VToolUnionDetails::AttrIdObject     = QStringLiteral("idObject");
const QString VToolUnionDetails::AttrNodeType     = QStringLiteral("nodeType");
const QString VToolUnionDetails::NodeTypeContour  = QStringLiteral("Contour");
const QString VToolUnionDetails::NodeTypeModeling = QStringLiteral("Modeling");

VToolUnionDetails::VToolUnionDetails(VDomDocument *doc, VContainer *data, const qint64 &id, const VDetail &d1,
                                     const VDetail &d2, const qint64 &d1P1, const qint64 &d1P2, const qint64 &d2P1,
                                     const qint64 &d2P2, const Tool::Sources &typeCreation)
    :VAbstractTool(doc, data, id), d1(d1), d2(d2), d1P1(d1P1), d1P2(d1P2), d2P1(d2P1), d2P2(d2P2)
{
    CorectPoints(this->d1, this->d1P1, this->d1P2);
    CorectPoints(this->d2, this->d2P1, this->d2P2);

    if (typeCreation == Tool::FromGui)
    {
        AddToFile();
    }
    else
    {
        RefreshDataInFile();
    }
}

void VToolUnionDetails::CorectPoints(const VDetail &detail, qint64 &p1, qint64 &p2)
{
    if (p1 == p2)
    {
        qWarning()<<"Union points equal.";
        return;
    }
    bool p1flag = false;
    bool p2flag = false;
    for (ptrdiff_t i = 0; i< detail.CountNode(); ++i)
    {
        if (detail.at(i).getTypeTool() == Tool::NodePoint)
        {
            if (p1 == detail.at(i).getId())
            {
                p1flag = true;
            }
            if (p2 == detail.at(i).getId())
            {
                p2flag = true;
            }
            if (p1flag == true && p2flag == false)
            {
                return;
            }
            if (p2flag == true && p1flag == false)
            {
                qint64 p = p1;
                p1 = p2;
                p2 = p;
                return;
            }
        }
    }
}

void VToolUnionDetails::AddToNewDetail(VContainer *data, VDetail newDetail, const VDetail &det, const ptrdiff_t &a,
                                       const ptrdiff_t &b, const qreal &dx, const qreal &dy, const qint64 &pRotate,
                                       const qreal &angle) const
{
    for (ptrdiff_t i = a; i <= b; ++i)
    {
        qint64 id = 0;
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
                    BiasRotatePoint(point, dx, dy, data->GeometricObject<const VPointF *>(pRotate)->toQPointF(),
                                    angle);
                    id = data->AddGObject(point);
                    VNodePoint::Create(doc, data, id, det.at(i).getId(), Document::FullParse, Tool::FromGui );
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
                    id = data->AddGObject(arc1);
                    VNodeArc::Create(doc, data, id, det.at(i).getId(), Document::FullParse, Tool::FromGui);
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
                    id = data->AddGObject(spl);
                    VNodeSpline::Create(doc, data, id, det.at(i).getId(), Document::FullParse, Tool::FromGui);
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
                    id = data->AddGObject(path);
                    VNodeSplinePath::Create(doc, data, id, det.at(i).getId(), Document::FullParse, Tool::FromGui);
                }
            }
            break;
            default:
                qWarning()<<"May be wrong tool type!!! Ignoring."<<Q_FUNC_INFO;
                break;
        }
        newDetail.append(VNodeDetail(id, det.at(i).getTypeTool(), NodeDetail::Contour));
    }
}

void VToolUnionDetails::UpdatePoints(const qint64 &idDetail, VContainer *data, const VDetail &det, const ptrdiff_t &a,
                                     const ptrdiff_t &b, const qreal &dx, const qreal &dy, const qint64 &pRotate,
                                     const qreal &angle) const
{
    qint64 idCount = 0;
    for (ptrdiff_t i = a; i <= b; ++i)
    {
        switch (det.at(i).getTypeTool())
        {
            case (Tool::NodePoint):
            {
                if (qFuzzyCompare(dx+1, 1) && qFuzzyCompare(dy+1, 1) && pRotate == 0)
                {
                    ++idCount;
                }
                else
                {
                    VPointF *point = new VPointF(*data->GeometricObject<const VPointF *>(det.at(i).getId()));
                    Q_ASSERT(point != 0);
                    BiasRotatePoint(point, dx, dy, data->GeometricObject<const VPointF *>(pRotate)->toQPointF(), angle);
                    ++idCount;
                    data->UpdateGObject(idDetail+idCount, point);
                }
            }
            break;
            case (Tool::NodeArc):
            {
                if (qFuzzyCompare(dx+1, 1) && qFuzzyCompare(dy+1, 1) && pRotate == 0)
                {
                    ++idCount;
                }
                else
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
                    qint64 idCenter = data->AddGObject(center);
                    VArc *arc1 = new VArc(*center, arc->GetRadius(), arc->GetFormulaRadius(), l1.angle(),
                                         QString().setNum(l1.angle()), l2.angle(), QString().setNum(l2.angle()));
                    Q_ASSERT(arc1);
                    ++idCount;
                    data->UpdateGObject(idDetail+idCount, arc1);
                }
            }
            break;
            case (Tool::NodeSpline):
            {
                if (qFuzzyCompare(dx+1, 1) && qFuzzyCompare(dy+1, 1) && pRotate == 0)
                {
                    ++idCount;
                }
                else
                {
                    const VSpline *spline = data->GeometricObject<const VSpline *>(det.at(i).getId());

                    VPointF *p1 = new VPointF(spline->GetP1());
                    Q_ASSERT(p1 != 0);
                    BiasRotatePoint(p1, dx, dy, data->GeometricObject<const VPointF *>(pRotate)->toQPointF(), angle);
                    qint64 idP1 = data->AddGObject(p1);

                    VPointF p2 = VPointF(spline->GetP2());
                    BiasRotatePoint(&p2, dx, dy, data->GeometricObject<const VPointF *>(pRotate)->toQPointF(), angle);

                    VPointF p3 = VPointF(spline->GetP3());
                    BiasRotatePoint(&p3, dx, dy, data->GeometricObject<const VPointF *>(pRotate)->toQPointF(), angle);

                    VPointF *p4 = new VPointF(spline->GetP4());
                    Q_ASSERT(p4 != 0);
                    BiasRotatePoint(p4, dx, dy, data->GeometricObject<const VPointF *>(pRotate)->toQPointF(), angle);
                    qint64 idP4 = data->AddGObject(p4);

                    VSpline *spl = new VSpline(*p1, p2.toQPointF(), p3.toQPointF(), *p4, spline->GetKcurve());
                    Q_ASSERT(spl != 0);
                    ++idCount;
                    data->UpdateGObject(idDetail+idCount, spl);
                }
            }
            break;
            case (Tool::NodeSplinePath):
            {
                if (qFuzzyCompare(dx+1, 1) && qFuzzyCompare(dy+1, 1) && pRotate == 0)
                {
                    ++idCount;
                }
                else
                {
                    VSplinePath *path = new VSplinePath();
                    Q_ASSERT(path != 0);
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
                    ++idCount;
                    data->UpdateGObject(idDetail+idCount, path);
                }
            }
            break;
            default:
                qWarning()<<"May be wrong tool type!!! Ignoring."<<Q_FUNC_INFO;
                break;
        }
    }
}

void VToolUnionDetails::BiasRotatePoint(VPointF *point, const qreal &dx, const qreal &dy, const QPointF &pRotate,
                                        const qreal angle) const
{
    point->setX(point->x()+dx);
    point->setY(point->y()+dy);
    QLineF line(pRotate, point->toQPointF());
    line.setAngle(angle);
    point->setX(line.p2().x());
    point->setY(line.p2().y());
}

void VToolUnionDetails::Create(QSharedPointer<DialogUnionDetails> &dialog, VMainGraphicsScene *scene, VDomDocument *doc,
                               VContainer *data)
{
    VDetail d1 = data->GetDetail(dialog->getD1());
    VDetail d2 = data->GetDetail(dialog->getD2());
    qint64 d1P1 = dialog->getD1P1();
    qint64 d1P2 = dialog->getD1P2();
    qint64 d2P1 = dialog->getD2P1();
    qint64 d2P2 = dialog->getD2P2();
    Create(0, d1, d2, dialog->getD1(), dialog->getD2(), d1P1, d1P2, d2P1, d2P2, scene, doc, data, Document::FullParse,
           Tool::FromGui);
}

void VToolUnionDetails::Create(const qint64 _id, const VDetail &d1, const VDetail &d2, const qint64 &d1id,
                               const qint64 &d2id, const qint64 &d1P1, const qint64 &d1P2, const qint64 &d2P1,
                               const qint64 &d2P2, VMainGraphicsScene *scene, VDomDocument *doc, VContainer *data,
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
    if (parse == Document::FullParse)
    {
        unionDetails = new VToolUnionDetails(doc, data, id, d1, d2, d1P1, d1P2, d2P1, d2P2, typeCreation);
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
        //Update corected points
        qint64 d1P1 = unionDetails->getD1P1();
        qint64 d1P2 = unionDetails->getD1P2();
        qint64 d2P1 = unionDetails->getD2P1();
        qint64 d2P2 = unionDetails->getD2P2();

        VDetail newDetail;
        unionDetails->AddToNewDetail(data, newDetail, d1, d1.indexOfNode(d1P1), d1.indexOfNode(d1P2));

        const VNodeDetail det1p2 = d1.at(d1.indexOfNode(d1P2));
        const VPointF *point2 = data->GeometricObject<const VPointF *>(det1p2.getId());
        const VNodeDetail det2p1 = d2.at(d2.indexOfNode(d2P1));
        const VPointF *point1 = data->GeometricObject<const VPointF *>(det2p1.getId());
        qreal dx = point2->x() - point1->x();
        qreal dy = point2->y() - point1->y();
        const VNodeDetail det1p1 = d1.at(d1.indexOfNode(d1P1));
        const VPointF *point3 = data->GeometricObject<const VPointF *>(det1p1.getId());
        const VNodeDetail det2p2 = d1.at(d1.indexOfNode(d2P2));
        const VPointF *point4 = data->GeometricObject<const VPointF *>(det2p2.getId());
        QLineF l1(point3->toQPointF(), point2->toQPointF());
        QLineF l2(point4->toQPointF(), point1->toQPointF());
        qreal angle = l2.angle() + l2.angleTo(l1);

        unionDetails->AddToNewDetail(data, newDetail, d2, d2.indexOfNode(d2P1)+1, d2.indexOfNode(d2P2)-1, dx, dy,
                                     d1P2, angle);

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
        unionDetails = qobject_cast<VToolUnionDetails *>(doc->getTool(id));

        unionDetails->UpdatePoints(id, data, d1, d1.indexOfNode(d1P1), d1.indexOfNode(d1P2));

        VNodeDetail det1p2 = d1.at(d1.indexOfNode(d1P2));
        const VPointF *point2 = data->GeometricObject<const VPointF *>(det1p2.getId());
        VNodeDetail det2p1 = d2.at(d2.indexOfNode(d2P1));
        const VPointF *point1 = data->GeometricObject<const VPointF *>(det2p1.getId());
        qreal dx = point2->x() - point1->x();
        qreal dy = point2->y() - point1->y();
        const VNodeDetail det1p1 = d1.at(d1.indexOfNode(d1P1));
        const VPointF *point3 = data->GeometricObject<const VPointF *>(det1p1.getId());
        const VNodeDetail det2p2 = d1.at(d1.indexOfNode(d2P2));
        const VPointF *point4 = data->GeometricObject<const VPointF *>(det2p2.getId());
        QLineF l1(point3->toQPointF(), point2->toQPointF());
        QLineF l2(point4->toQPointF(), point1->toQPointF());
        qreal angle = l2.angle() + l2.angleTo(l1);

        unionDetails->UpdatePoints(id, data, d2, d2.indexOfNode(d2P1)+1, d2.indexOfNode(d2P2)-1, dx, dy,
                                   d1P2, angle);
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
                QDomNodeList nodeList = domElement.childNodes();
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
                            vector[i].append(VNodeDetail(id, tool, nodeType, mx, my));
                        }
                    }
                }
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
    AddAttribute(domElement, AttrD1P1, d1P1);
    AddAttribute(domElement, AttrD1P2, d1P2);
    AddAttribute(domElement, AttrD2P1, d2P1);
    AddAttribute(domElement, AttrD2P2, d2P2);

    AddDetail(domElement, d1);
    AddDetail(domElement, d2);

    AddToModeling(domElement);
}

void VToolUnionDetails::RefreshDataInFile()
{
    QDomElement domElement = doc->elementById(QString().setNum(id));
    if (domElement.isElement())
    {
        domElement.setAttribute(AttrD1P1, d1P1);
        domElement.setAttribute(AttrD1P2, d1P2);
        domElement.setAttribute(AttrD2P1, d2P1);
        domElement.setAttribute(AttrD2P2, d2P2);

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

void VToolUnionDetails::AddNode(QDomElement &domElement, VNodeDetail &node)
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

QDomNode VToolUnionDetails::UpdateDetail(QDomNode &domNode, VDetail &d)
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
                    for (ptrdiff_t i = 0; i < d.CountNode(); ++i)
                    {
                        UpdateNode(domElement, d[i]);
                    }
                }
            }
        }
    }
    return domNode.nextSibling();
}

void VToolUnionDetails::UpdateNode(QDomNode &domNode, VNodeDetail &node)
{
    while (domNode.isNull() == false)
    {
        if (domNode.isElement())
        {
            QDomElement domElement = domNode.toElement();
            if (domElement.isNull() == false)
            {
                if (domElement.tagName() == VToolUnionDetails::TagNode)
                {
                    domElement.setAttribute( AttrIdObject, node.getId());
                    domElement.setAttribute(AttrMx, toMM(node.getMx()));
                    domElement.setAttribute(AttrMy, toMM(node.getMy()));
                    if (node.getTypeNode() == NodeDetail::Contour)
                    {
                        domElement.setAttribute(AttrNodeType, NodeTypeContour);
                    }
                    else
                    {
                        domElement.setAttribute(AttrNodeType, NodeTypeModeling);
                    }
                    switch (node.getTypeTool())
                    {
                        case (Tool::NodeArc):
                            domElement.setAttribute(AttrType, QStringLiteral("NodeArc"));
                            break;
                        case (Tool::NodePoint):
                            domElement.setAttribute(AttrType, QStringLiteral("NodePoint"));
                            break;
                        case (Tool::NodeSpline):
                            domElement.setAttribute(AttrType, QStringLiteral("NodeSpline"));
                            break;
                        case (Tool::NodeSplinePath):
                            domElement.setAttribute(AttrType, QStringLiteral("NodeSplinePath"));
                            break;
                        default:
                            qWarning()<<"May be wrong tool type!!! Ignoring."<<Q_FUNC_INFO;
                            break;
                    }
                }
            }
        }
        domNode = domNode.nextSibling();
    }
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
