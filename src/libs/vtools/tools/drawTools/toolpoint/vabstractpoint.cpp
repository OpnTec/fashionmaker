/************************************************************************
 **
 **  @file   vabstractpoint.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   19 6, 2015
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentine project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2015 Valentina project
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

#include "vabstractpoint.h"
#include "../vgeometry/vpointf.h"

const QString VAbstractPoint::TagName = QStringLiteral("point");

//---------------------------------------------------------------------------------------------------------------------
VAbstractPoint::VAbstractPoint(VAbstractPattern *doc, VContainer *data, quint32 id)
    :VDrawTool(doc, data, id)
{}

//---------------------------------------------------------------------------------------------------------------------
VAbstractPoint::~VAbstractPoint()
{}

//---------------------------------------------------------------------------------------------------------------------
QString VAbstractPoint::getTagName() const
{
    return VAbstractPoint::TagName;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief ShowTool  highlight tool.
 * @param id object id in container.
 * @param enable enable or disable highlight.
 */
void VAbstractPoint::ShowTool(quint32 id, bool enable)
{
    ShowItem(this, id, enable);
}

//---------------------------------------------------------------------------------------------------------------------
void VAbstractPoint::DeleteFromLabel()
{
    DeleteTool(); //Leave this method immediately after call!!!
}

//---------------------------------------------------------------------------------------------------------------------
QString VAbstractPoint::PointName(quint32 id) const
{
    try
    {
        return VAbstractTool::data.GeometricObject<VPointF>(id)->name();
    }
    catch (const VExceptionBadId &e)
    {
        qCDebug(vTool, "Error! Couldn't get point name. %s %s", e.ErrorMessage().toUtf8().constData(),
                e.DetailedInformation().toUtf8().constData());
        return QString("");// Return empty string for property browser
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VAbstractPoint::SetPointName(quint32 id, const QString &name)
{
    // Don't know if need check name here.
    QSharedPointer<VGObject> obj = VAbstractTool::data.GetGObject(id);
    obj->setName(name);
    SaveOption(obj);
}
