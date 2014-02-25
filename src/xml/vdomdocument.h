/************************************************************************
 **
 **  @file   vdomdocument.h
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

#ifndef VDOMDOCUMENT_H
#define VDOMDOCUMENT_H

#include "../container/vcontainer.h"

#include <QDomDocument>
#include <QDebug>

#ifdef Q_CC_GNU
    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Weffc++"
#endif

/**
 * @brief The VDomDocument class extend QDomDocument class.
 */
class VDomDocument : public QDomDocument
{
    Q_DECLARE_TR_FUNCTIONS(VDomDocument)
public:
    /**
     * @brief VDomDocument constructor.
     * @param data container with variables.
     * @param parent parent object
     */
    VDomDocument(VContainer *data);
    virtual ~VDomDocument(){}
    /**
     * @brief elementById find element by id.
     * @param id value id attribute.
     * @return dom element.
     */
    QDomElement    elementById(const QString& id);
    /**
     * @brief removeAllChilds remove all tag childs.
     * @param element tag.
     */
    void           removeAllChilds(QDomElement &element);
    template <typename T>
    /**
     * @brief SetAttribute set attribute in pattern file. Replace "," by ".".
     * @param domElement element in xml tree.
     * @param name name of attribute.
     * @param value value of attribute.
     */
    void SetAttribute(QDomElement &domElement, const QString &name, const T &value)
    {
        QString val = QString().setNum(value);
        val = val.replace(",", ".");
        domElement.setAttribute(name, val);
    }
    /**
     * @brief GetParametrLongLong return long long value of attribute.
     * @param domElement tag in xml tree.
     * @param name attribute name.
     * @return long long value.
     */
    quint32         GetParametrUInt(const QDomElement& domElement, const QString &name,
                                       const QString &defValue) const;
    /**
     * @brief GetParametrString return string value of attribute.
     * @param domElement tag in xml tree.
     * @param name attribute name.
     * @return attribute value.
     */
    QString        GetParametrString(const QDomElement& domElement, const QString &name,
                                     const QString &defValue = QString()) const;
    /**
     * @brief GetParametrDouble return double value of attribute.
     * @param domElement tag in xml tree.
     * @param name attribute name.
     * @return double value.
     */
    qreal          GetParametrDouble(const QDomElement& domElement, const QString &name, const QString &defValue) const;
    QString        UniqueTagText(const QString &tagName, const QString &defVal = QString()) const;
    /**
     * @brief ValidatePattern validate pattern file by xsd schema.
     * @param schema path to schema file.
     * @param fileName name of pattern file.
     * @param errorMsg error message.
     * @param errorLine number error line.
     * @param errorColumn number error column.
     * @return true if validation successful.
     */
    static bool ValidatePattern(const QString &schema, const QString &fileName, QString &errorMsg, qint64 &errorLine,
                         qint64 &errorColumn);
protected:
    /**
     * @brief data container with data.
     */
    VContainer     *data;
private:
    Q_DISABLE_COPY(VDomDocument)
    /**
     * @brief map use for finding element by id.
     */
    QHash<QString, QDomElement> map;

    /**
     * @brief find find element by id.
     * @param node node.
     * @param id id value.
     * @return true if found.
     */
    bool           find(const QDomElement &node, const QString& id);
};

template <>
inline void VDomDocument::SetAttribute<QString>(QDomElement &domElement, const QString &name, const QString &value)
{
    domElement.setAttribute(name, value);
}

#ifdef Q_CC_GNU
    #pragma GCC diagnostic pop
#endif

#endif // VDOMDOCUMENT_H
