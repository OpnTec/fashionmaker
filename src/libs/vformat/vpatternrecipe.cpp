/************************************************************************
 **
 **  @file   vpatternrecipe.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   8 7, 2019
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentina project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2019 Valentina project
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

#include "vpatternrecipe.h"
#include "../vmisc/projectversion.h"
#include "../vmisc/vabstractapplication.h"
#include "../ifc/xml/vabstractpattern.h"
#include "../ifc/exception/vexceptioninvalidhistory.h"
#include "../vpatterndb/vcontainer.h"
#include "../vgeometry/vpointf.h"
#include "../vgeometry/vcubicbezier.h"
#include "../vgeometry/vsplinepath.h"
#include "../vgeometry/vcubicbezierpath.h"
#include "../vtools/tools/drawTools/drawtools.h"

namespace
{
#define TagStep QStringLiteral("step")

#define AttrLabel QStringLiteral("label")
#define AttrLengthValue QStringLiteral("lengthValue")
#define AttrAngleValue QStringLiteral("angleValue")
#define AttrAngle1Value QStringLiteral("angle1Value")
#define AttrAngle2Value QStringLiteral("angle2Value")
#define AttrLength1Value QStringLiteral("length1Value")
#define AttrLength2Value QStringLiteral("length2Value")
#define AttrRadiusValue QStringLiteral("radiusValue")
#define AttrC1RadiusValue QStringLiteral("c1RadiusValue")
#define AttrC2RadiusValue QStringLiteral("c2RadiusValue")
#define AttrCRadiusValue QStringLiteral("cRadiusValue")
#define AttrRadius1Value QStringLiteral("radius1Value")
#define AttrRadius2Value QStringLiteral("radius2Value")
#define AttrRotationAngleValue QStringLiteral("rotationAngleValue")

//---------------------------------------------------------------------------------------------------------------------
inline QString FileComment()
{
    return QStringLiteral("Recipe created with Valentina v%1 (https://valentinaproject.bitbucket.io/).")
            .arg(APP_VERSION_STR);
}

//---------------------------------------------------------------------------------------------------------------------
template <typename T>
T *GetPatternTool(quint32 id)
{
    T* tool = qobject_cast<T*>(VAbstractPattern::getTool(id));
    if (not tool)
    {
        throw VExceptionInvalidHistory(QObject::tr("Cannot cast tool with id '%1'.").arg(id));
    }
    return tool;
}
}  // namespace

//---------------------------------------------------------------------------------------------------------------------
VPatternRecipe::VPatternRecipe(VContainer *data, VAbstractPattern *pattern, QObject *parent)
    : VDomDocument(parent),
      m_data(data),
      m_pattern(pattern)
{
    SCASSERT(data != nullptr)
    SCASSERT(pattern != nullptr)

    QDomElement recipeElement = createElement(QStringLiteral("recipe"));
    recipeElement.appendChild(createComment(FileComment()));
    SetAttribute(recipeElement, QStringLiteral("version"), QStringLiteral("0.1.0"));

    recipeElement.appendChild(Prerequisite());
    recipeElement.appendChild(Content());

    appendChild(recipeElement);
    insertBefore(createProcessingInstruction(QStringLiteral("xml"),
                                             QStringLiteral("version=\"1.0\" encoding=\"UTF-8\"")), firstChild());
}

//---------------------------------------------------------------------------------------------------------------------
QDomElement VPatternRecipe::Prerequisite()
{
    /*
     <prerequisite>
        <measurements>
            <m description="" full_name="Обхват талии" name="@От" value="65"/>
        </measurements>
        <increments>
            <increment description="" formula="height/2-15" name="#L_C"/>
        </increments>
        <previewCalculations>
            <increment description="" formula="height/2-15" name="#L_C"/>
        </previewCalculations>
    </prerequisite>
     */
    QDomElement prerequisiteElement = createElement(QStringLiteral("prerequisite"));

    prerequisiteElement.appendChild(CreateElementWithText(QStringLiteral("valentina"), APP_VERSION_STR));
    prerequisiteElement.appendChild(CreateElementWithText(QStringLiteral("unit"), UnitsToStr(qApp->patternUnit())));
    prerequisiteElement.appendChild(CreateElementWithText(QStringLiteral("description"), m_pattern->GetDescription()));
    prerequisiteElement.appendChild(CreateElementWithText(QStringLiteral("notes"), m_pattern->GetNotes()));
    prerequisiteElement.appendChild(Measurements());
    prerequisiteElement.appendChild(Increments());
    prerequisiteElement.appendChild(PreviewCalculations());

    return prerequisiteElement;
}

//---------------------------------------------------------------------------------------------------------------------
QDomElement VPatternRecipe::Measurements()
{
    QDomElement measurements = createElement(QStringLiteral("measurements"));

    QList<QSharedPointer<VMeasurement>> patternMeasurements = m_data->DataMeasurements().values();

    // Resore order
    std::sort(patternMeasurements.begin(), patternMeasurements.end(),
              [](const QSharedPointer<VMeasurement> &a, const QSharedPointer<VMeasurement> &b)
    {return a->Index() > b->Index();});

    for(auto &m : patternMeasurements)
    {
        measurements.appendChild(Measurement(m));
    }

    return measurements;
}

