/************************************************************************
 **
 **  @file   vmodelingtool.h
 **  @author Roman Telezhinsky <dismine@gmail.com>
 **  @date   November 15, 2013
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

#ifndef VMODELINGTOOL_H
#define VMODELINGTOOL_H

#include "../vabstracttool.h"
#include <QMenu>

/**
 * @brief The VModelingTool class
 */
class VModelingTool: public VAbstractTool
{
    Q_OBJECT
public:
                 /**
                  * @brief VModelingTool
                  * @param doc dom document container
                  * @param data
                  * @param id
                  * @param parent
                  */
                 VModelingTool(VDomDocument *doc, VContainer *data, qint64 id, QObject *parent = 0);
    virtual      ~VModelingTool(){}
    /**
     * @brief setDialog
     */
    virtual void setDialog(){}
    /**
     * @brief ignoreContextMenu
     * @param enable
     */
    inline void  ignoreContextMenu(bool enable) {ignoreContextMenuEvent = enable;}
public slots:
    /**
     * @brief FullUpdateFromGui
     * @param result
     */
    virtual void FullUpdateFromGui(int result)=0;
protected:
    /**
     * @brief ignoreContextMenuEvent
     */
    bool         ignoreContextMenuEvent;
    /**
     * @brief ignoreFullUpdate
     */
    bool         ignoreFullUpdate;
    /**
     * @brief AddToModeling
     * @param domElement
     */
    void         AddToModeling(const QDomElement &domElement);
    /**
     * @brief decrementReferens
     */
    virtual void decrementReferens();
    template <typename Dialog, typename Tool>
    /**
     * @brief ContextMenu
     * @param dialog
     * @param tool
     * @param event
     * @param showRemove
     */
    void ContextMenu(QSharedPointer<Dialog> &dialog, Tool *tool, QGraphicsSceneContextMenuEvent *event,
                     bool showRemove = true)
    {
        if (ignoreContextMenuEvent == false)
        {
            QMenu menu;
            QAction *actionOption = menu.addAction(tr("Option"));
            QAction *actionRemove = 0;
            if (showRemove)
            {
               actionRemove = menu.addAction(tr("Delete"));
               if (_referens > 1)
               {
                   actionRemove->setEnabled(false);
               }
               else
               {
                   actionRemove->setEnabled(true);
               }
            }
            QAction *selectedAction = menu.exec(event->screenPos());
            if (selectedAction == actionOption)
            {
                dialog = QSharedPointer<Dialog>(new Dialog(getData()));

                connect(qobject_cast< VMainGraphicsScene * >(tool->scene()),
                        &VMainGraphicsScene::ChoosedObject, dialog.data(), &Dialog::ChoosedObject);
                connect(dialog.data(), &Dialog::DialogClosed, tool, &Tool::FullUpdateFromGui);
                if (ignoreFullUpdate == false)
                {
                    connect(doc, &VDomDocument::FullUpdateFromFile, dialog.data(), &Dialog::UpdateList);
                }

               tool->setDialog();

               dialog->show();
            }
            if (showRemove)
            {
                if (selectedAction == actionRemove)
                {
                    //deincrement referens
                    RemoveReferens();
                    //remove form xml file
                    QDomElement domElement = doc->elementById(QString().setNum(id));
                    if (domElement.isElement())
                    {
                        QDomElement element;
                        bool ok = doc->GetActivCalculationElement(element);
                        if (ok)
                        {
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
    }
};

#endif // VMODELINGTOOL_H
