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
    :m_liLines(), m_liOutput()
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



