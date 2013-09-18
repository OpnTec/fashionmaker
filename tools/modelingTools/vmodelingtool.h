/****************************************************************************
 **
 **  Copyright (C) 2013 Valentina project All Rights Reserved.
 **
 **  This file is part of Valentina.
 **
 **  Tox is free software: you can redistribute it and/or modify
 **  it under the terms of the GNU General Public License as published by
 **  the Free Software Foundation, either version 3 of the License, or
 **  (at your option) any later version.
 **
 **  Tox is distributed in the hope that it will be useful,
 **  but WITHOUT ANY WARRANTY; without even the implied warranty of
 **  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 **  GNU General Public License for more details.
 **
 **  You should have received a copy of the GNU General Public License
 **  along with Valentina.  If not, see <http://www.gnu.org/licenses/>.
 **
 ****************************************************************************/

#ifndef VMODELINGTOOL_H
#define VMODELINGTOOL_H

#include "../vabstracttool.h"
#include <QMenu>

class VModelingTool: public VAbstractTool
{
    Q_OBJECT
public:
    VModelingTool(VDomDocument *doc, VContainer *data, qint64 id, QObject *parent = 0);
    virtual      ~VModelingTool();
    virtual void setDialog();
    void         ignoreContextMenu(bool enable);
public slots:
    virtual void FullUpdateFromGui(int result)=0;
protected:
    bool         ignoreContextMenuEvent;
    void         AddToModeling(const QDomElement &domElement);
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
                       //emit RemoveTool(tool);
                   }
               }
            }
        }
    }
};

#endif // VMODELINGTOOL_H
