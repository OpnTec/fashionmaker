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
 **  Valentina is distributed in the hope that it will be useful,
 **  but WITHOUT ANY WARRANTY; without even the implied warranty of
 **  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 **  GNU General Public License for more details.
 **
 **  You should have received a copy of the GNU General Public License
 **  along with Valentina.  If not, see <http://www.gnu.org/licenses/>.
 **
 ****************************************************************************/

#ifndef VDRAWTOOL_H
#define VDRAWTOOL_H

#include "../vabstracttool.h"

class VDrawTool : public VAbstractTool
{
    Q_OBJECT
public:
                 VDrawTool(VDomDocument *doc, VContainer *data, qint64 id, QObject *parent = 0);
    virtual      ~VDrawTool() {}
    virtual void setDialog() {}
    static void  AddRecord(const qint64 id, Tool::Tools toolType, VDomDocument *doc);
    void         ignoreContextMenu(bool enable) {ignoreContextMenuEvent = enable;}
public slots:
    virtual void ShowTool(qint64 id, Qt::GlobalColor color, bool enable);
    virtual void ChangedActivDraw(const QString newName);
    void         ChangedNameDraw(const QString oldName, const QString newName);
    virtual void FullUpdateFromGui(int result)=0;
    virtual void SetFactor(qreal factor);
protected:
    bool         ignoreContextMenuEvent;
    bool         ignoreFullUpdate;
    QString      nameActivDraw;
    static qreal factor;
    void         AddToCalculation(const QDomElement &domElement);
    template <typename Dialog, typename Tool>
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
    template <typename Item>
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
