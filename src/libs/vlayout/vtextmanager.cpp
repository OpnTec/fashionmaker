/************************************************************************
 **
 **  @file   vpatternpiecedata.cpp
 **  @author Bojan Kverh
 **  @date   July 19, 2016
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentine project, a pattern making
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

#include "../ifc/xml/vabstractpattern.h"
#include "../vpatterndb/floatItemData/vpiecelabeldata.h"
#include "../vmisc/vabstractapplication.h"
#include "../vmisc/vmath.h"
#include "../vpatterndb/vcontainer.h"
#include "vtextmanager.h"

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief TextLine::TextLine default constructor
 */
TextLine::TextLine()
    : m_qsText(),
      m_iFontSize(MIN_FONT_SIZE),
      bold(false),
      italic(false),
      m_eAlign(Qt::AlignCenter)
{}

QList<TextLine> VTextManager::m_patternLabelLines = QList<TextLine>();

namespace
{

//---------------------------------------------------------------------------------------------------------------------
QMap<QString, QString> PreparePlaceholders(const VAbstractPattern *doc)
{
    SCASSERT(doc != nullptr)

    QMap<QString, QString> placeholders;

    // Pattern tags
    QLocale locale(qApp->Settings()->GetLocale());
    placeholders.insert(pl_date, locale.toString(QDate::currentDate()));
    placeholders.insert(pl_time, locale.toString(QTime::currentTime()));
    placeholders.insert(pl_patternName, doc->GetPatternName());
    placeholders.insert(pl_patternNumber, doc->GetPatternNumber());
    placeholders.insert(pl_author, doc->GetCompanyName());
    placeholders.insert(pl_customer, doc->GetCustomerName());
    placeholders.insert(pl_pExt, QString("val"));
    placeholders.insert(pl_pFileName, QFileInfo(qApp->GetPPath()).baseName());
    placeholders.insert(pl_mFileName, QFileInfo(doc->MPath()).baseName());

    QString curSize;
    QString curHeight;
    QString mExt;
    if (qApp->patternType() == MeasurementsType::Multisize)
    {
        curSize = QString::number(VContainer::size());
        curHeight = QString::number(VContainer::height());
        mExt = "vst";
    }
    else if (qApp->patternType() == MeasurementsType::Individual)
    {
        mExt = "vit";
    }

    placeholders.insert(pl_size, curSize);
    placeholders.insert(pl_height, curHeight);
    placeholders.insert(pl_mExt, mExt);

    // Piece tags
    placeholders.insert(pl_pLetter, "");
    placeholders.insert(pl_pName, "");
    placeholders.insert(pl_pQuantity, "");
    placeholders.insert(pl_wOnFold, "");
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
QList<TextLine> PrepareLines(const QVector<VLabelTemplateLine> &lines)
{
    QList<TextLine> textLines;

    for (int i=0; i < lines.size(); ++i)
    {
        if (not lines.at(i).line.isEmpty())
        {
            TextLine tl;
            tl.m_qsText = lines.at(i).line;
            tl.m_eAlign = static_cast<Qt::Alignment>(lines.at(i).alignment);
            tl.m_iFontSize = lines.at(i).fontSizeIncrement;
            tl.bold = lines.at(i).bold;
            tl.italic = lines.at(i).italic;

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
QList<TextLine> VTextManager::GetAllSourceLines() const
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
        fnt.setBold(tl.bold);
        fnt.setItalic(tl.italic);
        QFontMetrics fm(fnt);
        const int iTW = fm.width(tl.m_qsText);
        if (iTW > iMaxLen)
        {
            iMaxLen = iTW;
            maxLine = tl;
        }
    }
    if (iMaxLen > fW)
    {
        QFont fnt = m_font;
        fnt.setBold(maxLine.bold);
        fnt.setItalic(maxLine.italic);

        int lineLength = 0;
        do
        {
            --iFS;
            fnt.setPixelSize(iFS + maxLine.m_iFontSize);
            QFontMetrics fm(fnt);
            lineLength = fm.width(maxLine.m_qsText);
        }
        while (lineLength > fW && iFS > MIN_FONT_SIZE);
    }
    SetFontSize(iFS);
    qDebug() << "Font size" << GetSourceLinesCount() << iFS;
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
void VTextManager::Update(const VAbstractPattern *pDoc)
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

        m_patternLabelLines = PrepareLines(lines);
    }

    m_liLines = m_patternLabelLines;
}
