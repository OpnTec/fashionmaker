/************************************************************************
 **
 **  @file vabstractnode.cpp
 **  @author Roman Telezhinsky <dismine@gmail.com>
 **  @date Friday November 15, 2013
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentine project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2013 Valentina project
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

#include "vabstractnode.h"
#include <QDebug>

const QString VAbstractNode::AttrIdObject = QStringLiteral("idObject");
const QString VAbstractNode::AttrTypeObject = QStringLiteral("typeObject");
const QString VAbstractNode::TypeObjectCalculation = QStringLiteral("Calculation");
const QString VAbstractNode::TypeObjectModeling = QStringLiteral("Modeling");

VAbstractNode::VAbstractNode(VDomDocument *doc, VContainer *data, qint64 id, qint64 idNode, Draw::Draws typeobject,
                             QObject *parent)
    : VAbstractTool(doc, data, id, parent), idNode(idNode), typeobject(typeobject)
{
    _referens = 0;
}

void VAbstractNode::AddToModeling(const QDomElement &domElement)
{
    QDomElement modelingElement;
    bool ok = doc->GetActivModelingElement(modelingElement);
    if (ok)
    {
        modelingElement.appendChild(domElement);
    }
    else
    {
        qCritical()<<tr("Can't find tag Modeling")<< Q_FUNC_INFO;
    }
    emit toolhaveChange();
}

void VAbstractNode::decrementReferens()
{
    if (_referens > 0)
    {
        --_referens;
    }
    if (_referens <= 0)
    {
        doc->DecrementReferens(idNode);
        QDomElement domElement = doc->elementById(QString().setNum(id));
        if (domElement.isElement())
        {
            QDomNode element = domElement.parentNode();
            if (element.isNull() == false)
            {
                element.removeChild(domElement);
            }
        }
    }
}
