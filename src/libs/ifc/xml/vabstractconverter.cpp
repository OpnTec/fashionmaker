/************************************************************************
 **
 **  @file   vabstractconverter.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   10 12, 2014
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentina project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
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

#include "vabstractconverter.h"

#include <QAbstractMessageHandler>
#include <QDir>
#include <QDomElement>
#include <QDomNode>
#include <QDomNodeList>
#include <QFile>
#include <QFileInfo>
#include <QLatin1String>
#include <QMap>
#include <QRegularExpression>
#include <QRegularExpressionMatch>
#include <QSourceLocation>
#include <QStaticStringData>
#include <QStringData>
#include <QStringDataPtr>
#include <QStringList>
#include <QTextDocument>
#include <QXmlSchema>
#include <QXmlSchemaValidator>

#include "../exception/vexception.h"
#include "../exception/vexceptionwrongid.h"
#include "vdomdocument.h"

//This class need for validation pattern file using XSD shema
class MessageHandler : public QAbstractMessageHandler
{
public:
    MessageHandler()
        : QAbstractMessageHandler(),
          m_messageType(QtMsgType()),
          m_description(),
          m_sourceLocation(QSourceLocation())
    {}

    QString statusMessage() const;
    qint64  line() const;
    qint64  column() const;
protected:
    // cppcheck-suppress unusedFunction
    virtual void handleMessage(QtMsgType type, const QString &description,
                               const QUrl &identifier, const QSourceLocation &sourceLocation) override;
private:
    QtMsgType       m_messageType;
    QString         m_description;
    QSourceLocation m_sourceLocation;
};

//---------------------------------------------------------------------------------------------------------------------
QString MessageHandler::statusMessage() const
{
    QTextDocument doc;
    doc.setHtml(m_description);
    return doc.toPlainText();
}

//---------------------------------------------------------------------------------------------------------------------
inline qint64  MessageHandler::line() const
{
    return m_sourceLocation.line();
}

//---------------------------------------------------------------------------------------------------------------------
inline qint64  MessageHandler::column() const
{
    return m_sourceLocation.column();
}

//---------------------------------------------------------------------------------------------------------------------
// cppcheck-suppress unusedFunction
void MessageHandler::handleMessage(QtMsgType type, const QString &description, const QUrl &identifier,
                                   const QSourceLocation &sourceLocation)
{
    Q_UNUSED(type)
    Q_UNUSED(identifier)

    m_messageType = type;
    m_description = description;
    m_sourceLocation = sourceLocation;
}

//---------------------------------------------------------------------------------------------------------------------
VAbstractConverter::VAbstractConverter(const QString &fileName)
    : VDomDocument(),
      m_ver(0x0),
      m_originalFileName(fileName),
      m_convertedFileName(fileName),
      m_tmpFile()
{
    setXMLContent(m_convertedFileName);// Throw an exception on error
    m_ver = GetFormatVersion(GetFormatVersionStr());
}

//---------------------------------------------------------------------------------------------------------------------
QString VAbstractConverter::Convert()
{
    if (m_ver == MaxVer())
    {
        return m_convertedFileName;
    }

    if (not IsReadOnly())
    {
        ReserveFile();
    }

    if (m_tmpFile.open())
    {
        m_convertedFileName = m_tmpFile.fileName();
    }
    else
    {
        throw VException(tr("Error openning a temp file: %1.").arg(m_tmpFile.errorString()));
    }

    m_ver < MaxVer() ? ApplyPatches() : DowngradeToCurrentMaxVersion();

    return m_convertedFileName;
}

//---------------------------------------------------------------------------------------------------------------------
int VAbstractConverter::GetCurrentFormatVersion() const
{
    return m_ver;
}

//---------------------------------------------------------------------------------------------------------------------
void VAbstractConverter::ReserveFile() const
{
    //It's not possible in all cases make conversion without lose data.
    //For such cases we will store old version in a reserve file.
    QString error;
    QFileInfo info(m_convertedFileName);
    const QString reserveFileName = QString("%1/%2(v%3).%4.bak")
            .arg(info.absoluteDir().absolutePath(), info.baseName(), GetFormatVersionStr(), info.completeSuffix());
    if (not SafeCopy(m_convertedFileName, reserveFileName, error))
    {
#ifdef Q_OS_WIN32
        qt_ntfs_permission_lookup++; // turn checking on
#endif /*Q_OS_WIN32*/
        const bool isFileWritable = info.isWritable();
#ifdef Q_OS_WIN32
        qt_ntfs_permission_lookup--; // turn it off again
