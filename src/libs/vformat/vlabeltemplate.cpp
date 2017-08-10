/************************************************************************
 **
 **  @file   vlabeltemplate.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   10 8, 2015
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

#include "vlabeltemplate.h"

const QString VLabelTemplate::TagTemplate = QStringLiteral("template");
const QString VLabelTemplate::TagLines    = QStringLiteral("lines");
const QString VLabelTemplate::TagLine     = QStringLiteral("line");

const QString VLabelTemplate::AttrText      = QStringLiteral("text");
const QString VLabelTemplate::AttrBold      = QStringLiteral("bold");
const QString VLabelTemplate::AttrItalic    = QStringLiteral("italic");
const QString VLabelTemplate::AttrAlignment = QStringLiteral("alignment");

//---------------------------------------------------------------------------------------------------------------------
VLabelTemplate::VLabelTemplate()
    : VDomDocument()
{}

//---------------------------------------------------------------------------------------------------------------------
void VLabelTemplate::CreateEmptyTemplate()
{
    clear();
    QDomElement templateElement = this->createElement(TagTemplate);

    QDomElement version = createElement(TagVersion);
    QDomText newNodeText = createTextNode("1.0"/*VLabelTemplateConverter::TemplateMaxVerStr*/);
    version.appendChild(newNodeText);
    templateElement.appendChild(version);

    templateElement.appendChild(createElement(TagLines));

    this->appendChild(templateElement);
    insertBefore(createProcessingInstruction("xml", "version=\"1.0\" encoding=\"UTF-8\""), this->firstChild());
}

//---------------------------------------------------------------------------------------------------------------------
void VLabelTemplate::AddLines(const QVector<VLabelTemplateLine> &lines)
{
    const QDomNodeList listLines = elementsByTagName(TagLines);
    if (listLines.size() == 0)
    {
        return;
    }

    QDomElement tagLines = listLines.at(0).toElement();
    if (not tagLines.isNull())
    {
        for (int i=0; i < lines.size(); ++i)
        {
            QDomElement tagLine = createElement(TagLine);

            SetAttribute(tagLine, AttrText, lines.at(i).line);
            SetAttribute(tagLine, AttrBold, lines.at(i).bold);
            SetAttribute(tagLine, AttrItalic, lines.at(i).italic);
            SetAttribute(tagLine, AttrAlignment, lines.at(i).alignment);

            tagLines.appendChild(tagLine);
        }
    }
}
