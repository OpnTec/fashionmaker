/************************************************************************
 **
 **  @file   vdrawtool.cpp
 **  @author Roman Telezhinsky <dismine@gmail.com>
 **  @date   November 15, 2013
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

#include "vdrawtool.h"

qreal VDrawTool::factor = 1;

VDrawTool::VDrawTool(VPattern *doc, VContainer *data, qint64 id)
    :VAbstractTool(doc, data, id), ignoreContextMenuEvent(false), ignoreFullUpdate(false),
      nameActivDraw(doc->GetNameActivDraw()), dialog(0)
{
    connect(this->doc, &VPattern::ChangedActivDraw, this, &VDrawTool::ChangedActivDraw);
    connect(this->doc, &VPattern::ChangedNameDraw, this, &VDrawTool::ChangedNameDraw);
    connect(this->doc, &VPattern::ShowTool, this, &VDrawTool::ShowTool);
}

VDrawTool::~VDrawTool()
{
    delete dialog;
}

void VDrawTool::ShowTool(qint64 id, Qt::GlobalColor color, bool enable)
{
    Q_UNUSED(id);
    Q_UNUSED(color);
    Q_UNUSED(enable);
}

void VDrawTool::ChangedActivDraw(const QString &newName)
{
    if (nameActivDraw == newName)
    {
        ignoreContextMenuEvent = false;
    }
    else
    {
        ignoreContextMenuEvent = true;
    }
}

void VDrawTool::ChangedNameDraw(const QString &oldName, const QString &newName)
{
    if (nameActivDraw == oldName)
    {
        nameActivDraw = newName;
    }
}

void VDrawTool::FullUpdateFromGui(int result)
{
    if (result == QDialog::Accepted)
    {
        QDomElement domElement = doc->elementById(QString().setNum(id));
        if (domElement.isElement())
        {
            SaveDialog(domElement);

            emit FullUpdateTree();
            emit toolhaveChange();
        }
    }
    delete dialog;
    dialog = 0;
}

void VDrawTool::SetFactor(qreal factor)
{
    if (factor <= 2 && factor >= 0.5)
    {
        this->factor = factor;
    }
}

void VDrawTool::AddToCalculation(const QDomElement &domElement)
{
    QDomElement calcElement;
    bool ok = doc->GetActivNodeElement(VPattern::TagCalculation, calcElement);
    if (ok)
    {
        qint64 id = doc->getCursor();
        if (id <= 0)
        {
            calcElement.appendChild(domElement);
        }
        else
        {
            QDomElement refElement = doc->elementById(QString().setNum(doc->getCursor()));
            if (refElement.isElement())
            {
                calcElement.insertAfter(domElement, refElement);
                doc->setCursor(0);
            }
            else
            {
                qCritical()<<tr("Can not find the element after which you want to insert.")<< Q_FUNC_INFO;
            }
        }
    }
    else
    {
        qCritical()<<tr("Can't find tag Calculation")<< Q_FUNC_INFO;
    }
    emit toolhaveChange();
}
