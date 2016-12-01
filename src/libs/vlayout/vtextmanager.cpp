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

#include "../ifc/xml/vabstractpattern.h"
#include "../vpatterndb/vpatternpiecedata.h"
#include "../vmisc/vabstractapplication.h"
#include "vtextmanager.h"

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief TextLine::TextLine default constructor
 */
TextLine::TextLine()
    :m_qsText(), m_iFontSize(MIN_FONT_SIZE), m_eFontWeight(QFont::Normal), m_eStyle(QFont::StyleNormal),
      m_eAlign(Qt::AlignCenter), m_iHeight(0)
{}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief VTextManager::VTextManager constructor
 */
VTextManager::VTextManager()
     :m_font(), m_liLines(), m_liOutput()
{}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief VTextManager::~VTextManager destructor
 */
VTextManager::~VTextManager()
{}

//---------------------------------------------------------------------------------------------------------------------
VTextManager::VTextManager(const VTextManager &text)
    : m_font(text.GetFont()), m_liLines(text.GetAllSourceLines()), m_liOutput(text.GetAllOutputLines())
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
    m_liOutput = text.GetAllOutputLines();
    return *this;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief GetSpacing returns the vertical spacing between the lines
 * @return spacing
 */
int VTextManager::GetSpacing() const
{
    return 2;
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
    m_font.setPixelSize(iFS);
}

