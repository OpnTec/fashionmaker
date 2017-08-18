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

#ifndef VDOMDOCUMENT_H
#define VDOMDOCUMENT_H

#include <QCoreApplication>
#include <QDomDocument>
#include <QDomElement>
#include <QDomNode>
#include <QHash>
#include <QLatin1String>
#include <QStaticStringData>
#include <QString>
#include <QStringData>
#include <QStringDataPtr>
#include <QtGlobal>
#include <QLocale>

#include "../ifc/ifcdef.h"
#include "../vmisc/def.h"
#include "../vmisc/diagnostic.h"
#include "../vmisc/logging.h"

class QDomElement;
class QDomNode;
template <typename T> class QVector;

Q_DECLARE_LOGGING_CATEGORY(vXML)

QT_WARNING_PUSH
QT_WARNING_DISABLE_GCC("-Weffc++")
QT_WARNING_DISABLE_GCC("-Wnon-virtual-dtor")

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
 * variables (that belong to the multisize measurements table). Increments are used to
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
    static const QString AttrId;
    static const QString AttrText;
    static const QString AttrBold;
    static const QString AttrItalic;
    static const QString AttrAlignment;
    static const QString AttrFSIncrement;

    static const QString TagVersion;
    static const QString TagUnit;
    static const QString TagLine;

    VDomDocument();
    virtual ~VDomDocument() Q_DECL_EQ_DEFAULT;
    QDomElement elementById(quint32 id, const QString &tagName = QString());

    template <typename T>
    void SetAttribute(QDomElement &domElement, const QString &name, const T &value) const;

    static quint32 GetParametrUInt(const QDomElement& domElement, const QString &name, const QString &defValue);
    static bool    GetParametrBool(const QDomElement& domElement, const QString &name, const QString &defValue);

    static NodeUsage GetParametrUsage(const QDomElement& domElement, const QString &name);
    static void      SetParametrUsage(QDomElement& domElement, const QString &name, const NodeUsage &value);

    static QString GetParametrString(const QDomElement& domElement, const QString &name,
                                     const QString &defValue = QString());
    static qreal   GetParametrDouble(const QDomElement& domElement, const QString &name, const QString &defValue);
    static quint32 GetParametrId(const QDomElement& domElement);

    Unit           MUnit() const;

    static void    ValidateXML(const QString &schema, const QString &fileName);
    virtual void   setXMLContent(const QString &fileName);
    static QString UnitsHelpString();

    virtual bool   SaveDocument(const QString &fileName, QString &error);
    QString        Major() const;
    QString        Minor() const;
    QString        Patch() const;
    static void    RemoveAllChildren(QDomElement &domElement);

    QDomNode       ParentNodeById(const quint32 &nodeId);
    QDomElement    CloneNodeById(const quint32 &nodeId);
    QDomElement    NodeById(const quint32 &nodeId);

    static bool    SafeCopy(const QString &source, const QString &destination, QString &error);

    QVector<VLabelTemplateLine> GetLabelTemplate(const QDomElement &element) const;
    void                        SetLabelTemplate(QDomElement &element, const QVector<VLabelTemplateLine> &lines);

protected:
    bool           setTagText(const QString &tag, const QString &text);
    bool           setTagText(const QDomElement &domElement, const QString &text);
    QString        UniqueTagText(const QString &tagName, const QString &defVal = QString()) const;

    void           TestUniqueId() const;
    void           CollectId(const QDomElement &node, QVector<quint32> &vector)const;

private:
    Q_DISABLE_COPY(VDomDocument)
    /** @brief Map used for finding element by id. */
    QHash<quint32, QDomElement> map;

    bool           find(const QDomElement &node, quint32 id);

    bool SaveCanonicalXML(QIODevice *file, int indent, QString &error) const;
};

//---------------------------------------------------------------------------------------------------------------------
template <typename T>
/**
 * @brief SetAttribute set attribute in pattern file. Replace "," by ".".
 * @param domElement element in xml tree.
 * @param name name of attribute.
 * @param value value of attribute.
 */
inline void VDomDocument::SetAttribute(QDomElement &domElement, const QString &name, const T &value) const
{
    // See specification for xs:decimal
    const QLocale locale = QLocale::c();
    domElement.setAttribute(name, locale.toString(value).remove(locale.groupSeparator()));
}

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
    domElement.setAttribute(name, value ? trueStr : falseStr);
}

//---------------------------------------------------------------------------------------------------------------------
template <>
inline void VDomDocument::SetAttribute<MeasurementsType>(QDomElement &domElement, const QString &name,
                                                              const MeasurementsType &value) const
{
    domElement.setAttribute(name, value == MeasurementsType::Multisize ? QStringLiteral("multisize") :
                                                                        QStringLiteral("individual"));
}

QT_WARNING_POP

#endif // VDOMDOCUMENT_H
