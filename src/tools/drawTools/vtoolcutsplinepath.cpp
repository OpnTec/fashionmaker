/************************************************************************
 **
 **  @file   vtoolcutsplinepath.cpp
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

#include "vtoolcutsplinepath.h"

const QString VToolCutSplinePath::ToolType = QStringLiteral("cutSplinePath");
const QString VToolCutSplinePath::AttrSplinePath = QStringLiteral("splinePath");

VToolCutSplinePath::VToolCutSplinePath(VDomDocument *doc, VContainer *data, const qint64 &id,
                                       const QString &formula, const qint64 &splinePathId,
                                       const Tool::Sources &typeCreation, QGraphicsItem *parent)
    :VToolPoint(doc, data, id, parent), formula(formula), splinePathId(splinePathId),
      dialogCutSplinePath(QSharedPointer<DialogCutSplinePath>()), firstSplinePath(), secondSplinePath()
{
    Q_ASSERT_X(splinePathId > 0, Q_FUNC_INFO, "splinePathId <= 0");
    if (typeCreation == Tool::FromGui)
    {
        AddToFile();
    }
}

void VToolCutSplinePath::setDialog()
{
    Q_ASSERT(dialogCutSplinePath.isNull() == false);
    VPointF point = VAbstractTool::data.GetPoint(id);
    dialogCutSplinePath->setFormula(formula);
    dialogCutSplinePath->setSplinePathId(splinePathId, id);
    dialogCutSplinePath->setPointName(point.name());
}

void VToolCutSplinePath::Create(QSharedPointer<DialogCutSplinePath> &dialog, VMainGraphicsScene *scene,
                            VDomDocument *doc, VContainer *data)
{
    QString pointName = dialog->getPointName();
    QString formula = dialog->getFormula();
    qint64 splinePathId = dialog->getSplinePathId();
    Create(0, pointName, formula, splinePathId, 5, 10, scene, doc, data, Document::FullParse, Tool::FromGui);
}

void VToolCutSplinePath::Create(const qint64 _id, const QString &pointName, const QString &formula,
                            const qint64 &splinePathId, const qreal &mx, const qreal &my,
                            VMainGraphicsScene *scene, VDomDocument *doc, VContainer *data,
                            const Document::Documents &parse, const Tool::Sources &typeCreation)
{

}

void VToolCutSplinePath::FullUpdateFromFile()
{
    QDomElement domElement = doc->elementById(QString().setNum(id));
    if (domElement.isElement())
    {
        formula = domElement.attribute(AttrLength, "");
        splinePathId = domElement.attribute(AttrSplinePath, "").toLongLong();
    }
    RefreshGeometry();
}

void VToolCutSplinePath::FullUpdateFromGui(int result)
{
    if (result == QDialog::Accepted)
    {
        QDomElement domElement = doc->elementById(QString().setNum(id));
        if (domElement.isElement())
        {
            domElement.setAttribute(AttrName, dialogCutSplinePath->getPointName());
            domElement.setAttribute(AttrLength, dialogCutSplinePath->getFormula());
            domElement.setAttribute(AttrSplinePath, QString().setNum(dialogCutSplinePath->getSplinePathId()));
            emit FullUpdateTree();
        }
    }
    dialogCutSplinePath.clear();
}

void VToolCutSplinePath::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    ContextMenu(dialogCutSplinePath, this, event);
}

void VToolCutSplinePath::AddToFile()
{
    VPointF point = VAbstractTool::data.GetPoint(id);
    QDomElement domElement = doc->createElement(TagName);

    AddAttribute(domElement, AttrId, id);
    AddAttribute(domElement, AttrName, point.name());
    AddAttribute(domElement, AttrMx, toMM(point.mx()));
    AddAttribute(domElement, AttrMy, toMM(point.my()));

    AddAttribute(domElement, AttrLength, formula);
    AddAttribute(domElement, AttrSplinePath, splinePathId);

    AddToCalculation(domElement);
}

void VToolCutSplinePath::RefreshGeometry()
{
    VToolPoint::RefreshPointGeometry(VDrawTool::data.GetPoint(id));
}
