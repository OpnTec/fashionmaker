#ifndef VABSTRACTTOOL_H
#define VABSTRACTTOOL_H

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#pragma GCC diagnostic ignored "-Wconversion"
#pragma GCC diagnostic ignored "-Wsign-conversion"
#pragma GCC diagnostic ignored "-Wctor-dtor-privacy"
#include <QMenu>
#include "../xml/vdomdocument.h"
#include "vdatatool.h"
#pragma GCC diagnostic pop
#include "../container/vcontainer.h"

namespace Tool{
    enum Enum
    {
        FromGui,
        FromFile
    };
}

class VAbstractTool:public VDataTool
{
    Q_OBJECT
public:
                 VAbstractTool(VDomDocument *doc, VContainer *data, qint64 id, Draw::Mode mode,
                               QObject *parent = 0);
    virtual      ~VAbstractTool();
    virtual void setDialog();
    static void AddRecord(const qint64 id, Tools::Enum toolType, VDomDocument *doc);
    void ignoreContextMenu(bool enable);
public slots:
    virtual void FullUpdateFromFile()=0;
    void         ChangedNameDraw(const QString oldName, const QString newName);
    virtual void ChangedActivDraw(const QString newName);
    virtual void FullUpdateFromGui(int result)=0;
    virtual void ShowTool(qint64 id, Qt::GlobalColor color, bool enable);
signals:
    void         toolhaveChange();
    void         ChoosedTool(qint64 id, Scene::Type type);
    void         FullUpdateTree();
    void         RemoveTool(QGraphicsItem *tool);
protected:
    VDomDocument *doc;
    qint64       id;
    bool         ignoreContextMenuEvent;
    QString      nameActivDraw;
    const Qt::GlobalColor baseColor;
    Qt::GlobalColor currentColor;
    Draw::Mode mode;
    virtual void AddToFile()=0;
    void         AddAttribute(QDomElement &domElement, const QString &name, const qint64 &value);
    void         AddAttribute(QDomElement &domElement, const QString &name, const qint32 &value);
    void         AddAttribute(QDomElement &domElement, const QString &name, const qreal &value);
    void         AddAttribute(QDomElement &domElement, const QString &name, const QString &value);
    void         AddToDraw(const QDomElement &domElement);
    const VContainer *getData() const;
    void         setData(const VContainer &value);
    template <typename Dialog, typename Tool>
    void ContextMenu(QSharedPointer<Dialog> &dialog, Tool *tool, QGraphicsSceneContextMenuEvent *event,
                     bool showRemove = true){
        if(!ignoreContextMenuEvent){
            QMenu menu;
            QAction *actionOption = menu.addAction("Властивості");
            QAction *actionRemove;
            if(showRemove){
                actionRemove = menu.addAction("Видалити");
            }
            QAction *selectedAction = menu.exec(event->screenPos());
            if(selectedAction == actionOption){
                dialog = QSharedPointer<Dialog>(new Dialog(getData()));

                connect(qobject_cast< VMainGraphicsScene * >(tool->scene()), &VMainGraphicsScene::ChoosedObject,
                        dialog.data(), &Dialog::ChoosedObject);
                connect(dialog.data(), &Dialog::DialogClosed, tool,
                        &Tool::FullUpdateFromGui);
                connect(doc, &VDomDocument::FullUpdateFromFile, dialog.data(), &Dialog::UpdateList);

                tool->setDialog();

                dialog->show();
            }
            if(selectedAction == actionRemove){
                //remove form xml file
                QDomElement domElement = doc->elementById(QString().setNum(id));
                if(domElement.isElement()){
                    QDomElement element;
                    bool ok = doc->GetActivCalculationElement(element);
                    if(ok){
                        element.removeChild(domElement);
                        //update xml file
                        emit FullUpdateTree();
                        //remove form scene
                        emit RemoveTool(tool);
                    }
                }
            }
        }
    }
private:
    VAbstractTool(const VAbstractTool &tool);
    const VAbstractTool &operator=(const VAbstractTool &tool);
};
#endif // VABSTRACTTOOL_H