//---------------------------------------------------------------------------------------------------------------------
QDomElement VPatternRecipe::Measurement(const QSharedPointer<VMeasurement> &m)
{
    /*
     * <measurements>
     *  <m description="" full_name="Обхват талии" name="@От" value="65"/>
     * </measurements>
     */
    QDomElement measurement = createElement(QStringLiteral("measurement"));

    SetAttribute(measurement, QStringLiteral("description"), m->GetDescription());
    SetAttribute(measurement, QStringLiteral("fullName"), m->GetGuiText());
    SetAttribute(measurement, QStringLiteral("name"), m->GetName());
    SetAttribute(measurement, QStringLiteral("formula"), m->GetFormula()); // TODO: localize
    SetAttribute(measurement, QStringLiteral("value"), *m->GetValue());

    return measurement;
}

//---------------------------------------------------------------------------------------------------------------------
QDomElement VPatternRecipe::Increments()
{
    QDomElement increments = createElement(QStringLiteral("increments"));

    QList<QSharedPointer<VIncrement>> patternIncrements = m_data->DataIncrements().values();

    // Resore order
    std::sort(patternIncrements.begin(), patternIncrements.end(),
              [](const QSharedPointer<VIncrement> &a, const QSharedPointer<VIncrement> &b)
    {return a->GetIndex() > b->GetIndex();});

    for(auto &incr : patternIncrements)
    {
        if (not incr->IsPreviewCalculation())
        {
            increments.appendChild(Increment(incr));
        }
    }

    return increments;
}

//---------------------------------------------------------------------------------------------------------------------
QDomElement VPatternRecipe::PreviewCalculations()
{
    QDomElement previewCalculations = createElement(QStringLiteral("previewCalculations"));

    QList<QSharedPointer<VIncrement>> patternIncrements = m_data->DataIncrements().values();

    // Resore order
    std::sort(patternIncrements.begin(), patternIncrements.end(),
              [](const QSharedPointer<VIncrement> &a, const QSharedPointer<VIncrement> &b)
    {return a->GetIndex() > b->GetIndex();});

    for(auto &incr : patternIncrements)
    {
        if (incr->IsPreviewCalculation())
        {
            previewCalculations.appendChild(Increment(incr));
        }
    }

    return previewCalculations;
}

//---------------------------------------------------------------------------------------------------------------------
QDomElement VPatternRecipe::Increment(const QSharedPointer<VIncrement> &incr)
{
    QDomElement measurement = createElement(QStringLiteral("increment"));

    SetAttribute(measurement, QStringLiteral("description"), incr->GetDescription());
    SetAttribute(measurement, QStringLiteral("name"), incr->GetName());

    if (incr->GetType() != VarType::IncrementSeparator)
    {
        SetAttribute(measurement, QStringLiteral("formula"), incr->GetFormula()); // TODO: localize
        SetAttribute(measurement, QStringLiteral("value"), *incr->GetValue());
    }
    else
    {
        SetAttribute(measurement, QStringLiteral("separator"), true);
    }

    return measurement;
}

//---------------------------------------------------------------------------------------------------------------------
QDomElement VPatternRecipe::Content()
{
    QDomElement content = createElement(QStringLiteral("content"));

    const QDomNodeList draws = m_pattern->documentElement().elementsByTagName(VAbstractPattern::TagDraw);
    for (int i=0; i < draws.size(); ++i)
    {
        QDomElement draw = draws.at(i).toElement();
        if (draw.isNull())
        {
            throw VExceptionInvalidHistory(tr("Invalid tag %1").arg(VAbstractPattern::TagDraw));
        }

        content.appendChild(Draft(draw));
    }

    return content;
}

//---------------------------------------------------------------------------------------------------------------------
QDomElement VPatternRecipe::Draft(const QDomElement &draft)
{
    QDomElement recipeDraft = createElement(QStringLiteral("draft"));

    const QString draftName = draft.attribute(QStringLiteral("name"));
    SetAttribute(recipeDraft, QStringLiteral("name"), draftName);

    QVector<VToolRecord> *history = m_pattern->getHistory();
    for (auto &record : *history)
    {
        if (record.getNameDraw() == draftName)
        {
            QDomElement step = Step(record);
            if (not step.isNull())
            {
                recipeDraft.appendChild(step);
            }
        }
    }

    return recipeDraft;
}

