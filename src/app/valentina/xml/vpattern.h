/************************************************************************
 **
 **  @file   vpattern.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   24 2, 2014
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentina project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2013-2015 Valentina project
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

#ifndef VPATTERN_H
#define VPATTERN_H

#include "../ifc/xml/vabstractpattern.h"
#include "../ifc/xml/vtoolrecord.h"
#include "../vpatterndb/vcontainer.h"
#include "../ifc/xml/vpatternconverter.h"

class VMainGraphicsScene;
class VNodeDetail;
struct VToolSinglePointInitData;
struct VToolLinePointInitData;
class VToolSeamAllowance;

/**
 * @brief The VPattern class working with pattern file.
 */
class VPattern : public VAbstractPattern
{
    Q_OBJECT
public:
    VPattern(VContainer *data, VMainGraphicsScene *sceneDraw, VMainGraphicsScene *sceneDetail,
             QObject *parent = nullptr);

    virtual void   CreateEmptyFile() override;

    void           Parse(const Document &parse);

    void           GarbageCollector(bool commit = false);

    void           setCurrentData();
    virtual void   UpdateToolData(const quint32 &id, VContainer *data) override;
    VContainer     GetCompleteData() const;

    virtual void   IncrementReferens(quint32 id) const override;
    virtual void   DecrementReferens(quint32 id) const override;

    quint32        SPointActiveDraw();

    QVector<quint32> GetActivePPPieces() const;

    virtual bool   SaveDocument(const QString &fileName, QString &error) override;

    QRectF         ActiveDrawBoundingRect() const;

    void AddEmptyIncrement(const QString &name, IncrementType type = IncrementType::Increment);
    void AddEmptyPreviewCalculation(const QString &name, IncrementType type = IncrementType::Increment);

    void AddEmptyIncrementAfter(const QString &after, const QString &name,
                                IncrementType type = IncrementType::Increment);
    void AddEmptyPreviewCalculationAfter(const QString &after, const QString &name,
                                         IncrementType type = IncrementType::Increment);

    void RemoveIncrement(const QString &name);
    void RemovePreviewCalculation(const QString &name);

    void MoveUpIncrement(const QString &name);
    void MoveUpPreviewCalculation(const QString &name);

    void MoveDownIncrement(const QString &name);
    void MoveDownPreviewCalculation(const QString &name);

    void SetIncrementName(const QString &name, const QString &text);
    void SetIncrementFormula(const QString &name, const QString &text);
    void SetIncrementDescription(const QString &name, const QString &text);

    void ReplaceNameInFormula(QVector<VFormulaField> &expressions, const QString &name, const QString &newName);

    virtual QString GenerateLabel(const LabelType &type, const QString &reservedName = QString())const override;
    virtual QString GenerateSuffix() const override;

    bool IsDefCustom() const;
    void SetDefCustom(bool value);

    int  GetDefCustomHeight() const;
    void SetDefCustomHeight(int value);

    int  GetDefCustomSize() const;
    void SetDefCustomSize(int value);

    bool IsReadOnly() const;
    void SetReadOnly(bool rOnly);

    QString GetLabelPrefix() const;
    void    SetLabelPrefix(const QString &prefix);

    void LiteParseIncrements();

    static const QString AttrReadOnly;
    static const QString AttrLabelPrefix;

    int ElementsToParse() const;

signals:
    void PreParseState();

public slots:
    virtual void LiteParseTree(const Document &parse) override;

protected:
    virtual void   customEvent(QEvent * event) override;

private slots:
    void RefreshPieceGeometry();

private:
    Q_DISABLE_COPY(VPattern)

    /** @brief data container with data. */
    VContainer     *data;

    VMainGraphicsScene *sceneDraw;
    VMainGraphicsScene *sceneDetail;
    QVector<vidtype> updatePieces{};

    /**
     * @brief m_parsing true if parsing a pattern. Helps to stop updating piece when new parsing happend before
     * finish */
    bool m_parsing{false};

