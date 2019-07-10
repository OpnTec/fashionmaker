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
#include "../vpatterndb/vcontainer.h"

namespace
{
Q_GLOBAL_STATIC_WITH_ARGS(const QString, TagRecipe, (QLatin1String("recipe")))
Q_GLOBAL_STATIC_WITH_ARGS(const QString, TagPrerequisite, (QLatin1String("prerequisite")))
Q_GLOBAL_STATIC_WITH_ARGS(const QString, TagValentina, (QLatin1String("valentina")))
Q_GLOBAL_STATIC_WITH_ARGS(const QString, TagDescription, (QLatin1String("description")))
Q_GLOBAL_STATIC_WITH_ARGS(const QString, TagNotes, (QLatin1String("notes")))
Q_GLOBAL_STATIC_WITH_ARGS(const QString, TagMeasurements, (QLatin1String("measurements")))
Q_GLOBAL_STATIC_WITH_ARGS(const QString, TagMeasurement, (QLatin1String("measurement")))
Q_GLOBAL_STATIC_WITH_ARGS(const QString, TagIncrements, (QLatin1String("increments")))
Q_GLOBAL_STATIC_WITH_ARGS(const QString, TagIncrement, (QLatin1String("increment")))
Q_GLOBAL_STATIC_WITH_ARGS(const QString, TagPreviewCalculations, (QLatin1String("previewCalculations")))
Q_GLOBAL_STATIC_WITH_ARGS(const QString, TagContent, (QLatin1String("content")))

Q_GLOBAL_STATIC_WITH_ARGS(const QString, AttrRecipeVersion, (QLatin1String("version")))
Q_GLOBAL_STATIC_WITH_ARGS(const QString, AttrDescription, (QLatin1String("description")))
Q_GLOBAL_STATIC_WITH_ARGS(const QString, AttrFullName, (QLatin1String("fullName")))
Q_GLOBAL_STATIC_WITH_ARGS(const QString, AttrFormula, (QLatin1String("formula")))
Q_GLOBAL_STATIC_WITH_ARGS(const QString, AttrValue, (QLatin1String("value")))
Q_GLOBAL_STATIC_WITH_ARGS(const QString, AttrIncrementSeparator, (QLatin1String("separator")))

Q_GLOBAL_STATIC_WITH_ARGS(const QString, RecipeFormatVersion, (QLatin1String("0.1")))

//---------------------------------------------------------------------------------------------------------------------
inline QString FileComment()
{
    return QStringLiteral("Recipe created with Valentina v%1 (https://valentinaproject.bitbucket.io/).")
            .arg(APP_VERSION_STR);
}
}

//---------------------------------------------------------------------------------------------------------------------
VPatternRecipe::VPatternRecipe(VContainer *data, VAbstractPattern *pattern, QObject *parent)
    : VDomDocument(parent)
{
    SCASSERT(data != nullptr)
    SCASSERT(pattern != nullptr)

    QDomElement recipeElement = createElement(*TagRecipe);
    recipeElement.appendChild(createComment(FileComment()));
    SetAttribute(recipeElement, *AttrRecipeVersion, *RecipeFormatVersion);

    appendChild(Prerequisite(data, pattern));
    appendChild(Content(data, pattern));

    appendChild(recipeElement);
    insertBefore(createProcessingInstruction(QStringLiteral("xml"),
                                             QStringLiteral("version=\"1.0\" encoding=\"UTF-8\"")), firstChild());
}

//---------------------------------------------------------------------------------------------------------------------
QDomElement VPatternRecipe::Prerequisite(VContainer *data, VAbstractPattern *pattern)
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
    QDomElement prerequisiteElement = createElement(*TagPrerequisite);

    prerequisiteElement.appendChild(CreateElementWithText(*TagValentina, APP_VERSION_STR));
    prerequisiteElement.appendChild(CreateElementWithText(TagUnit, UnitsToStr(qApp->patternUnit())));
    prerequisiteElement.appendChild(CreateElementWithText(*TagDescription, pattern->GetDescription()));
    prerequisiteElement.appendChild(CreateElementWithText(*TagNotes, pattern->GetNotes()));
    prerequisiteElement.appendChild(Measurements(data));
    prerequisiteElement.appendChild(Increments(data));
    prerequisiteElement.appendChild(PreviewCalculations(data));

    return prerequisiteElement;
}

//---------------------------------------------------------------------------------------------------------------------
QDomElement VPatternRecipe::Measurements(VContainer *data)
{
    QDomElement measurements = createElement(*TagMeasurements);

    QList<QSharedPointer<VMeasurement>> patternMeasurements = data->DataMeasurements().values();

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
    QDomElement measurement = createElement(*TagMeasurement);

    SetAttribute(measurement, *AttrDescription, m->GetDescription());
    SetAttribute(measurement, *AttrFullName, m->GetGuiText());
    SetAttribute(measurement, AttrName, m->GetName());
    SetAttribute(measurement, *AttrFormula, m->GetFormula()); // TODO: localize
    SetAttribute(measurement, *AttrValue, *m->GetValue());

    return measurement;
}

//---------------------------------------------------------------------------------------------------------------------
QDomElement VPatternRecipe::Increments(VContainer *data)
{
    QDomElement increments = createElement(*TagIncrements);

    QList<QSharedPointer<VIncrement>> patternIncrements = data->DataIncrements().values();

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
QDomElement VPatternRecipe::Increment(const QSharedPointer<VIncrement> &incr)
{
    QDomElement measurement = createElement(*TagIncrement);

    SetAttribute(measurement, *AttrDescription, incr->GetDescription());
    SetAttribute(measurement, AttrName, incr->GetName());

    if (incr->GetType() != VarType::IncrementSeparator)
    {
        SetAttribute(measurement, *AttrFormula, incr->GetFormula()); // TODO: localize
        SetAttribute(measurement, *AttrValue, *incr->GetValue());
    }
    else
    {
        SetAttribute(measurement, *AttrIncrementSeparator, true);
    }

    return measurement;
}

//---------------------------------------------------------------------------------------------------------------------
QDomElement VPatternRecipe::Content(VContainer *data, VAbstractPattern *pattern)
{
    QDomElement content = createElement(*TagContent);

    return content;
}

//---------------------------------------------------------------------------------------------------------------------
QDomElement VPatternRecipe::PreviewCalculations(VContainer *data)
{
    QDomElement previewCalculations = createElement(*TagPreviewCalculations);

    QList<QSharedPointer<VIncrement>> patternIncrements = data->DataIncrements().values();

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