//---------------------------------------------------------------------------------------------------------------------
QDomElement VPatternRecipe::Step(const VToolRecord &tool)
{
    // This check helps to find missed tools in the switch
    Q_STATIC_ASSERT_X(static_cast<int>(Tool::LAST_ONE_DO_NOT_USE) == 55, "Not all tools were used in history.");

    const QDomElement domElem = m_pattern->elementById(tool.getId());
    if (not domElem.isElement())
    {
        throw VExceptionInvalidHistory(tr("Can't find element by id '%1'").arg(tool.getId()));
    }
    try
    {
QT_WARNING_PUSH
QT_WARNING_DISABLE_GCC("-Wswitch-default")
        switch (tool.getTypeTool())
        {
            case Tool::Arrow:
            case Tool::SinglePoint:
            case Tool::DoublePoint:
            case Tool::LinePoint:
            case Tool::AbstractSpline:
            case Tool::Cut:
            case Tool::Midpoint:// Same as Tool::AlongLine, but tool will never has such type
            case Tool::ArcIntersectAxis:// Same as Tool::CurveIntersectAxis, but tool will never has such type
            case Tool::LAST_ONE_DO_NOT_USE:
                Q_UNREACHABLE(); //-V501
                break;
            case Tool::BasePoint:
                return BasePoint(tool);
            case Tool::EndLine:
                return EndLine(tool);
            case Tool::Line:
                return Line(tool);
            case Tool::AlongLine:
                return AlongLine(tool);
            case Tool::ShoulderPoint:
                return ShoulderPoint(tool);
            case Tool::Normal:
                return Normal(tool);
            case Tool::Bisector:
                return Bisector(tool);
            case Tool::LineIntersect:
                return LineIntersect(tool);
            case Tool::Spline:
                return Spline(tool);
            case Tool::CubicBezier:
                return CubicBezier(tool);
            case Tool::Arc:
                return Arc(tool);
            case Tool::ArcWithLength:
                return ArcWithLength(tool);
            case Tool::SplinePath:
                return SplinePath(tool);
            case Tool::CubicBezierPath:
                return CubicBezierPath(tool);
            case Tool::PointOfContact:
                return PointOfContact(tool);
            case Tool::Height:
                return Height(tool);
            case Tool::Triangle:
                return Triangle(tool);
            case Tool::PointOfIntersection:
                return PointOfIntersection(tool);
            case Tool::CutArc:
                return CutArc(tool);
            case Tool::CutSpline:
                return CutSpline(tool);
            case Tool::CutSplinePath:
                return CutSplinePath(tool);
            case Tool::LineIntersectAxis:
                return LineIntersectAxis(tool);
            case Tool::CurveIntersectAxis:
                return CurveIntersectAxis(tool);
            case Tool::PointOfIntersectionArcs:
                return PointOfIntersectionArcs(tool);
            case Tool::PointOfIntersectionCircles:
                return PointOfIntersectionCircles(tool);
            case Tool::PointOfIntersectionCurves:
                return PointOfIntersectionCurves(tool);
            case Tool::PointFromCircleAndTangent:
                return PointFromCircleAndTangent(tool);
            case Tool::PointFromArcAndTangent:
                return PointFromArcAndTangent(tool);
            case Tool::TrueDarts:
                return TrueDarts(tool);
            case Tool::EllipticalArc:
                return EllipticalArc(tool);
            case Tool::Rotation:
                return Rotation(tool);
            case Tool::FlippingByLine:
                return FlippingByLine(tool);
            case Tool::FlippingByAxis:
                return FlippingByAxis(tool);
            case Tool::Move:
                return Move(tool);
            //Because "history" not only show history of pattern, but help restore current data for each pattern's
            //piece, we need add record about details and nodes, but don't show them.
            case Tool::Piece:
            case Tool::UnionDetails:
            case Tool::NodeArc:
            case Tool::NodeElArc:
            case Tool::NodePoint:
            case Tool::NodeSpline:
            case Tool::NodeSplinePath:
            case Tool::Group:
            case Tool::PiecePath:
            case Tool::Pin:
            case Tool::PlaceLabel:
            case Tool::InsertNode:
            case Tool::DuplicateDetail:
                return QDomElement();
        }
QT_WARNING_POP
    }
    catch (const VExceptionBadId &e)
    {
        throw VExceptionInvalidHistory(e.ErrorMessage());
    }

    throw VExceptionInvalidHistory(tr("Can't create history record for the tool."));
}

//---------------------------------------------------------------------------------------------------------------------
QDomElement VPatternRecipe::BasePoint(const VToolRecord &record)
{
    auto *tool = GetPatternTool<VToolBasePoint>(record.getId());

    QDomElement step = createElement(TagStep);
    ToolAttributes(step, tool);
    return step;
}

//---------------------------------------------------------------------------------------------------------------------
QDomElement VPatternRecipe::EndLine(const VToolRecord &record)
{
    auto *tool = GetPatternTool<VToolEndLine>(record.getId());

    QDomElement step = createElement(TagStep);

    ToolAttributes(step, tool);
    SetAttribute(step, AttrBasePoint, tool->BasePointName());
    Formula(step, tool->GetFormulaLength(), AttrLength, AttrLengthValue);
    Formula(step, tool->GetFormulaAngle(), AttrAngle, AttrAngleValue);
    LineAttributes(step, tool);

    return step;
}

//---------------------------------------------------------------------------------------------------------------------
QDomElement VPatternRecipe::Line(const VToolRecord &record)
{
    auto *tool = GetPatternTool<VToolLine>(record.getId());

    QDomElement step = createElement(TagStep);

    SetAttribute(step, AttrType, QStringLiteral("segment"));
    SetAttribute(step, AttrFirstPoint, tool->FirstPointName());
    SetAttribute(step, AttrSecondPoint, tool->SecondPointName());
    LineAttributes(step, tool);

    return step;
}

