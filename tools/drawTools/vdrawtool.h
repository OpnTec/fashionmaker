#ifndef VDRAWTOOL_H
#define VDRAWTOOL_H

#include "../vabstracttool.h"
#include <QMenu>

class VDrawTool : public VAbstractTool
{
    Q_OBJECT
public:
                 VDrawTool(VDomDocument *doc, VContainer *data, qint64 id, QObject *parent = 0);
    virtual      ~VDrawTool();
    virtual void setDialog();
    static void  AddRecord(const qint64 id, Tools::Enum toolType, VDomDocument *doc);
    void         ignoreContextMenu(bool enable);
public slots:
    virtual void ShowTool(qint64 id, Qt::GlobalColor color, bool enable);
    virtual void ChangedActivDraw(const QString newName);
    void         ChangedNameDraw(const QString oldName, const QString newName);
    virtual void FullUpdateFromGui(int result)=0;
signals:
    void         RemoveTool(QGraphicsItem *tool);
protected:
    bool         ignoreContextMenuEvent;
    QString      nameActivDraw;
    void         AddToCalculation(const QDomElement &domElement);
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
};

#endif // VDRAWTOOL_H
