/************************************************************************
 **
 **  @file   vpatternpiecedata.cpp
 **  @author Bojan Kverh
 **  @date   July 19, 2016
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

#include <QDate>
#include <QFileInfo>
#include <QFontMetrics>
#include <QLatin1String>
#include <QRegularExpression>
#include <QApplication>
#include <QDebug>
#include <QFlags> // QFlags<Qt::Alignment>

#include "../ifc/xml/vabstractpattern.h"
#include "../vpatterndb/floatItemData/vpiecelabeldata.h"
#include "../vmisc/vabstractapplication.h"
#include "../vmisc/vmath.h"
#if QT_VERSION < QT_VERSION_CHECK(5, 14, 0)
#   include "../vmisc/vdatastreamenum.h"
#endif
#include "../vpatterndb/vcontainer.h"
#include "vtextmanager.h"

const quint32 TextLine::streamHeader = 0xA3881E49; // CRC-32Q string "TextLine"
const quint16 TextLine::classVersion = 1;

// Friend functions
//---------------------------------------------------------------------------------------------------------------------
QDataStream& operator<<(QDataStream &dataStream, const TextLine &data)
{
    dataStream << TextLine::streamHeader << TextLine::classVersion;

    // Added in classVersion = 1
    dataStream << data.m_qsText;
    dataStream << data.m_iFontSize;
    dataStream << data.m_bold;
    dataStream << data.m_italic;
    dataStream << data.m_eAlign;

    // Added in classVersion = 2

    return dataStream;
}

//---------------------------------------------------------------------------------------------------------------------
QDataStream& operator>>(QDataStream &dataStream, TextLine &data)
{
    quint32 actualStreamHeader = 0;
    dataStream >> actualStreamHeader;

    if (actualStreamHeader != TextLine::streamHeader)
    {
        QString message = QCoreApplication::tr("TextLine prefix mismatch error: actualStreamHeader = 0x%1 and "
                                               "streamHeader = 0x%2")
                .arg(actualStreamHeader, 8, 0x10, QChar('0'))
                .arg(TextLine::streamHeader, 8, 0x10, QChar('0'));
        throw VException(message);
    }

    quint16 actualClassVersion = 0;
    dataStream >> actualClassVersion;

    if (actualClassVersion > TextLine::classVersion)
    {
        QString message = QCoreApplication::tr("TextLine compatibility error: actualClassVersion = %1 and "
                                               "classVersion = %2")
                .arg(actualClassVersion).arg(TextLine::classVersion);
        throw VException(message);
    }

    dataStream >> data.m_qsText;
    dataStream >> data.m_iFontSize;
    dataStream >> data.m_bold;
    dataStream >> data.m_italic;
    dataStream >> data.m_eAlign;

//    if (actualClassVersion >= 2)
//    {

//    }

    return dataStream;
}

QVector<TextLine> VTextManager::m_patternLabelLines = QVector<TextLine>();
const quint32 VTextManager::streamHeader = 0x47E6A9EE; // CRC-32Q string "VTextManager"
const quint16 VTextManager::classVersion = 1;

// Friend functions
//---------------------------------------------------------------------------------------------------------------------
QDataStream& operator<<(QDataStream &dataStream, const VTextManager &data)
{
    dataStream << VTextManager::streamHeader << VTextManager::classVersion;

    // Added in classVersion = 1
    dataStream << data.m_font;
    dataStream << data.m_liLines;

    // Added in classVersion = 2

    return dataStream;
}

//---------------------------------------------------------------------------------------------------------------------
QDataStream& operator>>(QDataStream &dataStream, VTextManager &data)
{
    quint32 actualStreamHeader = 0;
    dataStream >> actualStreamHeader;

    if (actualStreamHeader != VTextManager::streamHeader)
    {
        QString message = QCoreApplication::tr("VTextManager prefix mismatch error: actualStreamHeader = 0x%1 and "
                                               "streamHeader = 0x%2")
                .arg(actualStreamHeader, 8, 0x10, QChar('0'))
                .arg(VTextManager::streamHeader, 8, 0x10, QChar('0'));
        throw VException(message);
    }

    quint16 actualClassVersion = 0;
    dataStream >> actualClassVersion;

    if (actualClassVersion > VTextManager::classVersion)
    {
        QString message = QCoreApplication::tr("VTextManager compatibility error: actualClassVersion = %1 and "
                                               "classVersion = %2")
                .arg(actualClassVersion).arg(VTextManager::classVersion);
        throw VException(message);
    }

    dataStream >> data.m_font;
    dataStream >> data.m_liLines;

//    if (actualClassVersion >= 2)
//    {

//    }

    return dataStream;
}

namespace
{

//---------------------------------------------------------------------------------------------------------------------
QMap<QString, QString> PreparePlaceholders(const VAbstractPattern *doc)
{
    SCASSERT(doc != nullptr)

    QMap<QString, QString> placeholders;

    // Pattern tags
    QLocale locale(qApp->Settings()->GetLocale());

    const QString date = locale.toString(QDate::currentDate(), doc->GetLabelDateFormat());
    placeholders.insert(pl_date, date);

    const QString time = locale.toString(QTime::currentTime(), doc->GetLabelTimeFormat());
    placeholders.insert(pl_time, time);

    placeholders.insert(pl_patternName, doc->GetPatternName());
    placeholders.insert(pl_patternNumber, doc->GetPatternNumber());
    placeholders.insert(pl_author, doc->GetCompanyName());

    if (qApp->patternType() == MeasurementsType::Individual)
    {
        placeholders.insert(pl_customer, qApp->GetCustomerName());
    }
    else
    {
        placeholders.insert(pl_customer, doc->GetCustomerName());
    }

    placeholders.insert(pl_pExt, QStringLiteral("val"));
    placeholders.insert(pl_pFileName, QFileInfo(qApp->GetPatternPath()).baseName());
    placeholders.insert(pl_mFileName, QFileInfo(doc->MPath()).baseName());

    QString curSize;
    QString curHeight;
    QString mExt;
    if (qApp->patternType() == MeasurementsType::Multisize)
    {
        curSize = QString::number(VContainer::size(valentinaNamespace));
        curHeight = QString::number(VContainer::height(valentinaNamespace));
        mExt = QStringLiteral("vst");
    }
    else if (qApp->patternType() == MeasurementsType::Individual)
    {
        curSize = QString::number(VContainer::size(valentinaNamespace));
        curHeight = QString::number(VContainer::height(valentinaNamespace));
        mExt = QStringLiteral("vit");
    }

    placeholders.insert(pl_size, curSize);
    placeholders.insert(pl_height, curHeight);
    placeholders.insert(pl_mExt, mExt);

    const QMap<int, QString> materials = doc->GetPatternMaterials();
    for (int i = 0; i < userMaterialPlaceholdersQuantity; ++i)
    {
        const QString number = QString::number(i + 1);

        QString value;
        if (materials.contains(i + 1))
        {
            value = materials.value(i + 1);
        }

        placeholders.insert(pl_userMaterial + number, value);
    }

    // Piece tags
    placeholders.insert(pl_pLetter, QString());
    placeholders.insert(pl_pAnnotation, QString());
    placeholders.insert(pl_pOrientation, QString());
    placeholders.insert(pl_pRotation, QString());
    placeholders.insert(pl_pTilt, QString());
    placeholders.insert(pl_pFoldPosition, QString());
    placeholders.insert(pl_pName, QString());
    placeholders.insert(pl_pQuantity, QString());
    placeholders.insert(pl_wOnFold, QString());
    placeholders.insert(pl_mFabric, QObject::tr("Fabric"));
    placeholders.insert(pl_mLining, QObject::tr("Lining"));
    placeholders.insert(pl_mInterfacing, QObject::tr("Interfacing"));
    placeholders.insert(pl_mInterlining, QObject::tr("Interlining"));
    placeholders.insert(pl_wCut, QObject::tr("Cut"));

    return placeholders;
}

//---------------------------------------------------------------------------------------------------------------------
void InitPiecePlaceholders(QMap<QString, QString> &placeholders, const QString &name, const VPieceLabelData& data)
{
    placeholders[pl_pLetter] = data.GetLetter();
    placeholders[pl_pAnnotation] = data.GetAnnotation();
    placeholders[pl_pOrientation] = data.GetOrientation();
    placeholders[pl_pRotation] = data.GetRotationWay();
    placeholders[pl_pTilt] = data.GetTilt();
    placeholders[pl_pFoldPosition] = data.GetFoldPosition();
    placeholders[pl_pName] = name;
    placeholders[pl_pQuantity] = QString::number(data.GetQuantity());

    if (data.IsOnFold())
    {
        placeholders[pl_wOnFold] = QObject::tr("on fold");
    }
}

//---------------------------------------------------------------------------------------------------------------------
QString ReplacePlaceholders(const QMap<QString, QString> &placeholders, QString line)
{
    QChar per('%');
    auto i = placeholders.constBegin();
    while (i != placeholders.constEnd())
    {
        line.replace(per+i.key()+per, i.value());
        ++i;
    }
    return line;
}

//---------------------------------------------------------------------------------------------------------------------
QVector<TextLine> PrepareLines(const QVector<VLabelTemplateLine> &lines)
{
    QVector<TextLine> textLines;

    for (auto &line : lines)
    {
        if (not line.line.isEmpty())
        {
            TextLine tl;
            tl.m_qsText = line.line;
            tl.m_eAlign = static_cast<Qt::Alignment>(line.alignment);
            tl.m_iFontSize = line.fontSizeIncrement;
            tl.m_bold = line.bold;
            tl.m_italic = line.italic;

            textLines << tl;
        }
    }

    return textLines;
}
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief VTextManager::VTextManager constructor
 */
