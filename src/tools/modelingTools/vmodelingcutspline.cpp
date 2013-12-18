/************************************************************************
 **
 **  @file   vmodelingcutspline.cpp
 **  @author Roman Telezhinsky <dismine@gmail.com>
 **  @date   15 12, 2013
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

#include "vmodelingcutspline.h"

const QString VModelingCutSpline::ToolType = QStringLiteral("cutSpline");
const QString VModelingCutSpline::AttrSpline = QStringLiteral("spline");

VModelingCutSpline::VModelingCutSpline(VDomDocument *doc, VContainer *data, const qint64 &id,
                                       const QString &formula, const qint64 &splineId,
                                       const Tool::Sources &typeCreation, QGraphicsItem *parent)
    :VModelingPoint(doc, data, id, parent), formula(formula), splineId(splineId),
      dialogCutSpline(QSharedPointer<DialogCutSpline>())
{
    Q_ASSERT_X(splineId > 0, Q_FUNC_INFO, "splineId <= 0");
    if (typeCreation == Tool::FromGui)
    {
        AddToFile();
    }
}

void VModelingCutSpline::setDialog()
{
    Q_ASSERT(dialogCutSpline.isNull() == false);
    VSpline spl = VAbstractTool::data.GetSplineModeling(id);
    dialogCutSpline->setFormula(formula);
    dialogCutSpline->setSplineId(splineId, id);
    dialogCutSpline->setPointName(spl.name());
}

VModelingCutSpline *VModelingCutSpline::Create(QSharedPointer<DialogCutSpline> &dialog,
                                               VDomDocument *doc, VContainer *data)
{
    QString pointName = dialog->getPointName();
    QString formula = dialog->getFormula();
    qint64 splineId = dialog->getSplineId();
    return Create(0, pointName, formula, splineId, 5, 10, doc, data, Document::FullParse, Tool::FromGui);
}

VModelingCutSpline *VModelingCutSpline::Create(const qint64 _id, const QString &pointName, const QString &formula,
                                               const qint64 &splineId, const qreal &mx, const qreal &my,
                                               VDomDocument *doc, VContainer *data, const Document::Documents &parse,
                                               const Tool::Sources &typeCreation)
{

}

void VModelingCutSpline::FullUpdateFromFile()
{
    QDomElement domElement = doc->elementById(QString().setNum(id));
    if (domElement.isElement())
    {
        formula = domElement.attribute(AttrLength, "");
        splineId = domElement.attribute(AttrSpline, "").toLongLong();
    }
    RefreshGeometry();
}

void VModelingCutSpline::FullUpdateFromGui(int result)
{
    if (result == QDialog::Accepted)
    {
        QDomElement domElement = doc->elementById(QString().setNum(id));
        if (domElement.isElement())
        {
            domElement.setAttribute(AttrName, dialogCutSpline->getPointName());
            domElement.setAttribute(AttrLength, dialogCutSpline->getFormula());
            domElement.setAttribute(AttrSpline, QString().setNum(dialogCutSpline->getSplineId()));
            emit FullUpdateTree();
        }
    }
    dialogCutSpline.clear();
}

void VModelingCutSpline::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    ContextMenu(dialogCutSpline, this, event);
}

void VModelingCutSpline::AddToFile()
{
    VPointF point = VAbstractTool::data.GetPointModeling(id);
    QDomElement domElement = doc->createElement(TagName);

    AddAttribute(domElement, AttrId, id);
    AddAttribute(domElement, AttrName, point.name());
    AddAttribute(domElement, AttrMx, toMM(point.mx()));
    AddAttribute(domElement, AttrMy, toMM(point.my()));

    AddAttribute(domElement, AttrLength, formula);
    AddAttribute(domElement, AttrSpline, splineId);

    AddToModeling(domElement);
}

void VModelingCutSpline::RefreshGeometry()
{
    VModelingPoint::RefreshPointGeometry(VModelingTool::data.GetPointModeling(id));
}