    VNodeDetail    ParseDetailNode(const QDomElement &domElement) const;

    void           ParseDrawElement(const QDomNode& node, const Document &parse);
    void           ParseDrawMode(const QDomNode& node, const Document &parse, const Draw &mode);
    void           ParseDetailElement(QDomElement &domElement, const Document &parse);
    void           ParseDetailInternals(const QDomElement &domElement, VPiece &detail) const;
    QVector<VPieceNode> ParseDetailNodes(const QDomElement &domElement, qreal width, bool closed) const;
    VPieceLabelData ParsePieceDataTag(const QDomElement &domElement, VPieceLabelData ppData) const;
    VPatternLabelData ParsePiecePatternInfo(const QDomElement &domElement, VPatternLabelData patternInfo) const;
    VGrainlineData ParsePieceGrainline(const QDomElement &domElement, VGrainlineData gGeometry) const;
    void           ParseDetails(const QDomElement &domElement, const Document &parse);

    void           ParsePointElement(VMainGraphicsScene *scene, QDomElement &domElement,
                                     const Document &parse, const QString &type);
    void           ParseLineElement(VMainGraphicsScene *scene, const QDomElement& domElement,
                                    const Document &parse);
    void           ParseSplineElement(VMainGraphicsScene *scene, QDomElement &domElement,
                                      const Document &parse, const QString& type);
    void           ParseArcElement(VMainGraphicsScene *scene, QDomElement &domElement,
                                   const Document &parse, const QString& type);
    void           ParseEllipticalArcElement(VMainGraphicsScene *scene, QDomElement &domElement, const Document &parse,
                                             const QString &type);
    void           ParseToolsElement(VMainGraphicsScene *scene, const QDomElement& domElement,
                                     const Document &parse, const QString& type);
    void           ParseOperationElement(VMainGraphicsScene *scene, QDomElement &domElement, const Document &parse,
                                         const QString& type);

    void           ParsePathElement(VMainGraphicsScene *scene, QDomElement &domElement, const Document &parse);

    void           ParseIncrementsElement(const QDomNode& node, const Document &parse);
    void           PrepareForParse(const Document &parse);
    void           ToolsCommonAttributes(const QDomElement &domElement, quint32 &id);
    void           PointsWithLineCommonAttributes(const QDomElement &domElement, VToolLinePointInitData &initData);
    void           PointsCommonAttributes(const QDomElement &domElement, VToolSinglePointInitData &initData);
    void           PointsCommonAttributes(const QDomElement &domElement, quint32 &id, qreal &mx, qreal &my);
    void           SplinesCommonAttributes(const QDomElement &domElement, quint32 &id, quint32 &idObject,
                                           quint32 &idTool);
    template <typename T>
    QRectF         ToolBoundingRect(const QRectF &rec, quint32 id) const;
    void           ParseCurrentPP();
    QString        GetLabelBase(quint32 index)const;

    void ParseToolBasePoint(VMainGraphicsScene *scene, const QDomElement &domElement, const Document &parse);
    void ParseToolEndLine(VMainGraphicsScene *scene, QDomElement &domElement, const Document &parse);
    void ParseToolAlongLine(VMainGraphicsScene *scene, QDomElement &domElement, const Document &parse);
    void ParseToolShoulderPoint(VMainGraphicsScene *scene, QDomElement &domElement, const Document &parse);
    void ParseToolNormal(VMainGraphicsScene *scene, QDomElement &domElement, const Document &parse);
    void ParseToolBisector(VMainGraphicsScene *scene, QDomElement &domElement, const Document &parse);
    void ParseToolLineIntersect(VMainGraphicsScene *scene, const QDomElement &domElement, const Document &parse);
    void ParseToolPointOfContact(VMainGraphicsScene *scene, QDomElement &domElement, const Document &parse);
    void ParseNodePoint(const QDomElement &domElement, const Document &parse);
    void ParsePinPoint(const QDomElement &domElement, const Document &parse);
    void ParsePlaceLabel(QDomElement &domElement, const Document &parse);
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
    void ParseToolPointOfIntersectionCircles(VMainGraphicsScene *scene, QDomElement &domElement, const Document &parse);
    void ParseToolPointOfIntersectionCurves(VMainGraphicsScene *scene, QDomElement &domElement, const Document &parse);
    void ParseToolPointFromCircleAndTangent(VMainGraphicsScene *scene, QDomElement &domElement,
                                            const Document &parse);
    void ParseToolPointFromArcAndTangent(VMainGraphicsScene *scene, const QDomElement &domElement,
                                         const Document &parse);
    void ParseToolTrueDarts(VMainGraphicsScene *scene, const QDomElement &domElement, const Document &parse);

