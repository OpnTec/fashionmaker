/************************************************************************
 **
 **  @file   vtooloptionspropertybrowser.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   22 8, 2014
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

#ifndef VTOOLOPTIONSPROPERTYBROWSER_H
#define VTOOLOPTIONSPROPERTYBROWSER_H

#include <QObject>
#include <QMap>

#include "../vpropertyexplorer/vproperty.h"
#include "../vpropertyexplorer/vpropertymodel.h"
#include "../vpropertyexplorer/vpropertyformview.h"

class QDockWidget;
class QGraphicsItem;
class QScrollArea;
class VFormula;

class VToolOptionsPropertyBrowser : public QObject
{
    Q_OBJECT
public:
    explicit VToolOptionsPropertyBrowser(QDockWidget *parent);
    void ClearPropertyBrowser();
public slots:
    void itemClicked(QGraphicsItem *item);
    void UpdateOptions();
    void RefreshOptions();
private slots:
    void userChangedData(VPE::VProperty* property);
private:
    Q_DISABLE_COPY(VToolOptionsPropertyBrowser)

    VPE::VPropertyModel* PropertyModel;
    VPE::VPropertyFormView* formView;

    QGraphicsItem *currentItem;
    QMap<VPE::VProperty *, QString> propertyToId;
    QMap<QString, VPE::VProperty *> idToProperty;

    void AddProperty(VPE::VProperty *property, const QString &id);
    void ShowItemOptions(QGraphicsItem *item);

    template<class Tool>
    void SetPointName(const QString &name);

    template<class Tool>
    void SetPointName1(const QString &name);

    template<class Tool>
    void SetPointName2(const QString &name);

    template<class Tool>
    void SetOperationSuffix(const QString &suffix);

    template<class Type>
    Type GetCrossPoint(const QVariant &value);

    template<class Tool>
    void SetCrossCirclesPoint(const QVariant &value);

    template<class Tool>
    void SetVCrossCurvesPoint(const QVariant &value);

    template<class Tool>
    void SetHCrossCurvesPoint(const QVariant &value);

    template<class Tool>
    void SetAxisType(const QVariant &value);

    template<class Tool>
    void AddPropertyObjectName(Tool *i, const QString &propertyName, bool readOnly = false);

    template<class Tool>
    void AddPropertyPointName1(Tool *i, const QString &propertyName);

    template<class Tool>
    void AddPropertyPointName2(Tool *i, const QString &propertyName);

    template<class Tool>
    void AddPropertyOperationSuffix(Tool *i, const QString &propertyName, bool readOnly = false);

    template<class Tool>
    void AddPropertyCrossPoint(Tool *i, const QString &propertyName);

    template<class Tool>
    void AddPropertyVCrossPoint(Tool *i, const QString &propertyName);

    template<class Tool>
    void AddPropertyHCrossPoint(Tool *i, const QString &propertyName);

    template<class Tool>
    void AddPropertyAxisType(Tool *i, const QString &propertyName);

    template<class Tool>
    void AddPropertyLineType(Tool *i, const QString &propertyName, const QMap<QString, QIcon> &styles);

    template<class Tool>
    void AddPropertyLineColor(Tool *i, const QString &propertyName, const QMap<QString, QString> &colors,
                              const QString &id);

    void AddPropertyFormula(const QString &propertyName, const VFormula &formula, const QString &attrName);

    QStringList PropertiesList() const;

    void ChangeDataToolSinglePoint(VPE::VProperty *property);
    void ChangeDataToolEndLine(VPE::VProperty *property);
    void ChangeDataToolAlongLine(VPE::VProperty *property);
    void ChangeDataToolArc(VPE::VProperty *property);
    void ChangeDataToolArcWithLength(VPE::VProperty *property);
    void ChangeDataToolBisector(VPE::VProperty *property);
    void ChangeDataToolTrueDarts(VPE::VProperty *property);
    void ChangeDataToolCutArc(VPE::VProperty *property);
    void ChangeDataToolCutSpline(VPE::VProperty *property);
    void ChangeDataToolCutSplinePath(VPE::VProperty *property);
    void ChangeDataToolHeight(VPE::VProperty *property);
    void ChangeDataToolLine(VPE::VProperty *property);
    void ChangeDataToolLineIntersect(VPE::VProperty *property);
    void ChangeDataToolNormal(VPE::VProperty *property);
    void ChangeDataToolPointOfContact(VPE::VProperty *property);
    void ChangeDataToolPointOfIntersection(VPE::VProperty *property);
    void ChangeDataToolPointOfIntersectionArcs(VPE::VProperty *property);
    void ChangeDataToolPointOfIntersectionCircles(VPE::VProperty *property);
    void ChangeDataToolPointOfIntersectionCurves(VPE::VProperty *property);
    void ChangeDataToolPointFromCircleAndTangent(VPE::VProperty *property);
    void ChangeDataToolPointFromArcAndTangent(VPE::VProperty *property);
    void ChangeDataToolShoulderPoint(VPE::VProperty *property);
    void ChangeDataToolSpline(VPE::VProperty *property);
    void ChangeDataToolCubicBezier(VPE::VProperty *property);
    void ChangeDataToolSplinePath(VPE::VProperty *property);
    void ChangeDataToolCubicBezierPath(VPE::VProperty *property);
    void ChangeDataToolTriangle(VPE::VProperty *property);
    void ChangeDataToolLineIntersectAxis(VPE::VProperty *property);
    void ChangeDataToolCurveIntersectAxis(VPE::VProperty *property);
    void ChangeDataToolRotation(VPE::VProperty *property);
    void ChangeDataToolMove(VPE::VProperty *property);
    void ChangeDataToolFlippingByLine(VPE::VProperty *property);
    void ChangeDataToolFlippingByAxis(VPE::VProperty *property);
    void ChangeDataToolEllipticalArc(VPE::VProperty *property);

    void ShowOptionsToolSinglePoint(QGraphicsItem *item);
    void ShowOptionsToolEndLine(QGraphicsItem *item);
    void ShowOptionsToolAlongLine(QGraphicsItem *item);
    void ShowOptionsToolArc(QGraphicsItem *item);
    void ShowOptionsToolArcWithLength(QGraphicsItem *item);
    void ShowOptionsToolBisector(QGraphicsItem *item);
    void ShowOptionsToolTrueDarts(QGraphicsItem *item);
    void ShowOptionsToolCutArc(QGraphicsItem *item);
    void ShowOptionsToolCutSpline(QGraphicsItem *item);
    void ShowOptionsToolCutSplinePath(QGraphicsItem *item);
    void ShowOptionsToolHeight(QGraphicsItem *item);
    void ShowOptionsToolLine(QGraphicsItem *item);
    void ShowOptionsToolLineIntersect(QGraphicsItem *item);
    void ShowOptionsToolNormal(QGraphicsItem *item);
    void ShowOptionsToolPointOfContact(QGraphicsItem *item);
    void ShowOptionsToolPointOfIntersection(QGraphicsItem *item);
    void ShowOptionsToolPointOfIntersectionArcs(QGraphicsItem *item);
    void ShowOptionsToolPointOfIntersectionCircles(QGraphicsItem *item);
    void ShowOptionsToolPointOfIntersectionCurves(QGraphicsItem *item);
    void ShowOptionsToolPointFromCircleAndTangent(QGraphicsItem *item);
    void ShowOptionsToolPointFromArcAndTangent(QGraphicsItem *item);
    void ShowOptionsToolShoulderPoint(QGraphicsItem *item);
    void ShowOptionsToolSpline(QGraphicsItem *item);
    void ShowOptionsToolCubicBezier(QGraphicsItem *item);
    void ShowOptionsToolSplinePath(QGraphicsItem *item);
    void ShowOptionsToolCubicBezierPath(QGraphicsItem *item);
    void ShowOptionsToolTriangle(QGraphicsItem *item);
    void ShowOptionsToolLineIntersectAxis(QGraphicsItem *item);
    void ShowOptionsToolCurveIntersectAxis(QGraphicsItem *item);
    void ShowOptionsToolRotation(QGraphicsItem *item);
    void ShowOptionsToolMove(QGraphicsItem *item);
    void ShowOptionsToolFlippingByLine(QGraphicsItem *item);
    void ShowOptionsToolFlippingByAxis(QGraphicsItem *item);
    void ShowOptionsToolEllipticalArc(QGraphicsItem *item);

    void UpdateOptionsToolSinglePoint();
    void UpdateOptionsToolEndLine();
    void UpdateOptionsToolAlongLine();
    void UpdateOptionsToolArc();
    void UpdateOptionsToolArcWithLength();
    void UpdateOptionsToolBisector();
    void UpdateOptionsToolTrueDarts();
    void UpdateOptionsToolCutArc();
    void UpdateOptionsToolCutSpline();
    void UpdateOptionsToolCutSplinePath();
    void UpdateOptionsToolHeight();
    void UpdateOptionsToolLine();
    void UpdateOptionsToolLineIntersect();
    void UpdateOptionsToolNormal();
    void UpdateOptionsToolPointOfContact();
    void UpdateOptionsToolPointOfIntersection();
    void UpdateOptionsToolPointOfIntersectionArcs();
    void UpdateOptionsToolPointOfIntersectionCircles();
    void UpdateOptionsToolPointOfIntersectionCurves();
    void UpdateOptionsToolPointFromCircleAndTangent();
    void UpdateOptionsToolPointFromArcAndTangent();
    void UpdateOptionsToolShoulderPoint();
    void UpdateOptionsToolSpline();
    void UpdateOptionsToolCubicBezier();
    void UpdateOptionsToolSplinePath();
    void UpdateOptionsToolCubicBezierPath();
    void UpdateOptionsToolTriangle();
    void UpdateOptionsToolLineIntersectAxis();
    void UpdateOptionsToolCurveIntersectAxis();
    void UpdateOptionsToolRotation();
    void UpdateOptionsToolMove();
    void UpdateOptionsToolFlippingByLine();
    void UpdateOptionsToolFlippingByAxis();
    void UpdateOptionsToolEllipticalArc();
};

#endif // VTOOLOPTIONSPROPERTYBROWSER_H
