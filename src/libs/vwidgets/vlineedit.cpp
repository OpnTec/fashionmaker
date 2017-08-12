/************************************************************************
 **
 **  @file
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   12 8, 2017
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentine project, a pattern making
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

#include "vlineedit.h"

//---------------------------------------------------------------------------------------------------------------------
VLineEdit::VLineEdit(QWidget *parent)
    : QLineEdit(parent),
      m_selectOnMousePress(false)
{}

//---------------------------------------------------------------------------------------------------------------------
VLineEdit::VLineEdit(const QString &contents, QWidget *parent)
    : QLineEdit(contents, parent),
      m_selectOnMousePress(false)
{}

//---------------------------------------------------------------------------------------------------------------------
void VLineEdit::focusInEvent(QFocusEvent *e)
{
    QLineEdit::focusInEvent(e);
    selectAll();
    m_selectOnMousePress = true;
}

//---------------------------------------------------------------------------------------------------------------------
void VLineEdit::focusOutEvent(QFocusEvent *e)
{
    const int start = selectionStart();
    const int selectionLength = selectedText().length();
    const bool wasTextSelected = hasSelectedText();

    QLineEdit::focusOutEvent(e);

    if (wasTextSelected)
    {
        setSelection(start, selectionLength);
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VLineEdit::mousePressEvent(QMouseEvent *e)
{
    QLineEdit::mousePressEvent(e);
    if(m_selectOnMousePress)
    {
        selectAll();
        m_selectOnMousePress = false;
    }
}