//---------------------------------------------------------------------------------------------------------------------
QDomElement VPatternRecipe::AlongLine(const VToolRecord &record)
{
    auto *tool = GetPatternTool<VToolAlongLine>(record.getId());

    QDomElement step = createElement(TagStep);

    ToolAttributes(step, tool);
    SetAttribute(step, AttrBasePoint, tool->BasePointName());
    SetAttribute(step, AttrSecondPoint, tool->SecondPointName());
    Formula(step, tool->GetFormulaLength(), AttrLength, AttrLengthValue);
    LineAttributes(step, tool);

    return step;
}

//---------------------------------------------------------------------------------------------------------------------
QDomElement VPatternRecipe::ShoulderPoint(const VToolRecord &record)
{
    auto *tool = GetPatternTool<VToolShoulderPoint>(record.getId());

    QDomElement step = createElement(TagStep);

    ToolAttributes(step, tool);
    SetAttribute(step, AttrP1Line, tool->BasePointName());
    SetAttribute(step, AttrP2Line, tool->SecondPointName());
    SetAttribute(step, AttrPShoulder, tool->ShoulderPointName());
    Formula(step, tool->GetFormulaLength(), AttrLength, AttrLengthValue);
    LineAttributes(step, tool);

    return step;
}

//---------------------------------------------------------------------------------------------------------------------
QDomElement VPatternRecipe::Normal(const VToolRecord &record)
{
    auto *tool = GetPatternTool<VToolNormal>(record.getId());

    QDomElement step = createElement(TagStep);

    ToolAttributes(step, tool);
    SetAttribute(step, AttrFirstPoint, tool->BasePointName());
    SetAttribute(step, AttrSecondPoint, tool->SecondPointName());
    Formula(step, tool->GetFormulaLength(), AttrLength, AttrLengthValue);
    SetAttribute(step, AttrAngle, tool->GetAngle());
    LineAttributes(step, tool);

    return step;
}

//---------------------------------------------------------------------------------------------------------------------
QDomElement VPatternRecipe::Bisector(const VToolRecord &record)
{
    auto *tool = GetPatternTool<VToolBisector>(record.getId());

    QDomElement step = createElement(TagStep);

    ToolAttributes(step, tool);
    SetAttribute(step, AttrFirstPoint, tool->FirstPointName());
    SetAttribute(step, AttrSecondPoint, tool->BasePointName());
    SetAttribute(step, AttrThirdPoint, tool->ThirdPointName());
    Formula(step, tool->GetFormulaLength(), AttrLength, AttrLengthValue);
    LineAttributes(step, tool);

    return step;
}

//---------------------------------------------------------------------------------------------------------------------
QDomElement VPatternRecipe::LineIntersect(const VToolRecord &record)
{
    auto *tool = GetPatternTool<VToolLineIntersect>(record.getId());

    QDomElement step = createElement(TagStep);

    ToolAttributes(step, tool);

    SetAttribute(step, AttrP1Line1, tool->Line1P1Name());
    SetAttribute(step, AttrP2Line1, tool->Line1P2Name());
    SetAttribute(step, AttrP1Line2, tool->Line2P1Name());
    SetAttribute(step, AttrP2Line2, tool->Line2P2Name());

    return step;
}

//---------------------------------------------------------------------------------------------------------------------
QDomElement VPatternRecipe::Spline(const VToolRecord &record)
{
    auto *tool = GetPatternTool<VToolSpline>(record.getId());
    VSpline spl = tool->getSpline();

    QDomElement step = createElement(TagStep);

    SetAttribute(step, AttrType, QStringLiteral("spline"));
    SetAttribute(step, AttrLabel, tool->name());

    SetAttribute(step, AttrPoint1, spl.GetP1().name());
    SetAttribute(step, AttrPoint4, spl.GetP4().name());

    SetAttribute(step, AttrAngle1, spl.GetStartAngleFormula());
    SetAttribute(step, AttrAngle1Value, spl.GetStartAngle());

    SetAttribute(step, AttrAngle2, spl.GetEndAngleFormula());
    SetAttribute(step, AttrAngle2Value, spl.GetEndAngle());

    SetAttribute(step, AttrLength1, spl.GetC1LengthFormula());
    SetAttribute(step, AttrLength1Value, spl.GetC1Length());

    SetAttribute(step, AttrLength2, spl.GetC2LengthFormula());
    SetAttribute(step, AttrLength2Value, spl.GetC2Length());

    CurveAttributes(step, tool);

    return step;
}

//---------------------------------------------------------------------------------------------------------------------
QDomElement VPatternRecipe::CubicBezier(const VToolRecord &record)
{
    auto *tool = GetPatternTool<VToolCubicBezier>(record.getId());
    VCubicBezier spl = tool->getSpline();

    QDomElement step = createElement(TagStep);

    ToolAttributes(step, tool);
    SetAttribute(step, AttrPoint1, spl.GetP1().name());
    SetAttribute(step, AttrPoint2, spl.GetP2().name());
    SetAttribute(step, AttrPoint3, spl.GetP3().name());
    SetAttribute(step, AttrPoint4, spl.GetP4().name());

    CurveAttributes(step, tool);

    return step;
}

