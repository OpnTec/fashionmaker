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
#include "../vwidgets/vmaingraphicsview.h"

#include <QMenu>
#include <QGraphicsSceneContextMenuEvent>

/**
 * @brief The VDrawTool abstract class for all draw tool.
 */
class VDrawTool : public VAbstractTool
{
    Q_OBJECT
public:

    VDrawTool(VAbstractPattern *doc, VContainer *data, quint32 id, QObject *parent = nullptr);
    virtual      ~VDrawTool() Q_DECL_OVERRIDE;

    /** @brief factor scene scale factor. */
    static qreal factor;

    /** @brief setDialog set dialog when user want change tool option. */
    virtual void setDialog() {}
    virtual void DialogLinkDestroy();
    static qreal CheckFormula(const quint32 &toolId, QString &formula, VContainer *data);

    QString      getLineType() const;
    virtual void SetTypeLine(const QString &value);

signals:
    void ChangedToolSelection(bool selected, quint32 object, quint32 tool);

public slots:
    virtual void ShowTool(quint32 id, bool enable);
    virtual void ChangedActivDraw(const QString &newName);
    void         ChangedNameDraw(const QString &oldName, const QString &newName);
    virtual void FullUpdateFromGuiOk(int result);
    virtual void FullUpdateFromGuiApply();
    virtual void SetFactor(qreal factor);
    virtual void EnableToolMove(bool move);
    virtual void Disable(bool disable, const QString &namePP)=0;
    virtual void DetailsMode(bool mode);
protected:

    enum class RemoveOption : bool {Disable = false, Enable = true};
    enum class Referens : bool {Follow = true, Ignore = false};

    /** @brief nameActivDraw name of tool's pattern peace. */
    QString      nameActivDraw;

    /** @brief dialog dialog options.*/
    DialogTool  *dialog;

    /** @brief typeLine line type. */
    QString      typeLine;

    bool         enabled;

    void         AddToCalculation(const QDomElement &domElement);

    /** @brief SaveDialog save options into file after change in dialog. */
    virtual void SaveDialog(QDomElement &domElement)=0;
    void         SaveDialogChange();
    virtual void AddToFile() Q_DECL_OVERRIDE;
    virtual void RefreshDataInFile() Q_DECL_OVERRIDE;
    void         SaveOption(QSharedPointer<VGObject> &obj);
    virtual void SaveOptions(QDomElement &tag, QSharedPointer<VGObject> &obj);

    QColor       CorrectColor(const QColor &color) const;
    bool         CorrectDisable(bool disable, const QString &namePP) const;

    void         ReadAttributes();
    virtual void ReadToolAttributes(const QDomElement &domElement)=0;

    template <typename Dialog, typename Tool>
    void ContextMenu(Tool *tool, QGraphicsSceneContextMenuEvent *event,
                     const RemoveOption &showRemove = RemoveOption::Enable,
                     const Referens &ref = Referens::Follow);

    template <typename Item>
    void ShowItem(Item *item, quint32 id, bool enable);

    template <typename T>
    QString ObjectName(quint32 id) const;

    template <typename T>
    static void InitDrawToolConnections(VMainGraphicsScene *scene, T *tool);
private:
    Q_DISABLE_COPY(VDrawTool)
};

//---------------------------------------------------------------------------------------------------------------------
template <typename Dialog, typename Tool>
/**
 * @brief ContextMenu show context menu for tool.
 * @param tool tool.
 * @param event context menu event.
 * @param showRemove true - tool enable option delete.
 * @param ref true - do not ignore referens value.
 */
void VDrawTool::ContextMenu(Tool *tool, QGraphicsSceneContextMenuEvent *event, const RemoveOption &showRemove,
                            const Referens &ref)
{
    SCASSERT(tool != nullptr);
    SCASSERT(event != nullptr);

    qCDebug(vTool, "Creating tool context menu.");
    QMenu menu;
    QAction *actionOption = menu.addAction(QIcon::fromTheme("preferences-other"), tr("Options"));
    QAction *actionRemove = menu.addAction(QIcon::fromTheme("edit-delete"), tr("Delete"));
    if (showRemove == RemoveOption::Enable)
    {
        if (ref == Referens::Follow)
        {
            if (_referens > 1)
            {
                qCDebug(vTool, "Remove disabled. Tool has childern.");
                actionRemove->setEnabled(false);
            }
            else
            {
                qCDebug(vTool, "Remove enabled. Tool has not childern.");
                actionRemove->setEnabled(true);
            }
        }
        else
        {
            qCDebug(vTool, "Remove enabled. Ignore referens value.");
            actionRemove->setEnabled(true);
        }
    }
    else
    {
        qCDebug(vTool, "Remove disabled.");
        actionRemove->setEnabled(false);
    }

    QAction *selectedAction = menu.exec(event->screenPos());
    if (selectedAction == actionOption)
    {
        qCDebug(vTool, "Show options.");
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
        qCDebug(vTool, "Deleting tool.");
        DeleteTool(); // do not catch exception here
        return; //Leave this method immediately after call!!!
    }
}

//---------------------------------------------------------------------------------------------------------------------
template <typename Item>
/**
 * @brief ShowItem highlight tool.
 * @param item tool.
 * @param id object id in container.
 * @param enable enable or disable highlight.
 */
void VDrawTool::ShowItem(Item *item, quint32 id, bool enable)
{
    SCASSERT(item != nullptr);
    if (id == item->id)
    {
        ShowVisualization(enable);
    }
}

//---------------------------------------------------------------------------------------------------------------------
template <typename T>
/**
 * @brief ObjectName get object (point, curve, arc) name.
 * @param id object id in container.
 */
QString VDrawTool::ObjectName(quint32 id) const
{
    try
    {
        return VAbstractTool::data.GeometricObject<T>(id)->name();
    }
    catch (const VExceptionBadId &e)
    {
        qCDebug(vTool, "Error! Couldn't get object name by id = %s. %s %s", qUtf8Printable(QString().setNum(id)),
                qUtf8Printable(e.ErrorMessage()),
                qUtf8Printable(e.DetailedInformation()));
        return QString("");// Return empty string for property browser
    }
}

//---------------------------------------------------------------------------------------------------------------------
template <typename T>
void VDrawTool::InitDrawToolConnections(VMainGraphicsScene *scene, T *tool)
{
    SCASSERT(scene != nullptr);
    SCASSERT(tool != nullptr);

    QObject::connect(tool, &T::ChoosedTool, scene, &VMainGraphicsScene::ChoosedItem);
    QObject::connect(tool, &T::ChangedToolSelection, scene, &VMainGraphicsScene::SelectedItem);
    QObject::connect(scene, &VMainGraphicsScene::NewFactor, tool, &T::SetFactor);
    QObject::connect(scene, &VMainGraphicsScene::DisableItem, tool, &T::Disable);
    QObject::connect(scene, &VMainGraphicsScene::EnableToolMove, tool, &T::EnableToolMove);
    QObject::connect(scene, &VMainGraphicsScene::CurveDetailsMode, tool, &T::DetailsMode);
    QObject::connect(scene, &VMainGraphicsScene::ItemSelection, tool, &T::ToolSelectionType);
}

#endif // VDRAWTOOL_H
