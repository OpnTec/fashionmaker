/************************************************************************
 **
 **  @file
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   12 9, 2016
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentine project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2016 Valentina project
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
};

class VAbstractSimple;
class VAbstractPattern;
class QDomElement;
class QPainter;
class QPointF;
class QStyleOptionGraphicsItem;
class QWidget;
class VContainer;

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
    virtual ~VAbstractOperation();

    static const QString TagItem;
    static const QString TagSource;
    static const QString TagDestination;

    virtual QString getTagName() const Q_DECL_OVERRIDE;

    void SetEnabled(bool enabled);

    QString Suffix() const;
    void    SetSuffix(const QString &suffix);

    virtual void GroupVisibility(quint32 object, bool visible) Q_DECL_OVERRIDE;
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) Q_DECL_OVERRIDE;

    static void ExtractData(VAbstractPattern *doc, const QDomElement &domElement, QVector<quint32> &source,
                            QVector<DestinationItem> &destination);
public slots:
    virtual void FullUpdateFromFile() Q_DECL_OVERRIDE;
    virtual void SetFactor(qreal factor) Q_DECL_OVERRIDE;

    virtual void AllowHover(bool enabled) Q_DECL_OVERRIDE;
    virtual void AllowSelecting(bool enabled) Q_DECL_OVERRIDE;

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

    virtual void Disable(bool disable, const QString &namePP) Q_DECL_OVERRIDE;
    void         ObjectSelected(bool selected, quint32 objId);
    void         DeleteFromLabel();
    void         LabelChangePosition(const QPointF &pos, quint32 labelId);
protected:
    QString suffix;

    QVector<quint32> source;
    QVector<DestinationItem> destination;

    QMap<quint32, VAbstractSimple *> operatedObjects;

    VAbstractOperation(VAbstractPattern *doc, VContainer *data, quint32 id, const QString &suffix,
                       const QVector<quint32> &source, const QVector<DestinationItem> &destination,
                       QGraphicsItem *parent = nullptr);

    virtual void AddToFile() Q_DECL_OVERRIDE;
    virtual void RefreshDataInFile() Q_DECL_OVERRIDE;

    void UpdateNamePosition(quint32 id);
    void SaveSourceDestination(QDomElement &tag);

    template <typename T>
    void ShowToolVisualization(bool show);

    VSimpleCurve *InitCurve(quint32 id, VContainer *data, GOType curveType);

    template <typename T>
    static void InitOperationToolConnections(VMainGraphicsScene *scene, T *tool);

    void InitOperatedObjects();
protected slots:
    void DoChangePosition(quint32 id, qreal mx, qreal my);
private:
    Q_DISABLE_COPY(VAbstractOperation)

    void AllowCurveHover(bool enabled, GOType type);
    void AllowCurveSelecting(bool enabled, GOType type);

    void ChangePosition(QGraphicsItem *item, quint32 id, const QPointF &pos);
};

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
    SCASSERT(scene != nullptr);
    SCASSERT(tool != nullptr);

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
}

#endif // VABSTRACTOPERATION_H