//---------------------------------------------------------------------------------------------------------------------
QDomElement VPatternRecipe::Arc(const VToolRecord &record)
{
    auto *tool = GetPatternTool<VToolArc>(record.getId());

    QDomElement step = createElement(TagStep);

    SetAttribute(step, AttrType, QStringLiteral("arc"));
    SetAttribute(step, AttrLabel, tool->name());
    SetAttribute(step, AttrCenter, tool->CenterPointName());
    Formula(step, tool->GetFormulaRadius(), AttrRadius, AttrRadiusValue);
    Formula(step, tool->GetFormulaF1(), AttrAngle1, AttrAngle1Value);
    Formula(step, tool->GetFormulaF2(), AttrAngle2, AttrAngle2Value);

    CurveAttributes(step, tool);

    return step;
}

//---------------------------------------------------------------------------------------------------------------------
QDomElement VPatternRecipe::ArcWithLength(const VToolRecord &record)
{
    auto *tool = GetPatternTool<VToolArcWithLength>(record.getId());

    QDomElement step = createElement(TagStep);

    ToolAttributes(step, tool);
    SetAttribute(step, AttrCenter, tool->CenterPointName());
    Formula(step, tool->GetFormulaRadius(), AttrRadius, AttrRadiusValue);
    Formula(step, tool->GetFormulaF1(), AttrAngle1, AttrAngle1Value);
    Formula(step, tool->GetFormulaLength(), AttrLength, AttrLengthValue);

    CurveAttributes(step, tool);

    return step;
}

//---------------------------------------------------------------------------------------------------------------------
QDomElement VPatternRecipe::SplinePath(const VToolRecord &record)
{
    auto *tool = GetPatternTool<VToolSplinePath>(record.getId());
    VSplinePath spl = tool->getSplinePath();

    QDomElement step = createElement(TagStep);

    SetAttribute(step, AttrType, QStringLiteral("splinePath"));
    SetAttribute(step, AttrLabel, tool->name());

    QDomElement nodes = createElement(QStringLiteral("nodes"));
    QVector<VSplinePoint> path = spl.GetSplinePath();

    if (path.isEmpty())
    {
        throw VExceptionInvalidHistory(QObject::tr("Empty list of nodes for tool with id '%1'.").arg(record.getId()));
    }

    for (auto &pathNode : path)
    {
        QDomElement node = createElement(QStringLiteral("node"));

        SetAttribute(node, AttrPSpline, pathNode.P().name());

        SetAttribute(node, AttrAngle1, pathNode.Angle1Formula());
        SetAttribute(node, AttrAngle1Value, pathNode.Angle1());

        SetAttribute(node, AttrAngle2, pathNode.Angle2Formula());
        SetAttribute(node, AttrAngle2Value, pathNode.Angle2());

        SetAttribute(node, AttrLength1, pathNode.Length1Formula());
        SetAttribute(node, AttrLength1Value, pathNode.Length1());

        SetAttribute(node, AttrLength2, pathNode.Length2Formula());
        SetAttribute(node, AttrLength2Value, pathNode.Length2());

        nodes.appendChild(node);
    }

    step.appendChild(nodes);

    CurveAttributes(step, tool);

    return step;
}

//---------------------------------------------------------------------------------------------------------------------
QDomElement VPatternRecipe::CubicBezierPath(const VToolRecord &record)
{
    auto *tool = GetPatternTool<VToolCubicBezierPath>(record.getId());
    VCubicBezierPath spl = tool->getSplinePath();

    QDomElement step = createElement(TagStep);

    ToolAttributes(step, tool);

    QDomElement nodes = createElement(QStringLiteral("nodes"));
    QVector<VSplinePoint> path = spl.GetSplinePath();

    if (path.isEmpty())
    {
        throw VExceptionInvalidHistory(QObject::tr("Empty list of nodes for tool with id '%1'.").arg(record.getId()));
    }

    for (auto &pathNode : path)
    {
        QDomElement node = createElement(QStringLiteral("node"));

        SetAttribute(node, AttrPSpline, pathNode.P().name());

        nodes.appendChild(node);
    }

    step.appendChild(nodes);

    CurveAttributes(step, tool);

    return step;
}

//---------------------------------------------------------------------------------------------------------------------
QDomElement VPatternRecipe::PointOfContact(const VToolRecord &record)
{
    auto *tool = GetPatternTool<VToolPointOfContact>(record.getId());

    QDomElement step = createElement(TagStep);

    ToolAttributes(step, tool);
    SetAttribute(step, AttrCenter, tool->ArcCenterPointName());
    SetAttribute(step, AttrFirstPoint, tool->FirstPointName());
    SetAttribute(step, AttrSecondPoint, tool->SecondPointName());
    Formula(step, tool->getArcRadius(), AttrRadius, AttrRadiusValue);

    return step;
}

