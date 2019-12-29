/************************************************************************
 **
 **  @file   vwatermark.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   25 12, 2019
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
#include "vwatermark.h"

#include "../vmisc/projectversion.h"

const QString VWatermark::TagWatermark = QStringLiteral("watermark");
const QString VWatermark::TagText      = QStringLiteral("text");
const QString VWatermark::TagImage     = QStringLiteral("image");

const QString VWatermark::AttrOpacity   = QStringLiteral("opacity");
const QString VWatermark::AttrRotation  = QStringLiteral("rotation");
const QString VWatermark::AttrFont      = QStringLiteral("font");
const QString VWatermark::AttrPath      = QStringLiteral("path");
const QString VWatermark::AttrGrayscale = QStringLiteral("grayscale");

namespace
{
//---------------------------------------------------------------------------------------------------------------------
QString FileComment()
{
    return QString("Watermark created with Valentina v%1 (https://valentinaproject.bitbucket.io/).")
            .arg(APP_VERSION_STR);
}
}

//---------------------------------------------------------------------------------------------------------------------
VWatermark::VWatermark()
{}

//---------------------------------------------------------------------------------------------------------------------
void VWatermark::CreateEmptyWatermark()
{
    this->clear();
    QDomElement wElement = this->createElement(TagWatermark);

    wElement.appendChild(createComment(FileComment()));
    wElement.appendChild(CreateElementWithText(TagVersion, "1.0.0" /*VWatermarkConverter::WatermarkMaxVerStr*/));
    wElement.appendChild(createElement(TagText));
    wElement.appendChild(createElement(TagImage));

    appendChild(wElement);
    insertBefore(createProcessingInstruction(QStringLiteral("xml"),
                                             QStringLiteral("version=\"1.0\" encoding=\"UTF-8\"")), this->firstChild());
}

//---------------------------------------------------------------------------------------------------------------------
bool VWatermark::SaveDocument(const QString &fileName, QString &error)
{
    // Update comment with Valentina version
    QDomNode commentNode = documentElement().firstChild();
    if (commentNode.isComment())
    {
        QDomComment comment = commentNode.toComment();
        comment.setData(FileComment());
    }

    return VDomDocument::SaveDocument(fileName, error);
}

//---------------------------------------------------------------------------------------------------------------------
VWatermarkData VWatermark::GetWatermark() const
{
    VWatermarkData data;

    QDomNode root = documentElement();
    if (not root.isNull() && root.isElement())
    {
        const QDomElement rootElement = root.toElement();
        data.opacity = GetParametrInt(rootElement, AttrOpacity, QChar('2'));

        QDomElement text = rootElement.firstChildElement(TagText);
        if (not text.isNull())
        {
            data.text = GetParametrEmptyString(text, AttrText);
            data.textRotation = GetParametrInt(text, AttrRotation, QChar('0'));
            data.font.fromString(GetParametrEmptyString(text, AttrFont));
        }

        QDomElement image = rootElement.firstChildElement(TagImage);
        if (not image.isNull())
        {
            data.path = GetParametrEmptyString(image, AttrPath);
            data.imageRotation = GetParametrInt(image, AttrRotation, QChar('0'));
            data.grayscale = GetParametrBool(image, AttrGrayscale, falseStr);
        }
    }

    return data;
}

//---------------------------------------------------------------------------------------------------------------------
void VWatermark::SetWatermark(const VWatermarkData &data)
{
    QDomNode root = documentElement();
    if (not root.isNull() && root.isElement())
    {
        QDomElement rootElement = root.toElement();
        SetAttribute(rootElement, AttrOpacity, data.opacity);

        QDomElement text = rootElement.firstChildElement(TagText);
        if (not text.isNull())
        {
            SetAttributeOrRemoveIf(text, AttrText, data.text, data.text.isEmpty());
            SetAttributeOrRemoveIf(text, AttrRotation, data.textRotation, data.textRotation == 0);
            const QString fontString = data.font.toString();
            SetAttributeOrRemoveIf(text, AttrFont, fontString, fontString.isEmpty());
        }

        QDomElement image = rootElement.firstChildElement(TagImage);
        if (not image.isNull())
        {
            SetAttributeOrRemoveIf(image, AttrPath, data.path, data.path.isEmpty());
            SetAttributeOrRemoveIf(image, AttrRotation, data.imageRotation, data.imageRotation == 0);
            SetAttributeOrRemoveIf(image, AttrGrayscale, data.grayscale, data.grayscale == false);
        }
    }
}
