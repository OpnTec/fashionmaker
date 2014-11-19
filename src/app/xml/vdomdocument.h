/************************************************************************
 **
 **  @file   vdomdocument.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   November 15, 2013
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentina project, a pattern making
 **  program that allows creating and modelling patterns of clothing.
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

#include <QDomDocument>
#include <QDebug>
#include "../container/vcontainer.h"

/*
   can be used like #if (V_FORMAT_VERSION >= V_FORMAT_VERSION_CHECK(4, 4, 0))
*/
#define V_FORMAT_VERSION_CHECK(major, minor, patch) ((major<<16)|(minor<<8)|(patch))

#ifdef Q_CC_GNU
    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Weffc++"
#endif

/**
 * @brief The VDomDocument class represents a Valentina document (.val file).
 *
 * A Valentina document describes the construction of a sewing pattern. The
 * information is stored in XML format. By parsing a VDomDocument, the contained
 * pattern is rendered to a Valentina graphics scene (VMainGraphicsScene).
 *
 * A sewing pattern consists of zero or more increments and one
 * or more pattern pieces.
 *
 * An increment is an auxiliary variable that is calculated from regular measurement
 * variables (that belong to the standard measurements table). Increments are used to
 * create a graduation schema for the sewing pattern.
 *
 * A pattern piece contains
 * 1) auxiliary pattern construction elements (calculation),
 * 2) pattern construction elements (modeling), and
 * 3) special markers, e.g. seam allowances (details).
 * Of these, 2) and 3) are visible in the final pattern (draw mode 'Modeling'),
 * 1) is only displayed when editing (draw mode 'Calculation') the pattern.
 */
class VDomDocument : public QDomDocument
{
    Q_DECLARE_TR_FUNCTIONS(VDomDocument)
public:
    static const QString    AttrId;
    static const QString    AttrUnit;
    static const QString    UnitMM;
    static const QString    UnitCM;
    static const QString    UnitINCH;
    static const QString    TagVersion;
    /**
     * @param data container with variables
     */
    VDomDocument(VContainer *data);
    virtual ~VDomDocument();
    QDomElement    elementById(const QString& id);
    void           removeAllChilds(QDomElement &element);
    template <typename T>
    /**
     * @brief SetAttribute set attribute in pattern file. Replace "," by ".".
     * @param domElement element in xml tree.
     * @param name name of attribute.
     * @param value value of attribute.
     */
    void SetAttribute(QDomElement &domElement, const QString &name, const T &value) const
    {
        QString val = QString().setNum(value);
        val = val.replace(",", ".");
        domElement.setAttribute(name, val);
    }
    quint32        GetParametrUInt(const QDomElement& domElement, const QString &name, const QString &defValue) const;
    bool           GetParametrBool(const QDomElement& domElement, const QString &name, const QString &defValue) const;
    QString        GetParametrString(const QDomElement& domElement, const QString &name,
                                     const QString &defValue = QString()) const;
    qreal          GetParametrDouble(const QDomElement& domElement, const QString &name, const QString &defValue) const;

    static void    ValidateXML(const QString &schema, const QString &fileName);
    void           setContent(const QString &fileName);
    static Unit    StrToUnits(const QString &unit);
    static QString UnitsToStr(const Unit &unit, const bool translate = false);
    virtual bool   SaveDocument(const QString &fileName, QString &error);
    QString        Major() const;
    QString        Minor() const;
    QString        Patch() const;
    static void    RemoveAllChild(QDomElement &domElement);

    QDomNode       ParentNodeById(const quint32 &nodeId);
    QDomElement    CloneNodeById(const quint32 &nodeId);
    QDomElement    NodeById(const quint32 &nodeId);
protected:
    /** @brief data container with data. */
    VContainer     *data;

    void           setTagText(const QString &tag, const QString &text);
    QString        UniqueTagText(const QString &tagName, const QString &defVal = QString()) const;
private:
    Q_DISABLE_COPY(VDomDocument)
    /** @brief Map used for finding element by id. */
    QHash<QString, QDomElement> map;

    bool           find(const QDomElement &node, const QString& id);
};

//---------------------------------------------------------------------------------------------------------------------
template <>
inline void VDomDocument::SetAttribute<QString>(QDomElement &domElement, const QString &name,
                                                const QString &value) const
{
    domElement.setAttribute(name, value);
}

//---------------------------------------------------------------------------------------------------------------------
template <>
inline void VDomDocument::SetAttribute<bool>(QDomElement &domElement, const QString &name, const bool &value) const
{
    QString string;
    if (value)
    {
        string = "true";
    }
    else
    {
        string = "false";
    }
    domElement.setAttribute(name, string);
}

//---------------------------------------------------------------------------------------------------------------------
template <>
inline void VDomDocument::SetAttribute<MeasurementsType>(QDomElement &domElement, const QString &name,
                                                              const MeasurementsType &value) const
{
    if (value == MeasurementsType::Standard)
    {
        domElement.setAttribute(name, "standard");
    }
    else
    {
        domElement.setAttribute(name, "individual");
    }
}

#ifdef Q_CC_GNU
    #pragma GCC diagnostic pop
#endif

#endif // VDOMDOCUMENT_H
