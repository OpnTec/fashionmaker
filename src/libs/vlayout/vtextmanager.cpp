#include <QRegularExpression>
#include <QFontMetrics>

#include "vtextmanager.h"

//---------------------------------------------------------------------------------------------------------------------
TextLine::TextLine()
    :m_qsText(), m_iFontSize(MIN_FONT_SIZE), m_eFontWeight(QFont::Normal), m_eStyle(QFont::StyleNormal),
      m_eAlign(Qt::AlignCenter), m_iHeight(0)
{}

//---------------------------------------------------------------------------------------------------------------------
VTextManager::VTextManager()
	 :m_font(), m_liLines(), m_liOutput()
{}

//---------------------------------------------------------------------------------------------------------------------
VTextManager::~VTextManager()
{}

//---------------------------------------------------------------------------------------------------------------------
int VTextManager::GetSpacing() const
{
    return 2;
}

//---------------------------------------------------------------------------------------------------------------------
void VTextManager::SetFont(const QFont& font)
{
    m_font = font;
}

//---------------------------------------------------------------------------------------------------------------------
const QFont& VTextManager::GetFont() const
{
    return m_font;
}

//---------------------------------------------------------------------------------------------------------------------
void VTextManager::SetFontSize(int iFS)
{
    m_font.setPixelSize(iFS);
}

//---------------------------------------------------------------------------------------------------------------------
void VTextManager::AddLine(const TextLine& tl)
{
    m_liLines << tl;
}

//---------------------------------------------------------------------------------------------------------------------
void VTextManager::Clear()
{
    m_liLines.clear();
}

//---------------------------------------------------------------------------------------------------------------------
int VTextManager::GetCount() const
{
    return m_liOutput.count();
}

//---------------------------------------------------------------------------------------------------------------------
int VTextManager::GetSourceLineCount() const
{
    return m_liLines.count();
}

//---------------------------------------------------------------------------------------------------------------------
const TextLine& VTextManager::GetLine(int i) const
{
    return m_liOutput[i];
}

//---------------------------------------------------------------------------------------------------------------------
bool VTextManager::IsBigEnough(qreal fW, qreal fH, int iFontSize)
{
    m_liOutput.clear();
    QFont fnt = m_font;
    int iY = 0;
    for (int i = 0; i < m_liLines.count(); ++i)
    {
        const TextLine& tl = m_liLines.at(i);
        TextLine tlOut = tl;
        fnt.setPixelSize(iFontSize + tl.m_iFontSize);
        QFontMetrics fm(fnt);
        tlOut.m_iHeight = fm.height();
        QStringList qslLines = SplitString(tlOut.m_qsText, fW, fm);
        for (int iL = 0; iL < qslLines.count(); ++iL)
        {
            tlOut.m_qsText = qslLines[iL];
            m_liOutput << tlOut;
            iY += tlOut.m_iHeight + GetSpacing();
        }
    }
    return iY < fH;
}

//---------------------------------------------------------------------------------------------------------------------
void VTextManager::FitFontSize(qreal fW, qreal fH)
{
    int iFontSize = GetFont().pixelSize();
    while (IsBigEnough(fW, fH, iFontSize) == true && iFontSize <= MAX_FONT_SIZE)
    {
        ++iFontSize;
    }
    while (IsBigEnough(fW, fH, iFontSize) == false && iFontSize >= MIN_FONT_SIZE)
    {
        --iFontSize;
    }
    SetFontSize(iFontSize);
}

//---------------------------------------------------------------------------------------------------------------------
void VTextManager::Update(const QString& qsName, const VPatternPieceData& data)
{
    Clear();
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
        AddLine(tl);
    }
    // name
    tl.m_qsText = qsName;
    if (tl.m_qsText.isEmpty() == false)
    {
        tl.m_eFontWeight = QFont::DemiBold;
        tl.m_iFontSize = 2;
        AddLine(tl);
    }
    // MCP
    QString qsText = "Cut %1 on %2%3";
    QStringList qslPlace;
    qslPlace << "" << " on Fold";
    tl.m_eFontWeight = QFont::Normal;
    tl.m_iFontSize = 0;
    for (int i = 0; i < data.GetMCPCount(); ++i)
    {
        MaterialCutPlacement mcp = data.GetMCP(i);
        if (mcp.m_iCutNumber > 0)
        {
            tl.m_qsText = qsText.arg(mcp.m_iCutNumber).arg(mcp.m_qsMaterialUserDef).
                    arg(qslPlace[int(mcp.m_ePlacement)]);
            AddLine(tl);
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VTextManager::Update(const VAbstractPattern *pDoc)
{
    Clear();
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
        AddLine(tl);
    }
    // Pattern name
    tl.m_qsText = pDoc->GetPatternName();
    if (tl.m_qsText.isEmpty() == false)
    {
        tl.m_eFontWeight = QFont::Normal;
        tl.m_eStyle = QFont::StyleNormal;
        tl.m_iFontSize = 2;
        AddLine(tl);
    }
    // Pattern number
    tl.m_qsText = pDoc->GetPatternNumber();
    if (tl.m_qsText.isEmpty() == false)
    {
        tl.m_eFontWeight = QFont::Normal;
        tl.m_eStyle = QFont::StyleNormal;
        tl.m_iFontSize = 0;
        AddLine(tl);
    }
    // Customer name
    tl.m_qsText = pDoc->GetCustomerName();
    if (tl.m_qsText.isEmpty() == false)
    {
        tl.m_eFontWeight = QFont::Normal;
        tl.m_eStyle = QFont::StyleItalic;
        tl.m_iFontSize = 0;
        AddLine(tl);
    }
    // Size
    tl.m_qsText = pDoc->GetPatternSize();
    if (tl.m_qsText.isEmpty() == false)
    {
        tl.m_eFontWeight = QFont::Normal;
        tl.m_eStyle = QFont::StyleNormal;
        tl.m_iFontSize = 0;
        AddLine(tl);
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
        AddLine(tl);
    }

}

//---------------------------------------------------------------------------------------------------------------------
QStringList VTextManager::SplitString(const QString &qs, qreal fW, const QFontMetrics &fm)
{
    QRegularExpression reg("\\s+");
    QStringList qslWords = qs.split(reg);
    QStringList qslLines;
    QString qsCurrent;
    for (int i = 0; i < qslWords.count(); ++i)
    {
        if (qsCurrent.length() > 0)
        {
            qsCurrent += QLatin1Literal(" ");
        }
        if (fm.width(qsCurrent + qslWords[i]) > fW)
        {
            qslLines << qsCurrent;
            qsCurrent = qslWords[i];
        }
        else
        {
            qsCurrent += qslWords[i];
        }
    }
    qslLines << qsCurrent;
    return qslLines;
}