//---------------------------------------------------------------------------------------------------------------------
QList<TextLine> VTextManager::GetAllSourceLines() const
{
    return m_liLines;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief AddSourceLine add new text line to the list
 * @param tl text line object to be added
 */
void VTextManager::AddSourceLine(const TextLine& tl)
{
    m_liLines << tl;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief ClearSourceLines deletes the list of texts
 */
void VTextManager::ClearSourceLines()
{
    m_liLines.clear();
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief VTextManager::GetOutputLinesCount returns the number of output text lines
 * @return number of output text lines
 */
int VTextManager::GetOutputLinesCount() const
{
    return m_liOutput.count();
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
QList<TextLine> VTextManager::GetAllOutputLines() const
{
    return m_liOutput;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief VTextManager::GetLine returns the i-th output text line
 * @param i index of the output text line
 * @return i-th output text line
 */
const TextLine& VTextManager::GetOutputLine(int i) const
{
    return m_liOutput[i];
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief VTextManager::IsBigEnough Checks if rectangle of size (fW, fH) is big enough to hold the text with base font
 * size iFontSize
 * @param fW rectangle width
 * @param fH rectangle height
 * @param iFontSize base font size
 * @param fMinW minimal required rectangle width to fit the text
 * @param fMinH minimal required rectangle height to fit the text
 * @return true, if rectangle of size (fW, fH)
 */
bool VTextManager::IsBigEnough(qreal fW, qreal fH, int iFontSize, qreal& fMinW, qreal& fMinH)
{
    m_liOutput.clear();
    QFont fnt = m_font;
    int iY = 0;
    fMinW = fW;
    fMinH = fH;

    for (int i = 0; i < m_liLines.count(); ++i)
    {
        const TextLine& tl = m_liLines.at(i);
        TextLine tlOut = tl;
        fnt.setPixelSize(iFontSize + tl.m_iFontSize);
        QFontMetrics fm(fnt);
        int iHorSp = fm.width(" ");
        tlOut.m_iHeight = fm.height();
        QStringList qslLines = SplitString(tlOut.m_qsText, fW, fm);
        for (int iL = 0; iL < qslLines.count(); ++iL)
        {
            // check if every line fits within the label width
            if (fm.width(qslLines[iL]) + iHorSp > fW)
            {
                fMinW = fm.width(qslLines[iL]) + iHorSp;
                return false;
            }
            tlOut.m_qsText = qslLines[iL];
            m_liOutput << tlOut;
            iY += tlOut.m_iHeight + GetSpacing();
        }
    }
    if (iY > fH)
    {
        fMinH = iY;
    }
    return iY <= fH;
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
    int iFontSize = GetFont().pixelSize();
    qreal fMinW;
    qreal fMinH;

    while (IsBigEnough(fW, fH, iFontSize, fMinW, fMinH) == true && iFontSize <= MAX_FONT_SIZE)
    {
        ++iFontSize;
    }
    while (IsBigEnough(fW, fH, iFontSize, fMinW, fMinH) == false && iFontSize >= MIN_FONT_SIZE)
    {
        --iFontSize;
    }
    SetFontSize(iFontSize);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief VTextManager::Update updates the text lines with detail data
 * @param qsName detail name
 * @param data reference to the detail data
 */
void VTextManager::Update(const QString& qsName, const VPatternPieceData& data)
{
    ClearSourceLines();
    TextLine tl;
    // all text must be centered and normal style!
    tl.m_eAlign = Qt::AlignCenter;
    tl.m_eStyle = QFont::StyleNormal;

    // letter
    tl.m_qsText = data.GetLetter();
    if (tl.m_qsText.isEmpty() == false)
    {
        tl.m_eFontWeight = QFont::Bold;
        tl.m_iFontSize = 6;
        AddSourceLine(tl);
    }
    // name
    tl.m_qsText = qsName;
    if (tl.m_qsText.isEmpty() == false)
    {
        tl.m_eFontWeight = QFont::DemiBold;
        tl.m_iFontSize = 2;
        AddSourceLine(tl);
    }
    // MCP
    QStringList qslMaterials;
    qslMaterials << QApplication::translate("Detail", "Fabric", 0)
                 << QApplication::translate("Detail", "Lining", 0)
                 << QApplication::translate("Detail", "Interfacing", 0)
                 << QApplication::translate("Detail", "Interlining", 0);
    QString qsText = QLatin1String("%1, ") + tr("cut") + QLatin1String(" %2%3");
    QStringList qslPlace;
    qslPlace << "" << QLatin1String(" ") + tr("on fold");
    tl.m_eFontWeight = QFont::Normal;
    tl.m_iFontSize = 0;
    for (int i = 0; i < data.GetMCPCount(); ++i)
    {
        MaterialCutPlacement mcp = data.GetMCP(i);
        if (mcp.m_iCutNumber > 0)//Not gonna happen because min value is 1 now, but decided to left
        {
            QString qsMat;
            if (mcp.m_eMaterial == MaterialType::mtUserDefined)
            {
                qsMat = mcp.m_qsMaterialUserDef;
            }
            else
            {
                qsMat = qslMaterials[int(mcp.m_eMaterial)];
            }
            tl.m_qsText = qsText.arg(qsMat).arg(mcp.m_iCutNumber).
                    arg(qslPlace[int(mcp.m_ePlacement)]);
            AddSourceLine(tl);
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief VTextManager::Update updates the text lines with pattern info
 * @param pDoc pointer to the abstract pattern object
 */
void VTextManager::Update(const VAbstractPattern *pDoc, qreal dSize, qreal dHeight)
{
    ClearSourceLines();
    TextLine tl;
    // all information must be centered
    tl.m_eAlign = Qt::AlignCenter;

    // Company name
    tl.m_qsText = pDoc->GetCompanyName();
    if (tl.m_qsText.isEmpty() == false)
    {
        tl.m_eFontWeight = QFont::DemiBold;
        tl.m_eStyle = QFont::StyleNormal;
        tl.m_iFontSize = 4;
        AddSourceLine(tl);
    }
    // Pattern name
    tl.m_qsText = pDoc->GetPatternName();
    if (tl.m_qsText.isEmpty() == false)
    {
        tl.m_eFontWeight = QFont::Normal;
        tl.m_eStyle = QFont::StyleNormal;
        tl.m_iFontSize = 2;
        AddSourceLine(tl);
    }
    // Pattern number
    tl.m_qsText = pDoc->GetPatternNumber();
    if (tl.m_qsText.isEmpty() == false)
    {
        tl.m_eFontWeight = QFont::Normal;
        tl.m_eStyle = QFont::StyleNormal;
        tl.m_iFontSize = 0;
        AddSourceLine(tl);
    }
    // Customer name
    tl.m_qsText = pDoc->GetCustomerName();
    if (tl.m_qsText.isEmpty() == false)
    {
        tl.m_eFontWeight = QFont::Normal;
        tl.m_eStyle = QFont::StyleItalic;
        tl.m_iFontSize = 0;
        AddSourceLine(tl);
    }
    // Size
    tl.m_qsText = pDoc->GetPatternSize();
    if (tl.m_qsText.isEmpty() == false)
    {
        // Such describing placeholders will help avoid mistake of localization.
        // Translators very often remove '%'.
        QString placeholder = QLatin1String("%") + qApp->TrVars()->PlaceholderToUser(pl_size) + QLatin1String("%");
        tl.m_qsText.replace(placeholder, QString::number(dSize));

        placeholder = QLatin1String("%") + qApp->TrVars()->PlaceholderToUser(pl_height) + QLatin1String("%");
        tl.m_qsText.replace(placeholder, QString::number(dHeight));

        tl.m_eFontWeight = QFont::Normal;
        tl.m_eStyle = QFont::StyleNormal;
        tl.m_iFontSize = 0;
        AddSourceLine(tl);
    }
    // Measurements
    tl.m_qsText = QFileInfo(pDoc->MPath()).fileName();
    if (tl.m_qsText.isEmpty() == false && pDoc->IsMeasurementsVisible() == true)
    {
        tl.m_eFontWeight = QFont::Normal;
        tl.m_eStyle = QFont::StyleNormal;
        tl.m_iFontSize = 0;
        AddSourceLine(tl);
    }
    // Date
    QDate date;
    if (pDoc->IsDateVisible() == true)
    {
        date = QDate::currentDate();
    }
    if (date.isValid() == true)
    {
        tl.m_qsText = date.toString("dd MMMM yyyy");
        tl.m_eFontWeight = QFont::Normal;
        tl.m_eStyle = QFont::StyleNormal;
        tl.m_iFontSize = 0;
        AddSourceLine(tl);
    }

}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief VTextManager::SplitString splits the string into several lines, which all fit into width fW
 * @param qs string to split
 * @param fW required width of every output string
 * @param fm font metrics of the font used
 * @return list of strings, each of which is not wider than fW using the font metrics fm
 */
QStringList VTextManager::SplitString(const QString &qs, qreal fW, const QFontMetrics &fm)
{
    QRegularExpression reg("\\s+");
    // split the string into words
    QStringList qslWords = qs.split(reg);
    QStringList qslLines;
    QString qsCurrent;
    for (int i = 0; i < qslWords.count(); ++i)
    {
        if (qsCurrent.length() > 0)
        {
            qsCurrent += QLatin1String(" ");
        }
        // check if another word can be added into current line
        if (fm.width(qsCurrent + qslWords[i]) > fW)
        {
            // if not, add the current line into the list of text lines
            if (qsCurrent.isEmpty() == false)
            {
                qslLines << qsCurrent;
            }
            // and set the current line to contain the current word
            qsCurrent = qslWords[i];
        }
        else
        {
            qsCurrent += qslWords[i];
        }
    }
    qslLines << qsCurrent;

    int i = 0;
    int iHorSp = fm.width(" ");
    while (i < qslLines.count()) {
        if (fm.width(qslLines[i]) + iHorSp > fW) {
            int iPos = qslLines[i].length() >> 1;
            QString qs1 = qslLines[i].left(iPos);
            QString qs2 = qslLines[i].mid(iPos);
            qslLines[i] = qs1;
            qslLines.insert(i + 1, qs2);
        }   else {
            ++i;
        }
    }

    return qslLines;
}



