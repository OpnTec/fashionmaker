/************************************************************************
 **
 **  @file   vdomdocument.cpp
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

#include "vdomdocument.h"
#include "../exception/vexceptionconversionerror.h"
#include "../exception/vexceptionemptyparameter.h"
#include "../exception/vexceptionbadid.h"

VDomDocument::VDomDocument(VContainer *data)
    : QDomDocument(), data(data), map(QHash<QString, QDomElement>())
{

}

QDomElement VDomDocument::elementById(const QString& id)
{
    if (map.contains(id))
    {
       QDomElement e = map[id];
       if (e.parentNode().nodeType() != QDomNode::BaseNode)
       {
           return e;
       }
       map.remove(id);
    }

    if (this->find(this->documentElement(), id))
    {
       return map[id];
    }

    return QDomElement();
}

void VDomDocument::removeAllChilds(QDomElement &element)
{
    QDomNode domNode = element.firstChild();
    while (domNode.isNull() == false)
    {
        if (domNode.isElement())
        {
            QDomElement domElement = domNode.toElement();
            if (domElement.isNull() == false)
            {
                element.removeChild(domElement);
            }
        }
        domNode = element.firstChild();
    }
}

bool VDomDocument::find(const QDomElement &node, const QString& id)
{
    if (node.hasAttribute("id"))
    {
        const QString value = node.attribute("id");
        this->map[value] = node;
        if (value == id)
        {
            return true;
        }
    }

    for (qint32 i=0; i<node.childNodes().length(); ++i)
    {
        QDomNode n = node.childNodes().at(i);
        if (n.isElement())
        {
            if (this->find(n.toElement(), id))
            {
                return true;
            }
        }
    }
    return false;
}

qint64 VDomDocument::GetParametrLongLong(const QDomElement &domElement, const QString &name,
                                         const QString &defValue) const
{
    Q_ASSERT_X(name.isEmpty() == false, Q_FUNC_INFO, "name of parametr is empty");
    Q_ASSERT_X(domElement.isNull() == false, Q_FUNC_INFO, "domElement is null");
    bool ok = false;
    const QString parametr = GetParametrString(domElement, name, defValue);
    const qint64 id = parametr.toLongLong(&ok);
    if (ok == false)
    {
        throw VExceptionConversionError(tr("Can't convert toLongLong parameter"), name);
    }
    return id;
}

QString VDomDocument::GetParametrString(const QDomElement &domElement, const QString &name,
                                        const QString &defValue) const
{
    Q_ASSERT_X(name.isEmpty() == false, Q_FUNC_INFO, "name of parametr is empty");
    Q_ASSERT_X(domElement.isNull() == false, Q_FUNC_INFO, "domElement is null");
    const QString parameter = domElement.attribute(name, defValue);
    if (parameter.isEmpty())
    {
        throw VExceptionEmptyParameter(tr("Got empty parameter"), name, domElement);
    }
    return parameter;
}

qreal VDomDocument::GetParametrDouble(const QDomElement &domElement, const QString &name, const QString &defValue) const
{
    Q_ASSERT_X(name.isEmpty() == false, Q_FUNC_INFO, "name of parametr is empty");
    Q_ASSERT_X(domElement.isNull() == false, Q_FUNC_INFO, "domElement is null");
    bool ok = false;
    QString parametr = GetParametrString(domElement, name, defValue);
    const qreal param = parametr.replace(",", ".").toDouble(&ok);
    if (ok == false)
    {
        throw VExceptionConversionError(tr("Can't convert toDouble parameter"), name);
    }
    return param;
}

QString VDomDocument::UniqueTagText(const QString &tagName, const QString &defVal) const
{
    const QDomNodeList nodeList = this->elementsByTagName(tagName);
    if (nodeList.isEmpty())
    {
        return defVal;
    }
    else
    {
        const QDomNode domNode = nodeList.at(0);
        if (domNode.isNull() == false && domNode.isElement())
        {
            const QDomElement domElement = domNode.toElement();
            if (domElement.isNull() == false)
            {
                return domElement.text();
            }
        }
    }
    return defVal;
}
