/************************************************************************
 **
 **  @file
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   12 9, 2016
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentina project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2016 Valentina project
 **  <https://gitlab.com/smart-pattern/valentina> All Rights Reserved.
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

#ifndef VABSTRACTOPERATION_H
#define VABSTRACTOPERATION_H

#include <QtGlobal>
#include <qcompilerdetection.h>
#include <QMap>
#include <QMetaObject>
#include <QObject>
#include <QString>
#include <QVector>
#include <QGraphicsLineItem>

#include "../vdrawtool.h"
#include "../vwidgets/vsimplecurve.h"
#include "../vwidgets/vsimplepoint.h"

struct DestinationItem
{
    quint32 id;
    qreal mx;
    qreal my;
    bool showLabel;
};

struct VAbstractOperationInitData : VAbstractToolInitData
{
    VAbstractOperationInitData()
        : VAbstractToolInitData()
    {}

    QString suffix{};
    QVector<quint32> source{};
    QVector<DestinationItem> destination{};
    QString visibilityGroupName{};
    QStringList visibilityGroupTags{};
    bool hasLinkedVisibilityGroup{false};
};

// FIXME. I don't know how to use QGraphicsItem properly, so just took first available finished class.
// QGraphicsItem itself produce case where clicking on empty space produce call to QGraphicsItem.
// And i don't know how to fix it.
class VAbstractOperation : public VDrawTool, public QGraphicsLineItem
{
    Q_OBJECT
    // Fix warning "Class implements the interface QGraphicsItem but does not list it
    // in Q_INTERFACES. qobject_cast to QGraphicsItem will not work!"
    Q_INTERFACES(QGraphicsItem)
public:
    virtual ~VAbstractOperation() Q_DECL_EQ_DEFAULT;

    static const QString TagItem;
    static const QString TagSource;
    static const QString TagDestination;

    virtual QString getTagName() const override;

    QString Suffix() const;
    void    SetSuffix(const QString &suffix);

    QVector<QString> SourceItems() const;

    virtual void GroupVisibility(quint32 object, bool visible) override;
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    virtual void ChangeLabelPosition(quint32 id, const QPointF &pos) override;

    virtual bool IsLabelVisible(quint32 id) const override;
    virtual void SetLabelVisible(quint32 id, bool visible) override;

    static void ExtractData(const QDomElement &domElement, VAbstractOperationInitData &initData);
public slots:
    virtual void FullUpdateFromFile() override;

    virtual void AllowHover(bool enabled) override;
    virtual void AllowSelecting(bool enabled) override;
    virtual void EnableToolMove(bool move) override;

    void         AllowPointHover(bool enabled);
    void         AllowPointSelecting(bool enabled);

    void         AllowPointLabelHover(bool enabled);
    void         AllowPointLabelSelecting(bool enabled);

    void         AllowSplineHover(bool enabled);
    void         AllowSplineSelecting(bool enabled);

    void         AllowSplinePathHover(bool enabled);
    void         AllowSplinePathSelecting(bool enabled);

    void         AllowArcHover(bool enabled);
    void         AllowArcSelecting(bool enabled);

    void         AllowElArcHover(bool enabled);
    void         AllowElArcSelecting(bool enabled);

    virtual void ToolSelectionType(const SelectionType &type) override;
    virtual void Disable(bool disable, const QString &namePP) override;
    void         ObjectSelected(bool selected, quint32 objId);
    void         DeleteFromLabel();
    void         LabelChangePosition(const QPointF &pos, quint32 labelId);
protected:
    QString suffix;

    QVector<quint32> source;
    QVector<DestinationItem> destination;

    QMap<quint32, VAbstractSimple *> operatedObjects;

    bool hasLinkedGroup{false};
    QString groupName{};
    QStringList groupTags{};

    VAbstractOperation(VAbstractPattern *doc, VContainer *data, quint32 id, const QString &suffix,
                       const QVector<quint32> &source, const QVector<DestinationItem> &destination,
                       QGraphicsItem *parent = nullptr);

    virtual void AddToFile() override;
    virtual void ChangeLabelVisibility(quint32 id, bool visible) override;
    virtual void ApplyToolOptions(const QList<quint32> &oldDependencies, const QList<quint32> &newDependencies,
                                  const QDomElement &oldDomElement, const QDomElement &newDomElement) override;
    virtual void PerformDelete() override;

    void UpdateNamePosition(quint32 id, const QPointF &pos);
    void SaveSourceDestination(QDomElement &tag);

    template <typename T>
    void ShowToolVisualization(bool show);

    template <typename T>
    void SetDialogVisibilityGroupData(QPointer<T> dialogTool);

    void InitCurve(quint32 id, VContainer *data, GOType curveType, SceneObject sceneType);

    template <typename T>
    static void InitOperationToolConnections(VMainGraphicsScene *scene, T *tool);

    void InitOperatedObjects();

    QString ComplexPointToolTip(quint32 itemId) const;
    QString ComplexCurveToolTip(quint32 itemId) const;
    QString VisibilityGroupToolTip() const;

    static void CreateVisibilityGroup(const VAbstractOperationInitData & initData);
private:
    Q_DISABLE_COPY(VAbstractOperation)

    void AllowCurveHover(bool enabled, GOType type);
    void AllowCurveSelecting(bool enabled, GOType type);

    bool NeedUpdateVisibilityGroup() const;
};

//---------------------------------------------------------------------------------------------------------------------
template<typename T>
void VAbstractOperation::SetDialogVisibilityGroupData(QPointer<T> dialogTool)
{
    SCASSERT(not dialogTool.isNull())

    vidtype group = doc->GroupLinkedToTool(m_id);
    dialogTool->SetGroupCategories(doc->GetGroupCategories());
    if (group != null_id)
    {
        dialogTool->SetHasLinkedVisibilityGroup(true);
        dialogTool->SetVisibilityGroupName(doc->GetGroupName(group));
        dialogTool->SetVisibilityGroupTags(doc->GetGroupTags(group));
    }
    else
    {
        dialogTool->SetHasLinkedVisibilityGroup(false);
    }
}

//---------------------------------------------------------------------------------------------------------------------
template <typename T>
void VAbstractOperation::ShowToolVisualization(bool show)
{
    if (show)
    {
        if (vis.isNull())
        {
            AddVisualization<T>();
            SetVisualization();
        }
        else
        {
            if (T *visual = qobject_cast<T *>(vis))
            {
                visual->show();
            }
        }
    }
    else
    {
        delete vis;
    }
}

//---------------------------------------------------------------------------------------------------------------------
template <typename T>
void VAbstractOperation::InitOperationToolConnections(VMainGraphicsScene *scene, T *tool)
{
    SCASSERT(scene != nullptr)
    SCASSERT(tool != nullptr)

    InitDrawToolConnections(scene, tool);

    QObject::connect(scene, &VMainGraphicsScene::EnablePointItemHover, tool, &T::AllowPointHover);
    QObject::connect(scene, &VMainGraphicsScene::EnablePointItemSelection, tool, &T::AllowPointSelecting);
    QObject::connect(scene, &VMainGraphicsScene::EnableLabelItemHover, tool, &T::AllowPointLabelHover);
    QObject::connect(scene, &VMainGraphicsScene::EnableLabelItemSelection, tool, &T::AllowPointLabelSelecting);

    QObject::connect(scene, &VMainGraphicsScene::EnableSplineItemHover, tool, &T::AllowSplineHover);
    QObject::connect(scene, &VMainGraphicsScene::EnableSplineItemSelection, tool, &T::AllowSplineSelecting);

    QObject::connect(scene, &VMainGraphicsScene::EnableSplinePathItemHover, tool, &T::AllowSplinePathHover);
    QObject::connect(scene, &VMainGraphicsScene::EnableSplinePathItemSelection, tool, &T::AllowSplinePathSelecting);

    QObject::connect(scene, &VMainGraphicsScene::EnableArcItemHover, tool, &T::AllowArcHover);
    QObject::connect(scene, &VMainGraphicsScene::EnableArcItemSelection, tool, &T::AllowArcSelecting);

    QObject::connect(scene, &VMainGraphicsScene::EnableElArcItemHover, tool, &T::AllowElArcHover);
    QObject::connect(scene, &VMainGraphicsScene::EnableElArcItemSelection, tool, &T::AllowElArcSelecting);
}

#endif // VABSTRACTOPERATION_H
