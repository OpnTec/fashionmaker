/************************************************************************
 **
 **  @file   vdrawtool.h
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

#ifndef VDRAWTOOL_H
#define VDRAWTOOL_H

#include "../vabstracttool.h"

#include <QMenu>
#include <QGraphicsSceneContextMenuEvent>

/**
 * @brief The VDrawTool class
 */
class VDrawTool : public VAbstractTool
{
    Q_OBJECT
public:
                 /**
                  * @brief VDrawTool
                  * @param doc dom document container
                  * @param data container with variables
                  * @param id object id in container
                  * @param parent parent object
                  */
                 VDrawTool(VDomDocument *doc, VContainer *data, qint64 id);
    virtual      ~VDrawTool() {}
    /**
     * @brief setDialog set dialog when user want change tool option.
     */
    virtual void setDialog() {}
    /**
     * @brief ignoreContextMenu
     * @param enable
     */
    void         ignoreContextMenu(bool enable) {ignoreContextMenuEvent = enable;}
public slots:
    /**
     * @brief ShowTool
     * @param id object id in container
     * @param color
     * @param enable
     */
    virtual void ShowTool(qint64 id, Qt::GlobalColor color, bool enable);
    /**
     * @brief ChangedActivDraw
     * @param newName
     */
    virtual void ChangedActivDraw(const QString &newName);
    /**
     * @brief ChangedNameDraw
     * @param oldName
     * @param newName
     */
    void         ChangedNameDraw(const QString &oldName, const QString &newName);
    /**
     * @brief FullUpdateFromGui
     * @param result
     */
    virtual void FullUpdateFromGui(int result)=0;
    /**
     * @brief SetFactor
     * @param factor
     */
    virtual void SetFactor(qreal factor);
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
     * @brief nameActivDraw
     */
    QString      nameActivDraw;
    /**
     * @brief factor
     */
    static qreal factor;
    /**
     * @brief AddToCalculation
     * @param domElement
     */
    void         AddToCalculation(const QDomElement &domElement);
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
        Q_ASSERT(tool != 0);
        Q_ASSERT(event != 0);
        if (ignoreContextMenuEvent == false)
        {
            QMenu menu;
            QAction *actionOption = menu.addAction(tr("Options"));
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
                    DeleteTool(tool);
                }
            }
        }
    }
    template <typename Item>
    /**
     * @brief ShowItem
     * @param item
     * @param id object id in container
     * @param color
     * @param enable
     */
    void ShowItem(Item *item, qint64 id, Qt::GlobalColor color, bool enable)
    {
        Q_ASSERT(item != 0);
        if (id == item->id)
        {
            if (enable == false)
            {
                currentColor = baseColor;
            }
            else
            {
                currentColor = color;
            }
            item->setPen(QPen(currentColor, widthHairLine/factor));
        }
    }
};

#endif // VDRAWTOOL_H