//---------------------------------------------------------------------------------------------------------------------
QDomElement VPatternRecipe::Height(const VToolRecord &record)
{
    auto *tool = GetPatternTool<VToolHeight>(record.getId());

    QDomElement step = createElement(TagStep);

    ToolAttributes(step, tool);
    SetAttribute(step, AttrBasePoint, tool->BasePointName());
    SetAttribute(step, AttrP1Line, tool->FirstLinePointName());
    SetAttribute(step, AttrP2Line, tool->SecondLinePointName());
    LineAttributes(step, tool);

    return step;
}

//---------------------------------------------------------------------------------------------------------------------
QDomElement VPatternRecipe::Triangle(const VToolRecord &record)
{
    auto *tool = GetPatternTool<VToolTriangle>(record.getId());

    QDomElement step = createElement(TagStep);

    ToolAttributes(step, tool);
    SetAttribute(step, AttrAxisP1, tool->AxisP1Name());
    SetAttribute(step, AttrAxisP2, tool->AxisP2Name());
    SetAttribute(step, AttrFirstPoint, tool->FirstPointName());
    SetAttribute(step, AttrSecondPoint, tool->SecondPointName());

    return step;
}

//---------------------------------------------------------------------------------------------------------------------
QDomElement VPatternRecipe::PointOfIntersection(const VToolRecord &record)
{
    auto *tool = GetPatternTool<VToolPointOfIntersection>(record.getId());

    QDomElement step = createElement(TagStep);

    ToolAttributes(step, tool);
    SetAttribute(step, AttrFirstPoint, tool->FirstPointName());
    SetAttribute(step, AttrSecondPoint, tool->SecondPointName());

    return step;
}

//---------------------------------------------------------------------------------------------------------------------
QDomElement VPatternRecipe::CutArc(const VToolRecord &record)
{
    auto *tool = GetPatternTool<VToolCutArc>(record.getId());

    QDomElement step = createElement(TagStep);

    ToolAttributes(step, tool);
    Formula(step, tool->GetFormula(), AttrLength, AttrLengthValue);
    SetAttribute(step, AttrArc, tool->CurveName());

    return step;
}

//---------------------------------------------------------------------------------------------------------------------
QDomElement VPatternRecipe::CutSpline(const VToolRecord &record)
{
    auto *tool = GetPatternTool<VToolCutSpline>(record.getId());

    QDomElement step = createElement(TagStep);

    ToolAttributes(step, tool);
    Formula(step, tool->GetFormula(), AttrLength, AttrLengthValue);
    SetAttribute(step, VToolCutSpline::AttrSpline, tool->CurveName());

    return step;
}

//---------------------------------------------------------------------------------------------------------------------
QDomElement VPatternRecipe::CutSplinePath(const VToolRecord &record)
{
    auto *tool = GetPatternTool<VToolCutSplinePath>(record.getId());

    QDomElement step = createElement(TagStep);

    ToolAttributes(step, tool);
    Formula(step, tool->GetFormula(), AttrLength, AttrLengthValue);
    SetAttribute(step, VToolCutSplinePath::AttrSplinePath, tool->CurveName());

    return step;
}

//---------------------------------------------------------------------------------------------------------------------
QDomElement VPatternRecipe::LineIntersectAxis(const VToolRecord &record)
{
    auto *tool = GetPatternTool<VToolLineIntersectAxis>(record.getId());

    QDomElement step = createElement(TagStep);

    ToolAttributes(step, tool);
    SetAttribute(step, AttrBasePoint, tool->BasePointName());
    SetAttribute(step, AttrP1Line, tool->FirstLinePoint());
    SetAttribute(step, AttrP2Line, tool->SecondLinePoint());
    Formula(step, tool->GetFormulaAngle(), AttrAngle, AttrAngleValue);
    LineAttributes(step, tool);

    return step;
}

//---------------------------------------------------------------------------------------------------------------------
QDomElement VPatternRecipe::CurveIntersectAxis(const VToolRecord &record)
{
    auto *tool = GetPatternTool<VToolCurveIntersectAxis>(record.getId());

    QDomElement step = createElement(TagStep);

    ToolAttributes(step, tool);
    SetAttribute(step, AttrBasePoint, tool->BasePointName());
    SetAttribute(step, AttrCurve, tool->CurveName());
    Formula(step, tool->GetFormulaAngle(), AttrAngle, AttrAngleValue);
    LineAttributes(step, tool);

    return step;
}

//---------------------------------------------------------------------------------------------------------------------
QDomElement VPatternRecipe::PointOfIntersectionArcs(const VToolRecord &record)
{
    auto *tool = GetPatternTool<VToolPointOfIntersectionArcs>(record.getId());

    QDomElement step = createElement(TagStep);

    ToolAttributes(step, tool);
    SetAttribute(step, AttrFirstArc, tool->FirstArcName());
    SetAttribute(step, AttrSecondArc, tool->FirstArcName());
    SetAttribute(step, AttrCrossPoint, static_cast<int>(tool->GetCrossCirclesPoint()));

    return step;
}

