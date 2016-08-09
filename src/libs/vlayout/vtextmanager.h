#ifndef VTEXTMANAGER_H
#define VTEXTMANAGER_H

#include <QDate>
#include <QFont>
#include <QList>
#include <QString>
#include <QStringList>
#include <Qt>
#include <QtGlobal>

#include "../ifc/xml/vabstractpattern.h"
#include "../vpatterndb/vpatternpiecedata.h"

class QFontMetrics;
class VAbstractPattern;
class VPatternPieceData;

#define MIN_FONT_SIZE               12
#define MAX_FONT_SIZE               128

/**
 * @brief The TextLine struct holds the information about one text line
 */
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

/**
 * @brief The VTextManager class this class is used to determine whether a collection of
 * text lines can fit into specified bounding box and with what font size
 */
class VTextManager
{
    Q_DECLARE_TR_FUNCTIONS(VTextManager)
public:
    VTextManager();
    virtual ~VTextManager();

    virtual int         GetSpacing() const;
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
    void                Update(const QString& qsName, const VPatternPieceData& data);
    void                Update(const VAbstractPattern* pDoc);

protected:
    static QStringList  SplitString(const QString& qs, qreal fW, const QFontMetrics& fm);

private:
    QFont               m_font;
    QList<TextLine>     m_liLines;
    QList<TextLine>     m_liOutput;
};

#endif // VTEXTMANAGER_H
