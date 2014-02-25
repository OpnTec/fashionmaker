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
#include "../../dialogs/tools/dialogtool.h"

/**
 * @brief The VDrawTool abstract class for all draw tool.
 */
class VDrawTool : public VAbstractTool
{
    Q_OBJECT
public:
                 /**
                  * @brief VDrawTool constructor.
                  * @param doc dom document container.
                  * @param data container with variables.
                  * @param id object id in container.
                  * @param parent parent object.
                  */
                 VDrawTool(VPattern *doc, VContainer *data, qint64 id);
    virtual      ~VDrawTool();
    /**
     * @brief setDialog set dialog when user want change tool option.
     */
    virtual void setDialog() {}
    /**
     * @brief ignoreContextMenu set ignore contect menu tool.
     * @param enable true - ignore.
     */
    void         ignoreContextMenu(bool enable) {ignoreContextMenuEvent = enable;}
public slots:
    /**
     * @brief ShowTool  highlight tool.
     * @param id object id in container.
     * @param color highlight color.
     * @param enable enable or disable highlight.
     */
    virtual void ShowTool(qint64 id, Qt::GlobalColor color, bool enable);
    /**
     * @brief ChangedActivDraw disable or enable context menu after change active pattern peace.
     * @param newName new name active pattern peace. name new active pattern peace.
     */
    virtual void ChangedActivDraw(const QString &newName);
    /**
     * @brief ChangedNameDraw save new name active pattern peace.
     * @param oldName old name.
     * @param newName new name active pattern peace. new name.
     */
    void         ChangedNameDraw(const QString &oldName, const QString &newName);
    /**
     * @brief FullUpdateFromGui refresh tool data after change in options.
     * @param result keep result working dialog.
     */
    virtual void FullUpdateFromGui(int result);
    /**
     * @brief SetFactor set current scale factor of scene.
     * @param factor scene scale factor.
     */
    virtual void SetFactor(qreal factor);
protected:
    /**
     * @brief ignoreContextMenuEvent ignore or not context menu events.
     */
    bool         ignoreContextMenuEvent;
    /**
     * @brief ignoreFullUpdate ignore or not full updates.
     */
    bool         ignoreFullUpdate;
    /**
     * @brief nameActivDraw name of tool's pattern peace.
     */
    QString      nameActivDraw;
    /**
     * @brief factor scene scale factor.
     */
    static qreal factor;
    /**
     * @brief dialog dialog options.
     */
    DialogTool *dialog;
    /**
     * @brief AddToCalculation add tool to calculation tag in pattern file.
     * @param domElement tag in xml tree.
     */
    void         AddToCalculation(const QDomElement &domElement);
    /**
     * @brief SaveDialog save options into file after change in dialog.
     */
    virtual void SaveDialog(QDomElement &domElement)=0;
    template <typename Dialog, typename Tool>
    /**
     * @brief ContextMenu show context menu for tool.
     * @param tool tool.
     * @param event context menu event.
     * @param showRemove true - tool have option delete.
     */
    void ContextMenu(Tool *tool, QGraphicsSceneContextMenuEvent *event, bool showRemove = true)
    {
        Q_CHECK_PTR(tool);
        Q_CHECK_PTR(event);
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
                QGraphicsScene *scene = tool->scene();
                QList<QGraphicsView *> list =  scene->views();
                dialog = new Dialog(getData(), list.first());

                connect(qobject_cast< VMainGraphicsScene * >(tool->scene()),
                        &VMainGraphicsScene::ChoosedObject, dialog, &DialogTool::ChoosedObject);
                connect(dialog, &DialogTool::DialogClosed, tool, &Tool::FullUpdateFromGui);
                if (ignoreFullUpdate == false)
                {
                    connect(doc, &VPattern::FullUpdateFromFile, dialog, &DialogTool::UpdateList);
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
     * @brief ShowItem highlight tool.
     * @param item tool.
     * @param id object id in container.
     * @param color highlight color.
     * @param enable enable or disable highlight.
     */
    void ShowItem(Item *item, qint64 id, Qt::GlobalColor color, bool enable)
    {
        Q_CHECK_PTR(item);
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
private:
    Q_DISABLE_COPY(VDrawTool)
};

#endif // VDRAWTOOL_H