//---------------------------------------------------------------------------------------------------------------------
QDomElement VPatternRecipe::PointOfIntersectionCircles(const VToolRecord &record)
{
    auto *tool = GetPatternTool<VToolPointOfIntersectionCircles>(record.getId());

    QDomElement step = createElement(TagStep);

    ToolAttributes(step, tool);
    SetAttribute(step, AttrC1Center, tool->FirstCircleCenterPointName());
    SetAttribute(step, AttrC2Center, tool->SecondCircleCenterPointName());
    Formula(step, tool->GetFirstCircleRadius(), AttrC1Radius, AttrC1RadiusValue);
    Formula(step, tool->GetSecondCircleRadius(), AttrC2Radius, AttrC2RadiusValue);
    SetAttribute(step, AttrCrossPoint, static_cast<int>(tool->GetCrossCirclesPoint()));

    return step;
}

//---------------------------------------------------------------------------------------------------------------------
QDomElement VPatternRecipe::PointOfIntersectionCurves(const VToolRecord &record)
{
    auto *tool = GetPatternTool<VToolPointOfIntersectionCurves>(record.getId());

    QDomElement step = createElement(TagStep);

    ToolAttributes(step, tool);
    SetAttribute(step, AttrCurve1, tool->FirstCurveName());
    SetAttribute(step, AttrCurve2, tool->FirstCurveName());
    SetAttribute(step, AttrVCrossPoint, static_cast<int>(tool->GetVCrossPoint()));
    SetAttribute(step, AttrHCrossPoint, static_cast<int>(tool->GetHCrossPoint()));

    return step;
}

//---------------------------------------------------------------------------------------------------------------------
QDomElement VPatternRecipe::PointFromCircleAndTangent(const VToolRecord &record)
{
    auto *tool = GetPatternTool<VToolPointFromCircleAndTangent>(record.getId());

    QDomElement step = createElement(TagStep);

    ToolAttributes(step, tool);
    SetAttribute(step, AttrCCenter, tool->CircleCenterPointName());
    SetAttribute(step, AttrTangent, tool->TangentPointName());
    Formula(step, tool->GetCircleRadius(), AttrCRadius, AttrCRadiusValue);
    SetAttribute(step, AttrCrossPoint, static_cast<int>(tool->GetCrossCirclesPoint()));

    return step;
}

//---------------------------------------------------------------------------------------------------------------------
QDomElement VPatternRecipe::PointFromArcAndTangent(const VToolRecord &record)
{
    auto *tool = GetPatternTool<VToolPointFromArcAndTangent>(record.getId());

    QDomElement step = createElement(TagStep);

    ToolAttributes(step, tool);
    SetAttribute(step, AttrArc, tool->ArcName());
    SetAttribute(step, AttrTangent, tool->TangentPointName());
    SetAttribute(step, AttrCrossPoint, static_cast<int>(tool->GetCrossCirclesPoint()));

    return step;
}

//---------------------------------------------------------------------------------------------------------------------
QDomElement VPatternRecipe::TrueDarts(const VToolRecord &record)
{
    auto *tool = GetPatternTool<VToolTrueDarts>(record.getId());

    QDomElement step = createElement(TagStep);

    SetAttribute(step, AttrType, VToolTrueDarts::ToolType);
    SetAttribute(step, AttrPoint1, tool->nameP1());
    SetAttribute(step, AttrPoint2, tool->nameP2());
    SetAttribute(step, AttrBaseLineP1, tool->BaseLineP1Name());
    SetAttribute(step, AttrBaseLineP2, tool->BaseLineP2Name());
    SetAttribute(step, AttrDartP1, tool->DartP1Name());
    SetAttribute(step, AttrDartP2, tool->DartP2Name());
    SetAttribute(step, AttrDartP3, tool->DartP3Name());

    return step;
}

//---------------------------------------------------------------------------------------------------------------------
QDomElement VPatternRecipe::EllipticalArc(const VToolRecord &record)
{
    auto *tool = GetPatternTool<VToolEllipticalArc>(record.getId());

    QDomElement step = createElement(TagStep);

    SetAttribute(step, AttrType, QStringLiteral("ellipticalArc"));
    SetAttribute(step, AttrLabel, tool->name());

    SetAttribute(step, AttrCenter, tool->CenterPointName());
    Formula(step, tool->GetFormulaRadius1(), AttrRadius1, AttrRadius1Value);
    Formula(step, tool->GetFormulaRadius2(), AttrRadius2, AttrRadius2Value);
    Formula(step, tool->GetFormulaF1(), AttrAngle1, AttrAngle1Value);
    Formula(step, tool->GetFormulaF2(), AttrAngle2, AttrAngle2Value);
    Formula(step, tool->GetFormulaRotationAngle(), AttrRotationAngle, AttrRotationAngleValue);

    CurveAttributes(step, tool);

    return step;
}

