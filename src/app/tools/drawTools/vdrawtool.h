/************************************************************************
 **
 **  @file   vdrawtool.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
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

    VDrawTool(VPattern *doc, VContainer *data, quint32 id);
    virtual      ~VDrawTool();

    /** @brief setDialog set dialog when user want change tool option. */
    virtual void setDialog() {}
    /**
     * @brief DialogLinkDestroy removes dialog pointer
     */
    virtual void DialogLinkDestroy();

    void         ignoreContextMenu(bool enable);
    static qreal CheckFormula(QString &formula, VContainer *data);
public slots:
    virtual void ShowTool(quint32 id, Qt::GlobalColor color, bool enable);
    virtual void ChangedActivDraw(const QString &newName);
    void         ChangedNameDraw(const QString &oldName, const QString &newName);
    virtual void FullUpdateFromGui(int result);
    /**
     * @brief FullUpdateFromGuiApply refresh tool data after change in options but do not delete dialog
     */
    virtual void FullUpdateFromGuiApply();
    virtual void SetFactor(qreal factor);
protected:
    /** @brief ignoreContextMenuEvent ignore or not context menu events. */
    bool         ignoreContextMenuEvent;

    /** @brief ignoreFullUpdate ignore or not full updates. */
    bool         ignoreFullUpdate;

    /** @brief nameActivDraw name of tool's pattern peace. */
    QString      nameActivDraw;

    /** @brief factor scene scale factor. */
    static qreal factor;

    /** @brief dialog dialog options.*/
    DialogTool *dialog;

    void         AddToCalculation(const QDomElement &domElement);

    /** @brief SaveDialog save options into file after change in dialog. */
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
        SCASSERT(tool != nullptr);
        SCASSERT(event != nullptr);
        if (ignoreContextMenuEvent == false)
        {
            QMenu menu;
            QAction *actionOption = menu.addAction(tr("Options"));
            QAction *actionRemove = nullptr;
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
                connect(dialog, &DialogTool::DialogApplied, tool, &Tool::FullUpdateFromGuiApply);
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
    void ShowItem(Item *item, quint32 id, Qt::GlobalColor color, bool enable)
    {
        SCASSERT(item != nullptr);
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
            item->setPen(QPen(currentColor, qApp->toPixel(qApp->widthHairLine())/factor));
        }
    }
private:
    Q_DISABLE_COPY(VDrawTool)
};

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief ignoreContextMenu set ignore contect menu tool.
 * @param enable true - ignore.
 */
inline void VDrawTool::ignoreContextMenu(bool enable)
{
    ignoreContextMenuEvent = enable;
}

#endif // VDRAWTOOL_H
