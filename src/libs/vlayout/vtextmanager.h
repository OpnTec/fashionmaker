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

#ifndef VTEXTMANAGER_H
#define VTEXTMANAGER_H

#include <QDate>
#include <QFont>
#include <QList>
#include <QString>
#include <QStringList>
#include <Qt>
#include <QtGlobal>
#include <QCoreApplication>

class VPieceLabelData;
class VAbstractPattern;

#define MIN_FONT_SIZE               5
#define MAX_FONT_SIZE               128

/**
 * @brief The TextLine struct holds the information about one text line
 */
struct TextLine
{
    QString             m_qsText;
    int                 m_iFontSize;  // 0 means default
    bool bold;
    bool italic;
    Qt::Alignment       m_eAlign;

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
    virtual ~VTextManager() = default;

    VTextManager(const VTextManager &text);
    VTextManager &operator=(const VTextManager &text);

    virtual int GetSpacing() const;

    void         SetFont(const QFont& font);
    const QFont& GetFont() const;
    void         SetFontSize(int iFS);
    void         FitFontSize(qreal fW, qreal fH);

    QList<TextLine> GetAllSourceLines() const;
    int             GetSourceLinesCount() const;
    const TextLine& GetSourceLine(int i) const;

    void Update(const QString& qsName, const VPieceLabelData& data);
    void Update(VAbstractPattern* pDoc);

private:
    QFont           m_font;
    QList<TextLine> m_liLines;

    static QList<TextLine> m_patternLabelLines;
};

#endif // VTEXTMANAGER_H
