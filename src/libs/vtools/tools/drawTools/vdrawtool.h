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
 **  Copyright (C) 2013-2015 Valentina project
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
#include "../../dialogs/tools/dialogtool.h"
#include "../../../vwidgets/vmaingraphicsscene.h"
#include "../../../vwidgets/vmaingraphicsview.h"

#include <QMenu>
#include <QGraphicsSceneContextMenuEvent>
#include <QGraphicsView>

/**
 * @brief The VDrawTool abstract class for all draw tool.
 */
class VDrawTool : public VAbstractTool
{
    Q_OBJECT
public:

    VDrawTool(VAbstractPattern *doc, VContainer *data, quint32 id, QObject *parent = nullptr);
    virtual      ~VDrawTool();

    /** @brief factor scene scale factor. */
    static qreal factor;

    /** @brief setDialog set dialog when user want change tool option. */
    virtual void setDialog() {}
    virtual void DialogLinkDestroy();
    static qreal CheckFormula(const quint32 &toolId, QString &formula, VContainer *data);

    QString      getLineType() const;
    virtual void SetTypeLine(const QString &value);

    QString      GetLineColor() const;
    virtual void SetLineColor(const QString &value);

public slots:
    virtual void ShowTool(quint32 id, bool enable);
    virtual void ChangedActivDraw(const QString &newName);
    void         ChangedNameDraw(const QString &oldName, const QString &newName);
    virtual void FullUpdateFromGuiOk(int result);
    virtual void FullUpdateFromGuiApply();
    virtual void SetFactor(qreal factor);
    virtual void EnableToolMove(bool move);
    virtual void Disable(bool disable, const QString &namePP)=0;
protected:

    /** @brief nameActivDraw name of tool's pattern peace. */
    QString      nameActivDraw;

    /** @brief dialog dialog options.*/
    DialogTool  *dialog;

    /** @brief typeLine line type. */
    QString      typeLine;

    /** @brief lineColor color line or curve, but not a point. */
    QString      lineColor;

    bool         enabled;

    void         AddToCalculation(const QDomElement &domElement);

    /** @brief SaveDialog save options into file after change in dialog. */
    virtual void SaveDialog(QDomElement &domElement)=0;
    void         SaveDialogChange();
    virtual void AddToFile();
    virtual void RefreshDataInFile();
    void         SaveOption(QSharedPointer<VGObject> &obj);
    virtual void SaveOptions(QDomElement &tag, QSharedPointer<VGObject> &obj);

    QColor       CorrectColor(const QColor &color) const;
    bool         CorrectDisable(bool disable, const QString &namePP) const;

    void         ReadAttributes();
    virtual void ReadToolAttributes(const QDomElement &domElement)=0;

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

        QMenu menu;
        QAction *actionOption = menu.addAction(QIcon::fromTheme("preferences-other"), tr("Options"));
        QAction *actionRemove = nullptr;
        actionRemove = menu.addAction(QIcon::fromTheme("edit-delete"), tr("Delete"));
        if (showRemove)
        {
            if (_referens > 1)
            {
                actionRemove->setEnabled(false);
            }
            else
            {
                actionRemove->setEnabled(true);
            }
        }
        else
        {
            actionRemove->setEnabled(false);
        }

        QAction *selectedAction = menu.exec(event->screenPos());
        if (selectedAction == actionOption)
        {
            qApp->getSceneView()->itemClicked(nullptr);
            dialog = new Dialog(getData(), id, qApp->getMainWindow());
            dialog->setModal(true);

            connect(dialog, &DialogTool::DialogClosed, tool, &Tool::FullUpdateFromGuiOk);
            connect(dialog, &DialogTool::DialogApplied, tool, &Tool::FullUpdateFromGuiApply);

            tool->setDialog();

            dialog->show();
        }
        if (selectedAction == actionRemove)
        {
            DeleteTool();
            return; //Leave this method immediately after call!!!
        }
    }
    template <typename Item>
    /**
     * @brief ShowItem highlight tool.
     * @param item tool.
     * @param id object id in container.
     * @param enable enable or disable highlight.
     */
    void ShowItem(Item *item, quint32 id, bool enable)
    {
        SCASSERT(item != nullptr);
        if (id == item->id)
        {
            ShowVisualization(enable);
        }
    }
private:
    Q_DISABLE_COPY(VDrawTool)
};

#endif // VDRAWTOOL_H
