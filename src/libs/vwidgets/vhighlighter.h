/************************************************************************
 **
 **  @file   vhighlighter.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   14 11, 2017
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentina project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2017 Valentina project
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
#ifndef VHIGHLIGHTER_H
#define VHIGHLIGHTER_H

#include <QSyntaxHighlighter>
#include <QVector>

class QString;
class QTextDocument;

struct ParenthesisInfo
{
    char character;
    int position;
};

class VTextBlockData : public QTextBlockUserData
{
public:
    VTextBlockData();

    QVector<ParenthesisInfo *> Parentheses();
    void insert(ParenthesisInfo *info);

private:
    QVector<ParenthesisInfo *> m_parentheses;
};

class VHighlighter : public QSyntaxHighlighter
{
public:
    VHighlighter(QTextDocument *document);

protected:
    virtual void highlightBlock(const QString &text);
};

#endif // VHIGHLIGHTER_H
