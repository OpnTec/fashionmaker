/************************************************************************
 **
 **  @file   vpattern.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   24 2, 2014
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

#ifndef VPATTERN_H
#define VPATTERN_H

#include "../ifc/xml/vabstractpattern.h"
#include "../ifc/xml/vtoolrecord.h"
#include "../vpatterndb/vcontainer.h"

class VDataTool;
class VMainGraphicsScene;

/**
 * @brief The VPattern class working with pattern file.
 */
class VPattern : public VAbstractPattern
{
    Q_OBJECT
public:
    VPattern(VContainer *data, Draw *mode, VMainGraphicsScene *sceneDraw, VMainGraphicsScene *sceneDetail,
             QObject *parent = nullptr);

    virtual void   CreateEmptyFile() Q_DECL_OVERRIDE;

    void           Parse(const Document &parse);

    void           setCurrentData();
    virtual void   UpdateToolData(const quint32 &id, VContainer *data) Q_DECL_OVERRIDE;

    virtual void   IncrementReferens(quint32 id) const Q_DECL_OVERRIDE;
    virtual void   DecrementReferens(quint32 id) const Q_DECL_OVERRIDE;

    quint32        SPointActiveDraw();

    virtual bool   SaveDocument(const QString &fileName, QString &error) const Q_DECL_OVERRIDE;

    QRectF         ActiveDrawBoundingRect() const;

    QString        GetAuthor() const;
    void           SetAuthor(const QString &text);

    void AddEmptyIncrement(const QString &name);
    void AddEmptyIncrementAfter(const QString &after, const QString &name);
    void RemoveIncrement(const QString &name);
    void MoveUpIncrement(const QString &name);
    void MoveDownIncrement(const QString &name);

    void SetIncrementName(const QString &name, const QString &text);
    void SetIncrementFormula(const QString &name, const QString &text);
    void SetIncrementDescription(const QString &name, const QString &text);

    virtual QString GenerateLabel(const LabelType &type, const QString &reservedName = QString())const Q_DECL_OVERRIDE;

public slots:
    void           LiteParseTree(const Document &parse);

protected:
    virtual void   customEvent(QEvent * event) Q_DECL_OVERRIDE;

private:
    Q_DISABLE_COPY(VPattern)

    /** @brief data container with data. */
    VContainer     *data;

    /** @brief mode current draw mode. */
    Draw           *mode;

    VMainGraphicsScene *sceneDraw;
    VMainGraphicsScene *sceneDetail;

    void           ParseDrawElement(const QDomNode& node, const Document &parse);
    void           ParseDrawMode(const QDomNode& node, const Document &parse, const Draw &mode);
    void           ParseDetailElement(const QDomElement &domElement,
                                      const Document &parse);
    void           ParseDetails(const QDomElement &domElement, const Document &parse);
    void           ParsePointElement(VMainGraphicsScene *scene, QDomElement &domElement,
                                     const Document &parse, const QString &type);
    void           ParseLineElement(VMainGraphicsScene *scene, const QDomElement& domElement,
                                    const Document &parse);
    void           ParseSplineElement(VMainGraphicsScene *scene, const QDomElement& domElement,
                                      const Document &parse, const QString& type);
    void           ParseArcElement(VMainGraphicsScene *scene, QDomElement &domElement,
                                   const Document &parse, const QString& type);
    void           ParseToolsElement(VMainGraphicsScene *scene, const QDomElement& domElement,
                                     const Document &parse, const QString& type);
    void           ParseIncrementsElement(const QDomNode& node);
    void           PrepareForParse(const Document &parse);
    void           ToolsCommonAttributes(const QDomElement &domElement, quint32 &id);
    void           PointsCommonAttributes(const QDomElement &domElement, quint32 &id, QString &name, qreal &mx,
                                          qreal &my, QString &typeLine, QString &lineColor);
    void           PointsCommonAttributes(const QDomElement &domElement, quint32 &id, QString &name, qreal &mx,
                                          qreal &my);
    void           PointsCommonAttributes(const QDomElement &domElement, quint32 &id, qreal &mx, qreal &my);
    void           SplinesCommonAttributes(const QDomElement &domElement, quint32 &id, quint32 &idObject,
                                           quint32 &idTool);
    template <typename T>
    QRectF ToolBoundingRect(const QRectF &rec, const quint32 &id) const;
    void           ParseCurrentPP();
    QString        GetLabelBase(unsigned int index)const;

