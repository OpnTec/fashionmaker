/************************************************************************
 **
 **  @file   vdrawtool.cpp
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

#include "vdrawtool.h"

#include <QDialog>
#include <QDomNode>
#include <QMessageLogger>
#include <QScopedPointer>
#include <QSharedPointer>
#include <QUndoStack>
#include <Qt>
#include <QtDebug>

#include "../ifc/ifcdef.h"
#include "../ifc/xml/vdomdocument.h"
#include "../ifc/xml/vabstractpattern.h"
#include "../../undocommands/addtocalc.h"
#include "../../undocommands/savetooloptions.h"
#include "../qmuparser/qmuparsererror.h"
#include "../vpatterndb/vcontainer.h"
#include "../vmisc/logging.h"
#include "../vabstracttool.h"

template <class T> class QSharedPointer;

qreal VDrawTool::factor = 1;

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief VDrawTool constructor.
 * @param doc dom document container.
 * @param data container with variables.
 * @param id object id in container.
 */
VDrawTool::VDrawTool(VAbstractPattern *doc, VContainer *data, quint32 id, QObject *parent)
    : VInteractiveTool(doc, data, id, parent),
      nameActivDraw(doc->GetNameActivPP()),
      typeLine(TypeLineLine),
      enabled(true)
{
    connect(this->doc, &VAbstractPattern::ChangedActivPP, this, &VDrawTool::ChangedActivDraw);
    connect(this->doc, &VAbstractPattern::ChangedNameDraw, this, &VDrawTool::ChangedNameDraw);
    connect(this->doc, &VAbstractPattern::ShowTool, this, &VDrawTool::ShowTool);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief ShowTool  highlight tool.
 * @param id object id in container.
 * @param enable enable or disable highlight.
 */
void VDrawTool::ShowTool(quint32 id, bool enable)
{
    Q_UNUSED(id)
    Q_UNUSED(enable)
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief ChangedActivDraw disable or enable context menu after change active pattern peace.
 * @param newName new name active pattern peace.
 */
void VDrawTool::ChangedActivDraw(const QString &newName)
{
    Disable(!(nameActivDraw == newName), newName);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief ChangedNameDraw save new name active pattern peace.
 * @param oldName old name.
 * @param newName new name active pattern peace. new name.
 */
void VDrawTool::ChangedNameDraw(const QString &oldName, const QString &newName)
{
    if (nameActivDraw == oldName)
    {
        nameActivDraw = newName;
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VDrawTool::SaveDialogChange()
{
    qCDebug(vTool, "Saving tool options after using dialog");
    QDomElement oldDomElement = doc->elementById(id);
    if (oldDomElement.isElement())
    {
        QDomElement newDomElement = oldDomElement.cloneNode().toElement();
        SaveDialog(newDomElement);

        SaveToolOptions *saveOptions = new SaveToolOptions(oldDomElement, newDomElement, doc, id);
        connect(saveOptions, &SaveToolOptions::NeedLiteParsing, doc, &VAbstractPattern::LiteParseTree);
        qApp->getUndoStack()->push(saveOptions);
    }
    else
    {
        qCDebug(vTool, "Can't find tool with id = %u", id);
    }
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief AddToFile add tag with informations about tool into file.
 */
void VDrawTool::AddToFile()
{
    QDomElement domElement = doc->createElement(getTagName());
    QSharedPointer<VGObject> obj = VAbstractTool::data.GetGObject(id);
    SaveOptions(domElement, obj);
    AddToCalculation(domElement);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief RefreshDataInFile refresh attributes in file. If attributes don't exist create them.
 */
void VDrawTool::RefreshDataInFile()
{
    QDomElement domElement = doc->elementById(id);
    if (domElement.isElement())
    {
        QSharedPointer<VGObject> obj = VAbstractTool::data.GetGObject(id);
        SaveOptions(domElement, obj);
    }
    else
    {
        qCDebug(vTool, "Can't find tool with id = %u", id);
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VDrawTool::SaveOption(QSharedPointer<VGObject> &obj)
{
    qCDebug(vTool, "Saving tool options");
    QDomElement oldDomElement = doc->elementById(id);
    if (oldDomElement.isElement())
    {
        QDomElement newDomElement = oldDomElement.cloneNode().toElement();

        SaveOptions(newDomElement, obj);

        SaveToolOptions *saveOptions = new SaveToolOptions(oldDomElement, newDomElement, doc, id);
        connect(saveOptions, &SaveToolOptions::NeedLiteParsing, doc, &VAbstractPattern::LiteParseTree);
        qApp->getUndoStack()->push(saveOptions);
    }
    else
    {
        qCDebug(vTool, "Can't find tool with id = %u", id);
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VDrawTool::SaveOptions(QDomElement &tag, QSharedPointer<VGObject> &obj)
{
    Q_UNUSED(obj)

    doc->SetAttribute(tag, VDomDocument::AttrId, id);
}

//---------------------------------------------------------------------------------------------------------------------
QColor VDrawTool::CorrectColor(const QColor &color) const
{
    if (enabled)
    {
        return color;
    }
    else
    {
        return Qt::gray;
    }
}

//---------------------------------------------------------------------------------------------------------------------
bool VDrawTool::CorrectDisable(bool disable, const QString &namePP) const
{
    if (disable)
    {
        return disable;
    }
    else
    {
        return !(nameActivDraw == namePP);
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VDrawTool::ReadAttributes()
{
    const QDomElement domElement = doc->elementById(id);
    if (domElement.isElement())
    {
        ReadToolAttributes(domElement);
    }
    else
    {
        qCDebug(vTool, "Can't find tool with id = %u", id);
    }
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief SetFactor set current scale factor of scene.
 * @param factor scene scale factor.
 */
void VDrawTool::SetFactor(qreal factor)
{
    CheckFactor(VDrawTool::factor, factor);
}

//---------------------------------------------------------------------------------------------------------------------
void VDrawTool::EnableToolMove(bool move)
{
    Q_UNUSED(move)
    // Do nothing.
}

//---------------------------------------------------------------------------------------------------------------------
void VDrawTool::DetailsMode(bool mode)
{
    Q_UNUSED(mode)
    // Do nothing.
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief AddToCalculation add tool to calculation tag in pattern file.
 * @param domElement tag in xml tree.
 */
void VDrawTool::AddToCalculation(const QDomElement &domElement)
{
    AddToCalc *addToCal = new AddToCalc(domElement, doc);
    connect(addToCal, &AddToCalc::NeedFullParsing, doc, &VAbstractPattern::NeedFullParsing);
    qApp->getUndoStack()->push(addToCal);
}

//---------------------------------------------------------------------------------------------------------------------
QString VDrawTool::getLineType() const
{
    return typeLine;
}

//---------------------------------------------------------------------------------------------------------------------
void VDrawTool::SetTypeLine(const QString &value)
{
    typeLine = value;

    QSharedPointer<VGObject> obj = VAbstractTool::data.GetGObject(id);
    SaveOption(obj);
}