VTextManager::VTextManager()
     : m_font(), m_liLines()
{}

//---------------------------------------------------------------------------------------------------------------------
VTextManager::VTextManager(const VTextManager &text)
    : m_font(text.GetFont()), m_liLines(text.GetAllSourceLines())
{}

//---------------------------------------------------------------------------------------------------------------------
VTextManager &VTextManager::operator=(const VTextManager &text)
{
    if ( &text == this )
    {
        return *this;
    }
    m_font = text.GetFont();
    m_liLines = text.GetAllSourceLines();
    return *this;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief GetSpacing returns the vertical spacing between the lines
 * @return spacing
 */
int VTextManager::GetSpacing() const
{
    return 0;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief SetFont set the text base font
 * @param font text base font
 */
void VTextManager::SetFont(const QFont& font)
{
    m_font = font;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief GetFont returns the text base font
 * @return text base font
 */
const QFont& VTextManager::GetFont() const
{
    return m_font;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief SetFontSize sets the font size
 * @param iFS font size in pixels
 */
void VTextManager::SetFontSize(int iFS)
{
    iFS < MIN_FONT_SIZE ? m_font.setPixelSize(MIN_FONT_SIZE) : m_font.setPixelSize(iFS);
}

//---------------------------------------------------------------------------------------------------------------------
QVector<TextLine> VTextManager::GetAllSourceLines() const
{
    return m_liLines;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief VTextManager::GetSourceLinesCount returns the number of input text lines
 * @return number of text lines that were added to the list by calling AddLine
 */
int VTextManager::GetSourceLinesCount() const
{
    return m_liLines.count();
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief VTextManager::GetSourceLine returns the reference to i-th text line
 * @param i index of the requested line
 * @return reference to the requested TextLine object
 */
const TextLine& VTextManager::GetSourceLine(int i) const
{
    Q_ASSERT(i >= 0);
    Q_ASSERT(i < m_liLines.count());
    return m_liLines.at(i);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief VTextManager::FitFontSize sets the font size just big enough, so that the text fits into rectangle of
 * size (fW, fH)
 * @param fW rectangle width
 * @param fH rectangle height
 */
void VTextManager::FitFontSize(qreal fW, qreal fH)
{
    int iFS = 0;
    if (GetSourceLinesCount() > 0)
    {//division by zero
        iFS = 3*qFloor(fH/GetSourceLinesCount())/4;
    }

    if (iFS < MIN_FONT_SIZE)
    {
        iFS = MIN_FONT_SIZE;
    }

    // get ratio between char width and height

    int iMaxLen = 0;
    TextLine maxLine;
    QFont fnt;
    for (int i = 0; i < GetSourceLinesCount(); ++i)
    {
        const TextLine& tl = GetSourceLine(i);
        fnt = m_font;
        fnt.setPixelSize(iFS + tl.m_iFontSize);
        fnt.setBold(tl.m_bold);
        fnt.setItalic(tl.m_italic);
        QFontMetrics fm(fnt);
#if QT_VERSION >= QT_VERSION_CHECK(5, 11, 0)
        const int iTW = fm.horizontalAdvance(tl.m_qsText);
#else
        const int iTW = fm.width(tl.m_qsText);
#endif
        if (iTW > iMaxLen)
        {
            iMaxLen = iTW;
            maxLine = tl;
        }
    }
    if (iMaxLen > fW)
    {
        QFont fnt = m_font;
        fnt.setBold(maxLine.m_bold);
        fnt.setItalic(maxLine.m_italic);

        int lineLength = 0;
        do
        {
            --iFS;
            fnt.setPixelSize(iFS + maxLine.m_iFontSize);
            QFontMetrics fm(fnt);
#if QT_VERSION >= QT_VERSION_CHECK(5, 11, 0)
            lineLength = fm.horizontalAdvance(maxLine.m_qsText);
#else
            lineLength = fm.width(maxLine.m_qsText);
#endif
        }
        while (lineLength > fW && iFS > MIN_FONT_SIZE);
    }
    SetFontSize(iFS);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief VTextManager::Update updates the text lines with detail data
 * @param qsName detail name
 * @param data reference to the detail data
 */
void VTextManager::Update(const QString& qsName, const VPieceLabelData& data)
{
    m_liLines.clear();

    QMap<QString, QString> placeholders = PreparePlaceholders(qApp->getCurrentDocument());
    InitPiecePlaceholders(placeholders, qsName, data);

    QVector<VLabelTemplateLine> lines = data.GetLabelTemplate();

    for (int i=0; i<lines.size(); ++i)
    {
        lines[i].line = ReplacePlaceholders(placeholders, lines.at(i).line);
    }

    m_liLines = PrepareLines(lines);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief VTextManager::Update updates the text lines with pattern info
 * @param pDoc pointer to the abstract pattern object
 */
void VTextManager::Update(VAbstractPattern *pDoc)
{
    m_liLines.clear();

    if (m_patternLabelLines.isEmpty() || pDoc->GetPatternWasChanged())
    {
        QVector<VLabelTemplateLine> lines = pDoc->GetPatternLabelTemplate();
        if (lines.isEmpty() && m_patternLabelLines.isEmpty())
        {
            return; // Nothing to parse
        }

        const QMap<QString, QString> placeholders = PreparePlaceholders(pDoc);

        for (int i=0; i<lines.size(); ++i)
        {
            lines[i].line = ReplacePlaceholders(placeholders, lines.at(i).line);
        }

        pDoc->SetPatternWasChanged(false);
        m_patternLabelLines = PrepareLines(lines);
    }

    m_liLines = m_patternLabelLines;
}