//---------------------------------------------------------------------------------------------------------------------
QDomElement VPatternRecipe::Rotation(const VToolRecord &record)
{
    auto *tool = GetPatternTool<VToolRotation>(record.getId());

    QDomElement step = createElement(TagStep);

    SetAttribute(step, AttrType, VToolRotation::ToolType);
    SetAttribute(step, AttrCenter, tool->OriginPointName());
    Formula(step, tool->GetFormulaAngle(), AttrAngle, AttrAngleValue);
    SetAttribute(step, AttrSuffix, tool->Suffix());

    step.appendChild(GroupOperationSource(tool, record.getId()));

    return step;
}

//---------------------------------------------------------------------------------------------------------------------
QDomElement VPatternRecipe::FlippingByLine(const VToolRecord &record)
{
    auto *tool = GetPatternTool<VToolFlippingByLine>(record.getId());

    QDomElement step = createElement(TagStep);

    SetAttribute(step, AttrType, VToolFlippingByLine::ToolType);
    SetAttribute(step, AttrP1Line, tool->FirstLinePointName());
    SetAttribute(step, AttrP2Line, tool->SecondLinePointName());
    SetAttribute(step, AttrSuffix, tool->Suffix());

    step.appendChild(GroupOperationSource(tool, record.getId()));

    return step;
}

//---------------------------------------------------------------------------------------------------------------------
QDomElement VPatternRecipe::FlippingByAxis(const VToolRecord &record)
{
    auto *tool = GetPatternTool<VToolFlippingByAxis>(record.getId());

    QDomElement step = createElement(TagStep);

    SetAttribute(step, AttrType, VToolFlippingByAxis::ToolType);
    SetAttribute(step, AttrCenter, tool->OriginPointName());
    SetAttribute(step, AttrAxisType, static_cast<int>(tool->GetAxisType()));
    SetAttribute(step, AttrSuffix, tool->Suffix());

    step.appendChild(GroupOperationSource(tool, record.getId()));

    return step;
}

//---------------------------------------------------------------------------------------------------------------------
QDomElement VPatternRecipe::Move(const VToolRecord &record)
{
    auto *tool = GetPatternTool<VToolMove>(record.getId());

    QDomElement step = createElement(TagStep);

    SetAttribute(step, AttrType, VToolMove::ToolType);
    Formula(step, tool->GetFormulaAngle(), AttrAngle, AttrAngleValue);
    Formula(step, tool->GetFormulaRotationAngle(), AttrRotationAngle, AttrRotationAngleValue);
    Formula(step, tool->GetFormulaLength(), AttrLength, AttrLengthValue);
    SetAttribute(step, AttrCenter, tool->OriginPointName());
    SetAttribute(step, AttrSuffix, tool->Suffix());

    step.appendChild(GroupOperationSource(tool, record.getId()));

    return step;
}

//---------------------------------------------------------------------------------------------------------------------
inline void VPatternRecipe::Formula(QDomElement &step, const VFormula &formula, const QString &formulaStr,
                             const QString &formulaValue)
{
    if (formula.error())
    {
        throw VExceptionInvalidHistory(QObject::tr("Invalid formula '%1' for tool with id '%2'. %3.")
                                       .arg(formula.GetFormula(FormulaType::FromSystem))
                                       .arg(formula.getToolId())
                                       .arg(formula.Reason()));
    }

    SetAttribute(step, formulaStr, formula.GetFormula(FormulaType::FromSystem));
    SetAttribute(step, formulaValue, formula.getDoubleValue());
}

//---------------------------------------------------------------------------------------------------------------------
template<typename T>
inline void VPatternRecipe::LineAttributes(QDomElement &step, T *tool)
{
    SetAttribute(step, AttrLineColor, tool->GetLineColor());
    SetAttribute(step, AttrTypeLine, tool->getLineType());
}

//---------------------------------------------------------------------------------------------------------------------
template<typename T>
void VPatternRecipe::CurveAttributes(QDomElement &step, T *tool)
{
    SetAttribute(step, AttrLineColor, tool->GetLineColor());
    SetAttribute(step, AttrPenStyle, tool->GetPenStyle());
    SetAttribute(step, AttrAScale, tool->GetApproximationScale());
    SetAttribute(step, AttrDuplicate, tool->GetDuplicate());
}

//---------------------------------------------------------------------------------------------------------------------
template<typename T>
inline void VPatternRecipe::ToolAttributes(QDomElement &step, T *tool)
{
    SetAttribute(step, AttrType, T::ToolType);
    SetAttribute(step, AttrLabel, tool->name());
}

//---------------------------------------------------------------------------------------------------------------------
QDomElement VPatternRecipe::GroupOperationSource(VAbstractOperation *tool, quint32 id)
{
    SCASSERT(tool)

    QDomElement nodes = createElement(QStringLiteral("nodes"));
    QVector<QString> names = tool->SourceItems();

    if (names.isEmpty())
    {
        throw VExceptionInvalidHistory(QObject::tr("Empty list of nodes for tool with id '%1'.").arg(id));
    }

    for (auto &nodeName : names)
    {
        QDomElement node = createElement(QStringLiteral("node"));
        SetAttribute(node, AttrItem, nodeName);
        nodes.appendChild(node);
    }

    return nodes;
}