    void ParseToolBasePoint(VMainGraphicsScene *scene, const QDomElement &domElement, const Document &parse);
    void ParseToolEndLine(VMainGraphicsScene *scene, QDomElement &domElement, const Document &parse);
    void ParseToolAlongLine(VMainGraphicsScene *scene, QDomElement &domElement, const Document &parse);
    void ParseToolShoulderPoint(VMainGraphicsScene *scene, QDomElement &domElement, const Document &parse);
    void ParseToolNormal(VMainGraphicsScene *scene, QDomElement &domElement, const Document &parse);
    void ParseToolBisector(VMainGraphicsScene *scene, QDomElement &domElement, const Document &parse);
    void ParseToolLineIntersect(VMainGraphicsScene *scene, const QDomElement &domElement, const Document &parse);
    void ParseToolPointOfContact(VMainGraphicsScene *scene, QDomElement &domElement, const Document &parse);
    void ParseNodePoint(const QDomElement &domElement, const Document &parse);
    void ParseToolHeight(VMainGraphicsScene *scene, const QDomElement &domElement, const Document &parse);
    void ParseToolTriangle(VMainGraphicsScene *scene, const QDomElement &domElement, const Document &parse);
    void ParseToolPointOfIntersection(VMainGraphicsScene *scene, const QDomElement &domElement, const Document &parse);
    void ParseToolCutSpline(VMainGraphicsScene *scene, QDomElement &domElement, const Document &parse);
    void ParseToolCutSplinePath(VMainGraphicsScene *scene, QDomElement &domElement, const Document &parse);
    void ParseToolCutArc(VMainGraphicsScene *scene, QDomElement &domElement, const Document &parse);
    void ParseToolLineIntersectAxis(VMainGraphicsScene *scene, QDomElement &domElement, const Document &parse);
    void ParseToolCurveIntersectAxis(VMainGraphicsScene *scene, QDomElement &domElement, const Document &parse);
    void ParseToolPointOfIntersectionArcs(VMainGraphicsScene *scene, const QDomElement &domElement,
                                          const Document &parse);
    void ParseToolPointOfIntersectionCircles(VMainGraphicsScene *scene, QDomElement &domElement,
                                             const Document &parse);
    void ParseToolPointFromCircleAndTangent(VMainGraphicsScene *scene, QDomElement &domElement,
                                            const Document &parse);
    void ParseToolPointFromArcAndTangent(VMainGraphicsScene *scene, const QDomElement &domElement,
                                         const Document &parse);
    void ParseToolTrueDarts(VMainGraphicsScene *scene, const QDomElement &domElement, const Document &parse);

    void ParseToolSpline(VMainGraphicsScene *scene, const QDomElement &domElement, const Document &parse);
    void ParseToolSplinePath(VMainGraphicsScene *scene, const QDomElement &domElement, const Document &parse);
    void ParseNodeSpline(const QDomElement &domElement, const Document &parse);
    void ParseNodeSplinePath(const QDomElement &domElement, const Document &parse);

    void ParseToolArc(VMainGraphicsScene *scene, QDomElement &domElement, const Document &parse);
    void ParseNodeArc(const QDomElement &domElement, const Document &parse);
    void ParseToolArcWithLength(VMainGraphicsScene *scene, QDomElement &domElement, const Document &parse);

    qreal EvalFormula(VContainer *data, const QString &formula, bool *ok) const;

    QDomElement MakeEmptyIncrement(const QString &name);
    QDomElement FindIncrement(const QString &name) const;
};

#endif // VPATTERN_H