    // TODO. Delete if minimal supported version is 0.2.7
    Q_STATIC_ASSERT_X(VPatternConverter::PatternMinVer < FORMAT_VERSION(0, 2, 7),
                      "Time to refactor the code.");
    void ParseOldToolSpline(VMainGraphicsScene *scene, QDomElement &domElement, const Document &parse);

    void ParseToolSpline(VMainGraphicsScene *scene, QDomElement &domElement, const Document &parse);
    void ParseToolCubicBezier(VMainGraphicsScene *scene, const QDomElement &domElement, const Document &parse);

    // TODO. Delete if minimal supported version is 0.2.7
    Q_STATIC_ASSERT_X(VPatternConverter::PatternMinVer < FORMAT_VERSION(0, 2, 7),
                      "Time to refactor the code.");
    void ParseOldToolSplinePath(VMainGraphicsScene *scene, QDomElement &domElement, const Document &parse);

    void ParseToolSplinePath(VMainGraphicsScene *scene, const QDomElement &domElement, const Document &parse);
    void ParseToolCubicBezierPath(VMainGraphicsScene *scene, const QDomElement &domElement, const Document &parse);
    void ParseNodeSpline(const QDomElement &domElement, const Document &parse);
    void ParseNodeSplinePath(const QDomElement &domElement, const Document &parse);

    void ParseToolArc(VMainGraphicsScene *scene, QDomElement &domElement, const Document &parse);
    void ParseNodeArc(const QDomElement &domElement, const Document &parse);
    void ParseToolArcWithLength(VMainGraphicsScene *scene, QDomElement &domElement, const Document &parse);

    void ParseToolEllipticalArc(VMainGraphicsScene *scene, QDomElement &domElement, const Document &parse);
    void ParseNodeEllipticalArc(const QDomElement &domElement, const Document &parse);

    void ParseToolRotation(VMainGraphicsScene *scene, QDomElement &domElement, const Document &parse);
    void ParseToolFlippingByLine(VMainGraphicsScene *scene, QDomElement &domElement, const Document &parse);
    void ParseToolFlippingByAxis(VMainGraphicsScene *scene, QDomElement &domElement, const Document &parse);
    void ParseToolMove(VMainGraphicsScene *scene, QDomElement &domElement, const Document &parse);

    qreal EvalFormula(VContainer *data, const QString &formula, bool *ok) const;

    QDomElement MakeEmptyIncrement(const QString &name, IncrementType type);
    QDomElement FindIncrement(const QString &name) const;

    void NewEmptyIncrement(const QString &type, const QString &name, IncrementType varType);
    void NewEmptyIncrementAfter(const QString &type, const QString &after, const QString &name, IncrementType varType);
    void RemoveIncrement(const QString &type, const QString &name);
    void MoveUpIncrement(const QString &type, const QString &name);
    void MoveDownIncrement(const QString &type, const QString &name);

    void SetIncrementAttribute(const QString &name, const QString &attr, const QString &text);

    QString LastDrawName() const;
    quint32 LastToolId() const;
};

#endif // VPATTERN_H
