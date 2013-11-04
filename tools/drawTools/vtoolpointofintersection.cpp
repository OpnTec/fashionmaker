#include "vtoolpointofintersection.h"

const QString VToolPointOfIntersection::ToolType = QStringLiteral("pointOfIntersection");

VToolPointOfIntersection::VToolPointOfIntersection(VDomDocument *doc, VContainer *data, const qint64 &id,
                                                   const qint64 &firstPointId, const qint64 &secondPointId,
                                                   Tool::Sources typeCreation, QGraphicsItem *parent)
    :VToolPoint(doc, data, id, parent), firstPointId(firstPointId), secondPointId(secondPointId),
      dialogPointOfIntersection(QSharedPointer<DialogPointOfIntersection>())
{
    ignoreFullUpdate = true;
    if (typeCreation == Tool::FromGui)
    {
        AddToFile();
    }
}

void VToolPointOfIntersection::setDialog()
{
    Q_ASSERT(dialogPointOfIntersection.isNull() == false);
    VPointF p = VAbstractTool::data.GetPoint(id);
    dialogPointOfIntersection->setFirstPointId(firstPointId, id);
    dialogPointOfIntersection->setSecondPointId(secondPointId, id);
    dialogPointOfIntersection->setPointName(p.name());
}

void VToolPointOfIntersection::Create(QSharedPointer<DialogPointOfIntersection> &dialog, VMainGraphicsScene *scene,
                                      VDomDocument *doc, VContainer *data)
{
    qint64 firstPointId = dialog->getFirstPointId();
    qint64 secondPointId = dialog->getSecondPointId();
    QString pointName = dialog->getPointName();
    Create(0, pointName, firstPointId, secondPointId, 5, 10, scene, doc, data, Document::FullParse, Tool::FromGui);
}

void VToolPointOfIntersection::Create(const qint64 _id, const QString &pointName, const qint64 &firstPointId,
                                      const qint64 &secondPointId, const qreal &mx, const qreal &my,
                                      VMainGraphicsScene *scene, VDomDocument *doc, VContainer *data,
                                      const Document::Documents &parse, Tool::Sources typeCreation)
{
    VPointF firstPoint = data->GetPoint(firstPointId);
    VPointF secondPoint = data->GetPoint(secondPointId);

    QPointF point(firstPoint.x(), secondPoint.y());
    qint64 id = _id;
    if (typeCreation == Tool::FromGui)
    {
        id = data->AddPoint(VPointF(point.x(), point.y(), pointName, mx, my));
    }
    else
    {
        data->UpdatePoint(id, VPointF(point.x(), point.y(), pointName, mx, my));
        if (parse != Document::FullParse)
        {
            doc->UpdateToolData(id, data);
        }
    }
    VDrawTool::AddRecord(id, Tool::Triangle, doc);
    if (parse == Document::FullParse)
    {
        VToolPointOfIntersection *point = new VToolPointOfIntersection(doc, data, id, firstPointId,
                                                                       secondPointId, typeCreation);
        scene->addItem(point);
        connect(point, &VToolPointOfIntersection::ChoosedTool, scene, &VMainGraphicsScene::ChoosedItem);
        connect(point, &VToolPointOfIntersection::RemoveTool, scene, &VMainGraphicsScene::RemoveTool);
        connect(scene, &VMainGraphicsScene::NewFactor, point, &VToolPointOfIntersection::SetFactor);
        doc->AddTool(id, point);
        doc->IncrementReferens(firstPointId);
        doc->IncrementReferens(secondPointId);
    }
}

void VToolPointOfIntersection::FullUpdateFromFile()
{
    QDomElement domElement = doc->elementById(QString().setNum(id));
    if (domElement.isElement())
    {
        firstPointId = domElement.attribute(AttrFirstPoint, "").toLongLong();
        secondPointId = domElement.attribute(AttrSecondPoint, "").toLongLong();
    }
    VToolPoint::RefreshPointGeometry(VDrawTool::data.GetPoint(id));
}

void VToolPointOfIntersection::FullUpdateFromGui(int result)
{
    if (result == QDialog::Accepted)
    {
        QDomElement domElement = doc->elementById(QString().setNum(id));
        if (domElement.isElement())
        {
            domElement.setAttribute(AttrName, dialogPointOfIntersection->getPointName());
            domElement.setAttribute(AttrFirstPoint, QString().setNum(dialogPointOfIntersection->getFirstPointId()));
            domElement.setAttribute(AttrSecondPoint, QString().setNum(dialogPointOfIntersection->getSecondPointId()));
            emit FullUpdateTree();
        }
    }
    dialogPointOfIntersection.clear();
}

void VToolPointOfIntersection::RemoveReferens()
{
    doc->DecrementReferens(firstPointId);
    doc->DecrementReferens(secondPointId);
}

void VToolPointOfIntersection::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    ContextMenu(dialogPointOfIntersection, this, event);
}

void VToolPointOfIntersection::AddToFile()
{
    VPointF point = VAbstractTool::data.GetPoint(id);
    QDomElement domElement = doc->createElement(TagName);

    AddAttribute(domElement, AttrId, id);
    AddAttribute(domElement, AttrType, ToolType);
    AddAttribute(domElement, AttrName, point.name());
    AddAttribute(domElement, AttrMx, toMM(point.mx()));
    AddAttribute(domElement, AttrMy, toMM(point.my()));

    AddAttribute(domElement, AttrFirstPoint, firstPointId);
    AddAttribute(domElement, AttrSecondPoint, secondPointId);

    AddToCalculation(domElement);
}
