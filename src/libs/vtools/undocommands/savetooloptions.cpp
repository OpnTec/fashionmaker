/************************************************************************
 **
 **  @file   savetooloptions.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   11 6, 2014
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentina project, a pattern making
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

#include "savetooloptions.h"

#include <QDomNode>

#include "../vmisc/def.h"
#include "../ifc/xml/vabstractpattern.h"
#include "vundocommand.h"

//---------------------------------------------------------------------------------------------------------------------
SaveToolOptions::SaveToolOptions(const QDomElement &oldXml, const QDomElement &newXml,
                                 const QList<quint32> &oldDependencies, const QList<quint32> &newDependencies,
                                 VAbstractPattern *doc, const quint32 &id, QUndoCommand *parent)
    : VUndoCommand(QDomElement(), doc, parent),
      oldXml(oldXml),
      newXml(newXml),
      oldDependencies(oldDependencies),
      newDependencies(newDependencies)
{
    setText(tr("save tool option"));
    nodeId = id;
}

//---------------------------------------------------------------------------------------------------------------------
void SaveToolOptions::undo()
{
    qCDebug(vUndo, "Undo.");

    QDomElement domElement = doc->elementById(nodeId);
    if (domElement.isElement())
    {
        domElement.parentNode().replaceChild(oldXml, domElement);

        DecrementReferences(Missing(newDependencies, oldDependencies));
        IncrementReferences(Missing(oldDependencies, newDependencies));

        emit NeedLiteParsing(Document::LiteParse);
    }
    else
    {
        qCDebug(vUndo, "Can't find tool with id = %u.", nodeId);
    }
}

//---------------------------------------------------------------------------------------------------------------------
void SaveToolOptions::redo()
{
    qCDebug(vUndo, "Redo.");

    QDomElement domElement = doc->elementById(nodeId);
    if (domElement.isElement())
    {
        domElement.parentNode().replaceChild(newXml, domElement);

        DecrementReferences(Missing(oldDependencies, newDependencies));
        IncrementReferences(Missing(newDependencies, oldDependencies));

        emit NeedLiteParsing(Document::LiteParse);
    }
    else
    {
        qCDebug(vUndo, "Can't find tool with id = %u.", nodeId);
    }
}

//---------------------------------------------------------------------------------------------------------------------
QVector<quint32> SaveToolOptions::Missing(const QList<quint32> &list1, const QList<quint32> &list2) const
{
#if QT_VERSION >= QT_VERSION_CHECK(5, 14, 0)
    auto set1 = QSet<quint32>(list1.begin(), list1.end());
    auto set2 = QSet<quint32>(list2.begin(), list2.end());
    QSet<quint32> substracted = set1.subtract(set2);
    return QVector<quint32>(substracted.begin(), substracted.end());
#else
    auto set1 = QSet<quint32>::fromList(list1);
    auto set2 = QSet<quint32>::fromList(list2);
    return set1.subtract(set2).toList().toVector();
#endif
}

//---------------------------------------------------------------------------------------------------------------------
bool SaveToolOptions::mergeWith(const QUndoCommand *command)
{
    const SaveToolOptions *saveCommand = static_cast<const SaveToolOptions *>(command);

    if (saveCommand->getToolId() != nodeId)
    {
        return false;
    }
    else
    {
#if QT_VERSION >= QT_VERSION_CHECK(5, 14, 0)
        auto currentSet = QSet<quint32>(newDependencies.begin(), newDependencies.end());
        auto candidateSet = QSet<quint32>(saveCommand->NewDependencies().begin(), saveCommand->NewDependencies().end());
#else
        auto currentSet = QSet<quint32>::fromList(newDependencies);
        auto candidateSet = QSet<quint32>::fromList(saveCommand->NewDependencies());
#endif

        if (currentSet != candidateSet)
        {
            return false;
        }
    }

    newXml = saveCommand->getNewXml();
    return true;
}