#endif /*Q_OS_WIN32*/

        if (not IsReadOnly() && isFileWritable)
        {
            const QString errorMsg(tr("Error creating a reserv copy: %1.").arg(error));
            throw VException(errorMsg);
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VAbstractConverter::Replace(QString &formula, const QString &newName, int position, const QString &token,
                                 int &bias) const
{
    formula.replace(position, token.length(), newName);
    bias = token.length() - newName.length();
}

//---------------------------------------------------------------------------------------------------------------------
void VAbstractConverter::CorrectionsPositions(int position, int bias, QMap<int, QString> &tokens) const
{
    if (bias == 0)
    {
        return;// Nothing to correct;
    }

    BiasTokens(position, bias, tokens);
}

//---------------------------------------------------------------------------------------------------------------------
void VAbstractConverter::BiasTokens(int position, int bias, QMap<int, QString> &tokens)
{
    QMap<int, QString> newTokens;
    QMap<int, QString>::const_iterator i = tokens.constBegin();
    while (i != tokens.constEnd())
    {
        if (i.key()<= position)
        { // Tokens before position "position" did not change his positions.
            newTokens.insert(i.key(), i.value());
        }
        else
        {
            newTokens.insert(i.key()-bias, i.value());
        }
        ++i;
    }
    tokens = newTokens;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief ValidateXML validate xml file by xsd schema.
 * @param schema path to schema file.
 */
void VAbstractConverter::ValidateXML(const QString &schema) const
{
    qCDebug(vXML, "Validation xml file %s.", qUtf8Printable(m_convertedFileName));
    QFile pattern(m_convertedFileName);
    if (not pattern.open(QIODevice::ReadOnly))
    {
        const QString errorMsg(tr("Can't open file %1:\n%2.").arg(m_convertedFileName, pattern.errorString()));
        throw VException(errorMsg);
    }

    QFile fileSchema(schema);
    if (not fileSchema.open(QIODevice::ReadOnly))
    {
        pattern.close();
        const QString errorMsg(tr("Can't open schema file %1:\n%2.").arg(schema, fileSchema.errorString()));
        throw VException(errorMsg);
    }

    MessageHandler messageHandler;
    QXmlSchema sch;
    sch.setMessageHandler(&messageHandler);
    if (sch.load(&fileSchema, QUrl::fromLocalFile(fileSchema.fileName()))==false)
    {
        pattern.close();
        fileSchema.close();
        VException e(messageHandler.statusMessage());
        e.AddMoreInformation(tr("Could not load schema file '%1'.").arg(fileSchema.fileName()));
        throw e;
    }
    qCDebug(vXML, "Schema loaded.");

    bool errorOccurred = false;
    if (sch.isValid() == false)
    {
        errorOccurred = true;
    }
    else
    {
        QXmlSchemaValidator validator(sch);
        if (validator.validate(&pattern, QUrl::fromLocalFile(pattern.fileName())) == false)
        {
            errorOccurred = true;
        }
    }

    if (errorOccurred)
    {
        pattern.close();
        fileSchema.close();
        VException e(messageHandler.statusMessage());
        e.AddMoreInformation(tr("Validation error file %3 in line %1 column %2").arg(messageHandler.line())
                             .arg(messageHandler.column()).arg(m_originalFileName));
        throw e;
    }
    pattern.close();
    fileSchema.close();
}

//---------------------------------------------------------------------------------------------------------------------
Q_NORETURN void VAbstractConverter::InvalidVersion(int ver) const
{
    if (ver < MinVer())
    {
        const QString errorMsg(tr("Invalid version. Minimum supported format version is %1").arg(MinVerStr()));
        throw VException(errorMsg);
    }

    if (ver > MaxVer())
    {
        const QString errorMsg(tr("Invalid version. Maximum supported format version is %1").arg(MaxVerStr()));
        throw VException(errorMsg);
    }

    const QString errorMsg(tr("Unexpected version \"%1\".").arg(ver, 0, 16));
    throw VException(errorMsg);
}

//---------------------------------------------------------------------------------------------------------------------
void VAbstractConverter::ValidateInputFile(const QString &currentSchema) const
{
    QString schema;
    try
    {
        schema = XSDSchema(m_ver);
    }
    catch(const VException &e)
    {
        if (m_ver < MinVer())
        { // Version less than minimally supported version. Can't do anything.
            throw;
        }
        else if (m_ver > MaxVer())
        { // Version bigger than maximum supported version. We still have a chance to open the file.
            try
            { // Try to open like the current version.
                ValidateXML(currentSchema);
            }
            catch(const VException &exp)
            { // Nope, we can't.
                Q_UNUSED(exp)
                throw e;
            }
        }
        else
        { // Unexpected version. Most time mean that we do not catch all versions between min and max.
            throw;
        }

        return; // All is fine and we can try to convert to current max version.
    }

    ValidateXML(schema);
}

//---------------------------------------------------------------------------------------------------------------------
void VAbstractConverter::Save()
{
    try
    {
        TestUniqueId();
    }
    catch (const VExceptionWrongId &e)
    {
        Q_UNUSED(e)
        VException ex(tr("Error no unique id."));
        throw ex;
    }

    m_tmpFile.resize(0);//clear previous content
    const int indent = 4;
    QTextStream out(&m_tmpFile);
    out.setCodec("UTF-8");
    save(out, indent);

    if (not m_tmpFile.flush())
    {
        VException e(m_tmpFile.errorString());
        throw e;
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VAbstractConverter::SetVersion(const QString &version)
{
    ValidateVersion(version);

    if (setTagText(TagVersion, version) == false)
    {
        VException e(tr("Could not change version."));
        throw e;
    }
}
