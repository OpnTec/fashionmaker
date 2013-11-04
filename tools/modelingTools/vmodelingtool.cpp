/****************************************************************************
 **
 **  Copyright (C) 2013 Valentina project All Rights Reserved.
 **
 **  This file is part of Valentina.
 **
 **  Tox is free software: you can redistribute it and/or modify
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
 ****************************************************************************/

#include "vmodelingtool.h"
#include <QDebug>

VModelingTool::VModelingTool(VDomDocument *doc, VContainer *data, qint64 id, QObject *parent)
    :VAbstractTool(doc, data, id, parent), ignoreContextMenuEvent(false), ignoreFullUpdate(false)
{
    _referens = 0;
}

void VModelingTool::AddToModeling(const QDomElement &domElement)
{
    QDomElement modelingElement;
    bool ok = doc->GetActivModelingElement(modelingElement);
    if (ok)
    {
        modelingElement.appendChild(domElement);
    }
    else
    {
        qCritical()<<"Can't find tag Modeling"<< Q_FUNC_INFO;
    }
    emit toolhaveChange();
}

void VModelingTool::decrementReferens()
{
    if (_referens > 0)
    {
        --_referens;
    }
    if (_referens <= 0)
    {
        RemoveReferens();
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
