/************************************************************************
 **
 **  @file   vplaintextedit.cpp
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
#include "vplaintextedit.h"

//---------------------------------------------------------------------------------------------------------------------
VPlainTextEdit::VPlainTextEdit(QWidget * parent)
    : QPlainTextEdit(parent),
      m_highlighter(document())
{
    connect(this, &QPlainTextEdit::cursorPositionChanged, this, &VPlainTextEdit::MatchParentheses);
}

//---------------------------------------------------------------------------------------------------------------------
VPlainTextEdit::VPlainTextEdit(const QString & text, QWidget * parent)
    : QPlainTextEdit(text, parent),
      m_highlighter(document())
{
    connect(this, &QPlainTextEdit::cursorPositionChanged, this, &VPlainTextEdit::MatchParentheses);
}

//---------------------------------------------------------------------------------------------------------------------
VPlainTextEdit::~VPlainTextEdit()
{
    document()->blockSignals(true); // prevent crash
}

//---------------------------------------------------------------------------------------------------------------------
void VPlainTextEdit::MatchParentheses()
{
    QList<QTextEdit::ExtraSelection> selections;
    setExtraSelections(selections);

    VTextBlockData *data = static_cast<VTextBlockData *>(textCursor().block().userData());

    if (data)
    {
        QVector<ParenthesisInfo *> infos = data->Parentheses();

        int pos = textCursor().block().position();
        for (int i = 0; i < infos.size(); ++i)
        {
            ParenthesisInfo *info = infos.at(i);

            int curPos = textCursor().position() - textCursor().block().position();
            if ((info->position == curPos - 1 || info->position == curPos) && info->character == '(')
            {
                CreateParenthesisSelection(pos + info->position, MatchLeftParenthesis(textCursor().block(), i + 1, 0));
                return;
            }
            else if ((info->position == curPos - 1 || info->position == curPos) && info->character == ')')
            {
                CreateParenthesisSelection(pos + info->position, MatchRightParenthesis(textCursor().block(), i - 1, 0));
                return;
            }
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
bool VPlainTextEdit::MatchLeftParenthesis(QTextBlock currentBlock, int i, int numLeftParentheses)
{
    VTextBlockData *data = static_cast<VTextBlockData *>(currentBlock.userData());
    QVector<ParenthesisInfo *> infos = data->Parentheses();

    int docPos = currentBlock.position();
    for (; i < infos.size(); ++i)
    {
        ParenthesisInfo *info = infos.at(i);

        if (info->character == '(')
        {
            ++numLeftParentheses;
            continue;
        }

        if (info->character == ')' && numLeftParentheses == 0)
        {
            CreateParenthesisSelection(docPos + info->position);
            return true;
        }
        else
        {
            --numLeftParentheses;
        }
    }

    currentBlock = currentBlock.next();
    if (currentBlock.isValid())
    {
        return MatchLeftParenthesis(currentBlock, 0, numLeftParentheses);
    }

    return false;
}

//---------------------------------------------------------------------------------------------------------------------
bool VPlainTextEdit::MatchRightParenthesis(QTextBlock currentBlock, int i, int numRightParentheses)
{
    VTextBlockData *data = static_cast<VTextBlockData *>(currentBlock.userData());
    QVector<ParenthesisInfo *> parentheses = data->Parentheses();

    int docPos = currentBlock.position();
    for (; i > -1 && parentheses.size() > 0; --i)
    {
        ParenthesisInfo *info = parentheses.at(i);
        if (info->character == ')')
        {
            ++numRightParentheses;
            continue;
        }

        if (info->character == '(' && numRightParentheses == 0)
        {
            CreateParenthesisSelection(docPos + info->position);
            return true;
        }
        else
        {
            --numRightParentheses;
        }
    }

    currentBlock = currentBlock.previous();
    if (currentBlock.isValid())
    {
        return MatchRightParenthesis(currentBlock, 0, numRightParentheses);
    }

    return false;
}

//---------------------------------------------------------------------------------------------------------------------
void VPlainTextEdit::CreateParenthesisSelection(int pos, bool match)
{
    QList<QTextEdit::ExtraSelection> selections = extraSelections();

    QTextEdit::ExtraSelection selection;
    QTextCharFormat format = selection.format;
    if (match)
    {
        format.setBackground(Qt::lightGray);
        format.setForeground(Qt::yellow);
    }
    else
    {
        format.setForeground(Qt::red);
    }

    selection.format = format;

    QTextCursor cursor = textCursor();
    cursor.setPosition(pos);
    cursor.movePosition(QTextCursor::NextCharacter, QTextCursor::KeepAnchor);
    selection.cursor = cursor;

    selections.append(selection);

    setExtraSelections(selections);
}
