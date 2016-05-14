/************************************************************************
 **
 **  @file   vtoolrotation.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   12 4, 2016
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

#ifndef VTOOLROTATION_H
#define VTOOLROTATION_H

#include "../vdrawtool.h"

struct DestinationItem
{
    quint32 id;
    qreal mx;
    qreal my;
};

class VAbstractSimple;
class VSimpleCurve;

class VToolRotation : public VDrawTool, public QGraphicsItem
{
    Q_OBJECT
    // Fix warning "Class implements the interface QGraphicsItem but does not list it
    // in Q_INTERFACES. qobject_cast to QGraphicsItem will not work!"
    Q_INTERFACES(QGraphicsItem)
public:
    virtual ~VToolRotation();
    virtual void setDialog() Q_DECL_OVERRIDE;
    static VToolRotation* Create(DialogTool *dialog, VMainGraphicsScene *scene, VAbstractPattern *doc,
                                 VContainer *data);
    static VToolRotation* Create(const quint32 _id, const quint32 &origin, QString &angle, const QString &suffix,
                                 const QVector<quint32> &source, const QVector<DestinationItem> &destination,
                                 VMainGraphicsScene *scene, VAbstractPattern *doc, VContainer *data,
                                 const Document &parse, const Source &typeCreation);
    // cppcheck-suppress duplInheritedMember
    static const QString TagName;
    static const QString TagItem;
    static const QString TagSource;
    static const QString TagDestination;
    virtual int type() const Q_DECL_OVERRIDE {return Type;}
    enum { Type = UserType + static_cast<int>(Tool::Rotation)};
    virtual QString getTagName() const Q_DECL_OVERRIDE;

    void setEnabled(bool enabled);

    QString Suffix() const;

    virtual void GroupVisibility(quint32 object, bool visible) Q_DECL_OVERRIDE;
    virtual void ShowVisualization(bool show) Q_DECL_OVERRIDE;

    virtual QRectF boundingRect() const Q_DECL_OVERRIDE;
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) Q_DECL_OVERRIDE;

public slots:
    virtual void FullUpdateFromFile() Q_DECL_OVERRIDE;

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
    virtual void AddToFile() Q_DECL_OVERRIDE;
    virtual void RefreshDataInFile() Q_DECL_OVERRIDE;
    virtual void SetVisualization() Q_DECL_OVERRIDE;
    virtual void SaveDialog(QDomElement &domElement) Q_DECL_OVERRIDE;
    virtual void ReadToolAttributes(const QDomElement &domElement) Q_DECL_OVERRIDE;
    virtual void SaveOptions(QDomElement &tag, QSharedPointer<VGObject> &obj) Q_DECL_OVERRIDE;
    virtual void contextMenuEvent ( QGraphicsSceneContextMenuEvent * event ) Q_DECL_OVERRIDE;

private slots:
    void DoChangePosition(quint32 id, qreal mx, qreal my);

private:
    Q_DISABLE_COPY(VToolRotation)
    quint32 origPointId;
    QString angle;
    QString suffix;

    QVector<quint32> source;
    QVector<DestinationItem> destination;

    QMap<quint32, VAbstractSimple *> rObjects;

    VToolRotation(VAbstractPattern *doc, VContainer *data, quint32 id, quint32 origPointId,
                  const QString &angle, const QString &suffix, const QVector<quint32> &source,
                  const QVector<DestinationItem> &destination, const Source &typeCreation,
                  QGraphicsItem *parent = nullptr);

    void UpdateNamePosition(quint32 id);

    static DestinationItem CreatePoint(quint32 idTool, quint32 idItem, const QPointF &origin, qreal angle,
                                       const QString &suffix, VContainer *data);

    template <class Item>
    static DestinationItem CreateItem(quint32 idTool, quint32 idItem, const QPointF &origin, qreal angle,
                                                   const QString &suffix, VContainer *data);
    static DestinationItem CreateArc(quint32 idTool, quint32 idItem, const QPointF &origin, qreal angle,
                                     const QString &suffix, VContainer *data);
    template <class Item>
    static DestinationItem CreateCurve(quint32 idTool, quint32 idItem, const QPointF &origin, qreal angle,
                                       const QString &suffix, VContainer *data);
    template <class Item>
    static DestinationItem CreateCurveWithSegments(quint32 idTool, quint32 idItem, const QPointF &origin, qreal angle,
                                                   const QString &suffix, VContainer *data);

    static void UpdatePoint(quint32 idTool, quint32 idItem, const QPointF &origin, qreal angle,
                            const QString &suffix, VContainer *data, quint32 id, qreal mx, qreal my);
    template <class Item>
    static void UpdateItem(quint32 idTool, quint32 idItem, const QPointF &origin, qreal angle,
                           const QString &suffix, VContainer *data, quint32 id);
    static void UpdateArc(quint32 idTool, quint32 idItem, const QPointF &origin, qreal angle,
                          const QString &suffix, VContainer *data, quint32 id);
    template <class Item>
    static void UpdateCurve(quint32 idTool, quint32 idItem, const QPointF &origin, qreal angle,
                            const QString &suffix, VContainer *data, quint32 id);
    template <class Item>
    static void UpdateCurveWithSegments(quint32 idTool, quint32 idItem, const QPointF &origin, qreal angle,
                                        const QString &suffix, VContainer *data, quint32 id);

    template <typename T>
    void ShowToolVisualization(bool show);

    void ChangePosition(QGraphicsItem *item, quint32 id, const QPointF &pos);

    VSimpleCurve *InitCurve(quint32 id, VContainer *data, GOType curveType);

    template <typename T>
    static void InitRotationToolConnections(VMainGraphicsScene *scene, T *tool);

    void AllowCurveHover(bool enabled, GOType type);
    void AllowCurveSelecting(bool enabled, GOType type);

    static void AddSourceObject(VAbstractPattern *doc, QDomElement &domElement, quint32 objId);
};

#endif // VTOOLROTATION_H
