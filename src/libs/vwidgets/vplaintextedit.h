/************************************************************************
 **
 **  @file   vplaintextedit.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   14 11, 2017
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentina project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2017 Valentina project
 **  <https://gitlab.com/smart-pattern/valentina> All Rights Reserved.
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
#ifndef VPLAINTEXTEDIT_H
#define VPLAINTEXTEDIT_H

#include <QPlainTextEdit>
#include "vhighlighter.h"

class VPlainTextEdit : public QPlainTextEdit
{
    Q_OBJECT
public:
    explicit VPlainTextEdit(QWidget * parent = nullptr);
    VPlainTextEdit(const QString & text, QWidget * parent = nullptr);
    virtual ~VPlainTextEdit();

    void SetMatchParenthesesEnabled(bool enabled);

    void SetFilter(const QString &filter);

    void appendPlainText(const QString &text);

private slots:
    void MatchParentheses();

private:
    VHighlighter m_highlighter;
    QString m_filter{};
    QStringList m_allLines{};

    bool MatchLeftParenthesis(QTextBlock currentBlock, int i, int numLeftParentheses);
    bool MatchRightParenthesis(QTextBlock currentBlock, int i, int numRightParentheses);
    void CreateParenthesisSelection(int pos, bool match = true);
    void Filter();
};

#endif // VPLAINTEXTEDIT_H
