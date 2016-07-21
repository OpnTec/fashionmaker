#ifndef VTEXTMANAGER_H
#define VTEXTMANAGER_H

#include <QString>
#include <QFont>
#include <QDate>

#include "../vpatterndb/vpatternpiecedata.h"

#define MIN_FONT_SIZE               12
#define MAX_FONT_SIZE               128

struct TextLine
{
    QString             m_qsText;
    int                 m_iFontSize;  // 0 means default
    QFont::Weight       m_eFontWeight;
    QFont::Style        m_eStyle;
    Qt::Alignment       m_eAlign;
    int                 m_iHeight;

    TextLine();
};

class VTextManager
{
public:
    VTextManager();
    ~VTextManager();

    int                 GetSpacing() const;
    void                SetFont(const QFont& font);
    const QFont&        GetFont() const;
    void                SetFontSize(int iFS);
    void                AddLine(const TextLine& tl);
    void                Clear();
    int                 GetCount() const;
    int                 GetSourceLineCount() const;
    const TextLine&     GetLine(int i) const;
    bool                IsBigEnough(qreal fW, qreal fH, int iFontSize);
    void                FitFontSize(qreal fW, qreal fH);
    /** @brief Update(const QString& qsName, const VPatternPieceData& data)
     *  Updates the manager with detail name and detail data
     */
    void                Update(const QString& qsName, const VPatternPieceData& data);
    /** @brief Updateconst QString& qsPattern, const QString& qsNumber, const QString& qsSize,
                               const QString& qsCompany, const QString& qsCustomer, const QDate& date)
        Updates the manager with pattern data
     */
    void                Update(const QString& qsPattern, const QString& qsNumber, const QString& qsSize,
                               const QString& qsCompany, const QString& qsCustomer, const QDate& date);

protected:
    QStringList         SplitString(const QString& qs, qreal fW, const QFontMetrics& fm);

private:
    QFont               m_font;
    QList<TextLine>     m_liLines;
    QList<TextLine>     m_liOutput;
};

#endif // VTEXTMANAGER_H
